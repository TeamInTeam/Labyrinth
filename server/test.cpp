#include <inttypes.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iostream>

// Testing.
#include "./server/game/game.h"

void DrawMap(int32_t mapp[6][6]);

int main() {

    int32_t arr[6][6]={{11,11,11,11,11,11},
                       {11,00,00,00,00,11},
                       {11,00,11,11,00,11},
                       {11,00,00,11,00,11},
                       {11,00,00,11,00,11},
                       {11,11,11,11,11,11}};
    LabyrinthMap game_map(6, 6, arr);
    int32_t mapp[6][6]={{1,1,1,1,1,1},
                       {1,0,0,0,0,1},
                       {1,0,1,1,0,1},
                       {1,0,0,1,0,1},
                       {1,0,0,1,0,1},
                       {1,1,1,1,1,1}};
    int32_t player_x = 4, player_y = 1;
    mapp[4][1] = 7;
    

    std::string command;
    
    while(true) {
        DrawMap(mapp);
        
        std::cin >> command;

        // Go Up/Down/Left/Right.
        if (command == "w" && game_map.CanGo(player_x - 1, player_y)) {
            mapp[player_x][player_y] = 0;
            player_x--;
            mapp[player_x][player_y] = 7;
        }
        if (command == "s" && game_map.CanGo(player_x +1, player_y)) {
            mapp[player_x][player_y] = 0;
            player_x++;
            mapp[player_x][player_y] = 7;
        }
        if (command == "a" && game_map.CanGo(player_x , player_y-1)) {
            mapp[player_x][player_y] = 0;
            player_y--;
            mapp[player_x][player_y] = 7;
        }
        if (command == "d" && game_map.CanGo(player_x, player_y+1)) {
            mapp[player_x][player_y] = 0;
            player_y++;
            mapp[player_x][player_y] = 7;
        }
    }
    return 0;
}

void DrawMap(int32_t mapp[6][6]) {
    std::cout << "\033[2J\033[1;1H";
    for (int i = 0; i < 6;i++) {
        for (int j = 0; j < 6; j++)
            std::cout << mapp[i][j];
        std::cout << std::endl;
    }
    std::cout << "Command: ";
}
