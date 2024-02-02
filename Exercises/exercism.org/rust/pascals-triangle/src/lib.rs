pub struct PascalsTriangle
{
    rows:u32,
}

impl PascalsTriangle {
    pub fn new(row_count: u32) -> Self {
        PascalsTriangle { rows: row_count }
    }

    pub fn rows(&self) -> Vec<Vec<u32>> {
        
        let mut c: u32 = 1;
        let mut result = Vec::<Vec<u32>>::new();

        for x in 0 .. self.rows 
        {
            let mut row = Vec::<u32>::new();

            for y in 0 ..= x
            {
                if x == 0 || y == 0 {
                    row.push(1);        
                } else {
                    c = c * ( x - y + 1) / y;
                    row.push(c);
                }
            }

            result.push(row);
        }

        return result;
    }
}
