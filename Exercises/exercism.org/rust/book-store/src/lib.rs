use std::cmp;

pub fn lowest_price(books: &[u32]) -> u32 {

    let mut copies: [u32; 5] = [0; 5];
    let mut groups: [u32; 5] = [0; 5];

    for b in books {
        copies[(*b - 1) as usize] += 1;
    }
    copies.sort();

    let mut used = 0;
    for i in 0 .. copies.len() {
        groups[i] = copies[i] - used;
        used += groups[i];
    }

    if groups[0] > 0 && groups[2] > 0 {
        let min = cmp::min(groups[0], groups[2]);
        groups[0] -= min;
        groups[2] -= min;
        groups[1] += min + min;
    }

    // print!("{:?}\n", books);
    // print!("{:?}\n", copies);
    // print!("{:?}\n", groups);

    let discounts = [3000, 2560, 2160, 1520, 800];
    return groups.iter().enumerate().map(|(i,v)| v * discounts[i] ).sum();
}