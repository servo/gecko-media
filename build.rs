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
		// "memory/mozjemalloc/mozjemalloc.cpp",
		"memory/fallible/fallible.cpp",
	];

	let cxx_flags = [
		"-I gecko/include",
		"-I gecko/glue/include",
		"--include gecko/glue/include/mozilla-config.h",
		"-D GECKO_MEDIA_CRATE=1",
		"-D IMPL_MFBT",
		"-D MOZ_MEMORY_IMPL=1",
		"-I gecko/src/memory/build",
		"-D _GNU_SOURCE=1", // "The strndup declaration in string.h is in an ifdef __USE_GNU section"
		"-fno-exceptions",
		"-D mozilla_Char16_h", // for falliable
		// "-D _GLIBCXX_USE_CXX11_ABI=0",
	];

	// TODO: If Windows: CXXFLAGS += -Z1

	env::set_var("CXXFLAGS", cxx_flags.join(" "));

	let mut b = gcc::Build::new();
	b.cpp(true);
	for file_path in src_files.iter().map(|&p| "gecko/src/".to_owned() + p.clone()) {
		b.file(file_path);
	}
	b.compile("geckomemory");
}

fn compile_gecko_media() {
	let src_files = [
		"dom/media/MediaInfo.cpp",
	];

	set_cxx_flags();

	let mut b = gcc::Build::new();
	b.cpp(true);
	b.cpp_link_stdlib("stdc++");
	for file_path in src_files.iter().map(|&p| "gecko/src/".to_owned() + p.clone()) {
		b.file(file_path);
	}
	b.compile("geckomedia");
}

fn set_cxx_flags() {
	let cxx_flags = [
		// "-lstdc++",
		"-I gecko/glue/include/stl_wrappers",
		"-I gecko/include",
		"-I gecko/glue/include",
		"--include gecko/glue/include/mozilla-config.h",
		"-fno-exceptions",
		"-D GECKO_MEDIA_CRATE=1",
		// "-D _GLIBCXX_USE_CXX11_ABI=0",
	];
	env::set_var("CXXFLAGS", cxx_flags.join(" "));
}

fn compile_tests() {
	let src_files = [
		"test.cpp",
	];

	set_cxx_flags();

	let mut b = gcc::Build::new();
	b.cpp(true);
	b.cpp_link_stdlib("stdc++");;
	for file_path in src_files.iter().map(|&p| "gecko/test/".to_owned() + p.clone()) {
		b.file(file_path);
	}
	b.compile("geckotest");
}


fn compile_glue() {
	let src_files = [
		"nsString.cpp",
	];

	set_cxx_flags();

	let mut b = gcc::Build::new();
	b.cpp(true);
	b.cpp_link_stdlib("stdc++");
	for file_path in src_files.iter().map(|&p| "gecko/glue/".to_owned() + p.clone()) {
		b.file(file_path);
	}
	b.compile("geckoglue");
}

fn main() {
	env::set_var("RUSTFLAGS", "-fno-exceptions");

  // for l in lib.libs {
    println!("cargo:rustc-link-lib=stdc++");
  // }
	// println!("cargo:rustc-flags=-fno-exceptions");

	compile_memory();
	compile_gecko_media();
	compile_glue();
	compile_tests();
}
