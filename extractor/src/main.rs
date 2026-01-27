mod common;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let jar_path = std::env::args().nth(1).unwrap_or_else(|| {
        let mut path = std::env::current_dir().unwrap();
        path.push("minecraftjar");
        path.push("1.21.11.jar");
        path.to_string_lossy().to_string()
    });
    let output_path = std::env::args().nth(2).unwrap_or_else(|| {
        let mut path = std::env::current_dir().unwrap();
        path.push("extractor");
        path.push("recipes.json");
        path.to_string_lossy().to_string()
    });

    match common::do_extract(&jar_path, &output_path) {
        Ok(n) => {
            println!("Extracted {} recipes", n);
            Ok(())
        }
        Err(e) => {
            eprintln!("extractor error: {}", e);
            Err(e)
        }
    }
}
