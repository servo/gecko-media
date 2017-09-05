#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>

#include "nsString.h"
#include "nsTArray.h"
// #include "mozilla/ArrayUtils.h"

// #error "WTF!"


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

// void TestArray() {
//   nsTArray<uint32_t> a;
//   uint32_t data[] = {0,1,2,3,4,5,6,7,8,9,10};
//   a.AppendElements(data, ArrayLength(data));

// }

}

extern "C" void TestGecko() {
  mozilla::TestString();
  // mozilla::TestArray();
}
