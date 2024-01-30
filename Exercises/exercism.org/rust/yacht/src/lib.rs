#[derive(Debug)]
pub enum Category {
    Ones,
    Twos,
    Threes,
    Fours,
    Fives,
    Sixes,
    FullHouse,
    FourOfAKind,
    LittleStraight,
    BigStraight,
    Choice,
    Yacht,
}

type Dice = [u8; 5];

pub fn score(mut dice: Dice, category: Category) -> u8 {
    match category 
    {
        Category::Ones => return dice.iter().filter(|x| **x == 1).sum(),
        Category::Twos => return dice.iter().filter(|x| **x == 2).sum(),
        Category::Threes => return dice.iter().filter(|x| **x == 3).sum(),
        Category::Fours => return dice.iter().filter(|x| **x == 4).sum(),
        Category::Fives => return dice.iter().filter(|x| **x == 5).sum(),
        Category::Sixes => return dice.iter().filter(|x| **x == 6).sum(),
        Category::FullHouse =>
        {
            let mut counter: [u8; 7] = [0; 7];
            dice.iter().for_each(|d| counter[*d as usize] += 1);
            
            return match counter.iter().any(|x| *x == 3) && counter.iter().any(|x| *x == 2)
            {
                true => dice.iter().sum(),
                _ => 0,
            }
        },
        Category::FourOfAKind =>
        {
            let mut counter: [u8; 7] = [0; 7];
            dice.iter().for_each(|d| counter[*d as usize] += 1);

            return match counter.iter().position(|x| *x >= 4)
            {
                Some(x) => (x * 4).try_into().unwrap(),
                _ => 0,
            }
        },
        Category::LittleStraight =>
        {
            dice.sort();
            return match dice.eq(&[1,2,3,4,5])
            {
                true => 30,
                _ => 0,
            }
        },
        Category::BigStraight =>
        {
            dice.sort();
            return match dice.eq(&[2,3,4,5,6])
            {
                true => 30,
                _ => 0,
            }
        },
        Category::Choice => return dice.iter().sum(),
        Category::Yacht =>
        {
            return match dice.iter().filter(|x| **x == dice[0]).count()
            {
                5 => 50,
                _ => 0,
            }
        },
        _ => return 0,
    }
}
