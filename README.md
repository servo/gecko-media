# Gecko Media Rust Crate

Firefox's Media stack in a Rust crate.

Due to our use of target dependencies declarations in Cargo.toml, a nightly Rust
toolchain is required to build this crate.

Build with `rustup run nightly cargo build`.

Run tests with `rustup run nightly cargo test`.

On Linux both commands currently require the --features pulseaudio argument.
Otherwise the tests will fail.
