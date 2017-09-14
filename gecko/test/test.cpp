#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include "AudioStream.h"
#include "mozilla/ArrayUtils.h"
#include "mozilla/Logging.h"
#include "mozilla/TimeStamp.h"
#include "nsClassHashtable.h"
#include "nsDataHashtable.h"
#include "nsRefPtrHashtable.h"
#include "nsString.h"
#include "nsPrintfCString.h"
#include "nsTArray.h"
// #include "nsThreadUtils.h"

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

// void TestThreads() {
//   RefPtr<nsIThread> thread;]]
//   nsresult rv = NS_NewThread(getter_AddRefs(thread));
//   int x = 0;
//   RefPtr<Runnable> task(NS_NewRunnableFunction("TestFunction", [&]() {
//     x = 1;
//   }));
//   assert(NS_SUCCEEDED(rv));
//   assert(thread != nullptr);
//   thread->Dispatch(task.forget());
//   thread->Shutdown();
//   assert(x == 1);
// }

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
      uint32_t mIndex = -1;

    private:
      ~Data() {}
    };
    nsRefPtrHashtable<nsRefPtrHashKey<Data>, Data> table;
    nsTArray<RefPtr<Data>> datas;
    for (uint32_t i = 0; i < 5; i++) {
      RefPtr<Data> d = new Data();
      d->mIndex = i;
      datas.AppendElement(d);
      table.Put(d, d);
    }
    for (uint32_t i = 0; i < 5; i++) {
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

} // namespace mozilla

extern "C" void
TestGecko()
{
  mozilla::LogModule::Init();
  mozilla::TestString();
  mozilla::TestArray();
  mozilla::TestAudioStream();
  mozilla::TestHashTables();
  mozilla::TestTimeStamp();
  // mozilla::TestThreads();
}
