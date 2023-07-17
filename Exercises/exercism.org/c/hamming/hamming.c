#include "hamming.h"
#include <string.h>

int compute(const char *lhs, const char *rhs)
{
    if (lhs && rhs)
    {
        int lhsl = strlen(lhs);
        int rhsl = strlen(rhs);

        if (lhsl == rhsl)
        {
            int distance = 0;
            
            for (int i = 0; i < lhsl; ++i)
            {
                if (lhs[i] != rhs[i])
                {
                    ++distance;
                }
            }

            return distance;
        }
    }

    return -1;
}