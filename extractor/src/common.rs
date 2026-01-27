use std::fs::File;
use std::io::{Read, Write};
use zip::ZipArchive;
use serde_json::{Value, json};

fn is_real_recipe(json: &Value) -> bool {
    match json.get("type").and_then(|v| v.as_str()) {
        Some(t) if t.starts_with("minecraft:crafting_") => true,
        Some("minecraft:smelting")
        | Some("minecraft:blasting")
        | Some("minecraft:smoking")
        | Some("minecraft:campfire_cooking")
        | Some("minecraft:stonecutting")
        | Some("minecraft:smithing")
        | Some("minecraft:smithing_transform")
        | Some("minecraft:smithing_trim") => true,
        _ => false,
    }
}

pub fn do_extract(jar_path: &str, output_path: &str) -> Result<usize, Box<dyn std::error::Error>> {
    let file = File::open(jar_path)?;
    let mut zip = ZipArchive::new(file)?;

    let mut recipes = Vec::new();

    for i in 0..zip.len() {
        let mut entry = zip.by_index(i)?;
        let name = entry.name().to_string();

        if !name.starts_with("data/") || !name.ends_with(".json") {
            continue;
        }
        if name.starts_with("data/minecraft/advancement/") {
            continue;
        }

        let mut contents = String::new();
        entry.read_to_string(&mut contents)?;

        let json_val: Value = match serde_json::from_str(&contents) {
            Ok(v) => v,
            Err(_) => continue,
        };

        if !is_real_recipe(&json_val) {
            continue;
        }

        recipes.push(json!({
            "path": name,
            "type": json_val.get("type"),
            "data": json_val
        }));
    }

    let mut out = File::create(output_path)?;
    out.write_all(serde_json::to_string_pretty(&recipes)?.as_bytes())?;

    Ok(recipes.len())
}
