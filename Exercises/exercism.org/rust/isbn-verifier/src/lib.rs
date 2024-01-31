/// Determines whether the supplied string is a valid ISBN number
pub fn is_valid_isbn(isbn: &str) -> bool {
    
    let mut cursor = 0;
    let mut sum:u32 = 0;
    for c in isbn.chars()
    {
        if cursor >= 10 {
            return false;
        }

        if c.is_digit(10) {
            sum += (10 - cursor) * c.to_digit(10).unwrap();
            cursor += 1;
        } else if c == 'X' && cursor == 9 {
            sum += 10;
            cursor += 1;
        } else if c == '-' {
            continue;
        } else {
            return false;
        }
    }

    return cursor == 10 && sum % 11 == 0;
}
