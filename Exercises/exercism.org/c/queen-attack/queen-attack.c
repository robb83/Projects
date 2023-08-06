#include "queen_attack.h"
#include <stdlib.h>

attack_status_t can_attack(position_t queen_1, position_t queen_2)
{
    if ((queen_1.column == queen_2.column && queen_1.row == queen_2.row) 
        || queen_1.column > 7 || queen_2.column > 7
        || queen_1.row > 7 || queen_2.row > 7)
    {
        return INVALID_POSITION;
    }

    if (queen_1.row == queen_2.row || queen_1.column == queen_2.column) {
        return CAN_ATTACK;
    }

    uint8_t dx = abs(queen_2.column - queen_1.column);
    uint8_t dy = abs(queen_2.row - queen_1.row);

    if (dx == dy) {
        return CAN_ATTACK;
    }

    return CAN_NOT_ATTACK;
}