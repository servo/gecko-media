#include "nsString.h"

#include <locale>
#include <codecvt>

// #error "WTF"

template class std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>;

NS_ConvertUTF16toUTF8::NS_ConvertUTF16toUTF8(const nsAString& aUTF16)
  : nsACString(std::wstring_convert<
    std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(aUTF16.AsStdStr()))
{
}
NS_ConvertUTF16toUTF8::NS_ConvertUTF16toUTF8(nsString aUTF16)
  : nsACString(std::wstring_convert<
    std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(aUTF16.AsStdStr()))
{
}

NS_ConvertUTF8toUTF16::NS_ConvertUTF8toUTF16(const nsACString& aUTF8)
  : nsAString(std::wstring_convert<
    std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(aUTF8.AsStdStr()))
{
}
