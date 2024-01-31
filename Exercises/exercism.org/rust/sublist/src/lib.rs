#[derive(Debug, PartialEq, Eq)]
pub enum Comparison {
    Equal,
    Sublist,
    Superlist,
    Unequal,
}

pub fn sublist<T: PartialEq>(_first_list: &[T], _second_list: &[T]) -> Comparison {
    
    match (_first_list.len(), _second_list.len())
    {
        (0, 0) => Comparison::Equal,
        (0, _) => Comparison::Sublist,
        (_, 0) => Comparison::Superlist,
        (a, b) if a > b => {
            match _first_list.windows(b).any(|x| x == _second_list) {
                true => Comparison::Superlist,
                _ => Comparison::Unequal
            }
        },
        (a, b) if b > a => {
            match _second_list.windows(a).any(|x| x == _first_list) {
                true => Comparison::Sublist,
                _ => Comparison::Unequal
            }
        },
        (_, _) => match _first_list == _second_list {
            true => Comparison::Equal,
            _ => Comparison::Unequal
        }
    }
}

/*
pub fn sublist<T: PartialEq>(_first_list: &[T], _second_list: &[T]) -> Comparison {
    let mut contains = 0;
    let exchanged:bool;

    let a:&[T];
    let b:&[T];

    if _first_list.len() > _second_list.len() {
        b = _first_list;
        a = _second_list;
        exchanged = true;
    } else {
        a = _first_list;
        b = _second_list;
        exchanged = false;
    }

    let mut cursor_a = 0;
    let mut cursor_b = 0;
    let mut cursor_b_start = 0;
    loop
    {
        if cursor_a >= a.len() || cursor_b >= b.len() {
            break;
        }

        if b[cursor_b] == a[cursor_a] {
            if contains == 0 {
                cursor_b_start = cursor_b;
            }
            contains += 1;
            cursor_a += 1;
        } else if contains > 0 {
            contains = 0;
            cursor_a = 0;
            cursor_b = cursor_b_start;
        }

        cursor_b += 1;     
    }

    if contains == a.len() && contains == b.len() {
        return Comparison::Equal;
    } else if contains == a.len() && b.len() > 0 && exchanged {
        return Comparison::Superlist;
    } else if contains == a.len() && b.len() > 0 {
        return Comparison::Sublist;
    } else {
        return Comparison::Unequal;
    }
}
*/