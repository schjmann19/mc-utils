use std::fs;
use std::path::Path;
use std::ffi::CStr;
use std::os::raw::c_char;
use serde_json::Value;

pub fn download_latest_jar(dest: &str) -> Result<(), Box<dyn std::error::Error>> {
    // Fetch latest version info from launcher metadata
    let version_url = "https://launcher.mojang.com/v1/metadata.json";
    let client = reqwest::blocking::Client::new();

    let resp = client.get(version_url).send()?;
    let metadata: Value = resp.json()?;

    let version = metadata
        .get("latest")
        .and_then(|l: &Value| l.get("release"))
        .and_then(|v: &Value| v.as_str())
        .ok_or("Failed to extract version from metadata")?;

    // Fetch version details to get SHA1 and download URL
    let version_detail_url = format!("https://launcher.mojang.com/v1/objects/{}/version.json", version);
    let resp = client.get(&version_detail_url).send()?;
    let version_json: Value = resp.json()?;

    let sha1 = version_json
        .get("downloads")
        .and_then(|d: &Value| d.get("client"))
        .and_then(|c: &Value| c.get("sha1"))
        .and_then(|s: &Value| s.as_str())
        .ok_or("Failed to extract SHA1")?;

    let jar_url = format!("https://launcher.mojang.com/v1/objects/{}/client.jar", sha1);

    // Create directory if needed
    let dest_path = Path::new(dest);
    if let Some(parent) = dest_path.parent() {
        fs::create_dir_all(parent)?;
    }

    // Download the JAR
    let resp = client.get(&jar_url).send()?;
    let bytes = resp.bytes()?;
    fs::write(dest, bytes)?;

    Ok(())
}

#[no_mangle]
pub extern "C" fn download_minecraft_jar(dest: *const c_char) -> i32 {
    if dest.is_null() {
        return -1;
    }

    unsafe {
        let dest_str = CStr::from_ptr(dest).to_string_lossy();
        match download_latest_jar(&dest_str) {
            Ok(_) => {
                println!("Downloaded Minecraft JAR to {}", dest_str);
                0
            }
            Err(e) => {
                eprintln!("Failed to download JAR: {}", e);
                1
            }
        }
    }
}
