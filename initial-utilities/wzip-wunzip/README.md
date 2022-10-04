# wzip - wunzip

`wzip` and `wunzip` are file compression tools using [RLE enconding](https://en.wikipedia.org/wiki/Run-length_encoding).

## Usage

```shell
make
echo "aaaaaabbbbbbcccccc" > /tmp/input.txt
./build/wzip /tmp/input.txt > /tmp/input.z
./build/wunzip /tmp/input.z
```
