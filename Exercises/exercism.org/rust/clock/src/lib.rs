use std::fmt;

#[derive(Debug, PartialEq)]
pub struct Clock
{
    hours:i32,
    minutes:i32,
}

impl Clock {

    pub fn new(hours: i32, minutes: i32) -> Self {
        let t = (((hours * 60 + minutes) % 1440) + 1440) % 1440;
        return Clock { hours: t / 60, minutes: t % 60 };
    }

    pub fn add_minutes(&self, minutes: i32) -> Self {
        let t = ((((self.hours * 60) + self.minutes + minutes) % 1440) + 1440) % 1440;
        return Clock { hours: t / 60, minutes: t % 60 };
    }
}

impl fmt::Display for Clock {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{:02}:{:02}", self.hours, self.minutes)
    }
}
