
#include <stdint.h>
#include <cstdio>
#include <list>
#include <algorithm>
#include <string>
#include <vector>

typedef std::pair<uint32_t, uint32_t> fast_huff;

struct huff {
    uint32_t weight;
    huff* left;
    huff* right;

    huff (uint32_t a, huff* b, huff* c) { weight = a; left = b; right = c; }

    uint64_t bits (uint32_t depth = 0) const {
        if (left && right) return left->bits(depth + 1) + right->bits(depth + 1);
        else return weight * depth;
    }

    void print (const std::string& a_str) {
        if (left && right) left->print(a_str + "1"), right->print(a_str + "0");
        else printf("%u %s\n", weight, a_str.c_str());
    }

    static bool less (const huff* a, const huff* b) { return a->weight < b->weight; }
    static bool greater (const huff* a, const huff* b) { return a->weight > b->weight; }
};

int slow_huff ()
{
    uint32_t val;
    uint32_t sum = 0;
    std::list<huff*> values;

    while (scanf("%u", &val) == 1) {
        values.push_back(new huff(val, NULL, NULL));
        sum += val;
    }

    while (values.size() > 1) {
        values.sort(huff::less);
        huff* a = values.front(); values.pop_front();
        huff* b = values.front(); values.pop_front();
        values.push_back(new huff(a->weight + b->weight, a, b));
    }

    printf("%u\n", sum);
    printf("%llu\n", values.front()->bits());
    printf("%lf\n", 1.0 * values.front()->bits() / sum);

    values.front()->print("");

    return 0;
}

int fast_huffman ()
{
    uint32_t val;
    uint32_t sum = 0;
    uint32_t cost = 0;

    std::vector<fast_huff> values;

    values.reserve(1 << 10);
    while (scanf("%u", &val) == 1) {
        values.push_back(std::make_pair(val, 0));
        sum += val;
    }

    std::make_heap(values.begin(), values.end(), std::greater<fast_huff>());

    uint32_t size = values.size();
    while (size > 1) {
        fast_huff a = values[0]; pop_heap(values.begin(), values.begin() + size--, std::greater<fast_huff>());
        fast_huff b = values[0]; pop_heap(values.begin(), values.begin() + size--, std::greater<fast_huff>());
        fast_huff c(a.first + b.first, a.first + b.first + a.second + b.second);

        values[size++] = c;
        std::push_heap(values.begin(), values.begin() + size, std::greater<fast_huff>());
    }

    cost = values[0].second;
    printf("%u\n", sum);
    printf("%u (%u)\n", cost, cost / 8);
    printf("%lf\n", 1.0 * cost / sum);

    return 0;
}

int main () {
    return fast_huffman();
}

