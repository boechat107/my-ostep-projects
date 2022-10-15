use initial_utilities_rust::open_as_bufreader;
use std::io::{stdin, BufRead};
use std::{env, process};

/// Read one line each time and searches for matches with "word". Only lines
/// containing "word" are printed to STDOUT.
///
/// The file reader and stdin reader should implement the trait "BufRead"; this
/// makes it easier for us to read one line at a time.
fn match_print<T>(input: T, word: &String)
where
    T: BufRead,
{
    for line in input.lines() {
        match line {
            Ok(s) if s.contains(word) => println!("{}", s),
            _ => (),
        }
    }
}

fn main() -> process::ExitCode {
    let args: Vec<String> = env::args().skip(1).collect();
    if args.len() < 1 {
        println!("wgrep: searchterm [file ...]");
        return process::ExitCode::FAILURE;
    }

    // The word we are looking for matches.
    let word = &args[0];
    // If we only get the word in the command-line, we expect text from STDIN.
    if args.len() == 1 {
        match_print(stdin().lock(), word);
    } else {
        // The application should support multiple input files. We skip the
        // "word" and use the remaining arguments as file paths.
        for fpath in args.iter().skip(1) {
            match open_as_bufreader(fpath) {
                Err(_) => {
                    println!("wgrep: cannot open file");
                    return process::ExitCode::FAILURE;
                }
                Ok(breader) => match_print(breader, word),
            }
        }
    }

    process::ExitCode::SUCCESS
}
