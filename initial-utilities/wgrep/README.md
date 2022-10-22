# wgrep

Mimics `grep`.

PS.: I used my own implementation of the GNU function "getline". My implementation is more inefficient since it doesn't reuse previously allocated memory; memory is freed and allocated for each new line.

## Usage

```shell
make
./build/wcat match wcat.c
# From STDIN.
./build/wcat match < wcat.c
```
