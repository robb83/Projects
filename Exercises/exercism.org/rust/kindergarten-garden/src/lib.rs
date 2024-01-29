pub fn plants(diagram: &str, student: &str) -> Vec<&'static str> {    
    let n = 2 * ((student.chars().next().unwrap() as u32) - ('A' as u32));
    let separator:u32 = diagram.find('\n').unwrap().try_into().unwrap();
    let offsets = [0, 1, separator + 1, separator + 2];

    let mut result = Vec::<&'static str>::new();    
    for o in offsets
    {
        match diagram.chars().nth((n + o).try_into().unwrap())
        {
            Some('C') => result.push("clover"),
            Some('G') => result.push("grass"),
            Some('R') => result.push("radishes"),
            Some('V') => result.push("violets"),
            _ => (),
        }
    }

    return result;
}
