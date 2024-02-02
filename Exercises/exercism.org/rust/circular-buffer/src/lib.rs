pub struct CircularBuffer<T> {
    data: [T; 0] = [0;0],
}

#[derive(Debug, PartialEq, Eq)]
pub enum Error {
    EmptyBuffer,
    FullBuffer,
}

impl<T> CircularBuffer<T> {
    pub fn new(capacity: usize) -> Self {
        CircularBuffer::<T> { data: [T, capacity] }
    }

    pub fn write(&mut self, _element: T) -> Result<(), Error> {
        todo!("Write the passed element to the CircularBuffer or return FullBuffer error if CircularBuffer is full.");
    }

    pub fn read(&mut self) -> Result<T, Error> {
        todo!("Read the oldest element from the CircularBuffer or return EmptyBuffer error if CircularBuffer is empty.");
    }

    pub fn clear(&mut self) {
        todo!("Clear the CircularBuffer.");
    }

    pub fn overwrite(&mut self, _element: T) {
        todo!("Write the passed element to the CircularBuffer, overwriting the existing elements if CircularBuffer is full.");
    }
}
