
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <map>

inline uint32_t Log2 (uint32_t a_value)
{
    register unsigned int r; // result of log2(v) will go here
    register unsigned int shift;

    r =     (a_value > 0xFFFF) << 4; a_value >>= r;
    shift = (a_value > 0xFF  ) << 3; a_value >>= shift; r |= shift;
    shift = (a_value > 0xF   ) << 2; a_value >>= shift; r |= shift;
    shift = (a_value > 0x3   ) << 1; a_value >>= shift; r |= shift;
    r |= (a_value >> 1);

    return r;
}

template<typename T>
class TwoDimArray {
    public:
        TwoDimArray (int n) { size_ = n; array_ = new T[n*n]; }
        ~TwoDimArray () { delete[] array_; }

        T& operator ()(int i, int j) {
            return array_[i*size_ + j];
        }

    protected:
        int size_;
        T* array_;
};

int main ()
{
    uint32_t i;
    uint32_t nvalues = 0;
    std::map<int32_t, uint32_t> histogram;

    while (scanf("%d", &i) == 1) {
        ++histogram[i];
        ++nvalues;
    }

    uint32_t n = histogram.size();
    fprintf(stderr, "size: %u\n", n);

    uint32_t max_groups = std::min(n, 256U);
    int32_t  values[n + 1];
    uint32_t frequencies[n + 1];

    TwoDimArray<uint32_t> best(n + 1);
    TwoDimArray<uint32_t> group(n + 1);
    uint32_t cost[max_groups + 1];

    // Linearize the thingy
    i = 1;
    for (std::map<int32_t, uint32_t>::const_iterator it = histogram.begin(); it != histogram.end(); ++it) {
        values[i] = it->first;
        frequencies[i] = it->second;
        ++i;
    }

    // Fixed cost
    for (int i = 1; i <= max_groups; ++i) {
        cost[i] = 3 * 8 * i + ceil(log2(i)) * nvalues;
    }

    group(1, 0) = 1;
    for (int g = 1; g <= max_groups; ++g) {
        for (int p = g; p <= n; ++p) {
            uint32_t cost = best(g, p - 1) + frequencies[p] * (1 + Log2(values[p] - values[group(g, p - 1)]));

            if (g == 1) {
                best(g, p) = cost;
                group(g, p) = 1;
            } else {
                // By default, we create a new group at p.
                best(g, p)  = best(g - 1, p - 1) + 1;
                group(g, p) = p;

                // Check if it's cheaper to use the previous group.
                if (p > g) {
                    // printf("(%d, %d): %d < %d\n", g, p, cost, best(g, p));
                    if (cost < best(g, p)) {
                        best(g, p)  = cost;
                        group(g, p) = group(g, p - 1);
                    }
                }
            }
        }
    }

    // for (int g = 1; g <= max_groups; ++g) {
    //     printf("%3d: ", g);
    //     for (int p = 1; p <= n; ++p) {
    //         if (p == n) {
    //             printf("%2d+%3d/%2d  ", best(g, p), cost[g], group(g, p));
    //         } else {
    //             printf("%2d/%2d  ", best(g, p), group(g, p));
    //         }
    //     }
    //     puts("");
    // }

    int32_t  groups[n];
    uint32_t best_group = 1;
    uint32_t best_cost  = best(1, n) + cost[best_group];

    for (uint32_t g = 1; g <= max_groups; ++g) {
        printf("%3u: %10u [%6u + %4u]\n", g, best(g, n) + cost[g], best(g, n), cost[g]);
        if (best(g, n) + cost[g] < best_cost) {
            best_group = g;
            best_cost  = best(g, n) + cost[g];
        }
    }

    for (uint32_t g = best_group, p = n; g > 0; --p, --g) {
        p = group(g, p);
        groups[g - 1] = values[p];
    }

    fprintf(stderr, "--- %u ---\n", best_group);
    for (int i = 0; i < best_group; ++i) {
        fprintf(stderr, "%2u: %d\n", i, groups[i]);
    }

    fprintf(stderr, "Best approximate result: %u (%u)\n", best_cost, best_cost / 8);
    fprintf(stderr, "Prefix cost: %u\n", static_cast<uint32_t>(ceil(log2(best_group)) * nvalues));
    fprintf(stderr, "Fixed cost: %u (%u)\n", cost[best_group], cost[best_group] / 8);

    return 0;
}

