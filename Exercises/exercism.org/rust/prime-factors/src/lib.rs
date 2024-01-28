pub fn factors(mut n: u64) -> Vec<u64> {
    let mut result = Vec::<u64>::new();
    let mut i = 2;

    loop
    {
        match n % i 
        {
            0 => {
                n = n / i;
                result.push(i);
            },
            _ => {
                i = i + 1;
            }
        }

        if n == 1
        {
            break;
        }
    }

    return result;
}
