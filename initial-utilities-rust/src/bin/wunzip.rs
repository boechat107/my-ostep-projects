use initial_utilities_rust::do_in_chunks;
use std::io::{stdout, Error, Write};
use std::{env, iter, process::ExitCode};

// We 4 bytes to store the RLE counter.
type RleCnt = u32;

const RLE_NBYTES: usize = 5;
const CNT_NBYTES: usize = 4;
// The reading buffer size is proportional to the size RLE entries.
const BUFFER_SIZE: usize = 200 * RLE_NBYTES;

fn decode_print_rle(filepath: &String) -> Result<(), Error> {
    let mut buffer = [0 as u8; BUFFER_SIZE];
    let mut std_out = stdout();
    // Closure that mutates its captured value (`std_out` only).
    let printer = |nbytes, data_buffer: &[u8]| {
        // We iterate over RLE entries in the buffer. The step is the size of
        // the RLE entries.
        for i in (0..nbytes).step_by(RLE_NBYTES) {
            // We try to convert a sequence of bytes into an integer.
            let cnt = RleCnt::from_be_bytes(
                // This is how we force a conversion that might fail.
                data_buffer[i..i + CNT_NBYTES].try_into().unwrap(),
            );
            std_out
                .write(
                    &iter::repeat(data_buffer[i + CNT_NBYTES])
                        .take(cnt as usize)
                        .collect::<Vec<u8>>(),
                )
                .expect("Failed to write on STDOUT");
        }
    };
    do_in_chunks(filepath, &mut buffer, printer)
}

fn main() -> ExitCode {
    let args: Vec<String> = env::args().skip(1).collect();
    if args.len() < 1 {
        println!("wunzip: file1 [file2 ...]");
        return ExitCode::FAILURE;
    }

    for filepath in args {
        if let Err(_) = decode_print_rle(&filepath) {
            println!("wunzip: cannot open file");
            return ExitCode::FAILURE;
        }
    }
    ExitCode::SUCCESS
}
