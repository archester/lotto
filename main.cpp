#include <iostream>
#include <functional>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <locale>
#include <chrono>
#include <cassert>

#include "drawingmachine.h"

const auto COLOR_GREEN  = std::string("\x1B[32m");
const auto COLOR_WHITE  = std::string("\x1B[37m");

const size_t DEFAULT_NUMBERS_QTY = 6;
const size_t DEFAULT_NUMBERS_TOTAL = 49;
const auto   DEFAULT_DRAWS_NUMBER = 100000000ll;

void parseParams(
        int argc, char* argv[],
        size_t& numbers_qty,
        size_t& numbers_total,
        long long& draws_number,
        size_t& hit_thresh_print)

{
    numbers_qty         = (argc > 1) ? atoi(argv[1]) : DEFAULT_NUMBERS_QTY;
    numbers_total       = (argc > 2) ? atoi(argv[2]) : DEFAULT_NUMBERS_TOTAL;
    draws_number        = (argc > 3) ? atoi(argv[3]) : DEFAULT_DRAWS_NUMBER;
    hit_thresh_print    = (argc > 4) ? atoll(argv[4]) : numbers_qty;
}

int main(int argc, char* argv[])
{
    auto start_time = std::chrono::steady_clock::now();

    /* get simulation params */
    size_t numbers_qty;
    size_t numbers_total;
    long long draws_number;
    size_t hit_thresh_print;

    parseParams(argc, argv,
                numbers_qty, numbers_total,
                draws_number, hit_thresh_print);

    assert(numbers_qty <= numbers_total);

    auto draws_count = 0ll;
    std::vector<long long> hit_counts(numbers_qty + 1);

    DrawingMachine<> drawingMachine(numbers_qty, numbers_total);

    /* draw one set of numbers that will be compared agains all other draws */
    const auto my_numbers = drawingMachine.draw_numbers(true);

    auto is_hit = [&] (int num)
    {
        return std::find(my_numbers.begin(), my_numbers.end(), num) != my_numbers.end();
        //the array where we search is sorted yet still this one is slower:
        //return std::binary_search(my_numbers.begin(), my_numbers.end(), num);
    };

    /* try to improve how numbers are being displayed */
    std::cout.imbue(std::locale(""));

    /* main simulation loop */
    while (draws_count < draws_number)
    {
        /* draw new numbers */
        auto numbers = drawingMachine.draw_numbers();

        /* calculate hits */
        auto hit_count = std::count_if(numbers.begin(), numbers.end(), is_hit);
        ++hit_counts[hit_count];

        /* print if hit threshold reached */
        if (hit_count >= hit_thresh_print)
        {
            std::sort(numbers.begin(), numbers.end());

            std::cout << std::fixed << draws_count << ")\t";
            for (auto& i: numbers)
            {
                std::cout << (is_hit(i) ? COLOR_GREEN : COLOR_WHITE) << i << "\t";
            }
            std::cout << COLOR_WHITE << "\n";
            std::cout.flush();
        }
        draws_count++;
    }

    /* print stats when done */
    std::cout << "------------------------------------------\n";
    std::cout << "TOTAL DRAWS: " << draws_count << "\n";
    size_t i = 0;
    std::for_each(hit_counts.begin(), hit_counts.end(), [&](long long& val) {
        std::cout << i++ << "\t" << val << "\t(" << (static_cast<long double>(val)/draws_count) * 100.0  << "%)" << "\n";
    });

    /* print time it all took */
    auto end_time = std::chrono::steady_clock::now();
    auto diff_time = end_time - start_time;
    std::cout << "Execution time: " << std::chrono::duration <double, std::milli> (diff_time).count() << " ms\n";

    return 0;
}

