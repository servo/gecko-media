#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>

#include "AudioStream.h"
#include "nsString.h"
#include "nsTArray.h"
#include "mozilla/ArrayUtils.h"

#define SIMPLE_STRING "I'm a simple ASCII string"
#define UTF8_STRING "Андропов, Брежнев, Горбачёв, Ленин, Маленков, Сталин, Хрущёв, Черненко"

namespace mozilla {

void TestString() {
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
}

void TestArray() {
  nsTArray<uint32_t> a;
  uint32_t data[] = {0,1,2,3,4,5,6,7,8,9,10};
  a.AppendElements(data, ArrayLength(data));
  assert(memcmp(a.Elements(), data, ArrayLength(data) * sizeof(data[0])) == 0);
  auto i = 0;
  for (auto value : a) {
    assert(data[i++] == value);
  }
}

  void TestAudioStream() {
    class FakeChunk : public AudioStream::Chunk {
    public:
      FakeChunk() {}
      const AudioDataValue* Data() const { return NULL; }
      uint32_t Frames() const { return 0; }
      uint32_t Channels() const { return 2; }
      uint32_t Rate() const { return 44100; }
      AudioDataValue* GetWritable() const { return NULL; }
    };
    class FakeAudioSource : public AudioStream::DataSource {
    public:
      FakeAudioSource() {}
      UniquePtr<AudioStream::Chunk> PopFrames(uint32_t aFrames) {
        return MakeUnique<FakeChunk>();
      };
      bool Ended() const { return false; };
      void Drained() {};
    };

    auto* dataSource = new FakeAudioSource();
    auto* audioStream = new AudioStream(*dataSource);
    // FIXME: This doesn't work because CubebUtils doesn't build yet.
    // auto result = audioStream->Init(2, 1, 44100);
    // printf("result: %d\n", result);
    printf("audioStream: %p\n", audioStream);
  }
}

extern "C" void TestGecko() {
  mozilla::TestString();
  mozilla::TestArray();
  mozilla::TestAudioStream();
}
