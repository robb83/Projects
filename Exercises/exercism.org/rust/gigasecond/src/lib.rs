use time::PrimitiveDateTime as DateTime;
use std::time::Duration;

// Returns a DateTime one billion seconds after start.
pub fn after(start: DateTime) -> DateTime {
    let duration = Duration::from_secs(1000000000);
    return start + duration
}
