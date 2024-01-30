pub fn actions(n: u8) -> Vec<&'static str> {
    let mut result = Vec::<&str>::new();
    if (n & 0x1) > 0 { result.push("wink"); }
    if (n & 0x2) > 0 { result.push("double blink"); }
    if (n & 0x4) > 0 { result.push("close your eyes"); }
    if (n & 0x8) > 0 { result.push("jump"); }
    if (n & 0x10) > 0 { result.reverse(); }

    return result;
}
