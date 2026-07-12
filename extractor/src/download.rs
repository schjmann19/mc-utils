use serde_json::Value;
use std::ffi::CStr;
use std::fs;
use std::os::raw::c_char;
use std::path::Path;

pub fn download_latest_jar(dest: &str) -> Result<(), Box<dyn std::error::Error>> {
    // Fetch latest version info from launcher metadata
    let version_url = "https://piston-meta.mojang.com/mc/game/version_manifest.json";
    let client = reqwest::blocking::Client::new();

    let resp = client.get(version_url).send()?;
    let manifest: Value = resp.json()?;

    // Get the first version (latest including snapshots)
    let versions = manifest
        .get("versions")
        .and_then(|v: &Value| v.as_array())
        .ok_or("Failed to get versions array")?;

    let version_info = versions.first().ok_or("No versions available")?;

    let version_json_url = version_info
        .get("url")
        .and_then(|u| u.as_str())
        .ok_or("Failed to find version JSON URL")?;

    // Fetch version details to get client JAR download URL
    let resp = client.get(version_json_url).send()?;
    let version_json: Value = resp.json()?;

    let jar_url = version_json
        .get("downloads")
        .and_then(|d: &Value| d.get("client"))
        .and_then(|c: &Value| c.get("url"))
        .and_then(|u: &Value| u.as_str())
        .ok_or("Failed to extract JAR URL")?;

    // Create directory if needed
    let dest_path = Path::new(dest);
    if let Some(parent) = dest_path.parent() {
        fs::create_dir_all(parent)?;
    }

    // Download the JAR
    let resp = client.get(jar_url).send()?;
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
