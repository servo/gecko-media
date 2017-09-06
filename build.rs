extern crate gcc;
use std::env;

fn make_builder() -> gcc::Build {
	let mut b = gcc::Build::new();
	b.cpp(true);

	#[cfg(target_os = "macos")]
	b.flag("-std=c++11");

	b.include("gecko/glue/include/stl_wrappers");
	b.include("gecko/include");
	b.include("gecko/glue/include");

	#[cfg(target_os = "macos")]
	env::set_var("CXXFLAGS", "--include gecko/glue/include/mozilla-config-x86_64-apple-darwin.h");

	#[cfg(target_os = "linux")]
	env::set_var("CXXFLAGS", "--include gecko/glue/include/mozilla-config-x86_64-apple-darwin.h");

	b.flag("-fno-exceptions");
	b.define("GECKO_MEDIA_CRATE", "1");

	b
}

fn compile_tests() {
	let mut b = make_builder();

	let src_files = [
		"test.cpp",
	];

	for file_path in src_files.iter().map(|&p| "gecko/test/".to_owned() + p.clone()) {
		b.file(file_path);
	}
	b.compile("geckotest");
}

fn compile_gecko_media() {
	let mut b = make_builder();

	let src_files = [
		"dom/media/MediaInfo.cpp",
		"mfbt/Assertions.cpp",
		"mfbt/Unused.cpp",
		"xpcom/ds/nsTArray.cpp",
	];
	for file_path in src_files.iter().map(|&p| "gecko/src/".to_owned() + p.clone()) {
		b.file(file_path);
	}

	let glue_files = [
		"nsString.cpp",
	];
	for file_path in glue_files.iter().map(|&p| "gecko/glue/".to_owned() + p.clone()) {
		b.file(file_path);
	}

	b.compile("gecko_media");
}

fn main() {
	compile_gecko_media();
	compile_tests();
}
