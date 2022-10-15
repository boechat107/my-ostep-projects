use std::fs;
use std::io::BufReader;

pub fn open_as_bufreader(
    file_path: &String,
) -> Result<BufReader<fs::File>, std::io::Error> {
    let fp = fs::File::open(file_path);
    match fp {
        Err(x) => Err(x),
        Ok(x) => Ok(BufReader::new(x)),
    }
}
