import json
import os
import platform
import re
import subprocess
import sys
from pathlib import Path
from shutil import copy2
from shutil import rmtree
from os import listdir

with open(os.path.join("data", "header_files.json")) as f:
    header_files = json.load(f)

with open(os.path.join("data", "objdir_files.json")) as f:
    objdir_files = json.load(f)

with open(os.path.join("data", "src_files.json")) as f:
    src_files = json.load(f)

def get_obj_dir_path(src_dir):
    if sys.platform == 'darwin':
        obj_dir = "obj-x86_64-apple-darwin%s" % platform.release()
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
    for (dst, src) in header_files.items():
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
    return not int(had_error)

def ensure_dir_exists(dst_path):
    dirname = os.path.dirname(dst_path)
    if not os.path.exists(dirname):
        os.makedirs(dirname)

def copy_files(src_dir, dst_dir):
    for (dst, src) in header_files.items():
        ensure_dir_exists(dst_dir + "include/" + dst)
        copy2(src_dir + src, dst_dir + "include/" + dst)
    for src in src_files:
        ensure_dir_exists(dst_dir + "src/" + src)
        copy2(src_dir + src, dst_dir + "src/" + src)
    obj_include_dir = get_obj_dir_include_path(src_dir)
    for src in objdir_files:
        ensure_dir_exists(dst_dir + "include/" + src)
        copy2(obj_include_dir + src, dst_dir + "include/" + src)

def remove_previous_copy(src_dir, dst_dir):
    rmtree(dst_dir + "include/")
    rmtree(dst_dir + "src/")

def write_gecko_revision_file(src_dir):
    if not os.path.isdir(os.path.join(src_dir, ".hg")):
        return
    result = subprocess.run(['hg', 'log', src_dir, '--limit', '1'], stdout=subprocess.PIPE)
    first_line = result.stdout.splitlines()[0]
    revision = re.match(b'changeset:\s+(.*)', first_line).group(1)
    with open('GECKO_REVISION', 'w') as f:
        f.write(revision.decode('utf-8') + '\n')

def write_unified_cpp_file(dir):
    cpps = sorted([f for f in listdir(dir)
      if f.endswith(".cpp") and not os.path.basename(f).startswith("nsT") and not "SSE" in f])
    with open(dir + os.path.sep + "unified.cpp", "w") as f:
      for cpp_file in cpps:
        f.write("#include \"{}\"\n".format(cpp_file))

def is_preprocessor_line(line):
    return (line.startswith("#if") or
            line.startswith("#endif") or
            line.startswith("#else") or
            line.startswith("#elif"))

def strip_empty_ifdefs(lines):
    starts = []
    counts = [0]
    i = 0
    while i < len(lines):
        line = lines[i]
        if line.startswith("#if"):
            counts += [0]
            starts += [i]
            i += 1
        elif line.startswith("#endif"):
            if counts[-1] == 0:
                # Empty block. Discard.
                lines[starts[-1]:i+1] = []
                i = starts[-1]
            else:
                i += 1
            starts.pop()
            counts.pop()
        elif not line.startswith("#"):
            counts[-1] += 1
            i += 1
        else:
            i += 1
    return lines

def copy_media_prefs(src_pref_file, dst_pref_file, prefix):
    is_media_pref = re.compile(r'pref\("media.')
    with open(src_pref_file, "r") as src:
        # Filter out non media pref and non preprocessor lines.
        lines = []
        for line in src:
            if is_media_pref.match(line) or is_preprocessor_line(line):
                lines += [line];
        lines = strip_empty_ifdefs(lines)
        with open(dst_pref_file, "w") as dst:
            for line in lines:
                dst.write(line)

def gather_files(path, extensions):
    all_files = []
    for root, dirs, files in os.walk(path):
        for f in files:
            _, extension = os.path.splitext(f)
            if extension in extensions:
                all_files.append(os.path.join(root, f))

    return all_files

def check_for_glue_duplicates(dst_dir, sub_dir, extensions):
    glue_headers = dict([(os.path.basename(f), f)  for f in gather_files(os.path.join(dst_dir, "glue"), extensions)])
    gecko_headers = gather_files(os.path.join(dst_dir, sub_dir), extensions)
    duplicates = []
    for header in gecko_headers:
        gecko_filename = os.path.basename(header)
        if gecko_filename in glue_headers.keys():
            duplicates.append(glue_headers[gecko_filename])

    return duplicates

def check_for_duplicates(dst_dir):
    duplicates = check_for_glue_duplicates(dst_dir, "include", [".h",])
    duplicates.extend(check_for_glue_duplicates(dst_dir, "src", [".c", ".cpp"]))
    for filename in duplicates:
        print("Duplicate file found: %s" % filename)

    if duplicates:
        print("To remove them: rm %s" % ' '.join(duplicates))
    return len(duplicates)

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
    return int(check_for_duplicates(dst_dir))

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
