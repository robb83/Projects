pub fn reverse(input: &str) -> String {
    let mut result = String::new();
    result.reserve(input.len());

    for c in input.chars().rev() { 
        result.push(c);
    }

    return result;
}
