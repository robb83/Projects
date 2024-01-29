pub fn brackets_are_balanced(string: &str) -> bool {
    let mut stack = Vec::<char>::new();

    for c in string.chars()
    {
        if c == '{' || c == '[' || c == '('
        {
            stack.push(c);
        } 
        else if c == '}' || c == ']' || c == ')'
        {
            let x = stack.pop();
            match (c, x)
            {
                ('}', Some('{')) | (']', Some('[')) | (')', Some('(')) => (),
                _ => return false,
            }
        }
    }
    
    return stack.len() == 0;
}