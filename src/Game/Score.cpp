#include "Score.h"

Score::Score()
:score(0)
{

}

void Score::increment()
{
    score++;
}

void Score::decrement()
{
    score--;
}

int Score::get_score()
{
    return score;
}
