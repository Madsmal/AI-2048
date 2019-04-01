//
//  main.c
//  2048_AI
//
//  Created by 张世凡 on 11/03/2019.
//  Copyright © 2019 张世凡. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "game.h"
int main()
{
    unsigned long int game_board = 0;
    srand((unsigned int)time(NULL));
    initboard(&game_board);
    init_move_effect_table();
    init_potential_merge_table();
    init_evaluation_table();
    
    while(1)
    {
        if(checkgame(game_board) != 0)
        {
            printf("game over\n");
            exit(0);
        }
        newcell(&game_board);
        printboard(game_board);
        checkmove(&game_board, computer_player(game_board), 0);
    }
    return 0;
}




