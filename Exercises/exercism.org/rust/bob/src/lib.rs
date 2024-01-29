pub fn reply(mut message: &str) -> &str {    

    message = message.trim();
    let uppercase = message.chars().filter(|x| x.is_alphabetic()).all(|x| x.is_uppercase()) && message.chars().filter(|x| x.is_alphabetic()).count() > 0;
    let question = message.ends_with("?");

    match message
    {
        x if x.is_empty() => "Fine. Be that way!",
        x if uppercase && question => "Calm down, I know what I'm doing!",
        x if question => "Sure.",
        x if uppercase => "Whoa, chill out!",
        _ => "Whatever."
    }
}
