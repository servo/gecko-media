#include "MediaDecoderFFI.h"
#include <unordered_map>

#include "mozilla/Assertions.h"
#include "mozilla/Range.h"
#include "nsThreadUtils.h"

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

extern "C" void
free_rust_vec_u8(const uint8_t* aVec, size_t aLength);

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
    free_rust_vec_u8(r.begin().get(), r.length());
  }
}
