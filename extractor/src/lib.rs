mod common;
mod download;

use std::ffi::CStr;
use std::os::raw::c_char;

#[no_mangle]
pub extern "C" fn extract_recipes(jar_path: *const c_char, output_path: *const c_char) -> i32 {
    if jar_path.is_null() || output_path.is_null() { return -1; }
    unsafe {
        let jp = CStr::from_ptr(jar_path).to_string_lossy();
        let op = CStr::from_ptr(output_path).to_string_lossy();
        match common::do_extract(&jp, &op) {
            Ok(n) => { println!("Extracted {} recipes", n); 0 },
            Err(e) => { eprintln!("extractor error: {}", e); 1 }
        }
    }
}

pub use common::do_extract;
pub use download::download_latest_jar;
