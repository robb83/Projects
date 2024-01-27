pub fn find(array: &[i32], key: i32) -> Option<usize> {
    let result = array.binary_search(&key);
    if result.is_ok()
    {
        return Some(result.unwrap());
    }

    return None;
}
