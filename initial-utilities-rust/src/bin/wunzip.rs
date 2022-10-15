use initial_utilities_rust::open_as_bufreader;
use std::io::{stdout, Error, Read, Write};
use std::{env, iter, process::ExitCode};

// We 4 bytes to store the RLE counter.
type RleCnt = u32;

const RLE_NBYTES: usize = 5;
const CNT_NBYTES: usize = 4;
// The reading buffer size is proportional to the size RLE entries.
const BUFFER_SIZE: usize = 200 * RLE_NBYTES;

fn decode_print_rle(filepath: &String) -> Result<(), Error> {
    let mut breader = open_as_bufreader(filepath)?;
    let mut buffer = [0 as u8; BUFFER_SIZE];
    let mut std_out = stdout();
    // We read big chunks of data to avoid invoking too many IO operations.
    // TODO: is there an abstraction for this type of loop?
    // do_by_chunk(filepath, chunk size, f: T -> T, state: T)
    // "f" is function that updates the state and has some side-effects.
    while let Ok(nbytes) = breader.read(&mut buffer) {
        // We iterate over RLE entries in the buffer. The step is the size of
        // the RLE entries.
        for i in (0..nbytes).step_by(RLE_NBYTES) {
            // We try to convert a sequence of bytes into an integer.
            let cnt = RleCnt::from_be_bytes(
                // This is how we force a conversion that might fail.
                buffer[i..i + CNT_NBYTES].try_into().unwrap(),
            );
            std_out
                .write(
                    &iter::repeat(buffer[i + CNT_NBYTES])
                        .take(cnt as usize)
                        .collect::<Vec<u8>>(),
                )
                .expect("Failed to write on STDOUT");
        }
        // Checking if we reached the end of file.
        if nbytes < BUFFER_SIZE {
            break;
        }
    }
    Ok(())
}

fn main() -> ExitCode {
    let args: Vec<String> = env::args().skip(1).collect();
    if args.len() < 1 {
        println!("wunzip: file1 [file2 ...]");
        return ExitCode::FAILURE;
    }

    for filepath in args {
        if let Err(_) = decode_print_rle(&filepath) {
            return ExitCode::FAILURE;
        }
    }
    ExitCode::SUCCESS
}
