#ifndef DRAWINGMACHINE_H
#define DRAWINGMACHINE_H

#include <random>
#include <algorithm>
#include "common.h"

template <size_t numbers_draw, typename GEN = std::default_random_engine>
class DrawingMachine
{
    using rand_dev_t    = std::random_device;
    using rand_gen_t    = GEN;
    using rand_distr_t  = std::uniform_int_distribution<int>;

    rand_dev_t      rand_dev_;
    rand_gen_t      rand_gen_;
    rand_distr_t    rand_distr_;

    size_t          numbers_draw_;

public:
    DrawingMachine(size_t numbers_total);

    numbers_t<numbers_draw> draw_numbers(bool sort = false);
};


template <size_t numbers_draw, typename GEN>
DrawingMachine<numbers_draw, GEN>::DrawingMachine(size_t numbers_total):
    rand_dev_(),
    rand_gen_(rand_dev_()),
    rand_distr_(1, numbers_total),
    numbers_draw_(numbers_draw)
{
}

template <size_t numbers_draw, typename GEN>
numbers_t<numbers_draw> DrawingMachine<numbers_draw, GEN>::draw_numbers(bool sort)
{
    numbers_t<numbers_draw> numbers;

    for (size_t i = 0; i < numbers_draw;)
    {
        auto val = rand_distr_(rand_gen_);
        if (
                std::find(numbers.begin(), numbers.begin() + i, val) == (numbers.begin() + i)
           )
        {
            numbers[i++] = val;
        }
    }

    if (sort)
    {
        std::sort(numbers.begin(), numbers.end());
    }

    return numbers;
}


#endif // DRAWINGMACHINE_H
