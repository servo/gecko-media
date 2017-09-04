#ifndef gecko_media_nsString_h
#define gecko_media_nsString_h

#include <string>
#include <locale>
#include <codecvt>

template<typename StdStringType>
class BaseString {
public:
  BaseString(const StdStringType& aString)
    : mString(aString)
  {
  }
  BaseString(StdStringType&& aString)
    : mString(move(aString))
  {
  }

  BaseString(typename StdStringType::const_pointer aLiteral)
    : mString(aLiteral)
  {
  }

  typename StdStringType::const_pointer get() const {
    return mString.c_str();
  }

  const StdStringType& AsStdStr() const {
    return mString;
  }

  size_t Length() const {
    return mString.size();
  }

protected:
  void Assign(const StdStringType& aString) {
    mString = aString;
  }
  StdStringType mString;
};


class nsACString;

class nsAString : public BaseString<std::u16string> {
public:
  nsAString(std::u16string&& aStdString)
    : BaseString<std::u16string>(move(aStdString))
  {
  }
  nsAString(const char16_t* aLiteral)
    : BaseString<std::u16string>(aLiteral)
  {
  }

  nsAString()
    : BaseString<std::u16string>(u"")
  {
  }

  nsAString& operator=(const nsAString& aOther) {
    Assign(aOther.mString);
    return *this;
  }

protected:
  void Assign(const std::u16string& aStdString) {
    mString = aStdString;
  }

  // std::u16string mString;
};

class nsString : public nsAString {
public:
  nsString(std::u16string&& aStdString)
   : nsAString(move(aStdString))
  {
  }

  nsString(const char16_t* aLiteral)
    : nsAString(aLiteral)
  {
  }

  nsString()
  {
  }

  nsString(const nsAString& aOther)
    : nsAString(aOther)
  {
  }

  nsString(const nsString& aOther)
    : nsAString(aOther)
  {
  }

  nsString(nsString&& aOther)
    : nsAString(std::move(aOther))
  {
  }

  nsString& operator=(const nsString& aOther) {
    Assign(aOther.mString);
    return *this;
  }

private:
};


class nsACString : public BaseString<std::string> {
public:
  nsACString(std::string&& aStdString)
    : BaseString<std::string>(move(aStdString))
  {
  }
  nsACString(const char* aLiteral)
    : BaseString<std::string>(aLiteral)
  {
  }

  nsACString()
    : BaseString<std::string>("")
  {
  }

  nsACString(const nsACString& aOther)
    : BaseString<std::string>(aOther.AsStdStr())
  {
  }
};

class nsCString : public nsACString {
public:
  nsCString(std::string&& aStdString)
    : nsACString(move(aStdString))
  {
  }
  nsCString(const char* aLiteral)
    : nsACString(aLiteral)
  {
  }

  nsCString() {
  }

  nsCString(const nsACString &aOther)
    : nsACString(aOther)
  {
  }

  nsCString(const nsCString &aOther)
    : nsACString(aOther)
  {
  }
};

class NS_ConvertUTF16toUTF8 : public nsACString
{
public:
  NS_ConvertUTF16toUTF8(const nsAString& aUTF16)
    : nsACString(std::wstring_convert<
      std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(aUTF16.AsStdStr()))
  {
  }
  NS_ConvertUTF16toUTF8(nsString aUTF16)
    : nsACString(std::wstring_convert<
      std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(aUTF16.AsStdStr()))
  {
  }
};

class NS_ConvertUTF8toUTF16 : public nsAString
{
public:
  NS_ConvertUTF8toUTF16(const nsACString& aUTF8)
    : nsAString(std::wstring_convert<
      std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(aUTF8.AsStdStr()))
  {
  }
};

#define NS_LITERAL_STRING(s) static_cast<const nsString&>(nsString(u"" s))
#define NS_LITERAL_CSTRING(s) static_cast<const nsCString&>(nsCString(s))

inline nsString EmptyString() {
  return NS_LITERAL_STRING("");
}

inline nsCString EmptyCString() {
  return NS_LITERAL_CSTRING("");
}


#endif // gecko_media_nsString_h
