use initial_utilities_rust::do_in_chunks;
use std::{env, io::stdout, io::Write, process};

const BUFFER_SIZE: usize = 1000;

fn main() -> process::ExitCode {
    let mut std_out = stdout();
    let mut buffer = [0 as u8; BUFFER_SIZE];
    // We skip the first argument because it is the name of the command-line
    // program. The remaining arguments should be file paths.
    for filepath in env::args().skip(1) {
        // We just print the contents of the buffer on STDOUT.
        let printer = |nbytes, data_buffer: &[u8]| {
            std_out
                .write(&data_buffer[0..nbytes])
                .expect("Failed to write to STDOUT");
        };
        // We read the input file in chunks.
        match do_in_chunks(&filepath, &mut buffer, printer) {
            Err(_) => {
                println!("wcat: cannot open file");
                return process::ExitCode::FAILURE;
            }
            _ => (),
        }
    }
    process::ExitCode::SUCCESS
}
