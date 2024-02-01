use rand::thread_rng;
use rand::Rng;

pub fn encode(key: &str, s: &str) -> Option<String> {
    if key.len() == 0 || key.chars().any(|x| !x.is_ascii_lowercase() || !x.is_alphabetic()) { return None; }

    return Some(s.bytes()
            .collect::<Vec<u8>>()
            .chunks(key.len())
            .map(|x| x.iter().zip(key.bytes()))
            .flatten()
            .map(|(m,k)| char::from_u32(((26 + (m - 97) + (k - 97)) % 26 + 97).into()).unwrap())
            .collect::<String>());
}

pub fn decode(key: &str, s: &str) -> Option<String> {
    if key.len() == 0 || key.chars().any(|x| !x.is_ascii_lowercase() || !x.is_alphabetic()) { return None; }

    return Some(s.bytes()
            .collect::<Vec<u8>>()
            .chunks(key.len())
            .map(|x| x.iter().zip(key.bytes()))
            .flatten()
            .map(|(m,k)| char::from_u32(((26 + (m - 97) - (k - 97)) % 26 + 97).into()).unwrap())
            .collect::<String>());
}

pub fn encode_random(s: &str) -> (String, String) {
    let mut rng = thread_rng();
    let key = (0 .. 101).map(|_| char::from_u32(rng.gen_range(97 .. 123)).expect("ITSWORKS")).collect::<String>();
    let encoded = encode(&key, s).expect("ITSWORKS");
    return (key, encoded);
}
