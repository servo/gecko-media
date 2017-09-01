#ifndef gecko_media_nsDataHashTable_h
#define gecko_media_nsDataHashTable_h

#include "nsString.h" // for nsCString, used as nsCStringHashKey.
#include <unordered_map>

typedef nsCString nsCStringHashKey;

template<class Key, class Value>
class nsDataHashtable {
public:

    void Put(Key aKey, Value aValue) {
        mMap.insert(std::make_pair(aKey, aValue));
    }

    //
/*
  nsAutoPtr<MetadataTags> tags(aTags);
  for (auto iter = aTags->Iter(); !iter.Done(); iter.Next()) {
    aInfo->mTags.AppendElement(MetadataTag(iter.Key(), iter.Data()));
  }
*/

private:
    std::unordered_map<Key, Value> mMap;
};

#endif // gecko_media_nsDataHashTable_h
