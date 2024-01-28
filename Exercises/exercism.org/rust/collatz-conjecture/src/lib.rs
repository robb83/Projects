pub fn collatz(n: u64) -> Option<u64> {
    if n == 0 || n > 1_000_000
    {
        return None;
    }

    let mut steps:u64 = 0;
    let mut tmp = n;
    while tmp > 1
    {
        if tmp % 2 != 0
        {
            tmp = tmp * 3 + 1;
        } 
        else 
        {            
            tmp = tmp / 2;
        }

        steps += 1;
    }

    return Some(steps);
}
