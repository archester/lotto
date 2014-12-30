#include <iostream>
#include <functional>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <locale>
#include <chrono>

#include "drawingmachine.h"
#include "common.h"

const auto COLOR_GREEN  = std::string("\x1B[32m");
const auto COLOR_WHITE  = std::string("\x1B[37m");

const size_t NUMBERS_QTY = 6;
const size_t NUMBERS_TOTAL = 49;
const auto DRAWS_NUMBER = 100000000ll;
const size_t HIT_THRESH_PRINT = NUMBERS_QTY;

int main()
{
    auto start_time = std::chrono::steady_clock::now();

    auto draws_count = 0ll;
    std::vector<long long> hit_counts(NUMBERS_QTY + 1);

    DrawingMachine<NUMBERS_QTY> drawingMachine(NUMBERS_TOTAL);

    const auto my_numbers = drawingMachine.draw_numbers(true);
    /*assumes*/
    auto is_hit = [&] (int num)
    {
        return std::find(my_numbers.begin(), my_numbers.end(), num) != my_numbers.end();
        //the array where we search is sorted yet still this one is slower:
        //return std::binary_search(my_numbers.begin(), my_numbers.end(), num);
    };
    std::cout.imbue(std::locale(""));

    while (draws_count < DRAWS_NUMBER)
    {
        /* draw new numbers */
        auto numbers = drawingMachine.draw_numbers();

        /* calculate hits */
        auto hit_count = std::count_if(numbers.begin(), numbers.end(), is_hit);
        ++hit_counts[hit_count];

        /* print if hit threshold reached */
        if (hit_count >= HIT_THRESH_PRINT)
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
