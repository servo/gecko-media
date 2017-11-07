#include <assert.h>
#include <map>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include "AudioStream.h"
#include "DecoderTraits.h"
#include "GeckoMedia.h"
#include "GeckoMediaDecoder.h"
#include "GeckoMediaDecoderOwner.h"
#include "ImageContainer.h"
#include "MediaData.h"
#include "MediaDecoder.h"
#include "MediaStreamGraph.h"
#include "PlatformDecoderModule.h"
#include "VideoUtils.h"
#include "gecko_media_prefs.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/Logging.h"
#include "mozilla/Preferences.h"
#include "mozilla/SharedThreadPool.h"
#include "mozilla/TaskQueue.h"
#include "mozilla/TimeStamp.h"
#include "nsClassHashtable.h"
#include "nsDataHashtable.h"
#include "nsPrintfCString.h"
#include "nsRefPtrHashtable.h"
#include "nsString.h"
#include "nsTArray.h"
#include "nsThreadManager.h"
#include "nsThreadUtils.h"

#define SIMPLE_STRING "I'm a simple ASCII string"
#define UTF8_STRING                                                            \
  "Андропов, Брежнев, Горбачёв, Ленин, Маленков, Сталин, Хрущёв, Черненко"

namespace mozilla {

void
TestString()
{
  std::string std_utf8 = SIMPLE_STRING;
  std::u16string std_utf16 = u"" SIMPLE_STRING;

  nsCString moz_utf8(NS_LITERAL_CSTRING(SIMPLE_STRING));
  assert(std_utf8 == moz_utf8.get());
  assert(std_utf8.size() == moz_utf8.Length());

  nsString moz_utf16(NS_LITERAL_STRING(SIMPLE_STRING));
  assert(std_utf16 == moz_utf16.get());

  std_utf16 = u"" UTF8_STRING;
  moz_utf16 = NS_ConvertUTF8toUTF16(NS_LITERAL_CSTRING(UTF8_STRING));
  assert(std_utf16 == moz_utf16.get());

  nsPrintfCString pcs("Hi there %s", "Bob");
  assert(pcs.EqualsASCII("Hi there Bob"));
}

void
TestArray()
{
  nsTArray<uint32_t> a;
  uint32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  a.AppendElements(data, ArrayLength(data));
  assert(memcmp(a.Elements(), data, ArrayLength(data) * sizeof(data[0])) == 0);
  auto i = 0;
  for (auto value : a) {
    assert(data[i++] == value);
  }
}

struct PrefCallbackTestData
{
  const char* mName;
  int32_t mCount;
};

void
PrefChanged(const char* aPref, void* aClosure)
{
  PrefCallbackTestData* data =
    reinterpret_cast<PrefCallbackTestData*>(aClosure);
  assert(strcmp(aPref, data->mName) == 0);
  data->mCount += 1;
}

std::map<const char*, bool> sBoolPrefs;
std::map<const char*, int32_t> sIntPrefs;
std::map<const char*, const char*> sStrPrefs;

void
pref(const char* aName, bool aValue)
{
  sBoolPrefs[aName] = aValue;
}

void
pref(const char* aName, const char* aValue)
{
  sStrPrefs[aName] = aValue;
}

void
pref(const char* aName, int32_t aValue)
{
  sIntPrefs[aName] = aValue;
}

void
InitPrefs()
{
#include "../glue/prefs_common.cpp"
#if defined(MOZ_WIDGET_ANDROID)
#include "../glue/prefs_android.cpp"
#else
#include "../glue/prefs_desktop.cpp"
#endif
}

void
TestPreferences()
{
  // Check all string prefs work.
  for (auto itr : sStrPrefs) {
    nsAutoCString utf8;
    nsresult rv = Preferences::GetCString(itr.first, utf8);
    assert(NS_SUCCEEDED(rv));
    assert(utf8.EqualsASCII(itr.second));

    nsAutoString utf16;
    rv = Preferences::GetString(itr.first, utf16);
    assert(NS_SUCCEEDED(rv));
    assert(utf8.EqualsASCII(itr.second));

    assert(NS_ConvertUTF8toUTF16(utf8) == utf16);
  }

  // Check all bool prefs work.
  for (auto itr : sBoolPrefs) {
    assert(Preferences::GetBool(itr.first) == itr.second);
  }

  // Check all int prefs work.
  for (auto itr : sIntPrefs) {
    assert(Preferences::GetInt(itr.first) == itr.second);
  }

  // Check unknown pref provides the default.
  assert(Preferences::GetBool("unknown.pref", false) == false);
  assert(Preferences::GetBool("unknown.pref", true) == true);

  // Check pref change callback calls us back immediately.
  const char* PREF_VOLUME_SCALE = "media.volume_scale";
  PrefCallbackTestData data;
  data.mName = PREF_VOLUME_SCALE;
  data.mCount = 0;
  Preferences::RegisterCallbackAndCall(PrefChanged, PREF_VOLUME_SCALE, &data);
  assert(data.mCount == 1);
}

void
TestAudioStream()
{
  class FakeChunk : public AudioStream::Chunk
  {
  public:
    FakeChunk() {}
    const AudioDataValue* Data() const { return NULL; }
    uint32_t Frames() const { return 0; }
    uint32_t Channels() const { return 2; }
    uint32_t Rate() const { return 44100; }
    AudioDataValue* GetWritable() const { return NULL; }
  };
  class FakeAudioSource : public AudioStream::DataSource
  {
  public:
    FakeAudioSource() {}
    UniquePtr<AudioStream::Chunk> PopFrames(uint32_t aFrames)
    {
      return MakeUnique<FakeChunk>();
    };
    bool Ended() const { return false; };
    void Drained(){};
  };

  auto* dataSource = new FakeAudioSource();
  auto* audioStream = new AudioStream(*dataSource);
  auto rv = audioStream->Init(2, 2, 44100);
  assert(NS_SUCCEEDED(rv));
  assert(audioStream != nullptr);

  assert(audioStream->GetOutChannels() == 2);

  audioStream->SetVolume(0.5);
  audioStream->Start();
  usleep(2000);
  audioStream->Shutdown();
}

void
TestThreads()
{
  RefPtr<nsIThread> thread;
  nsresult rv = NS_NewThread(getter_AddRefs(thread));
  int x = 0;
  RefPtr<Runnable> task(
    NS_NewRunnableFunction("TestFunction", [&]() { x = 1; }));
  assert(NS_SUCCEEDED(rv));
  assert(thread != nullptr);
  thread->Dispatch(task.forget());
  thread->Shutdown();
  assert(x == 1);
}

template<typename FunctionType>
void
RunOnTaskQueue(TaskQueue* aQueue, FunctionType aFun)
{
  nsCOMPtr<nsIRunnable> r = NS_NewRunnableFunction("RunOnTaskQueue", aFun);
  aQueue->Dispatch(r.forget());
}

void
TestMozPromise()
{
  class MOZ_STACK_CLASS AutoTaskQueue
  {
  public:
    AutoTaskQueue()
      : mTaskQueue(new TaskQueue(GetMediaThreadPool(MediaThreadType::PLAYBACK)))
    {
    }

    ~AutoTaskQueue() { mTaskQueue->AwaitShutdownAndIdle(); }

    TaskQueue* Queue() { return mTaskQueue; }

  private:
    RefPtr<TaskQueue> mTaskQueue;
  };

  typedef MozPromise<int, double, false> TestPromise;
  typedef TestPromise::ResolveOrRejectValue RRValue;
#define DO_FAIL                                                                \
  []() {                                                                       \
    assert(true == false);                                                     \
    return TestPromise::CreateAndReject(0, __func__);                          \
  }

#define EXPECT_EQ(a, b) assert(a == b)

  AutoTaskQueue atq;
  RefPtr<TaskQueue> queue = atq.Queue();
  RunOnTaskQueue(queue, [queue]() -> void {
    TestPromise::CreateAndResolve(42, __func__)
      ->Then(queue,
             __func__,
             [queue](int aResolveValue) -> void {
               EXPECT_EQ(aResolveValue, 42);
               queue->BeginShutdown();
             },
             DO_FAIL);
  });

  RunOnTaskQueue(queue, [queue]() -> void {
    TestPromise::CreateAndReject(42.0, __func__)
      ->Then(queue, __func__, DO_FAIL, [queue](int aRejectValue) -> void {
        EXPECT_EQ(aRejectValue, 42.0);
        queue->BeginShutdown();
      });
  });
}

void
TestHashTables()
{
  static const char* values[] = { "zero", "one", "two", "three", "four" };
  {
    nsDataHashtable<nsUint32HashKey, const char*> table;
    for (uint32_t i = 0; i < ArrayLength(values); i++) {
      table.Put(i, values[i]);
    }
    for (uint32_t i = 0; i < ArrayLength(values); i++) {
      assert(table.Get(i) == values[i]);
    }
  }
  {
    nsClassHashtable<nsPtrHashKey<const char>, nsCString> table;
    for (const char* value : values) {
      table.Put(value, new nsCString(value));
    }
    for (uint32_t i = 0; i < ArrayLength(values); i++) {
      assert(table.Get(values[i])->EqualsASCII(values[i]));
    }

    for (auto iter = table.Iter(); !iter.Done(); iter.Next()) {
      const char* key = iter.Key();
      const nsCString* data = iter.UserData();
      assert(data->EqualsASCII(key));
      assert(key != nullptr);
    }
  }
  {
    class Data
    {
    public:
      NS_INLINE_DECL_REFCOUNTING(Data);
      int32_t mIndex = -1;

    private:
      ~Data() {}
    };
    nsRefPtrHashtable<nsRefPtrHashKey<Data>, Data> table;
    nsTArray<RefPtr<Data>> datas;
    for (int32_t i = 0; i < 5; i++) {
      RefPtr<Data> d = new Data();
      d->mIndex = i;
      datas.AppendElement(d);
      table.Put(d, d);
    }
    for (int32_t i = 0; i < 5; i++) {
      RefPtr<Data> d;
      bool found = table.Get(datas[i], getter_AddRefs(d));
      assert(found);
      assert(d == datas[i]);
      assert(d->mIndex == i);
    }
  }
}

void
TestTimeStamp()
{
  TimeStamp now = TimeStamp::Now();
  TimeStamp ahead = now + TimeDuration::FromSeconds(10);
  TimeDuration delta = ahead - now;
  assert(delta == TimeDuration::FromSeconds(10));
}

// Copy of BlankVideoDataCreator::Create(), use BlankVideoDataCreator
// directly once it's in our source.
already_AddRefed<MediaData>
CreateBlankVideoData(uint32_t aFrameWidth, uint32_t aFrameHeight)
{
  // Create a fake YUV buffer in a 420 format. That is, an 8bpp Y plane,
  // with a U and V plane that are half the size of the Y plane, i.e 8 bit,
  // 2x2 subsampled. Have the data pointer of each frame point to the
  // first plane, they'll always be zero'd memory anyway.
  const CheckedUint32 size = CheckedUint32(aFrameWidth) * aFrameHeight;
  if (!size.isValid()) {
    // Overflow happened.
    return nullptr;
  }
  auto frame = MakeUniqueFallible<uint8_t[]>(size.value());
  if (!frame) {
    return nullptr;
  }
  memset(frame.get(), 0, aFrameWidth * aFrameHeight);
  VideoData::YCbCrBuffer buffer;

  // Y plane.
  buffer.mPlanes[0].mData = frame.get();
  buffer.mPlanes[0].mStride = aFrameWidth;
  buffer.mPlanes[0].mHeight = aFrameHeight;
  buffer.mPlanes[0].mWidth = aFrameWidth;
  buffer.mPlanes[0].mOffset = 0;
  buffer.mPlanes[0].mSkip = 0;

  // Cb plane.
  buffer.mPlanes[1].mData = frame.get();
  buffer.mPlanes[1].mStride = (aFrameWidth + 1) / 2;
  buffer.mPlanes[1].mHeight = (aFrameHeight + 1) / 2;
  buffer.mPlanes[1].mWidth = (aFrameWidth + 1) / 2;
  buffer.mPlanes[1].mOffset = 0;
  buffer.mPlanes[1].mSkip = 0;

  // Cr plane.
  buffer.mPlanes[2].mData = frame.get();
  buffer.mPlanes[2].mStride = (aFrameWidth + 1) / 2;
  buffer.mPlanes[2].mHeight = (aFrameHeight + 1) / 2;
  buffer.mPlanes[2].mWidth = (aFrameWidth + 1) / 2;
  buffer.mPlanes[2].mOffset = 0;
  buffer.mPlanes[2].mSkip = 0;

  VideoInfo info;
  info.mDisplay = gfx::IntSize(aFrameWidth, aFrameHeight);
  gfx::IntRect picture(0, 0, aFrameWidth, aFrameHeight);
  RefPtr<layers::ImageContainer> imageContainer = new layers::ImageContainer();
  return VideoData::CreateAndCopyData(
    info,
    imageContainer,
    0,
    media::TimeUnit::Zero(),
    media::TimeUnit::FromSeconds(1000.0 / 30.0),
    buffer,
    false,
    media::TimeUnit::Zero(),
    picture);
}

void
TestVideoData()
{
  RefPtr<MediaData> frame = CreateBlankVideoData(320, 240);
  assert(frame != nullptr);
}

// Copy of BlankAudioDataCreator::Create(), use BlankAudioDataCreator
// directly once it's in our source.
already_AddRefed<MediaData>
CreateBlankAudioData()
{
  const int64_t offset = 0;
  const media::TimeUnit timestamp = media::TimeUnit::Zero();
  const media::TimeUnit duration = media::TimeUnit::FromSeconds(1000.0 / 30.0);
  const uint32_t channelCount = 2;
  const uint32_t sampleRate = 44100;
  int64_t frameSum = 0;

  // Convert duration to frames. We add 1 to duration to account for
  // rounding errors, so we get a consistent tone.
  CheckedInt64 frames =
    UsecsToFrames(duration.ToMicroseconds() + 1, sampleRate);
  if (!frames.isValid() || !channelCount || !sampleRate ||
      frames.value() > (UINT32_MAX / channelCount)) {
    return nullptr;
  }
  AlignedAudioBuffer samples(frames.value() * channelCount);
  if (!samples) {
    return nullptr;
  }
  // Fill the sound buffer with an A4 tone.
  static const float pi = 3.14159265f;
  static const float noteHz = 440.0f;
  for (int i = 0; i < frames.value(); i++) {
    float f = sin(2 * pi * noteHz * frameSum / sampleRate);
    for (unsigned c = 0; c < channelCount; c++) {
      samples[i * channelCount + c] = AudioDataValue(f);
    }
    frameSum++;
  }
  RefPtr<AudioData> data(new AudioData(offset,
                                       timestamp,
                                       duration,
                                       uint32_t(frames.value()),
                                       Move(samples),
                                       channelCount,
                                       sampleRate));
  return data.forget();
}

void
TestAudioData()
{
  RefPtr<MediaData> frame = CreateBlankAudioData();
  assert(frame != nullptr);
}

extern void
Test_MediaDataDecoder();

extern void
Test_MediaMIMETypes();

extern void
Test_MP4Demuxer();

void
TestDecoderTraits()
{
  assert(DecoderTraits::CanHandleContainerType(
           MediaContainerType(MEDIAMIMETYPE("audio/mp4")), nullptr) ==
         CANPLAY_MAYBE);
  assert(DecoderTraits::CanHandleContainerType(
           MediaContainerType(MEDIAMIMETYPE("video/mp4")), nullptr) ==
         CANPLAY_MAYBE);
  assert(DecoderTraits::CanHandleContainerType(
           MediaContainerType(MEDIAMIMETYPE("audio/wav")), nullptr) ==
         CANPLAY_MAYBE);
  assert(DecoderTraits::CanHandleContainerType(
         MediaContainerType(MEDIAMIMETYPE("audio/mp3")), nullptr) ==
         CANPLAY_MAYBE);
}

class OwningBufferMediaResource : public BufferMediaResource
{
public:
  static already_AddRefed<BufferMediaResource> Create(const char* aFilename)
  {
    FILE* f = fopen(aFilename, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t* data = new uint8_t[fsize];
    size_t x = fread(data, fsize, 1, f);
    assert(x == 1);
    fclose(f);
    RefPtr<BufferMediaResource> resource =
      new OwningBufferMediaResource(data, fsize);
    return resource.forget();
  }
private:
  OwningBufferMediaResource(const uint8_t* aBuffer, size_t aLength)
    : BufferMediaResource(aBuffer, aLength)
    , mBuffer(aBuffer)
  {
  }
  ~OwningBufferMediaResource() {
    delete[] mBuffer;
  }
  const uint8_t* mBuffer;
};

void
TestGeckoDecoder()
{
  GeckoMediaDecoderOwner owner;
  MediaDecoderInit decoderInit(&owner,
                               0.05,  // volume
                               true,  // mPreservesPitch
                               1.0,   // mPlaybackRate
                               false, // mMinimizePreroll
                               false, // mHasSuspendTaint
                               false, // mLooping
                               MediaContainerType(MEDIAMIMETYPE("audio/wav")));
  RefPtr<GeckoMediaDecoder> decoder = new GeckoMediaDecoder(decoderInit);

  RefPtr<BufferMediaResource> resource =
    OwningBufferMediaResource::Create("gecko/test/audiotest.wav");
  decoder->Load(resource);

  decoder->Play();

  SpinEventLoopUntil([decoder, &owner]() { return decoder->IsEnded() || owner.HasError(); });
  MOZ_ASSERT(!owner.HasError());
  decoder->Shutdown();
}

} // namespace mozilla

extern "C" void
TestGecko()
{
  mozilla::TestPreferences();
  mozilla::TestString();
  mozilla::TestArray();
  mozilla::TestAudioStream();
  mozilla::TestHashTables();
  mozilla::TestTimeStamp();
  mozilla::TestThreads();
  mozilla::TestMozPromise();
  mozilla::TestVideoData();
  mozilla::TestAudioData();
  mozilla::Test_MediaDataDecoder();
  mozilla::Test_MediaMIMETypes();
  mozilla::Test_MP4Demuxer();
  mozilla::TestDecoderTraits();
  mozilla::TestGeckoDecoder();
}
