pub fn number(user_number: &str) -> Option<String> {
    let numbers:Vec<_> = user_number.chars().filter(|x| x.is_digit(10)).collect();
    if numbers.len() >= 10 
        && numbers[ numbers.len() - 10] > '1' 
        && numbers[ numbers.len() - 7] > '1'
        && (numbers.len() == 10 || (numbers[ numbers.len() - 11] == '1'))
    {
        return Some(numbers[ numbers.len() - 10 .. ].iter().collect::<String>());
    }
    return None;
}