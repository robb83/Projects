pub fn rotate(input: &str, key: u8) -> String {
    return input
        .chars()
        .map(|x| 
        match x {
            m if m >= 'a' && m <= 'z' => char::from_u32((((26 + ((m as u8) - 97) + key)) % 26 + 97).into()).unwrap(),
            m if m >= 'A' && m <= 'Z' => char::from_u32((((26 + ((m as u8) - 65) + key)) % 26 + 65).into()).unwrap(),
            m => m,
        })
        .collect::<String>()
}