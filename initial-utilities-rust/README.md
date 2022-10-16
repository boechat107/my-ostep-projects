# Utilities in Rust

* `wcat`
* `wgrep`
* `wzip` and `wunzip`

The implementations use some interesting concepts:

* Closures with mutable references
* Passing functions as arguments to other functions
* Abstraction to read files by chunks

## Usage

``` shell
# Compile the source code
cargo build -r

# wcat
./target/release/wcat src/bin/wcat.rs

# wgrep
./target/release match \
  ../initial-utilities/wgrep/wgrep.c \
  README.md \
  src/bin/wgrep.rs
```
