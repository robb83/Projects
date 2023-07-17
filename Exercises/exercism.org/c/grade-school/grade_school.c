#include "grade_school.h"
#include <stdio.h>

void init_roster(roster_t* actual)
{
    actual->count = 0;
}

roster_t get_grade(roster_t* roster, uint8_t desired_grade)
{
    roster_t result = { 0 };
    
    for (size_t i = 0; i < roster->count; ++i)
    {
        if (roster->students[i].grade == desired_grade)
        {
            result.students[result.count++] = roster->students[i];
        }
    }

    return result;
}

bool add_student(roster_t* roster, char* name, uint8_t grade)
{
    if (roster)
    {
        if (roster->count + 1 >= MAX_STUDENTS)
        {
            return false;
        }

        // Invalid name
        if (!name || strlen(name) + 1 > MAX_NAME_LENGTH)
        {
            return false;
        }

        // Already exists
        for (size_t i = 0; i < roster->count; ++i)
        {
            if (strcmp(roster->students[i].name, name) == 0)
            {
                return false;
            }
        }

        size_t insert_here = roster->count;

        for (size_t i = 0; i < roster->count; ++i)
        {
            if (roster->students[i].grade < grade)
            {
                continue;
            }

            if (roster->students[i].grade > grade)
            {
                insert_here = i;
                break;
            }

            if (strcmp(name, roster->students[i].name) < 0)
            {
                insert_here = i;
                break;
            }
        }

        if (roster->count > 0 && insert_here < roster->count)
        {
            for (size_t  i = roster->count - 1; i >= insert_here && i < MAX_STUDENTS; --i)
            {
                roster->students[i + 1] = roster->students[i];
            }
        }

        roster->students[insert_here].grade = grade;
        strcpy(roster->students[insert_here].name, name);
        roster->count++;

        return true;
    }

    return false;
}