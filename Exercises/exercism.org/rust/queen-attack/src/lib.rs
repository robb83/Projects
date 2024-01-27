#[derive(Debug)]
pub struct ChessPosition
{
    row:i32,
    column:i32,
}

#[derive(Debug)]
pub struct Queen
{
    pos:ChessPosition,
}

impl ChessPosition {
    pub fn new(rank: i32, file: i32) -> Option<Self> {
        if file < 0 || file > 7 || rank < 0 || rank > 7
        {
            return None;
        }

        return Some(ChessPosition{ row: rank, column: file });
    }
}

impl Queen {
    pub fn new(position: ChessPosition) -> Self {
        return Queen { pos: position };
    }

    pub fn can_attack(&self, other: &Queen) -> bool {
        if self.pos.row == other.pos.row || self.pos.column == other.pos.column 
        {
            return true;
        }

        let dx = (other.pos.column - self.pos.column).abs();
        let dy = (other.pos.row - self.pos.row).abs();

        if dx == dy
        {
            return true;
        }

        return false;
    }
}
