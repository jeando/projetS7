#include "Score.h"

Score::Score()
:score(0)
{

}

void Score::increment()
{
    score++;
}

int Score::get_score()
{
    return score;
}
