/// What should the type of _function be?
pub fn map<T, U>(input: Vec<U>, _function: impl FnMut(U) -> T) -> Vec<T> {
    return input.into_iter().map(_function).collect();
}
