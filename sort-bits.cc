#include <vector>
#include <stdio.h>
#include <algorithm>

int main ()
{
    std::vector<std::pair<int, int> > values;
    std::vector<std::pair<int, int> > buffer;

    int n;
    while (scanf("%d", &n) == 1) {
        values.push_back(std::make_pair(n, values.size()));
    }

    std::sort(values.begin(), values.end());
    n = values.size();
    buffer.resize(n);

    for (int size = 2; size < n*2; size *= 2) {
        for (int i = 0; i < n; i += size) {
            int a = i;
            int b = i + size/2;
            int c = i + size/2;
            int d = (i + size < n ? i + size : n);
            int e = i;

            while (a < b && c < d) {
                //++comparisons;
                printf("%d", values[c].second < values[a].second ? 0 : 1);
                if (values[c].second < values[a].second) {
                    buffer[e++] = values[c++];
                } else {
                    buffer[e++] = values[a++];
                }
            }

            while (a < b) { buffer[e++] = values[a++]; }
            while (c < d) { buffer[e++] = values[c++]; }
            for (int j = i; j < e; ++j) values[j] = buffer[j];
        }
    }

    return 0;
}
