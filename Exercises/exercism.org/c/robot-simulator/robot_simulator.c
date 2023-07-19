#include "robot_simulator.h"

robot_status_t robot_create(robot_direction_t direction, int x, int y)
{
    robot_status_t robot_status = { direction, {x, y} };
    return robot_status;
}

void robot_move(robot_status_t *robot, const char *commands)
{
    if (!commands)
    {
        return;
    }

    int dx[4] = { 0, 1, 0, -1 };
    int dy[4] = { 1, 0, -1, 0 };

    char command;
    while ((command = *commands) != 0)
    {
        if (command == 'A')
        {
            robot->position.x += dx[robot->direction];
            robot->position.y += dy[robot->direction];
        }
        else if (command == 'L')
        {
            robot->direction = (robot->direction - 1) % 4) + 4) % 4;
        }
        else if (command == 'R')
        {
            robot->direction = (robot->direction + 1) % 4) + 4) % 4;
        }

        ++commands;
    }
}