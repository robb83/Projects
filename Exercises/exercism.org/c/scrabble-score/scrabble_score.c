#include "scrabble_score.h"

                         //  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P,  Q, R, S, T, U, V, W, X, Y,  Z 
static const int scores[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

unsigned int score(const char *word)
{

    int score = 0;
    if (word) 
    {
        char ch;
        while  ((ch = *word++) != '\0') 
        {
            if (ch >= 'A' && ch <= 'Z') {
                score += scores[ch - 'A'];
            } 
            else if (ch >= 'a' && ch <= 'z') 
            {
                score += scores[ch - 'a'];
            }
        }
    }

    return score;
}