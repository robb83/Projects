use std::collections::BTreeMap;

pub fn transform(h: &BTreeMap<i32, Vec<char>>) -> BTreeMap<char, i32> {
    let mut result = BTreeMap::new();

    for (key, value) in h.iter() {
        for v in value {
            result.insert(v.to_ascii_lowercase(), *key);
        }
    }

    return result;
}
