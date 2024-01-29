pub fn series(digits: &str, len: usize) -> Vec<String> {
    let mut result = Vec::<String>::new();

    if digits.len() >= len
    {
        let end = digits.len() - len;
        let mut cursor = 0;
        while cursor <= end
        {
            result.push(digits[cursor .. (cursor + len)].to_string());
            cursor += 1;
        }
    }
    return result;
}
