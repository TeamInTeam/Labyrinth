#include <vector>
#include <cstdlib>

int32_t game_map_scheme[5][5] = {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1}
};

std::vector<std::vector<int32_t> > Converter(int32_t game_map_scheme[5][5], size_t m, size_t n) {
    std::vector<std::vector<int32_t>> result;
    result.resize(m);
    for (size_t i = 0; i < m; i++) {
        result[i].resize(n);
        for (size_t j = 0; j < n; j++)
            result[i][j] = game_map_scheme[i][j];
    }

    return result;
}
