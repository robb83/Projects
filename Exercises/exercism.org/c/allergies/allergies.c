#include "allergies.h"

bool is_allergic_to(allergen_t allergen, int value)
{
    return (value & (1 << allergen)) > 0;
}

allergen_list_t get_allergens(int value)
{
    allergen_list_t result = { 0 };

    bool t;
    for (int i = 0; i < ALLERGEN_COUNT; ++i) {
        t = (value & (1 << i)) > 0;
        result.allergens[i] = t;
        if (t) {
            ++result.count;
        }
    }

    return result;
}