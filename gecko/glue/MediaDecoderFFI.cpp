#include "MediaDecoderFFI.h"
#include "RustFunctions.h"
#include "mozilla/Assertions.h"
#include "mozilla/Range.h"
#include "nsThreadUtils.h"
#include <unordered_map>

static std::unordered_map<size_t, mozilla::Range<const uint8_t>> sBlobs;

void
MediaDecoder_Load(size_t aId,
                  const uint8_t* aData,
                  size_t aDataLength,
                  const char* aMimeType)
{
  MOZ_ASSERT(NS_IsMainThread());
  // TODO: Create decoder for aData.
  // TODO: Ensure decoder owns aData, rather than storing it in sBlobs.
  sBlobs.insert({ aId, mozilla::Range<const uint8_t>(aData, aDataLength) });
}

void
MediaDecoder_Unload(size_t aId)
{
  MOZ_ASSERT(NS_IsMainThread());
  // TODO: GetDecoder(aId)->Shutdown();
  // TODO: Release reference to data in MediaDecoder destructor.
  auto itr = sBlobs.find(aId);
  if (itr != sBlobs.end()) {
    mozilla::Range<const uint8_t> r = itr->second;
    sBlobs.erase(itr);
    FreeRustVecU8(r.begin().get(), r.length());
  }
}
