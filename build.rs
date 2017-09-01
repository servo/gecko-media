extern crate gcc;
use std::env;

// Gecko's /memory/ directory.
fn compile_memory() {
	// mozalloc
	// fallible
	// build
	// mozjemalloc
	let src_files = [
		"memory/mozalloc/mozalloc.cpp",
		"memory/mozalloc/mozalloc_abort.cpp",
		"memory/mozalloc/mozalloc_oom.cpp",
		"memory/mozjemalloc/mozjemalloc.cpp",
		"memory/fallible/fallible.cpp",
	];

	let cxx_flags = [
		"-I gecko/include",
		"-I gecko/glue/include",
		"--include gecko/glue/include/mozilla-config.h",
		"-D GECKO_MEDIA_CRATE=1",
		"-D IMPL_MFBT",
		"-I gecko/src/memory/build",
		"-D _GNU_SOURCE=1", // "The strndup declaration in string.h is in an ifdef __USE_GNU section"
		"-fno-exceptions",
		"-D mozilla_Char16_h", // for falliable
	];

	// TODO: If Windows: CXXFLAGS += -Z1

	env::set_var("CXXFLAGS", cxx_flags.join(" "));

    let mut b = gcc::Build::new();
    b.cpp(true);
    for file_path in src_files.iter().map(|&p| "gecko/src/".to_owned() + p.clone()) {
    	b.file(file_path);
    }
    b.compile("memory");

}

fn compile_gecko_media() {
	let glue_src_files = [
		"nsString.cpp",
	];

	let src_files = [
		"dom/media/MediaInfo.cpp",
	];

	let cxx_flags = [
		"-I gecko/include",
		"-I gecko/glue/include",
		"--include gecko/glue/include/mozilla-config.h",
		"-D GECKO_MEDIA_CRATE=1"
	];

	env::set_var("CXXFLAGS", cxx_flags.join(" "));

    let mut b = gcc::Build::new();
    b.cpp(true);
    for file_path in glue_src_files.iter().map(|&p| "gecko/glue/".to_owned() + p.clone()) {
    	b.file(file_path);
    }
    for file_path in src_files.iter().map(|&p| "gecko/src/".to_owned() + p.clone()) {
    	b.file(file_path);
    }
    b.compile("geckomedia");

}

fn main() {
	compile_memory();
	compile_gecko_media();
}
