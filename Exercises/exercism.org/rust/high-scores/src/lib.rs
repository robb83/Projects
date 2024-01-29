#[derive(Debug)]
pub struct HighScores
{
    _scores: Vec::<u32>,
}

impl HighScores {
    pub fn new(scores: &[u32]) -> Self {
        HighScores { _scores: scores.to_vec() }
    }

    pub fn scores(&self) -> &[u32] {
        &self._scores[..]
    }

    pub fn latest(&self) -> Option<u32> {
        self._scores.iter().last().copied()
    }

    pub fn personal_best(&self) -> Option<u32> {
        self._scores.iter().max().copied()
    }

    pub fn personal_top_three(&self) -> Vec<u32> {
        let mut result = self._scores.to_vec();
        result.sort_by(|a, b| b.cmp(a));
        result.truncate(3);
        return result;
    }
}
