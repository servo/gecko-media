#include "RustServices.h"
#include "mozilla/Assertions.h"

static RustServicesFnTable sRustServices = {0};

/* static */
void
RustServices::Init(const RustServicesFnTable& aServices)
{
  sRustServices = aServices;
}

/* static */
uint64_t
RustServices::TimeNow()
{
  MOZ_ASSERT(sRustServices.mGetTimeNowFn);
  return (*sRustServices.mGetTimeNowFn)();
}
