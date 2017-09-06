import os
import re
import subprocess
import sys
from pathlib import Path
from shutil import copyfile
from shutil import rmtree

if sys.version_info[0] < 3:
    raise Exception("Python 3 or a more recent version is required.")

# Imports Gecko code for building in Servo.

header_files = [
    ("dom/media/AudioSampleFormat.h", "AudioSampleFormat.h"),
    ("dom/media/Intervals.h", "Intervals.h"),
    ("dom/media/MediaData.h", "MediaData.h"),
    ("dom/media/MediaInfo.h", "MediaInfo.h"),
    ("dom/media/SharedBuffer.h", "SharedBuffer.h"),
    ("dom/media/StreamTracks.h", "StreamTracks.h"),
    ("dom/media/TimeUnits.h", "TimeUnits.h"),
    ("dom/media/TrackID.h", "TrackID.h"),
    ("gfx/2d/BaseCoord.h", "mozilla/gfx/BaseCoord.h"),
    ("gfx/2d/BasePoint.h", "mozilla/gfx/BasePoint.h"),
    ("gfx/2d/BasePoint3D.h", "mozilla/gfx/BasePoint3D.h"),
    ("gfx/2d/BasePoint4D.h", "mozilla/gfx/BasePoint4D.h"),
    ("gfx/2d/BaseRect.h", "mozilla/gfx/BaseRect.h"),
    ("gfx/2d/BaseSize.h", "mozilla/gfx/BaseSize.h"),
    ("gfx/2d/BaseMargin.h", "mozilla/gfx/BaseMargin.h"),
    ("gfx/2d/Coord.h", "mozilla/gfx/Coord.h"),
    ("gfx/2d/NumericTools.h", "mozilla/gfx/NumericTools.h"),
    ("gfx/2d/Point.h", "mozilla/gfx/Point.h"),
    ("gfx/2d/Rect.h", "mozilla/gfx/Rect.h"),
    ("gfx/2d/Tools.h", "mozilla/gfx/Tools.h"),
    ("gfx/2d/Types.h", "mozilla/gfx/Types.h"),
    ("gfx/layers/ImageTypes.h", "ImageTypes.h"),
    ("memory/fallible/fallible.h", "mozilla/fallible.h"),
    ("memory/mozalloc/mozalloc_abort.h", "mozilla/mozalloc_abort.h"),
    ("memory/mozalloc/mozalloc_oom.h", "mozilla/mozalloc_oom.h"),
    ("memory/mozalloc/throw_gcc.h", "mozilla/throw_gcc.h"),
    ("mozglue/misc/Printf.h", "mozilla/Printf.h"),
    ("mfbt/Alignment.h", "mozilla/Alignment.h"),
    ("mfbt/AllocPolicy.h", "mozilla/AllocPolicy.h"),
    ("mfbt/AlreadyAddRefed.h", "mozilla/AlreadyAddRefed.h"),
    ("mfbt/Array.h", "mozilla/Array.h"),
    ("mfbt/ArrayUtils.h", "mozilla/ArrayUtils.h"),
    ("mfbt/Assertions.h", "mozilla/Assertions.h"),
    ("mfbt/Attributes.h", "mozilla/Attributes.h"),
    ("mfbt/Atomics.h", "mozilla/Atomics.h"),
    ("mfbt/BinarySearch.h", "mozilla/BinarySearch.h"),
    ("mfbt/Casting.h", "mozilla/Casting.h"),
    ("mfbt/Char16.h", "mozilla/Char16.h"),
    ("mfbt/Compiler.h", "mozilla/Compiler.h"),
    ("mfbt/CheckedInt.h", "mozilla/CheckedInt.h"),
    ("mfbt/Compiler.h", "mozilla/Compiler.h"),
    ("mfbt/DebugOnly.h", "mozilla/DebugOnly.h"),
    ("mfbt/EndianUtils.h", "mozilla/EndianUtils.h"),
    ("mfbt/EnumeratedArray.h", "mozilla/EnumeratedArray.h"),
    ("mfbt/FloatingPoint.h", "mozilla/FloatingPoint.h"),
    ("mfbt/IntegerTypeTraits.h", "mozilla/IntegerTypeTraits.h"),
    ("mfbt/IntegerPrintfMacros.h", "mozilla/IntegerPrintfMacros.h"),
    ("mfbt/Likely.h", "mozilla/Likely.h"),
    ("mfbt/MacroArgs.h", "mozilla/MacroArgs.h"),
    ("mfbt/MacroForEach.h", "mozilla/MacroForEach.h"),
    ("mfbt/MathAlgorithms.h", "mozilla/MathAlgorithms.h"),
    ("mfbt/Maybe.h", "mozilla/Maybe.h"),
    ("mfbt/MemoryReporting.h", "mozilla/MemoryReporting.h"),
    ("mfbt/Move.h", "mozilla/Move.h"),
    ("mfbt/OperatorNewExtensions.h", "mozilla/OperatorNewExtensions.h"),
    ("mfbt/Pair.h", "mozilla/Pair.h"),
    ("mfbt/PodOperations.h", "mozilla/PodOperations.h"),
    ("mfbt/RefCountType.h", "mozilla/RefCountType.h"),
    ("mfbt/RefPtr.h", "mozilla/RefPtr.h"),
    ("mfbt/ReverseIterator.h", "mozilla/ReverseIterator.h"),
    ("mfbt/Span.h", "mozilla/Span.h"),
    ("mfbt/StaticAnalysisFunctions.h", "mozilla/StaticAnalysisFunctions.h"),
    ("mfbt/Sprintf.h", "mozilla/Sprintf.h"),
    ("mfbt/TaggedAnonymousMemory.h", "mozilla/TaggedAnonymousMemory.h"),
    ("mfbt/TemplateLib.h", "mozilla/TemplateLib.h"),
    ("mfbt/Types.h", "mozilla/Types.h"),
    ("mfbt/TypeTraits.h", "mozilla/TypeTraits.h"),
    ("mfbt/UniquePtr.h", "mozilla/UniquePtr.h"),
    ("mfbt/UniquePtrExtensions.h", "mozilla/UniquePtrExtensions.h"),
    ("mfbt/Unused.h", "mozilla/Unused.h"),
    ("mozglue/misc/TimeStamp.h", "mozilla/TimeStamp.h"),
    ("xpcom/ds/ArrayIterator.h", "mozilla/ArrayIterator.h"),
    ("xpcom/base/nscore.h", "nscore.h"),
    ("xpcom/base/nsError.h", "nsError.h"),
    ("xpcom/base/nsDebug.h", "nsDebug.h"),
    ("xpcom/build/nsXPCOM.h", "nsXPCOM.h"),
    ("xpcom/build/nsXPCOMCID.h", "nsXPCOMCID.h"),
    ("xpcom/string/nsStringFwd.h", "nsStringFwd.h"),
]

src_files = [
    "dom/media/MediaInfo.cpp",
    "mfbt/Assertions.cpp",
    "mfbt/Unused.cpp",
    "mozglue/misc/Printf.h",
    "xpcom/ds/nsTArray.cpp",
]

objdir_files = [
        "ErrorList.h",
]

def get_obj_dir_path(src_dir):
    if sys.platform == 'darwin':
        obj_dir = "obj-x86_64-apple-darwin16.7.0"
    elif sys.platform == 'linux':
        obj_dir = "obj-x86_64-pc-linux-gnu"
    else:
        raise Exception("Unsupported platform: {}".format(sys.platform))
    return os.path.join(src_dir, obj_dir, "dist", "include/")

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
    obj_dir = get_obj_dir_path(src_dir)
    for src in objdir_files:
        if not(Path(obj_dir + src).is_file()):
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
    obj_dir = get_obj_dir_path(src_dir)
    for src in objdir_files:
        ensure_dir_exists(dst_dir + "include/" + src)
        copyfile(obj_dir + src, dst_dir + "include/" + src)

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

def main(args):
    if len(args) != 2:
        print("Usage: python3 import.py <src_dir> <dst_dir>")
        return 1

    src_dir = os.path.abspath(args[0]) + os.path.sep
    dst_dir = os.path.abspath(args[1]) + os.path.sep

    if not verify_files_present(src_dir, dst_dir):
        return 1
    remove_previous_copy(src_dir, dst_dir)
    copy_files(src_dir, dst_dir)
    write_gecko_revision_file(src_dir)
    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
