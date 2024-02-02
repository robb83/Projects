use std::collections::HashMap;

/// Count occurrences of words.
pub fn word_count(words: &str) -> HashMap<String, u32> {
    
    let mut statistic = HashMap::<String, u32>::new();

    let binding = words.to_lowercase();
    let w = binding
        .split(|c: char| !c.is_alphanumeric() && c != '\'')
        .map(|s| s.trim_start_matches(['\'', '\"']).trim_end_matches(['\'', '\"']).to_string())
        .filter(|s| !s.is_empty());

    for x in w
    {
        *statistic.entry(x).or_insert(0) += 1;
    }

    return statistic;
}
