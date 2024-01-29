pub fn raindrops(n: u32) -> String {
    let mut result = String::new();
    let data: [(u32, &str); 3] = [(3, "Pling"), (5, "Plang"), (7, "Plong")];

    for d in data
    {
        if n % d.0 == 0
        {
            result.push_str(d.1);
        }
    }

    if result.len() == 0
    {
        result.push_str(&n.to_string());
    }

    return result;
}
