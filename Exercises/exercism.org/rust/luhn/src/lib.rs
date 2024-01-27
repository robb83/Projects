/// Check a Luhn checksum.
pub fn is_valid(code: &str) -> bool {
    let mut sum:u32 = 0;
    let mut digit:u32;
    let mut digits:u32 = 0;
    let mut flip:bool = false;

    for ch in code.chars().rev() {
        if ch == ' ' 
        {
             continue; 
        }

        if !ch.is_ascii_digit()
        {
             return false; 
        }

        digit = ch as u32 - '0' as u32;

        if flip 
        {
            digit += digit;
            if digit > 9 
            {
                digit -= 9;
            } 
        }

        sum += digit;
        flip = !flip;
        digits += 1;
    }

    return (digits > 1) && (sum % 10 == 0);
}