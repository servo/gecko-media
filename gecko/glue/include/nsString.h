#ifndef gecko_media_nsString_h
#define gecko_media_nsString_h

#include <string>

class nsACString;

class nsAString {
public:
  nsAString(std::u16string&& aStdString)
    : mString(move(aStdString))
  {
  }
  nsAString(const char16_t* aLiteral)
    : mString(aLiteral)
  {
  }

  nsAString() {
  }

  nsAString(const nsAString& aOther) {
    mString = aOther.mString;
  }

  const std::u16string& AsStdStr() const {
    return mString;
  }

private:
  std::u16string mString;
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

  nsString() {
  }

  nsString(const nsAString& aOther)
    : nsAString(aOther)
  {
  }

  nsString(const nsString& aOther)
    : nsAString(aOther)
  {
  }
private:
};


class nsACString {
public:
  nsACString(std::string&& aStdString)
    : mString(move(aStdString))
  {
  }
  nsACString(const char* aLiteral)
    : mString(aLiteral)
  {
  }

  nsACString() {
  }

  nsACString(const nsACString& aOther) {
    mString = aOther.mString;
  }

  const std::string& AsStdStr() const {
    return mString;
  }

private:
  std::string mString;
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
  NS_ConvertUTF16toUTF8(const nsAString& aUTF16);
};

class NS_ConvertUTF8toUTF16 : public nsAString
{
public:
  NS_ConvertUTF8toUTF16(const nsACString& aUTF8);
};

inline nsString NS_LITERAL_STRING(const char* aLiteral) {
  return NS_ConvertUTF8toUTF16(aLiteral);
}

inline nsString NS_LITERAL_CSTRING(const char* aLiteral) {
  return NS_ConvertUTF8toUTF16(aLiteral);
}

inline nsString EmptyString() {
  return NS_LITERAL_STRING("");
}

#endif // gecko_media_nsString_h
