pub fn abbreviate(phrase: &str) -> String {
    let mut result = String::new();
    let mut chars = phrase.chars();
    let mut is_previous_uppercase:bool;
    let mut c:Option<char>;
    let mut ch:char;

    loop
    {
        c = chars.next();
        
        if c.is_none()
        {
            break;
        }

        ch = c.unwrap();
        is_previous_uppercase = ch.is_ascii_uppercase();

        if (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')
        {
            result.push(ch.to_ascii_uppercase());
            loop
            {
                c = chars.next();

                if c.is_none() 
                {
                    break;
                }

                ch = c.unwrap();

                if !is_previous_uppercase && ch.is_ascii_uppercase()
                {
                    result.push(ch.to_ascii_uppercase());
                    is_previous_uppercase = ch.is_ascii_uppercase();
                    continue;
                }

                is_previous_uppercase = ch.is_ascii_uppercase();
                if ch.is_ascii_whitespace() || ch == '-' || ch == '_'
                {
                    break;
                }
            }
        }
    }

    return result;
}
