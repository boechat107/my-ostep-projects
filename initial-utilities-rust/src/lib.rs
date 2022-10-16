use std::fs;
use std::io::{BufReader, Error, Read};

pub fn open_as_bufreader(
    file_path: &String,
) -> Result<BufReader<fs::File>, std::io::Error> {
    let fp = fs::File::open(file_path);
    match fp {
        Err(x) => Err(x),
        Ok(x) => Ok(BufReader::new(x)),
    }
}

/// Reads the file in chunks of the size of `buffer` and applies `f` to each
/// chunk by calling `f(nbytes, buffer)`.
///
/// The function assumes that `f` is a closure that mutates its captured values
/// and produces side-effects only.
pub fn do_in_chunks<F>(
    filepath: &String,
    buffer: &mut [u8],
    mut f: F,
) -> Result<(), Error>
where
    F: FnMut(usize, &[u8]) -> (),
{
    let mut breader = open_as_bufreader(filepath)?;
    // We ignore reading errors; we only care about opening file errors.
    while let Ok(nbytes) = breader.read(buffer) {
        f(nbytes, buffer);
        // Checking if we reached the end of file.
        if nbytes < buffer.len() {
            break;
        }
    }
    Ok(())
}
