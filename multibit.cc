#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <map>
#include <algorithm>

struct solution {
    uint32_t groups[32];
    uint32_t group_count;
    uint32_t cost;
};

static void solve (const uint32_t* histogram, const uint32_t a_max_bits, uint32_t a_current_bit, solution* a_current, solution* a_best)
{
    if (a_current_bit == a_max_bits) {
        if (a_current->cost < a_best->cost) {
            memcpy(a_best, a_current, sizeof(solution));
        }
        
        return;
    }

    for (uint32_t group_size = 1; group_size <= a_max_bits - a_current_bit; ++group_size) {
        uint32_t group_cost = 0;
        uint32_t covered    = 0;

        covered = a_current_bit + group_size;
        for (uint32_t bit = a_current_bit; bit < covered; ++bit) {
            group_cost += histogram[bit];
        }

        if (covered == a_max_bits) {
            group_cost *= covered + a_current->group_count;
        } else {
            group_cost *= covered + a_current->group_count + 1;
        }

        if (group_cost + a_current->cost < a_best->cost) { // do some pruning
            a_current->cost += group_cost;
            a_current->groups[a_current->group_count] = group_size;
            ++a_current->group_count;
            solve(histogram, a_max_bits, covered, a_current, a_best);
            --a_current->group_count;
            a_current->cost -= group_cost;
        }
    }
}

uint32_t read (int32_t* values)
{
    uint32_t ret;
    for (ret = 0; scanf("%d", values + ret) == 1; ++ret);
    return ret;
}

uint32_t make_bit_histogram (uint32_t nvalues, const int32_t* values, uint32_t* histogram)
{
    uint32_t max_bits = 0;

    memset(histogram, 0, 32*4);
    for (uint32_t i = 0; i < nvalues; ++i) {
        uint32_t entry = abs(values[i]);
        uint32_t bits  = floor(log2(entry<1?1:entry)) + 1;

        ++histogram[bits - 1];
        if (bits > max_bits) {
            max_bits = bits;
        }
    }

    return max_bits;
}

uint32_t optimize (uint32_t max_bits, const uint32_t* histogram)
{
    solution initial;
    solution best;
    
    memset(&initial, 0, sizeof(initial));
    memset(&best, 0, sizeof(best));

    best.cost = static_cast<uint32_t>(-1);
    solve(histogram, max_bits, 0, &initial, &best);

    return best.cost;
}

int32_t  values[100000000]; // 100 M
uint32_t bit_histogram[32];

uint32_t groups[100000]; // 100 K
uint32_t bit_histogram2[32];

int main ()
{
    uint32_t nvalues    = read(values);
    uint32_t max_bits   = make_bit_histogram(nvalues, values, bit_histogram);
    uint32_t cost       = optimize(max_bits, bit_histogram) + nvalues;

    printf("%u\n", nvalues);
    printf("%u (%u)\n", cost, cost / 8);
    printf("%lf\n", 1.0 * cost / nvalues);

    return 0;
}

