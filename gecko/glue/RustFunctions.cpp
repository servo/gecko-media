#include "RustFunctions.h"
#include "mozilla/Assertions.h"

static const RustFunctions* sRustFunctions = nullptr;

void
InitializeRustFunctions(const RustFunctions* aFunctions)
{
  MOZ_ASSERT(!sRustFunctions);
  sRustFunctions = aFunctions;
}

void
CallGeckoProcessEvents(rust_msg_sender_t* aSender)
{
  MOZ_ASSERT(sRustFunctions);
  sRustFunctions->CallGeckoProcessEvents(aSender);
}

void
FreeProcessEventsSender(rust_msg_sender_t* aSender)
{
  MOZ_ASSERT(sRustFunctions);
  sRustFunctions->FreeProcessEventsSender(aSender);
}

void
FreeRustVecU8(const uint8_t* aVec, size_t aLength)
{
  MOZ_ASSERT(sRustFunctions);
  sRustFunctions->FreeRustVecU8(aVec, aLength);
}
