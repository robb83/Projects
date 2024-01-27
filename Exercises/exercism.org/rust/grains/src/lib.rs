pub fn square(s: u32) -> u64 {
    if s > 0 && s < 65
    {
        let num:u64 = 2;
        return num.pow(s - 1);
    }

    panic!("Square must be between 1 and 64");
}

pub fn total() -> u64 {
    return u64::MAX;
}
