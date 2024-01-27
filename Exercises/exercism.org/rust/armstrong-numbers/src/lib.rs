pub fn is_armstrong_number(num: u32) -> bool {

    let mut digits = Vec::<u32>::new();
    let mut sum:u32 = 0;
    let mut temp:u32 = num;
    let mut i = 9;

    loop
    {
        if i > 0 && temp > 0 
        {
            i -= 1;
            digits.push(temp % 10);
            temp = temp / 10;
            continue
        }

        break
    }
    
    for d in &digits
    {
        sum += d.pow(digits.len().try_into().unwrap());
    }

    return sum == num;
}
