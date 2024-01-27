pub fn check(candidate: &str) -> bool {
    for (i, ch) in candidate.char_indices()
    {
        if ch == ' ' || ch == '-'
        {
            continue;
        }
    
        let ch1 = ch.to_ascii_lowercase();
        for ch2 in candidate.chars().skip(i + 1)
        {
            if ch1 == ch2.to_ascii_lowercase()
            {
                return false;
            }
        }
    }

    return true;
}
