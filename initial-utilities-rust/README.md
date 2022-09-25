# Utilities in Rust

* `wcat` (needs improvements to handle big files)
* `wgrep`

## Usage

``` shell
# Compile the source code
cargo build -r
# wcat
./target/release/wcat src/bin/wcat.rs
# wgrep
./target/debug/wgrep match \
  ../initial-utilities/wgrep/wgrep.c \
  README.md \
  src/bin/wgrep.rs
```
