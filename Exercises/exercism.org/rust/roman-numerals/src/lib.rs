use std::fmt::{Display, Formatter, Result};

pub struct Roman
{
    value:u32,
}

impl Display for Roman {
    fn fmt(&self, _f: &mut Formatter<'_>) -> Result {
        
        const ROMAN_NUMBERS_1000: [&str; 4] = ["", "M", "MM", "MMM"];
        const ROMAN_NUMBERS_100: [&str; 10] = ["", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"];
        const ROMAN_NUMBERS_10: [&str; 10] = ["", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"];
        const ROMAN_NUMBERS_1: [&str; 10] = ["" , "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"];

        let mut result = String::new();
        let mut c:u32;

        c = self.value / 1000;
        result.push_str(ROMAN_NUMBERS_1000[c as usize]);

        c = (self.value % 1000) / 100;
        result.push_str(ROMAN_NUMBERS_100[c as usize]);

        c = (self.value % 100) / 10;
        result.push_str(ROMAN_NUMBERS_10[c as usize]);

        c = self.value % 10;
        result.push_str(ROMAN_NUMBERS_1[c as usize]);

        write!(_f, "{}", result)
    }
}

impl From<u32> for Roman {
    fn from(num: u32) -> Self {
        return Roman { value: num };
    }
}
