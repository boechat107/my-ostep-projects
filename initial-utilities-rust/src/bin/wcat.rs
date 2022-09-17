use std::{env, fs, process};

fn main() -> process::ExitCode {
    // We skip the first argument because it is the name of the command-line
    // program.
    for cmd_arg in env::args().skip(1) {
        // WARNING: this implementation puts the whole file in memory. The
        // implementation in C handles better this situation by reading the
        // file into chunks.
        let read_res = fs::read_to_string(cmd_arg);
        // We could have used something simpler like "expect()", but the
        // results would be different of the C implementation.
        match read_res {
            Err(_) => {
                println!("wcat: cannot open file");
                return process::ExitCode::FAILURE;
            }
            Ok(file_content) => print!("{}", file_content),
        }
    }
    process::ExitCode::SUCCESS
}
