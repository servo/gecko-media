
namespace CrashReporter {

void SetCurrentThreadName(const char* aName)
{
#ifndef GECKO_MEDIA_CRATE
  if (PR_GetThreadPrivate(sTLSThreadInfoKey)) {
    // Explicitly set TLS value to null (and call the dtor function ) before
    // acquiring sMutex to avoid reentrant deadlock.
    PR_SetThreadPrivate(sTLSThreadInfoKey, nullptr);
  }

  StaticMutexAutoLock lock(sMutex);

  if (!sInitialized) {
    return;
  }

  ThreadAnnotationSpan* threadInfo =
    sThreadAnnotations->AddThreadAnnotation(CurrentThreadId(),
                                            aName);
  // This may destroy the old insatnce.
  PR_SetThreadPrivate(sTLSThreadInfoKey, threadInfo);
#endif
}

} // CrashReporter namespace.
