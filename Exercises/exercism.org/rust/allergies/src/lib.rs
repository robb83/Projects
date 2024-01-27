pub struct Allergies
{
    score:u32,
}

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum Allergen {
    Eggs = 1,
    Peanuts = 2,
    Shellfish = 4,
    Strawberries = 8,
    Tomatoes = 16,
    Chocolate = 32,
    Pollen = 64,
    Cats = 128,
}

impl From<u32> for Allergen {
    fn from(value: u32) -> Self {
        match value {
            1 => Allergen::Eggs,
            2 => Allergen::Peanuts,
            4 => Allergen::Shellfish,
            8 => Allergen::Strawberries,
            16 => Allergen::Tomatoes,
            32 => Allergen::Chocolate,
            64 => Allergen::Pollen,
            128 => Allergen::Cats,
            _ => panic!("Invalid value for conversion to Allergen enum"),
        }
    }
}

impl Allergies {
    pub fn new(score: u32) -> Self {
        return Allergies { score: score };
    }

    pub fn is_allergic_to(&self, allergen: &Allergen) -> bool {
        return self.score & ( *allergen as u32) > 0;
    }

    pub fn allergies(&self) -> Vec<Allergen> {
        let mut result = Vec::<Allergen>::new();

        for i in 0..8
        {
            let v:u32 = 1 << i;
            if (self.score & v) > 0
            {
                result.push(v.into());
            }
        }

        return result;
    }
}
