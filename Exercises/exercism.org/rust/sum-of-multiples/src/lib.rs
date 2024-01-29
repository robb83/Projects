pub fn sum_of_multiples(limit: u32, factors: &[u32]) -> u32 {

    let mut m = Vec::<u32>::new();

    for f in factors
    {
        for i in 1..limit
        {
            if i * f >= limit
            {
                break;
            }
            
            m.push(i * f);            
        }
    }

    m.sort();
    m.dedup();

    return m.into_iter().sum();
}
