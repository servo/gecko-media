#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include "AudioStream.h"
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
#include "MediaData.h"
#include "ImageContainer.h"

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

#include "../glue/prefs_common.cpp"
#if defined(MOZ_WIDGET_ANDROID)
#include "../glue/prefs_android.cpp"
#define PLATFORM_STRING_PREFS sAndroidStringPrefs
#define PLATFORM_BOOL_PREFS sAndroidBoolPrefs
#define PLATFORM_INT_PREFS sAndroidIntPrefs
#else
#include "../glue/prefs_desktop.cpp"
#define PLATFORM_STRING_PREFS sDesktopStringPrefs
#define PLATFORM_BOOL_PREFS sDesktopBoolPrefs
#define PLATFORM_INT_PREFS sDesktopIntPrefs
#endif

template<class PrefType>
static nsTArray<PrefType>
MergePrefList(const PrefType* aCommonPrefs,
              size_t aCommonPrefsLen,
              const PrefType* aPlatformPrefs,
              size_t aPlatformPrefsLen)
{
  nsTArray<PrefType> rv;
  const PrefType* commonEnd = aCommonPrefs + aCommonPrefsLen;
  const PrefType* platformEnd = aPlatformPrefs + aPlatformPrefsLen;
  while (aCommonPrefs < commonEnd && aPlatformPrefs < platformEnd) {
    int cmp = strcmp(aCommonPrefs->mName, aPlatformPrefs->mName);
    if (cmp < 0) {
      rv.AppendElement(*aCommonPrefs);
      aCommonPrefs++;
    } else if (cmp > 0) {
      rv.AppendElement(*aPlatformPrefs);
      aPlatformPrefs++;
    } else {
      // Platform overrides.
      rv.AppendElement(*aPlatformPrefs);
      aPlatformPrefs++;
      aCommonPrefs++;
    }
  }
  while (aCommonPrefs < commonEnd) {
    rv.AppendElement(*aCommonPrefs);
    aCommonPrefs++;
  }
  while (aPlatformPrefs < platformEnd) {
    rv.AppendElement(*aPlatformPrefs);
    aPlatformPrefs++;
  }
  return rv;
}

void
TestPreferences()
{
  // Check all string prefs work.
  nsTArray<StringPref> stringPrefs =
    MergePrefList(sCommonStringPrefs,
                  sizeof(sCommonStringPrefs) / sizeof(StringPref),
                  PLATFORM_STRING_PREFS,
                  sizeof(PLATFORM_STRING_PREFS) / sizeof(StringPref));
  for (const StringPref& pref : stringPrefs) {
    nsAutoCString utf8;
    nsresult rv = Preferences::GetCString(pref.mName, utf8);
    assert(NS_SUCCEEDED(rv));
    assert(utf8.EqualsASCII(pref.mValue));

    nsAutoString utf16;
    rv = Preferences::GetString(pref.mName, utf16);
    assert(NS_SUCCEEDED(rv));
    assert(utf8.EqualsASCII(pref.mValue));

    assert(NS_ConvertUTF8toUTF16(utf8) == utf16);
  }

  // Check all bool prefs work.
  nsTArray<BoolPref> boolPrefs =
    MergePrefList(sCommonBoolPrefs,
                  sizeof(sCommonBoolPrefs) / sizeof(BoolPref),
                  PLATFORM_BOOL_PREFS,
                  sizeof(PLATFORM_BOOL_PREFS) / sizeof(IntPref));
  for (const BoolPref& pref : boolPrefs) {
    assert(Preferences::GetBool(pref.mName) == pref.mValue);
  }

  // Check all int prefs work.
  nsTArray<IntPref> intPrefs =
    MergePrefList(sCommonIntPrefs,
                  sizeof(sCommonIntPrefs) / sizeof(IntPref),
                  PLATFORM_INT_PREFS,
                  sizeof(PLATFORM_INT_PREFS) / sizeof(IntPref));
  for (const IntPref& pref : intPrefs) {
    assert(Preferences::GetInt(pref.mName) == pref.mValue);
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
CreateBlankVideoData(uint32_t aFrameWidth,
                     uint32_t aFrameHeight)
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
  return VideoData::CreateAndCopyData(info,
                                      imageContainer,
                                      0,
                                      media::TimeUnit::Zero(),
                                      media::TimeUnit::FromSeconds(1000.0 / 30.0),
                                      buffer,
                                      false,
                                      media::TimeUnit::Zero(),
                                      picture);
}

void TestVideoData()
{
  RefPtr<MediaData> frame = CreateBlankVideoData(320, 240);
  assert(frame != nullptr);
}

} // namespace mozilla

extern "C" void
TestGecko()
{
  // TODO: Move these Init calls to a top-level function? See also XPCOMInit.
  mozilla::LogModule::Init();
  NS_SetMainThread();
  nsThreadManager::get().Init();
  NS_InitMinimalXPCOM();
  mozilla::SharedThreadPool::InitStatics();

  mozilla::TestPreferences();
  mozilla::TestString();
  mozilla::TestArray();
  mozilla::TestAudioStream();
  mozilla::TestHashTables();
  mozilla::TestTimeStamp();
  mozilla::TestThreads();
  mozilla::TestMozPromise();
  mozilla::TestVideoData();

  NS_ShutdownXPCOM(nullptr);
}
