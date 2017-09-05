extern crate gcc;
use std::env;

fn set_cxx_flags() {
	let cxx_flags = [
		"-I gecko/glue/include/stl_wrappers",
		"-I gecko/include",
		"-I gecko/glue/include",
		"--include gecko/glue/include/mozilla-config.h",
		"-fno-exceptions",
		"-D GECKO_MEDIA_CRATE=1",
	];
	env::set_var("CXXFLAGS", cxx_flags.join(" "));
}

fn compile_tests() {
	let src_files = [
		"test.cpp",
	];
	let mut b = gcc::Build::new();
	b.cpp(true);
	for file_path in src_files.iter().map(|&p| "gecko/test/".to_owned() + p.clone()) {
		b.file(file_path);
	}
	b.compile("geckotest");
}


fn compile_gecko_media() {
	let mut b = gcc::Build::new();
	b.cpp(true);

	let src_files = [
		"dom/media/MediaInfo.cpp",
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
	set_cxx_flags();
	compile_gecko_media();
	compile_tests();
}
