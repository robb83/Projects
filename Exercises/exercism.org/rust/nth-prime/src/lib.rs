pub fn nth(n: u32) -> u32 {
    match n {
        0 => 2,
        1 => 3,
        2 => 5,
        3 => 7,
        4 => 11,
        5 => 13,
        _ => {
            let mut counter = 5;
            let mut divider = 15;

            loop
            {
                if is_prime(divider)
                {
                    counter += 1;
                    if counter == n 
                    {
                        break;
                    }
                }
        
                divider += 2;
            }

            divider
        }
    }
}

fn is_prime(n: u32) -> bool 
{
    for x in 2..n
    {
        if n % x  == 0
        {
            return false;
        }
    }

    return true;
}