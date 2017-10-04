import os
import re
import subprocess
import sys
from pathlib import Path
from shutil import copyfile
from shutil import rmtree
from os import listdir

if sys.version_info[0] < 3:
    raise Exception("Python 3 or a more recent version is required.")

# Imports Gecko code for building in Servo.

header_files = [
    ("dom/media/AudioConverter.h", "AudioConverter.h"),
    ("dom/media/AudioDeviceInfo.h", "mozilla/dom/AudioDeviceInfo.h"),
    ("dom/media/AudioSampleFormat.h", "AudioSampleFormat.h"),
    ("dom/media/AudioStream.h", "AudioStream.h"),
    ("dom/media/CubebUtils.h", "CubebUtils.h"),
    ("dom/media/Intervals.h", "Intervals.h"),
    ("dom/media/MediaData.h", "MediaData.h"),
    ("dom/media/MediaInfo.h", "MediaInfo.h"),
    ("dom/media/SharedBuffer.h", "SharedBuffer.h"),
    ("dom/media/StreamTracks.h", "StreamTracks.h"),
    ("dom/media/TimeUnits.h", "TimeUnits.h"),
    ("dom/media/TrackID.h", "TrackID.h"),
    ("dom/media/VideoLimits.h", "VideoLimits.h"),
    ("dom/media/VideoUtils.h", "VideoUtils.h"),
    ("gfx/2d/2D.h", "mozilla/gfx/2D.h"),
    ("gfx/2d/BaseCoord.h", "mozilla/gfx/BaseCoord.h"),
    ("gfx/2d/BaseMargin.h", "mozilla/gfx/BaseMargin.h"),
    ("gfx/2d/BasePoint.h", "mozilla/gfx/BasePoint.h"),
    ("gfx/2d/BasePoint3D.h", "mozilla/gfx/BasePoint3D.h"),
    ("gfx/2d/BasePoint4D.h", "mozilla/gfx/BasePoint4D.h"),
    ("gfx/2d/BaseRect.h", "mozilla/gfx/BaseRect.h"),
    ("gfx/2d/BaseSize.h", "mozilla/gfx/BaseSize.h"),
    ("gfx/2d/Coord.h", "mozilla/gfx/Coord.h"),
    ("gfx/2d/FontVariation.h", "mozilla/gfx/FontVariation.h"),
    ("gfx/2d/GenericRefCounted.h", "mozilla/GenericRefCounted.h"),
    ("gfx/2d/LoggingConstants.h", "mozilla/gfx/LoggingConstants.h"),
    ("gfx/2d/Matrix.h", "mozilla/gfx/Matrix.h"),
    ("gfx/2d/MatrixFwd.h", "mozilla/gfx/MatrixFwd.h"),
    ("gfx/2d/NumericTools.h", "mozilla/gfx/NumericTools.h"),
    ("gfx/2d/Point.h", "mozilla/gfx/Point.h"),
    ("gfx/2d/Quaternion.h", "mozilla/gfx/Quaternion.h"),
    ("gfx/2d/Rect.h", "mozilla/gfx/Rect.h"),
    ("gfx/2d/Tools.h", "mozilla/gfx/Tools.h"),
    ("gfx/2d/Triangle.h", "mozilla/gfx/Triangle.h"),
    ("gfx/2d/Types.h", "mozilla/gfx/Types.h"),
    ("gfx/2d/UserData.h", "mozilla/gfx/UserData.h"),
    ("gfx/layers/ImageTypes.h", "ImageTypes.h"),
    ("gfx/thebes/gfxPrefs.h", "gfxPrefs.h"),
    ("gfx/thebes/gfxTypes.h", "gfxTypes.h"),
    ("gfx/ycbcr/YCbCrUtils.h", "YCbCrUtils.h"),
    ("intl/unicharutil/util/nsUnicharUtils.h", "nsUnicharUtils.h"),
    ("intl/strres/nsStringBundleService.h", "nsStringBundleService.h"),
    ("layout/style/ServoUtils.h", "mozilla/ServoUtils.h"),
    ("media/libcubeb/include/cubeb-stdint.h", "mozilla/media/libcubeb/include/cubeb-stdint.h"),
    ("media/libcubeb/include/cubeb.h", "mozilla/media/libcubeb/include/cubeb.h"),
    ("media/libcubeb/include/cubeb_export.h", "mozilla/media/libcubeb/include/cubeb_export.h"),
    ("media/libcubeb/src/android/audiotrack_definitions.h", "mozilla/media/libcubeb/src/android/audiotrack_definitions.h"),
    ("media/libcubeb/src/android/sles_definitions.h", "mozilla/media/libcubeb/src/android/sles_definitions.h"),
    ("media/libcubeb/src/audiotrack_definitions.h", "mozilla/media/libcubeb/src/audiotrack_definitions.h"),
    ("media/libcubeb/src/cubeb-internal.h", "mozilla/media/libcubeb/src/cubeb-internal.h"),
    ("media/libcubeb/src/cubeb-sles.h", "mozilla/media/libcubeb/src/cubeb-sles.h"),
    ("media/libcubeb/src/cubeb-speex-resampler.h", "mozilla/media/libcubeb/src/cubeb-speex-resampler.h"),
    ("media/libcubeb/src/cubeb_array_queue.h", "mozilla/media/libcubeb/src/cubeb_array_queue.h"),
    ("media/libcubeb/src/cubeb_assert.h", "mozilla/media/libcubeb/src/cubeb_assert.h"),
    ("media/libcubeb/src/cubeb_log.h", "mozilla/media/libcubeb/src/cubeb_log.h"),
    ("media/libcubeb/src/cubeb_mixer.h", "mozilla/media/libcubeb/src/cubeb_mixer.h"),
    ("media/libcubeb/src/cubeb_osx_run_loop.h", "mozilla/media/libcubeb/src/cubeb_osx_run_loop.h"),
    ("media/libcubeb/src/cubeb_panner.h", "mozilla/media/libcubeb/src/cubeb_panner.h"),
    ("media/libcubeb/src/cubeb_resampler.h", "mozilla/media/libcubeb/src/cubeb_resampler.h"),
    ("media/libcubeb/src/cubeb_resampler_internal.h", "mozilla/media/libcubeb/src/cubeb_resampler_internal.h"),
    ("media/libcubeb/src/cubeb_ring_array.h", "mozilla/media/libcubeb/src/cubeb_ring_array.h"),
    ("media/libcubeb/src/cubeb_ringbuffer.h", "mozilla/media/libcubeb/src/cubeb_ringbuffer.h"),
    ("media/libcubeb/src/cubeb_strings.h", "mozilla/media/libcubeb/src/cubeb_strings.h"),
    ("media/libcubeb/src/cubeb_utils.h", "mozilla/media/libcubeb/src/cubeb_utils.h"),
    ("media/libcubeb/src/cubeb_utils_unix.h", "mozilla/media/libcubeb/src/cubeb_utils_unix.h"),
    ("media/libcubeb/src/cubeb_utils_win.h", "mozilla/media/libcubeb/src/cubeb_utils_win.h"),
    ("memory/build/malloc_decls.h", "malloc_decls.h"),
    ("memory/build/mozjemalloc_types.h", "mozjemalloc_types.h"),
    ("memory/build/mozmemory_wrap.h", "mozmemory_wrap.h"),
    ("memory/fallible/fallible.h", "mozilla/fallible.h"),
    ("memory/mozalloc/mozalloc.h", "mozilla/mozalloc.h"),
    ("memory/mozalloc/mozalloc_abort.h", "mozilla/mozalloc_abort.h"),
    ("memory/mozalloc/mozalloc_oom.h", "mozilla/mozalloc_oom.h"),
    ("memory/mozalloc/throw_gcc.h", "mozilla/throw_gcc.h"),
    ("mfbt/Alignment.h", "mozilla/Alignment.h"),
    ("mfbt/AllocPolicy.h", "mozilla/AllocPolicy.h"),
    ("mfbt/AlreadyAddRefed.h", "mozilla/AlreadyAddRefed.h"),
    ("mfbt/Array.h", "mozilla/Array.h"),
    ("mfbt/ArrayUtils.h", "mozilla/ArrayUtils.h"),
    ("mfbt/Atomics.h", "mozilla/Atomics.h"),
    ("mfbt/Attributes.h", "mozilla/Attributes.h"),
    ("mfbt/BinarySearch.h", "mozilla/BinarySearch.h"),
    ("mfbt/BufferList.h", "mozilla/BufferList.h"),
    ("mfbt/Casting.h", "mozilla/Casting.h"),
    ("mfbt/Char16.h", "mozilla/Char16.h"),
    ("mfbt/ChaosMode.h", "mozilla/ChaosMode.h"),
    ("mfbt/CheckedInt.h", "mozilla/CheckedInt.h"),
    ("mfbt/Compiler.h", "mozilla/Compiler.h"),
    ("mfbt/DebugOnly.h", "mozilla/DebugOnly.h"),
    ("mfbt/double-conversion/source/double-conversion.h", "mozilla/double-conversion.h"),
    ("mfbt/double-conversion/source/utils.h", "mozilla/double-conversion/utils.h"),
    ("mfbt/EndianUtils.h", "mozilla/EndianUtils.h"),
    ("mfbt/EnumeratedArray.h", "mozilla/EnumeratedArray.h"),
    ("mfbt/EnumSet.h", "mozilla/EnumSet.h"),
    ("mfbt/FloatingPoint.h", "mozilla/FloatingPoint.h"),
    ("mfbt/GuardObjects.h", "mozilla/GuardObjects.h"),
    ("mfbt/HashFunctions.h", "mozilla/HashFunctions.h"),
    ("mfbt/IndexSequence.h", "mozilla/IndexSequence.h"),
    ("mfbt/IndexSequence.h", "mozilla/IndexSequence.h"),
    ("mfbt/IntegerPrintfMacros.h", "mozilla/IntegerPrintfMacros.h"),
    ("mfbt/IntegerPrintfMacros.h", "mozilla/IntegerPrintfMacros.h"),
    ("mfbt/IntegerTypeTraits.h", "mozilla/IntegerTypeTraits.h"),
    ("mfbt/IntegerTypeTraits.h", "mozilla/IntegerTypeTraits.h"),
    ("mfbt/Likely.h", "mozilla/Likely.h"),
    ("mfbt/LinkedList.h", "mozilla/LinkedList.h"),
    ("mfbt/MacroArgs.h", "mozilla/MacroArgs.h"),
    ("mfbt/MacroForEach.h", "mozilla/MacroForEach.h"),
    ("mfbt/MathAlgorithms.h", "mozilla/MathAlgorithms.h"),
    ("mfbt/Maybe.h", "mozilla/Maybe.h"),
    ("mfbt/MemoryChecking.h", "mozilla/MemoryChecking.h"),
    ("mfbt/MemoryReporting.h", "mozilla/MemoryReporting.h"),
    ("mfbt/Move.h", "mozilla/Move.h"),
    ("mfbt/NotNull.h", "mozilla/NotNull.h"),
    ("mfbt/Opaque.h", "mozilla/Opaque.h"),
    ("mfbt/OperatorNewExtensions.h", "mozilla/OperatorNewExtensions.h"),
    ("mfbt/Pair.h", "mozilla/Pair.h"),
    ("mfbt/PodOperations.h", "mozilla/PodOperations.h"),
    ("mfbt/Range.h", "mozilla/Range.h"),
    ("mfbt/RangedPtr.h", "mozilla/RangedPtr.h"),
    ("mfbt/ReentrancyGuard.h", "mozilla/ReentrancyGuard.h"),
    ("mfbt/RefCounted.h", "mozilla/RefCounted.h"),
    ("mfbt/RefCountType.h", "mozilla/RefCountType.h"),
    ("mfbt/RefPtr.h", "mozilla/RefPtr.h"),
    ("mfbt/Result.h", "mozilla/Result.h"),
    ("mfbt/ReverseIterator.h", "mozilla/ReverseIterator.h"),
    ("mfbt/Scoped.h", "mozilla/Scoped.h"),
    ("mfbt/ScopeExit.h", "mozilla/ScopeExit.h"),
    ("mfbt/Span.h", "mozilla/Span.h"),
    ("mfbt/Sprintf.h", "mozilla/Sprintf.h"),
    ("mfbt/StaticAnalysisFunctions.h", "mozilla/StaticAnalysisFunctions.h"),
    ("mfbt/TaggedAnonymousMemory.h", "mozilla/TaggedAnonymousMemory.h"),
    ("mfbt/TemplateLib.h", "mozilla/TemplateLib.h"),
    ("mfbt/ThreadLocal.h", "mozilla/ThreadLocal.h"),
    ("mfbt/Tuple.h", "mozilla/Tuple.h"),
    ("mfbt/TypedEnumBits.h", "mozilla/TypedEnumBits.h"),
    ("mfbt/Types.h", "mozilla/Types.h"),
    ("mfbt/TypeTraits.h", "mozilla/TypeTraits.h"),
    ("mfbt/UniquePtr.h", "mozilla/UniquePtr.h"),
    ("mfbt/UniquePtrExtensions.h", "mozilla/UniquePtrExtensions.h"),
    ("mfbt/Unused.h", "mozilla/Unused.h"),
    ("mfbt/Variant.h", "mozilla/Variant.h"),
    ("mfbt/Vector.h", "mozilla/Vector.h"),
    ("mfbt/WeakPtr.h", "mozilla/WeakPtr.h"),
    ("mozglue/build/arm.h", "mozilla/arm.h"),
    ("mozglue/build/SSE.h", "mozilla/SSE.h"),
    ("mozglue/misc/MutexPlatformData_posix.h", "MutexPlatformData_posix.h"),
    ("mozglue/misc/PlatformConditionVariable.h", "mozilla/PlatformConditionVariable.h"),
    ("mozglue/misc/PlatformMutex.h", "mozilla/PlatformMutex.h"),
    ("mozglue/misc/Printf.h", "mozilla/Printf.h"),
    ("mozglue/misc/TimeStamp_windows.h", "mozilla/TimeStamp_windows.h"),
    ("mozglue/misc/TimeStamp.h", "mozilla/TimeStamp.h"),
    ("nsprpub/pr/include/nspr.h", "nspr.h"),
    ("nsprpub/pr/include/obsolete/probslet.h", "nspr/obsolete/probslet.h"),
    ("nsprpub/pr/include/obsolete/protypes.h", "nspr/obsolete/protypes.h"),
    ("nsprpub/pr/include/obsolete/prsem.h", "nspr/obsolete/prsem.h"),
    ("nsprpub/pr/include/pratom.h", "pratom.h"),
    ("nsprpub/pr/include/prbit.h", "prbit.h"),
    ("nsprpub/pr/include/prclist.h", "prclist.h"),
    ("nsprpub/pr/include/prcmon.h", "prcmon.h"),
    ("nsprpub/pr/include/prcvar.h", "prcvar.h"),
    ("nsprpub/pr/include/prdtoa.h", "prdtoa.h"),
    ("nsprpub/pr/include/prerr.h", "prerr.h"),
    ("nsprpub/pr/include/prerror.h", "prerror.h"),
    ("nsprpub/pr/include/pripcsem.h", "pripcsem.h"),
    ("nsprpub/pr/include/private/pprio.h", "nspr/private/pprio.h"),
    ("nsprpub/pr/include/private/pprmwait.h", "nspr/private/pprmwait.h"),
    ("nsprpub/pr/include/private/pprthred.h", "nspr/private/pprthred.h"),
    ("nsprpub/pr/include/private/primpl.h", "nspr/private/primpl.h"),
    ("nsprpub/pr/include/private/prpriv.h", "nspr/private/prpriv.h"),
    ("nsprpub/pr/include/prpdce.h", "prpdce.h"),
    ('media/libsoundtouch/src/AAFilter.h', 'mozilla/media/libsoundtouch/src/AAFilter.h'),
    ('media/libsoundtouch/src/cpu_detect.h', 'mozilla/media/libsoundtouch/src/cpu_detect.h'),
    ('media/libsoundtouch/src/FIFOSampleBuffer.h', 'mozilla/media/libsoundtouch/src/FIFOSampleBuffer.h'),
    ('media/libsoundtouch/src/FIFOSamplePipe.h', 'mozilla/media/libsoundtouch/src/FIFOSamplePipe.h'),
    ('media/libsoundtouch/src/FIRFilter.h', 'mozilla/media/libsoundtouch/src/FIRFilter.h'),
    ('media/libsoundtouch/src/InterpolateCubic.h', 'mozilla/media/libsoundtouch/src/InterpolateCubic.h'),
    ('media/libsoundtouch/src/InterpolateLinear.h', 'mozilla/media/libsoundtouch/src/InterpolateLinear.h'),
    ('media/libsoundtouch/src/InterpolateShannon.h', 'mozilla/media/libsoundtouch/src/InterpolateShannon.h'),
    ('media/libsoundtouch/src/RateTransposer.h', 'mozilla/media/libsoundtouch/src/RateTransposer.h'),
    ('media/libsoundtouch/src/SoundTouch.h', 'mozilla/media/libsoundtouch/src/SoundTouch.h'),
    ('media/libsoundtouch/src/soundtouch_config.h', 'mozilla/media/libsoundtouch/src/soundtouch_config.h'),
    ('media/libsoundtouch/src/soundtouch_perms.h', 'mozilla/media/libsoundtouch/src/soundtouch_perms.h'),
    ('media/libsoundtouch/src/SoundTouchFactory.h', 'mozilla/media/libsoundtouch/src/SoundTouchFactory.h'),
    ('media/libsoundtouch/src/STTypes.h', 'mozilla/media/libsoundtouch/src/STTypes.h'),
    ('media/libsoundtouch/src/TDStretch.h', 'mozilla/media/libsoundtouch/src/TDStretch.h'),
    ('media/libspeex_resampler/src/arch.h', 'mozilla/media/libspeex_resampler/src/arch.h'),
    ('media/libspeex_resampler/src/fixed_generic.h', 'mozilla/media/libspeex_resampler/src/fixed_generic.h'),
    ('media/libspeex_resampler/src/simd_detect.h', 'mozilla/media/libspeex_resampler/src/simd_detect.h'),
    ('media/libspeex_resampler/src/speex_resampler.h', 'mozilla/media/libspeex_resampler/src/speex_resampler.h'),
    ('media/libspeex_resampler/src/stack_alloc.h', 'mozilla/media/libspeex_resampler/src/stack_alloc.h'),
    ("nsprpub/pr/include/prlink.h", "prlink.h"),
    ("nsprpub/pr/include/prlock.h", "prlock.h"),
    ("nsprpub/pr/include/prlog.h", "prlog.h"),
    ("nsprpub/pr/include/prlong.h", "prlong.h"),
    ("nsprpub/pr/include/prmem.h", "prmem.h"),
    ("nsprpub/pr/include/prmon.h", "prmon.h"),
    ("nsprpub/pr/include/prmwait.h", "prmwait.h"),
    ("nsprpub/pr/include/prnetdb.h", "prnetdb.h"),
    ("nsprpub/pr/include/prproces.h", "prproces.h"),
    ("nsprpub/pr/include/prrng.h", "prrng.h"),
    ("nsprpub/pr/include/prrwlock.h", "prrwlock.h"),
    ("nsprpub/pr/include/prshm.h", "prshm.h"),
    ("nsprpub/pr/include/prshma.h", "prshma.h"),
    ("nsprpub/pr/include/prsystem.h", "prsystem.h"),
    ("nsprpub/pr/include/prtime.h", "prtime.h"),
    ("nsprpub/pr/include/prtpool.h", "prtpool.h"),
    ("nsprpub/pr/include/prtrace.h", "prtrace.h"),
    ("nsprpub/pr/include/prtypes.h", "prtypes.h"),
    ("nsprpub/lib/libc/include/plstr.h", "plstr.h"),
    # TODO: import more cfg files as needed.
    ("nsprpub/pr/include/md/_linux.cfg", "md/_linux.cfg"),
    ("nsprpub/pr/include/md/_linux.h", "md/_linux.h"),
    ("nsprpub/pr/include/md/_pth.h", "md/_pth.h"),
    ("nsprpub/pr/include/md/_unix_errors.h", "md/_unix_errors.h"),
    ("nsprpub/pr/include/md/_unixos.h", "md/_unixos.h"),
    ("nsprpub/pr/include/md/prosdep.h", "md/prosdep.h"),
    ("toolkit/components/startup/StartupTimeline.h", "mozilla/StartupTimeline.h"),
    ("toolkit/components/telemetry/Telemetry.h", "mozilla/Telemetry.h"),
    ("toolkit/crashreporter/nsExceptionHandler.h", "nsExceptionHandler.h"),
    ("toolkit/crashreporter/ThreadAnnotation.h", "ThreadAnnotation.h"),
    ("xpcom/base/ClearOnShutdown.h", "mozilla/ClearOnShutdown.h"),
    ("xpcom/base/DeferredFinalize.h", "mozilla/DeferredFinalize.h"),
    ("xpcom/base/Logging.h", "mozilla/Logging.h"),
    ("xpcom/base/nsAlgorithm.h", "nsAlgorithm.h"),
    ("xpcom/base/nsAutoRef.h", "nsAutoRef.h"),
    ("xpcom/base/nscore.h", "nscore.h"),
    ("xpcom/base/nsCRTGlue.h", "nsCRTGlue.h"),
    ("xpcom/base/nsCycleCollectionParticipant.h", "nsCycleCollectionParticipant.h"),
    ("xpcom/base/nsCycleCollectionTraversalCallback.h", "nsCycleCollectionTraversalCallback.h"),
    ("xpcom/base/nsDebug.h", "nsDebug.h"),
    ("xpcom/base/nsError.h", "nsError.h"),
    ("xpcom/base/nsID.h", "nsID.h"),
    ("xpcom/base/nsIClassInfoImpl.h", "nsIClassInfoImpl.h"),
    ("xpcom/base/nsIInterfaceRequestorUtils.h", "nsIInterfaceRequestorUtils.h"),
    ("xpcom/base/nsISupportsBase.h", "nsISupportsBase.h"),
    ("xpcom/base/nsISupportsImpl.h", "nsISupportsImpl.h"),
    ("xpcom/base/nsISupportsUtils.h", "nsISupportsUtils.h"),
    ("xpcom/base/nsIWeakReferenceUtils.h", "nsIWeakReferenceUtils.h"),
    ("xpcom/base/nsMemory.h", "nsMemory.h"),
    ("xpcom/base/NSPRLogModulesParser.h", "NSPRLogModulesParser.h"),
    ("xpcom/base/nsQueryObject.h", "nsQueryObject.h"),
    ("xpcom/base/nsTraceRefcnt.h", "nsTraceRefcnt.h"),
    ("xpcom/base/nsWeakReference.h", "nsWeakReference.h"),
    ("xpcom/base/StaticMutex.h", "mozilla/StaticMutex.h"),
    ("xpcom/base/StaticPtr.h", "mozilla/StaticPtr.h"),
    ("xpcom/build/IOInterposer.h", "mozilla/IOInterposer.h"),
    ("xpcom/build/nsXPCOM.h", "nsXPCOM.h"),
    ("xpcom/build/nsXPCOMCID.h", "nsXPCOMCID.h"),
    ("xpcom/build/nsXPCOMCIDInternal.h", "nsXPCOMCIDInternal.h"),
    ("xpcom/build/nsXPCOMPrivate.h", "nsXPCOMPrivate.h"),
    ("xpcom/build/Services.h", "mozilla/Services.h"),
    ("xpcom/build/XREAppData.h", "mozilla/XREAppData.h"),
    ("xpcom/build/XREChildData.h", "XREChildData.h"),
    ("xpcom/build/xrecore.h", "xrecore.h"),
    ("xpcom/build/XREShellData.h", "XREShellData.h"),
    ("xpcom/components/GenericFactory.h", "mozilla/GenericFactory.h"),
    ("xpcom/components/Module.h", "mozilla/Module.h"),
    ("xpcom/components/ModuleUtils.h", "mozilla/ModuleUtils.h"),
    ("xpcom/components/nsCategoryManager.h", "nsCategoryManager.h"),
    ("xpcom/components/nsCategoryManagerUtils.h", "nsCategoryManagerUtils.h"),
    ("xpcom/components/nsComponentManagerUtils.h", "nsComponentManagerUtils.h"),
    ("xpcom/components/nsServiceManagerUtils.h", "nsServiceManagerUtils.h"),
    ("xpcom/ds/ArenaAllocator.h", "mozilla/ArenaAllocator.h"),
    ("xpcom/ds/ArenaAllocatorExtensions.h", "mozilla/ArenaAllocatorExtensions.h"),
    ("xpcom/ds/ArrayIterator.h", "mozilla/ArrayIterator.h"),
    ("xpcom/ds/nsArrayEnumerator.h", "nsArrayEnumerator.h"),
    ("xpcom/ds/nsBaseHashtable.h", "nsBaseHashtable.h"),
    ("xpcom/ds/nsClassHashtable.h", "nsClassHashtable.h"),
    ("xpcom/ds/nsCRT.h", "nsCRT.h"),
    ("xpcom/ds/nsDataHashtable.h", "nsDataHashtable.h"),
    ("xpcom/ds/nsEnumeratorUtils.h", "nsEnumeratorUtils.h"),
    ("xpcom/ds/nsHashKeys.h", "nsHashKeys.h"),
    ("xpcom/ds/nsInterfaceHashtable.h", "nsInterfaceHashtable.h"),
    ("xpcom/ds/nsObserverList.h", "nsObserverList.h"),
    ("xpcom/ds/nsObserverService.h", "nsObserverService.h"),
    ("xpcom/ds/nsPointerHashKeys.h", "nsPointerHashKeys.h"),
    ("xpcom/ds/nsQuickSort.h", "nsQuickSort.h"),
    ("xpcom/ds/nsRefPtrHashtable.h", "nsRefPtrHashtable.h"),
    ("xpcom/ds/nsStringEnumerator.h", "nsStringEnumerator.h"),
    ("xpcom/ds/nsSupportsPrimitives.h", "nsSupportsPrimitives.h"),
    ("xpcom/ds/nsTHashtable.h", "nsTHashtable.h"),
    ("xpcom/ds/PLDHashTable.h", "PLDHashTable.h"),
    ("xpcom/glue/FileUtils.h", "mozilla/FileUtils.h"),
    ("xpcom/glue/nsStringGlue.h", "nsStringGlue.h"),
    ("xpcom/io/nsAppDirectoryServiceDefs.h", "nsAppDirectoryServiceDefs.h"),
    ("xpcom/io/nsAppFileLocationProvider.h", "nsAppFileLocationProvider.h"),
    ("xpcom/io/nsDirectoryService.h", "nsDirectoryService.h"),
    ("xpcom/io/nsDirectoryServiceAtomList.h", "nsDirectoryServiceAtomList.h"),
    ("xpcom/io/nsDirectoryServiceDefs.h", "nsDirectoryServiceDefs.h"),
    ("xpcom/io/nsDirectoryServiceUtils.h", "nsDirectoryServiceUtils.h"),
    ("xpcom/io/nsLocalFile.h", "nsLocalFile.h"),
    ("xpcom/io/nsLocalFileUnix.h", "nsLocalFileUnix.h"),
    ("xpcom/io/SpecialSystemDirectory.h", "SpecialSystemDirectory.h"),
    ("xpcom/reflect/xptinfo/XPTInterfaceInfoManager.h", "mozilla/XPTInterfaceInfoManager.h"),
    ("xpcom/reflect/xptinfo/xptinfo.h", "xptinfo.h"),
    ("xpcom/string/nsASCIIMask.h", "nsASCIIMask.h"),
    ("xpcom/string/nsAString.h", "nsAString.h"),
    ("xpcom/string/nsCharTraits.h", "nsCharTraits.h"),
    ("xpcom/string/nsDependentString.h", "nsDependentString.h"),
    ("xpcom/string/nsDependentSubstring.h", "nsDependentSubstring.h"),
    ("xpcom/string/nsLiteralString.h", "nsLiteralString.h"),
    ("xpcom/string/nsPrintfCString.h", "nsPrintfCString.h"),
    ("xpcom/string/nsPromiseFlatString.h", "nsPromiseFlatString.h"),
    ("xpcom/string/nsReadableUtils.h", "nsReadableUtils.h"),
    ("xpcom/string/nsReadableUtilsImpl.h", "nsReadableUtilsImpl.h"),
    ("xpcom/string/nsString.h", "nsString.h"),
    ("xpcom/string/nsStringBuffer.h", "nsStringBuffer.h"),
    ("xpcom/string/nsStringFlags.h", "nsStringFlags.h"),
    ("xpcom/string/nsStringFwd.h", "nsStringFwd.h"),
    ("xpcom/string/nsStringIterator.h", "nsStringIterator.h"),
    ("xpcom/string/nsSubstring.h", "nsSubstring.h"),
    ("xpcom/string/nsSubstringTuple.h", "nsSubstringTuple.h"),
    ("xpcom/string/nsTDependentString.h", "nsTDependentString.h"),
    ("xpcom/string/nsTDependentSubstring.h", "nsTDependentSubstring.h"),
    ("xpcom/string/nsTLiteralString.h", "nsTLiteralString.h"),
    ("xpcom/string/nsTPromiseFlatString.h", "nsTPromiseFlatString.h"),
    ("xpcom/string/nsTString.h", "nsTString.h"),
    ("xpcom/string/nsTStringRepr.h", "nsTStringRepr.h"),
    ("xpcom/string/nsTSubstring.h", "nsTSubstring.h"),
    ("xpcom/string/nsTSubstringTuple.h", "nsTSubstringTuple.h"),
    ("xpcom/string/nsUTF8Utils.h", "nsUTF8Utils.h"),
    ("xpcom/threads/AbstractEventQueue.h", "mozilla/AbstractEventQueue.h"),
    ("xpcom/threads/AbstractThread.h", "mozilla/AbstractThread.h"),
    ("xpcom/threads/CondVar.h", "mozilla/CondVar.h"),
    ("xpcom/threads/CooperativeThreadPool.h", "mozilla/CooperativeThreadPool.h"),
    ("xpcom/threads/DeadlockDetector.h", "mozilla/DeadlockDetector.h"),
    ("xpcom/threads/EventQueue.h", "mozilla/EventQueue.h"),
    ("xpcom/threads/HangMonitor.h", "mozilla/HangMonitor.h"),
    ("xpcom/threads/InputEventStatistics.h", "InputEventStatistics.h"),
    ("xpcom/threads/LabeledEventQueue.h", "mozilla/LabeledEventQueue.h"),
    ("xpcom/threads/LeakRefPtr.h", "mozilla/LeakRefPtr.h"),
    ("xpcom/threads/MainThreadIdlePeriod.h", "MainThreadIdlePeriod.h"),
    ("xpcom/threads/MainThreadQueue.h", "MainThreadQueue.h"),
    ("xpcom/threads/MainThreadUtils.h", "MainThreadUtils.h"),
    ("xpcom/threads/Monitor.h", "mozilla/Monitor.h"),
    ("xpcom/threads/MozPromise.h", "mozilla/MozPromise.h"),
    ("xpcom/threads/Mutex.h", "mozilla/Mutex.h"),
    ("xpcom/threads/nsICancelableRunnable.h", "nsICancelableRunnable.h"),
    ("xpcom/threads/nsILabelableRunnable.h", "nsILabelableRunnable.h"),
    ("xpcom/threads/nsMemoryPressure.h", "nsMemoryPressure.h"),
    ("xpcom/threads/nsThreadManager.h", "nsThreadManager.h"),
    ("xpcom/threads/nsThreadPool.h", "nsThreadPool.h"),
    ("xpcom/threads/nsThreadUtils.h", "nsThreadUtils.h"),
    ("xpcom/threads/nsThreadSyncDispatch.h", "nsThreadSyncDispatch.h"),
    ("xpcom/threads/nsTimerImpl.h", "nsTimerImpl.h"),
    ("xpcom/threads/PrioritizedEventQueue.h", "PrioritizedEventQueue.h"),
    ("xpcom/threads/Queue.h", "mozilla/Queue.h"),
    ("xpcom/threads/ReentrantMonitor.h", "mozilla/ReentrantMonitor.h"),
    ("xpcom/threads/RecursiveMutex.h", "mozilla/RecursiveMutex.h"),
    ("xpcom/threads/Scheduler.h", "mozilla/Scheduler.h"),
    ("xpcom/threads/SchedulerGroup.h", "mozilla/SchedulerGroup.h"),
    ("xpcom/threads/SharedThreadPool.h", "mozilla/SharedThreadPool.h"),
    ("xpcom/threads/StateWatching.h", "mozilla/StateWatching.h"),
    ("xpcom/threads/SynchronizedEventQueue.h", "mozilla/SynchronizedEventQueue.h"),
    ("xpcom/threads/SystemGroup.h", "mozilla/SystemGroup.h"),
    ("xpcom/threads/TaskCategory.h", "mozilla/TaskCategory.h"),
    ("xpcom/threads/TaskDispatcher.h", "mozilla/TaskDispatcher.h"),
    ("xpcom/threads/TaskQueue.h", "mozilla/TaskQueue.h"),
    ("xpcom/threads/ThreadEventQueue.h", "mozilla/ThreadEventQueue.h"),
    ("xpcom/threads/ThreadEventTarget.h", "mozilla/ThreadEventTarget.h"),
    ("xpcom/threads/TimerThread.h", "TimerThread.h"),
    ("xpcom/threads/RWLock.h", "mozilla/RWLock.h"),
    ("xpcom/typelib/xpt/xpt_arena.h", "xpt_arena.h"),
    ("xpcom/typelib/xpt/xpt_struct.h", "xpt_struct.h"),
    ("xpcom/typelib/xpt/xpt_xdr.h", "xpt_xdr.h"),
]

src_files = [
    "dom/media/AudioStream.cpp",
    "dom/media/CubebUtils.cpp",
    "dom/media/MediaInfo.cpp",
    "media/libcubeb/src/cubeb.c",
    "media/libcubeb/src/cubeb_alsa.c",
    "media/libcubeb/src/cubeb_audiotrack.c",
    "media/libcubeb/src/cubeb_audiounit.cpp",
    "media/libcubeb/src/cubeb_jack.cpp",
    "media/libcubeb/src/cubeb_log.cpp",
    "media/libcubeb/src/cubeb_mixer.cpp",
    "media/libcubeb/src/cubeb_opensl.c",
    "media/libcubeb/src/cubeb_osx_run_loop.c",
    "media/libcubeb/src/cubeb_panner.cpp",
    "media/libcubeb/src/cubeb_pulse.c",
    "media/libcubeb/src/cubeb_resampler.cpp",
    "media/libcubeb/src/cubeb_sndio.c",
    "media/libcubeb/src/cubeb_strings.c",
    "media/libcubeb/src/cubeb_wasapi.cpp",
    "media/libcubeb/src/cubeb_winmm.c",
    "media/libsoundtouch/src/AAFilter.cpp",
    "media/libsoundtouch/src/cpu_detect_x86.cpp",
    "media/libsoundtouch/src/FIFOSampleBuffer.cpp",
    "media/libsoundtouch/src/FIRFilter.cpp",
    "media/libsoundtouch/src/InterpolateCubic.cpp",
    "media/libsoundtouch/src/InterpolateLinear.cpp",
    "media/libsoundtouch/src/InterpolateShannon.cpp",
    "media/libsoundtouch/src/mmx_optimized.cpp",
    "media/libsoundtouch/src/RateTransposer.cpp",
    "media/libsoundtouch/src/SoundTouch.cpp",
    "media/libsoundtouch/src/SoundTouchFactory.cpp",
    "media/libsoundtouch/src/sse_optimized.cpp",
    "media/libsoundtouch/src/TDStretch.cpp",
    "media/libspeex_resampler/src/resample.c",
    "media/libspeex_resampler/src/resample_neon.c",
    "media/libspeex_resampler/src/resample_sse.c",
    "media/libspeex_resampler/src/simd_detect.cpp",
    "memory/fallible/fallible.cpp",
    "memory/mozalloc/mozalloc.cpp",
    "memory/mozalloc/mozalloc_abort.cpp",
    "memory/mozalloc/mozalloc_oom.cpp",
    "mfbt/Assertions.cpp",
    "mfbt/ChaosMode.cpp",
    "mfbt/HashFunctions.cpp",
    "mfbt/Unused.cpp",
    "mozglue/misc/ConditionVariable_posix.cpp",
    "mozglue/misc/Mutex_posix.cpp",
    "mozglue/misc/Printf.h",
    "mozglue/misc/Printf.cpp",
    "mozglue/misc/TimeStamp_darwin.cpp",
    "mozglue/misc/TimeStamp_posix.cpp",
    "mozglue/misc/TimeStamp_windows.cpp",
    "mozglue/misc/TimeStamp.cpp",
    "nsprpub/pr/src/io/priometh.c",
    "nsprpub/pr/src/io/prfdcach.c",
    "nsprpub/pr/src/io/prlayer.c",
    "nsprpub/pr/src/io/prlog.c",
    "nsprpub/pr/src/io/prmapopt.c",
    "nsprpub/pr/src/io/prmwait.c",
    "nsprpub/pr/src/io/prprf.c",
    "nsprpub/pr/src/linking/prlink.c",
    "nsprpub/pr/src/malloc/prmem.c",
    "nsprpub/pr/src/md/prosdep.c",
    "nsprpub/pr/src/md/unix/linux.c",
    "nsprpub/pr/src/md/unix/unix.c",
    "nsprpub/pr/src/md/unix/unix_errors.c",
    "nsprpub/pr/src/memory/prseg.c",
    "nsprpub/pr/src/misc/pratom.c",
    "nsprpub/pr/src/misc/prdtoa.c",
    "nsprpub/pr/src/misc/prenv.c",
    "nsprpub/pr/src/misc/prerr.c",
    "nsprpub/pr/src/misc/prerror.c",
    "nsprpub/pr/src/misc/prerrortable.c",
    "nsprpub/pr/src/misc/prinit.c",
    "nsprpub/pr/src/misc/prinrval.c",
    "nsprpub/pr/src/misc/prtime.c",
    "nsprpub/pr/src/misc/prnetdb.c",
    "nsprpub/pr/src/pthreads/ptio.c",
    "nsprpub/pr/src/pthreads/ptmisc.c",
    "nsprpub/pr/src/pthreads/ptsynch.c",
    "nsprpub/pr/src/pthreads/ptthread.c",
    "nsprpub/pr/src/threads/prcmon.c",
    "nsprpub/pr/src/threads/prrwlock.c",
    "nsprpub/pr/src/threads/prtpd.c",
    "tools/profiler/core/platform.cpp",
    "toolkit/crashreporter/nsExceptionHandler.cpp",
    "toolkit/crashreporter/ThreadAnnotation.cpp",
    "toolkit/library/StaticXULComponentsStart.cpp",
    "toolkit/library/StaticXULComponentsEnd/StaticXULComponentsEnd.cpp",
    "xpcom/base/ClearOnShutdown.cpp",
    "xpcom/base/nsCOMPtr.cpp",
    "xpcom/base/nsClassInfoImpl.cpp",
    "xpcom/base/nsID.cpp",
    "xpcom/base/nsISupportsImpl.cpp",
    "xpcom/base/nsMemory.cpp",
    "xpcom/base/nsWeakReference.cpp",
    "xpcom/components/GenericFactory.cpp",
    "xpcom/ds/PLDHashTable.cpp",
    "xpcom/ds/nsCOMArray.cpp",
    "xpcom/ds/nsEnumeratorUtils.cpp",
    "xpcom/ds/nsQuickSort.cpp",
    "xpcom/ds/nsSupportsPrimitives.cpp",
    "xpcom/ds/nsTArray.cpp",
    "xpcom/ds/nsTObserverArray.cpp",
    "xpcom/io/nsDirectoryService.cpp",
    "xpcom/string/nsASCIIMask.cpp",
    "xpcom/string/nsDependentString.cpp",
    "xpcom/string/nsDependentSubstring.cpp",
    "xpcom/string/nsPromiseFlatString.cpp",
    "xpcom/string/nsReadableUtils.cpp",
    "xpcom/string/nsReadableUtilsSSE2.cpp",
    "xpcom/string/nsString.cpp",
    "xpcom/string/nsStringComparator.cpp",
    "xpcom/string/nsStringObsolete.cpp",
    "xpcom/string/nsSubstring.cpp",
    "xpcom/string/nsSubstringTuple.cpp",
    "xpcom/string/nsTDependentString.cpp",
    "xpcom/string/nsTDependentSubstring.cpp",
    "xpcom/string/nsTextFormatter.cpp",
    "xpcom/string/nsTPromiseFlatString.cpp",
    "xpcom/string/nsTString.cpp",
    "xpcom/string/nsTStringComparator.cpp",
    "xpcom/string/nsTStringObsolete.cpp",
    "xpcom/string/nsTSubstring.cpp",
    "xpcom/string/nsTSubstringTuple.cpp",
    "xpcom/string/nsUTF8UtilsSSE2.cpp",
    "xpcom/string/precompiled_templates.cpp",
    "xpcom/threads/EventQueue.cpp",
    "xpcom/threads/nsILabelableRunnable.cpp",
    "xpcom/threads/nsThreadManager.cpp",
    "xpcom/threads/nsThreadPool.cpp",
    "xpcom/threads/nsThreadUtils.cpp",
    "xpcom/threads/nsTimerImpl.cpp",
    "xpcom/threads/PrioritizedEventQueue.cpp",
    "xpcom/threads/SharedThreadPool.cpp",
    "xpcom/threads/SynchronizedEventQueue.cpp",
    "xpcom/threads/SystemGroup.cpp",
    "xpcom/threads/TaskQueue.cpp",
    "xpcom/threads/ThreadEventQueue.cpp",
    "xpcom/threads/ThreadEventTarget.cpp",
    "xpcom/threads/TimerThread.cpp",
]

objdir_files = [
    "cubeb/cubeb.h",
    "cubeb/cubeb_export.h",
    "ErrorList.h",
    "js-config.h",
    "mozilla/TelemetryHistogramEnums.h",
    "mozilla/TelemetryScalarEnums.h",
    "nsIArray.h",
    "nsIArrayExtensions.h",
    "nsIAtom.h",
    "nsIAudioDeviceInfo.h",
    "nsICategoryManager.h",
    "nsIClassInfo.h",
    "nsIComponentManager.h",
    "nsIComponentRegistrar.h",
    "nsIConsoleMessage.h",
    "nsIConsoleService.h",
    "nsIContentPolicy.h",
    "nsID.h",
    "nsIDirectoryService.h",
    "nsIErrorService.h",
    "nsIEventTarget.h",
    "nsIFactory.h",
    "nsIFile.h",
    "nsIHashable.h",
    "nsIID.h",
    "nsIIdlePeriod.h",
    "nsIIdleRunnable.h",
    "nsIInputStream.h",
    "nsIInterfaceInfoManager.h",
    "nsIInterfaceRequestor.h",
    "nsILoadGroup.h",
    "nsIMemory.h",
    "nsIMutableArray.h",
    "nsINamed.h",
    "nsINetUtil.h",
    "nsIObserver.h",
    "nsIObserverService.h",
    "nsIPersistentProperties2.h",
    "nsIPrefBranch.h",
    "nsIPrefService.h",
    "nsIProperties.h",
    "nsIRequest.h",
    "nsIRunnable.h",
    "nsISerialEventTarget.h",
    "nsIServiceManager.h",
    "nsISimpleEnumerator.h",
    "nsIStringBundle.h",
    "nsIStringBundleOverride.h",
    "nsIStringEnumerator.h",
    "nsISupports.h",
    "nsISupportsBase.h",
    "nsISupportsPrimitives.h",
    "nsISupportsPriority.h",
    "nsISupportsUtils.h",
    "nsIThread.h",
    "nsIThreadInternal.h",
    "nsIThreadManager.h",
    "nsIThreadPool.h",
    "nsITimer.h",
    "nsIWeakReference.h",
    "nspr/plhash.h",
    "nspr/prcpucfg.h",
    "nspr/prenv.h",
    "nspr/prinet.h",
    "nspr/prinit.h",
    "nspr/prinrval.h",
    "nspr/prio.h",
    "nspr/prprf.h",
    "nspr/prthread.h",
    "nspr/prwin16.h",
    "nsrootidl.h",
    "nsStaticAtom.h",
    "soundtouch/FIFOSamplePipe.h",
    "soundtouch/SoundTouch.h",
    "soundtouch/soundtouch_config.h",
    "soundtouch/SoundTouchFactory.h",
    "soundtouch/STTypes.h",
    "speex/speex_resampler.h",
]

objdir_config_files = [
    "system_wrappers/prcpucfg.h",
    "system_wrappers/prthread.h",
    "system_wrappers/prwin16.h",
    "system_wrappers/soundtouch/SoundTouchFactory.h",
]

def get_obj_dir_path(src_dir):
    if sys.platform == 'darwin':
        obj_dir = "obj-x86_64-apple-darwin16.7.0"
    elif sys.platform == 'linux':
        obj_dir = "obj-x86_64-pc-linux-gnu"
    else:
        raise Exception("Unsupported platform: {}".format(sys.platform))
    return os.path.join(src_dir, obj_dir)

def get_obj_dir_include_path(src_dir):
    return os.path.join(get_obj_dir_path(src_dir), "dist", "include/")

def get_obj_dir_config_path(src_dir):
    return os.path.join(get_obj_dir_path(src_dir), "config/")

def verify_files_present(src_dir, dst_dir):
    had_error = False
    for (src, dst) in header_files:
        p = Path(src_dir + src)
        if not p.is_file():
            print("ERROR: Header file '{}' is not a valid file.".format(src))
            had_error = True;
    for src in src_files:
        if not Path(src_dir + src).is_file():
            print("ERROR: Source file '{}' is not a valid file.".format(src))
            had_error = True;
    obj_include_dir = get_obj_dir_include_path(src_dir)
    for src in objdir_files:
        if not(Path(obj_include_dir + src).is_file()):
            print("ERROR: Object dir file '{}' is not a valid file.".format(src))
            had_error = True;
    obj_config_dir = get_obj_dir_config_path(src_dir)
    for src in objdir_config_files:
        if not(Path(obj_config_dir + src).is_file()):
            print("ERROR: Object dir file '{}' is not a valid file.".format(src))
            had_error = True;
    return not int(had_error)

def ensure_dir_exists(dst_path):
    dirname = os.path.dirname(dst_path)
    if not os.path.exists(dirname):
        os.makedirs(dirname)

def copy_files(src_dir, dst_dir):
    for (src, dst) in header_files:
        ensure_dir_exists(dst_dir + "include/" + dst)
        copyfile(src_dir + src, dst_dir + "include/" + dst)
    for src in src_files:
        ensure_dir_exists(dst_dir + "src/" + src)
        copyfile(src_dir + src, dst_dir + "src/" + src)
    obj_include_dir = get_obj_dir_include_path(src_dir)
    for src in objdir_files:
        ensure_dir_exists(dst_dir + "include/" + src)
        copyfile(obj_include_dir + src, dst_dir + "include/" + src)
    obj_config_dir = get_obj_dir_config_path(src_dir)
    for src in objdir_config_files:
        ensure_dir_exists(dst_dir + "include/" + src)
        copyfile(obj_config_dir + src, dst_dir + "include/" + src)

def remove_previous_copy(src_dir, dst_dir):
    rmtree(dst_dir + "include/")
    rmtree(dst_dir + "src/")

def write_gecko_revision_file(src_dir):
    result = subprocess.run(['hg', 'log', src_dir, '--limit', '1'], stdout=subprocess.PIPE)
    first_line = result.stdout.splitlines()[0]
    revision = re.match(b'changeset:\s+(.*)', first_line).group(1)
    with open('GECKO_REVISION', 'w') as f:
        f.write(revision.decode('utf-8') + '\n')
        f.close()

def write_unified_cpp_file(dir):
    cpps = sorted([f for f in listdir(dir)
      if f.endswith(".cpp") and not os.path.basename(f).startswith("nsT") and not "SSE" in f])
    with open(dir + os.path.sep + "unified.cpp", "w") as f:
      for cpp_file in cpps:
        f.write("#include \"{}\"\n".format(cpp_file))

def extract_pref_from_line(line):
    m = re.match(r'pref\((".*?"),\s*(.*)\);', line)
    if not m:
        print("extract_pref_from_line() failed on {}".format(line))
        sys.exit(1)
    name = m.group(1)
    val = m.group(2)
    kind = ""
    if val == "true" or val == "false":
        kind = "bool"
    elif val.startswith('"') and val.endswith('"'):
        kind = "string"
    else:
        kind = "int"
    return (name, val, kind)

def is_preprocessor_line(line):
    return (line.startswith("#if") or
            line.startswith("#endif") or
            line.startswith("#else"))

def extract_preferences(lines):
    # Returns a triple of lists of ((pref,value),[include-guard]).
    #   ([((bool_pref, value), [guards])],
    #    [((int_pref, value), [guards])],
    #    [((string_pref, value), [guards])])
    i = 0
    guards = []
    bool_prefs = []
    int_prefs = []
    string_prefs = []
    prefs = dict({"bool": [], "int": [], "string": []});
    while i < len(lines):
        line = lines[i]
        if line.startswith("#if"):
            guards.append(line)
        elif line.startswith("pref"):
            (pref, value, kind) = extract_pref_from_line(line)
            prefs[kind] += [((pref, value), guards[:])]
        elif line.startswith("#else"):
            guards.append(line)
        elif line.startswith("#endif"):
            if guards[-1].startswith("#else"):
                guards.pop()
            guards.pop()
        i += 1
    # Sort the lists based on pref name, so we can binary search it in
    # the Preferences implementation.
    bool_prefs = list(sorted(prefs["bool"], key=lambda x: x[0][0]))
    int_prefs = list(sorted(prefs["int"], key=lambda x: x[0][0]))
    string_prefs = list(sorted(prefs["string"], key=lambda x: x[0][0]))
    return (bool_prefs, int_prefs, string_prefs)

def write_pref_group(dst, prefs, prefix, pref_type):
    dst.write("static const " + pref_type + " s" + prefix + "" + pref_type + "s[] = {\n")
    for ((name, value),guards) in prefs:
        for guard in guards:
            dst.write(guard)
        dst.write("  { " + name + ", " + value + " },\n")
        for guard in guards:
            if not guard.startswith("#else"):
                dst.write("#endif // " + guard)
    dst.write("};\n\n")

def copy_media_prefs(src_pref_file, dst_pref_file, prefix):
    is_media_pref = re.compile(r'pref\("media.')
    with open(src_pref_file, "r") as src:
        # Filter out non media pref and non preprocessor lines.
        lines = []
        for line in src:
            if is_media_pref.match(line) or is_preprocessor_line(line):
                lines += [line]

        (bool_prefs, int_prefs, string_prefs) = extract_preferences(lines)
        with open(dst_pref_file, "w") as dst:
            write_pref_group(dst, bool_prefs, prefix, "BoolPref")
            write_pref_group(dst, int_prefs, prefix, "IntPref")
            write_pref_group(dst, string_prefs, prefix, "StringPref")

def main(args):
    if len(args) != 2:
        print("Usage: python3 import.py <src_dir> <dst_dir>")
        return 1

    src_dir = os.path.abspath(args[0]) + os.path.sep
    dst_dir = os.path.abspath(args[1]) + os.path.sep

    if not verify_files_present(src_dir, dst_dir):
        return 1

    # Create copies of Gecko's media prefs.
    copy_media_prefs(src_dir + "modules/libpref/init/all.js", dst_dir + "glue/prefs_common.cpp", "Common")
    copy_media_prefs(src_dir + "browser/app/profile/firefox.js", dst_dir + "glue/prefs_desktop.cpp", "Desktop")
    copy_media_prefs(src_dir + "mobile/android/app/mobile.js", dst_dir + "glue/prefs_android.cpp", "Android")

    remove_previous_copy(src_dir, dst_dir)
    copy_files(src_dir, dst_dir)
    # Gecko's string classes only build in unified mode...
    write_unified_cpp_file(dst_dir + "src/xpcom/string")
    write_gecko_revision_file(src_dir)
    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
