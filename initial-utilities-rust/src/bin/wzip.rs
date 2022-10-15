use initial_utilities_rust::open_as_bufreader;
use std::io::{stdout, Read, Stdout, Write};
use std::{env, process::ExitCode};

// We 4 bytes to store the RLE counter.
type RleCnt = u32;
// We use only 1 byte for the character (ASCII).
type RleChar = u8;

const BUFFER_SIZE: usize = 1000;

/// Returns the RLE encoded bytes from a character and its counter.
fn to_rle_bytes(raw_char: RleChar, cnt: RleCnt) -> Vec<u8> {
    let mut cnt_bytes = cnt.to_be_bytes().to_vec();
    cnt_bytes.push(raw_char);
    cnt_bytes
}

fn print_bytes(std_out: &mut Stdout, rle_bytes: &Vec<u8>) {
    std_out
        .write(&rle_bytes)
        .expect("Failed to write on STDOUT");
}

fn read_print_rle(filepath: &String) -> Result<(), std::io::Error> {
    let mut breader = open_as_bufreader(filepath)?;
    let mut buffer = [0 as u8; BUFFER_SIZE];
    let mut char_cnt = 0 as RleCnt;
    let mut last_char = 0 as RleChar;
    let mut std_out = stdout();
    // We keep reading chunks of bytes from the file.
    while let Ok(nbytes) = breader.read(&mut buffer) {
        for i in 0..nbytes {
            if buffer[i] == last_char {
                char_cnt += 1;
            } else {
                if char_cnt > 0 {
                    print_bytes(
                        &mut std_out,
                        &to_rle_bytes(last_char, char_cnt),
                    );
                }
                last_char = buffer[i];
                char_cnt = 1;
            }
        }
        // We check if we reached the end of file.
        if nbytes < BUFFER_SIZE {
            break;
        }
    }
    Ok(())
}

fn main() -> ExitCode {
    let args: Vec<String> = env::args().skip(1).collect();
    if args.len() < 1 {
        println!("wzip: file1 [file2 ...]");
        return ExitCode::FAILURE;
    }

    for filepath in args {
        if let Err(_) = read_print_rle(&filepath) {
            println!("wzip: cannot open file");
            return ExitCode::FAILURE;
        }
    }
    ExitCode::SUCCESS
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_rle_encoding() {
        assert_eq!(to_rle_bytes(65, 3), [0, 0, 0, 3, 65]);
        assert_eq!(to_rle_bytes(65, 256), [0, 0, 1, 0, 65]);
    }
}
