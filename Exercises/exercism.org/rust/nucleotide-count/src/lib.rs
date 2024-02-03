use std::collections::HashMap;

pub fn count(nucleotide: char, dna: &str) -> Result<usize, char> {
    // 'A' for adenine, 'C' for cytosine, 'G' for guanine, and 'T' for thymine.
    let nucleotids = ['A', 'C', 'G', 'T'];
    let mut result = 0;

    if !nucleotids.contains(&nucleotide) {
        return Err(nucleotide);
    }
    
    for c in dna.chars()
    {
        if nucleotids.contains(&c) {
            if c == nucleotide {
                result += 1;
            }
        } else {
            return Err(c);
        }
    }

    return Ok(result);
}

pub fn nucleotide_counts(dna: &str) -> Result<HashMap<char, usize>, char> {
    let mut result = HashMap::from([ ('A', 0 as usize), ('C', 0 as usize), ('G', 0 as usize), ('T', 0 as usize) ]);

    for c in dna.chars()
    {
        if !result.contains_key(&c) {
            return Err(c);
        }

        *result.get_mut(&c).unwrap() += 1;
    }

    return Ok(result);
}
