use rand::Rng;

// https://en.wikipedia.org/wiki/Modular_exponentiation
fn modular_pow(base:u64, exponent:u64, modulus:u64) -> u64
{
    if modulus == 1
    {
        return 0
    }

    let mut c:u64 = 1;
    for _i in 0..exponent
    {
        c = (c * base) % modulus;
    }

    return c
}

// https://en.wikipedia.org/wiki/Modular_exponentiation
fn modular_pow2(mut base:u64, mut exponent:u64, modulus:u64) -> u64
{
    if modulus == 1 { return 0; }

    // Assert :: (modulus - 1) * (modulus - 1) does not overflow base

    let mut result:u64 = 1;
    base = base % modulus;

    while exponent > 0
    {
        if exponent % 2 == 1
        {
            result = (result * base) % modulus;
        }

        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }

    return result
}

fn is_prime(n: u64) -> bool 
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

pub fn private_key(p: u64) -> u64 {
    
    let primes:Vec<u64> = (3..p).step_by(2).filter(|y| is_prime(*y)).collect();

    let mut rng = rand::thread_rng();
    return primes[rng.gen_range(0..primes.len())];
}

pub fn public_key(p: u64, g: u64, a: u64) -> u64 {
    return modular_pow2(g, a, p);
}

pub fn secret(p: u64, b_pub: u64, a: u64) -> u64 {
    return modular_pow2(b_pub, a, p);
}
