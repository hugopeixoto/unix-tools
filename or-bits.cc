
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[])
{
    char x;
    std::vector<bool> values;

    values.reserve(100000);
    while (scanf("%c", &x) == 1) {
        values.push_back(x == '1');
    }

    int group_size = (argc > 1 ? atoi(argv[1]) : 4);

    while (values.size() > group_size) {
        while (values.size() % group_size != 0) values.push_back(false);
        std::vector<bool> new_values;
        
        for (int i = 0; i < values.size(); i += group_size) {
            bool f = false;
            for (int j = 0; j < group_size; ++j) {
                f = f || values[i + j];
            }

            if (f) {
                new_values.push_back(true);
                for (int j = 0; j < group_size; ++j) {
                    printf("%d", values[i + j]?1:0);
                }
                puts("");
            } else {
                new_values.push_back(false);
            }
        }

        values = new_values;
    }

    for (int i = 0; i < group_size; ++i) {
        printf("%d", values[i]?1:0);
    }
    puts("");

    return 0;
}
