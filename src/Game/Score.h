#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

class Score
{
    public:
        Score();
        void increment();
        void decrement();
        int get_score();

    private:
        unsigned int score;
};

#endif // SCORE_H_INCLUDED
