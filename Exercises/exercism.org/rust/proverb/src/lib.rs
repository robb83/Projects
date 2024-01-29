pub fn build_proverb(list: &[&str]) -> String {
    if list.len() == 0 { return String::new(); }
    let d = [format!("And all for the want of a {}.", list[0])];
    return list.windows(2).map(|x| format!("For want of a {} the {} was lost.", x[0], x[1])).chain(d).collect::<Vec::<String>>().join("\n");
}
