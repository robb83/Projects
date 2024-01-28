/// Return the Hamming distance between the strings,
/// or None if the lengths are mismatched.
pub fn hamming_distance(s1: &str, s2: &str) -> Option<usize> {
    if s1.len() != s2.len()
    {
        return None;
    }

    return Some(s1.chars().zip(s2.chars()).filter(|x| x.0 != x.1).count());
}

// pub fn hamming_distance(s1: &str, s2: &str) -> Option<usize> {
//     if s1.len() != s2.len()
//     {
//         return None;
//     }

//     let mut chars1 = s1.chars();
//     let mut chars2 = s2.chars();
//     let mut differences = 0;

//     loop
//     {
//         let ch1 = chars1.next();
//         let ch2 = chars2.next();

//         if ch1.is_none() || ch2.is_none()
//         {
//             break;
//         }

//         if ch1.unwrap() != ch2.unwrap()
//         {
//             differences += 1;
//         }
//     }

//     return Some(differences);
// }
