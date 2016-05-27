#include <stdint.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

int main ()
{
  std::vector<int32_t> T;
  std::vector<int32_t> rotations;

  auto cmp_rotations = [&](int32_t a, int32_t b) {
    int32_t i = a, j = b;
    while (T[i] == T[j]) {
      i = (i + 1) % T.size();
      j = (j + 1) % T.size();
      if (i == a) {
        return false;
      }
    }

    return T[i] < T[j];
  };

  int32_t v;
  for (size_t i = 0; scanf("%d", &v) == 1; ++i) {
    T.push_back(v);
    rotations.push_back(i);
  }

  std::sort(rotations.begin(), rotations.end(), cmp_rotations);

  for (auto r : rotations) {
    printf("%d\n", T[(r + T.size() - 1) % T.size()]);
  }

  return 0;
}
