//
//  game.c
//  2048_AI
//
//  Created by 张世凡 on 11/03/2019.
//  Copyright © 2019 张世凡. All rights reserved.
//

#include "game.h"
#include <stdlib.h>
#include <string.h>

unsigned long int _global_move_effect_table[2][TABLE_SIZE];
int _global_evaluation_table[13][TABLE_SIZE];
int _potential_merge_table[2][TABLE_SIZE];
int _global_merge_score_table[TABLE_SIZE];
int _transposition_table[HASH_TABLE_SIZE];
int _merge_score[BRUTEFORCE_SEARCH_SPACE];
int _search_depth = SEARCH_DEPTH;
int _cut_off_depth = CUT_OFF_DEPTH;
int _expectimax_search(unsigned long int* game_board, unsigned long int ori_game_board, int *total_score, int *total, int current_depth);
unsigned long int* get_global_move_effect_table(int index) {
    return _global_move_effect_table[index];
}

int checkmove(unsigned long int* game_board, char cmd, int sum)
{
    if (sum == -1 || cmd == 'x')
        return -1;
    int avail = 0;
    switch (cmd) {
        case 'd':
        {
            unsigned long int r;
            r = *game_board & 0x000000000000FFFFULL;
            if(r != _global_move_effect_table[0][r]) {
                *game_board &= 0xFFFFFFFFFFFF0000ULL;
                *game_board += _global_move_effect_table[0][r];
                avail = 1;
            }
            r = (*game_board & 0x00000000FFFF0000ULL) >> 16;
            if(r != _global_move_effect_table[0][r]) {
                *game_board &= 0xFFFFFFFF0000FFFFULL;
                *game_board += _global_move_effect_table[0][r] << 16;
                avail = 1;
            }
            r = (*game_board & 0x0000FFFF00000000ULL) >> 32;
            if(r != _global_move_effect_table[0][r]) {
                *game_board &= 0xFFFF0000FFFFFFFFULL;
                *game_board += _global_move_effect_table[0][r] << 32;
                avail = 1;
            }
            r = (*game_board & 0xFFFF000000000000ULL) >> 48;
            if(r != _global_move_effect_table[0][r]) {
                *game_board &= 0x0000FFFFFFFFFFFFULL;
                *game_board += _global_move_effect_table[0][r] << 48;
                avail = 1;
            }
            break;
        }
        case 's':
        {
            unsigned long int r;
            r = (*game_board & 0x000000000000000FULL) + ((*game_board & 0x00000000000F0000ULL) >> 12) + ((*game_board & 0x0000000F00000000ULL) >> 24) + ((*game_board & 0x000F000000000000ULL) >> 36);
            if(r != _global_move_effect_table[0][r]) {
                *game_board &= 0xFFF0FFF0FFF0FFF0ULL;
                *game_board += (_global_move_effect_table[0][r] & 0xF) + ((_global_move_effect_table[0][r] & 0xF0) << 12) + ((_global_move_effect_table[0][r] & 0xF00) << 24) + ((_global_move_effect_table[0][r] & 0xF000) << 36);
                avail = 1;
            }
            r = ((*game_board & 0x00000000000000F0ULL) >> 4) + ((*game_board & 0x0000000000F00000ULL) >> 16) + ((*game_board & 0x000000F000000000ULL) >> 28) + ((*game_board & 0x00F0000000000000ULL) >> 40);
            if(r != _global_move_effect_table[0][r]) {
                *game_board &= 0xFF0FFF0FFF0FFF0FULL;
                *game_board += ((_global_move_effect_table[0][r] & 0xF) << 4) + ((_global_move_effect_table[0][r] & 0xF0) << 16) + ((_global_move_effect_table[0][r] & 0xF00) << 28) + ((_global_move_effect_table[0][r] & 0xF000) << 40);
                avail = 1;
            }
            r = ((*game_board & 0x0000000000000F00ULL) >> 8) + ((*game_board & 0x000000000F000000ULL) >> 20) + ((*game_board & 0x00000F0000000000ULL) >> 32) + ((*game_board & 0x0F00000000000000ULL) >> 44);
            if(r != _global_move_effect_table[0][r]) {
                *game_board &= 0xF0FFF0FFF0FFF0FFULL;
                *game_board += ((_global_move_effect_table[0][r] & 0xF) << 8) + ((_global_move_effect_table[0][r] & 0xF0) << 20) + ((_global_move_effect_table[0][r] & 0xF00) << 32) + ((_global_move_effect_table[0][r] & 0xF000) << 44);
                avail = 1;
            }
            r = ((*game_board & 0x000000000000F000ULL) >> 12) + ((*game_board & 0x00000000F0000000ULL) >> 24) + ((*game_board & 0x0000F00000000000ULL) >> 36) + ((*game_board & 0xF000000000000000ULL) >> 48);
            if(r != _global_move_effect_table[0][r]) {
                *game_board &= 0x0FFF0FFF0FFF0FFFULL;
                *game_board += ((_global_move_effect_table[0][r] & 0xF) << 12) + ((_global_move_effect_table[0][r] & 0xF0) << 24) + ((_global_move_effect_table[0][r] & 0xF00) << 36) + ((_global_move_effect_table[0][r] & 0xF000) << 48);
                avail = 1;
            }
            break;
        }
        case 'a':
        {
            unsigned long int r;
            r = *game_board & 0x000000000000FFFFULL;
            if(r != _global_move_effect_table[1][r]) {
                *game_board &= 0xFFFFFFFFFFFF0000ULL;
                *game_board += _global_move_effect_table[1][r];
                avail = 1;
            }
            r = (*game_board & 0x00000000FFFF0000ULL) >> 16;
            if(r != _global_move_effect_table[1][r]) {
                *game_board &= 0xFFFFFFFF0000FFFFULL;
                *game_board += _global_move_effect_table[1][r] << 16;
                avail = 1;
            }
            r = (*game_board & 0x0000FFFF00000000ULL) >> 32;
            if(r != _global_move_effect_table[1][r]) {
                *game_board &= 0xFFFF0000FFFFFFFFULL;
                *game_board += _global_move_effect_table[1][r] << 32;
                avail = 1;
            }
            r = (*game_board & 0xFFFF000000000000ULL) >> 48;
            if(r != _global_move_effect_table[1][r]) {
                *game_board &= 0x0000FFFFFFFFFFFFULL;
                *game_board += _global_move_effect_table[1][r] << 48;
                avail = 1;
            }
            break;
        }
        case 'w':
        {
            unsigned long int r;
            r = (*game_board & 0x000000000000000FULL) + ((*game_board & 0x00000000000F0000ULL) >> 12) + ((*game_board & 0x0000000F00000000ULL) >> 24) + ((*game_board & 0x000F000000000000ULL) >> 36);
            if(r != _global_move_effect_table[1][r]) {
                *game_board &= 0xFFF0FFF0FFF0FFF0ULL;
                *game_board += (_global_move_effect_table[1][r] & 0xF) + ((_global_move_effect_table[1][r] & 0xF0) << 12) + ((_global_move_effect_table[1][r] & 0xF00) << 24) + ((_global_move_effect_table[1][r] & 0xF000) << 36);
                avail = 1;
            }
            r = ((*game_board & 0x00000000000000F0ULL) >> 4) + ((*game_board & 0x0000000000F00000ULL) >> 16) + ((*game_board & 0x000000F000000000ULL) >> 28) + ((*game_board & 0x00F0000000000000ULL) >> 40);
            if(r != _global_move_effect_table[1][r]) {
                *game_board &= 0xFF0FFF0FFF0FFF0FULL;
                *game_board += ((_global_move_effect_table[1][r] & 0xF) << 4) + ((_global_move_effect_table[1][r] & 0xF0) << 16) + ((_global_move_effect_table[1][r] & 0xF00) << 28) + ((_global_move_effect_table[1][r] & 0xF000) << 40);
                avail = 1;
            }
            r = ((*game_board & 0x0000000000000F00ULL) >> 8) + ((*game_board & 0x000000000F000000ULL) >> 20) + ((*game_board & 0x00000F0000000000ULL) >> 32) + ((*game_board & 0x0F00000000000000ULL) >> 44);
            if(r != _global_move_effect_table[1][r]) {
                *game_board &= 0xF0FFF0FFF0FFF0FFULL;
                *game_board += ((_global_move_effect_table[1][r] & 0xF) << 8) + ((_global_move_effect_table[1][r] & 0xF0) << 20) + ((_global_move_effect_table[1][r] & 0xF00) << 32) + ((_global_move_effect_table[1][r] & 0xF000) << 44);
                avail = 1;
            }
            r = ((*game_board & 0x000000000000F000ULL) >> 12) + ((*game_board & 0x00000000F0000000ULL) >> 24) + ((*game_board & 0x0000F00000000000ULL) >> 36) + ((*game_board & 0xF000000000000000ULL) >> 48);
            if(r != _global_move_effect_table[1][r]) {
                *game_board &= 0x0FFF0FFF0FFF0FFFULL;
                *game_board += ((_global_move_effect_table[1][r] & 0xF) << 12) + ((_global_move_effect_table[1][r] & 0xF0) << 24) + ((_global_move_effect_table[1][r] & 0xF00) << 36) + ((_global_move_effect_table[1][r] & 0xF000) << 48);
                avail = 1;
            }
            break;
        }
        default:
            break;
    }
   
    return avail == 1 ? sum : -1;
}

void newcell(unsigned long int* game_board)
{
    int i, j, k;
    while (1)
    {
        i = rand() % 4;
        j = rand() % 4;
        if(get_block(*game_board, i, j) == 0) {
            k = (rand() % 10 == 0 ? 2 : 1);
            set_block(game_board, i, j, k);
            break;
        }
    }
}

int checkgame(unsigned long int game_board)
{
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(get_block(game_board, i, j) == 0)
                return 0;
    
    return try_random_move(game_board) == 'x' ? -1 : 0;
}


void initboard(unsigned long int* game_board) {
    *game_board = 0;
}

int get_score(unsigned long int game_board) {
    unsigned long int u = 1;
    int score = 0;
    for (int i = 0; i <= 3; i++)
        for (int j = 0; j <= 3; j++)
            if (get_block(game_board, i, j) != 0)
                score += (u << get_block(game_board, i, j));
    return score;
}

void init_move_effect_table() {
    int index = 0;
    while(index < TABLE_SIZE) {
        int arr[4];
        arr[0] = index & 15;
        arr[1] = (index >> 4) & 15;
        arr[2] = (index >> 8) & 15;
        arr[3] = (index >> 12) & 15;
        for(int i=0;i<=2;i++) {
            if(arr[i]==0) {
                int j=i;
                while(++j<=3 && arr[j]==0);
                if(j != 4) {
                    arr[i] = arr[j];
                    arr[j] = 0;
                }
            }
        }
        for(int i=0;i<=2;i++) {
            if(arr[i]!=0 && arr[i]==arr[i+1]) {
                arr[i]++;
                arr[i+1] = 0;
            }
        }
        for(int i=0;i<=2;i++) {
            if(arr[i]==0) {
                int j=i;
                while(++j<=3 && arr[j]==0);
                if(j != 4) {
                    arr[i] = arr[j];
                    arr[j] = 0;
                }
            }
        }
        _global_move_effect_table[0][index] = arr[0] + (arr[1] << 4) + (arr[2] << 8) + (arr[3] << 12);
        index++;
    }
    
    index = 0;
    while(index < TABLE_SIZE) {
        int arr[4];
        arr[3] = index & 15;
        arr[2] = (index >> 4) & 15;
        arr[1] = (index >> 8) & 15;
        arr[0] = (index >> 12) & 15;
        for(int i=0;i<=2;i++) {
            if(arr[i]==0) {
                int j=i;
                while(++j<=3 && arr[j]==0);
                if(j != 4) {
                    arr[i] = arr[j];
                    arr[j] = 0;
                }
            }
        }
        for(int i=0;i<=2;i++) {
            if(arr[i]!=0 && arr[i]==arr[i+1]) {
                arr[i]++;
                arr[i+1] = 0;
            }
        }
        for(int i=0;i<=2;i++) {
            if(arr[i]==0) {
                int j=i;
                while(++j<=3 && arr[j]==0);
                if(j != 4) {
                    arr[i] = arr[j];
                    arr[j] = 0;
                }
            }
        }
        _global_move_effect_table[1][index] = arr[3] + (arr[2] << 4) + (arr[1] << 8) + (arr[0] << 12);
        index++;
    }
}

inline void set_block(unsigned long int* game_board, unsigned long int i, unsigned long int j, unsigned long int pow) {
    unsigned long int u = 1;
    char off_set = (j<<2)+(i<<4);
    // we cannot perform u << 64 (when i=j=3), therefore calculate (u << 63 + u << 63)
    *game_board -= (*game_board & ((u << (off_set+3)) + (u << (off_set+3)) - (u << (off_set))));
    
    *game_board += (pow << (off_set));
}
/* Return the row of index */
inline unsigned int get_row(unsigned long int game_board, unsigned long int index) {
    return ((game_board >> (index << 4)) & 0xFFFF);
}
/* Return the column of index */
inline unsigned int get_column(unsigned long int game_board, unsigned long int index) {
    return (get_block(game_board, 0, index) + (get_block(game_board, 1, index) << 4) +
            (get_block(game_board, 2, index) << 8) + (get_block(game_board, 3, index) << 12));
}
/* Return the block of row i and column j */
inline unsigned char get_block(unsigned long int game_board, unsigned long int i, unsigned long int j) {
    return ((game_board >> ((j+(i<<2)) << 2)) & 0xF);
}
/* Reverse a row or column */
inline void reverse(int *value) {
    int ret = 0;
    ret += (*value & 0xF) << 12;
    ret += (*value & 0xF0) << 4;
    ret += (*value & 0xF00) >> 4;
    ret += (*value & 0xF000) >> 12;
    *value = ret;
}

void printboard(unsigned long int game_board)
{
    int i, j, x;
    for (i = 1; i <= 9; i++)
    {
        if (i % 2 == 1)
        {
            for (j = 1; j <= 21; j++)
            {
                if (j % 5 == 1)
                    printf("+");
                else
                    printf("-");
                
            }
            printf("\n");
        }
        else
        {
            printf("|");
            for (x = 0; x < 4; x++)
            {
                char block = get_block(game_board, 3-((i-1)>>1), 3-x);
                int u = 1;
                if (block != 0)
                    printf("%4d|", u << block);
                else
                    printf("    |");
            }
            printf("\n");
        }
    }
    printf("YOUR SCORE: %d\n", get_score(game_board));
}

void rotate(unsigned long int *game_board) {
    unsigned long int _game_board = 0;
    _game_board += (*game_board & 0x000000000000000FULL) << 48;
    _game_board += (*game_board & 0x00000000000000F0ULL) << 28;
    _game_board += (*game_board & 0x0000000000000F00ULL) << 8;
    _game_board += (*game_board & 0x000000000000F000ULL) >> 12;
    _game_board += (*game_board & 0x00000000000F0000ULL) << 36;
    _game_board += (*game_board & 0x0000000000F00000ULL) << 16;
    _game_board += (*game_board & 0x000000000F000000ULL) >> 4;
    _game_board += (*game_board & 0x00000000F0000000ULL) >> 24;
    _game_board += (*game_board & 0x0000000F00000000ULL) << 24;
    _game_board += (*game_board & 0x000000F000000000ULL) << 4;
    _game_board += (*game_board & 0x00000F0000000000ULL) >> 16;
    _game_board += (*game_board & 0x0000F00000000000ULL) >> 36;
    _game_board += (*game_board & 0x000F000000000000ULL) << 12;
    _game_board += (*game_board & 0x00F0000000000000ULL) >> 8;
    _game_board += (*game_board & 0x0F00000000000000ULL) >> 28;
    _game_board += (*game_board & 0xF000000000000000ULL) >> 48;
    *game_board = _game_board;
}

char computer_player(unsigned long int game_board)
{
    char cmd = 'x', cmd_arr[4], max_cmd = 0, repeated = 1;
    memset(cmd_arr, 0, 4*sizeof(char));
    if(enable_bruteforce_search(game_board)) {
        repeated = BRUTEFORCE_REPEATED;
    }
    while (--repeated >= 0) {
        reset_search_depth();
        reset_cut_off_depth();
        
        int op = open_square(game_board);
        if(op >= 8) {
            set_search_depth(5);
            set_cut_off_depth(2);
        }
        else if (op >= 6) {
            set_search_depth(5);
            set_cut_off_depth(3);
        }
        else if (op >= 2) {
            set_search_depth(6);
            set_cut_off_depth(3);
        }
        else {
            set_search_depth(8);
            set_cut_off_depth(4);
        }
        
        if(enable_bruteforce_search(game_board)) {
            set_search_depth(BRUTEFORCE_SEARCH_DEPTH);
            cmd = bruteforce_search(game_board);
            /* If bruteforce search fails, use expectimax search */
            while(try_move(game_board, cmd) == -1 && get_search_depth() > 0) {
                cmd = expectimax_search(game_board);
                set_search_depth(get_search_depth()-1);
            }
        }
        else {
            while(try_move(game_board, cmd) == -1 && get_search_depth() > 0) {
                cmd = expectimax_search(game_board);
                set_search_depth(get_search_depth()-1);
            }
        }
        
        if(get_search_depth() == 0)
            cmd = try_random_move(game_board);
        switch (cmd) {
            case 'd':
                cmd_arr[0]++;
                break;
            case 's':
                cmd_arr[1]++;
                break;
            case 'a':
                cmd_arr[2]++;
                break;
            case 'w':
                cmd_arr[3]++;
                break;
            default:
                break;
        }
    }
    for(int i=0;i<4;i++)
        if(max_cmd < cmd_arr[i]) {
            max_cmd = cmd_arr[i];
        }
    for(int i=0;i<4;i++)
        if(max_cmd == cmd_arr[i]) {
            cmd = i;
        }
    switch (cmd) {
        case 0:
            cmd = 'd';
            break;
        case 1:
            cmd = 's';
            break;
        case 2:
            cmd = 'a';
            break;
        case 3:
            cmd = 'w';
            break;
        default:
            cmd = 'x';
            break;
    }
    return cmd;
}

char try_random_move(unsigned long int game_board) {
    unsigned long int _game_board = game_board;
    if(checkmove(&game_board, 'd', 0) != -1)
        return 'd';
    game_board = _game_board;
    if(checkmove(&game_board, 's', 0) != -1)
        return 's';
    game_board = _game_board;
    if(checkmove(&game_board, 'a', 0) != -1)
        return 'a';
    game_board = _game_board;
    if(checkmove(&game_board, 'w', 0) != -1)
        return 'w';
    return 'x';
}

int try_move(unsigned long int game_board, char cmd) {
    return checkmove(&game_board, cmd, 0);
}

/*  Index 0-3: row 0-3
 Index 4-7: column 0-3
 Index 8-10: column 1-3 with reverse monotonicity
 Index 11-12: column 1-2 without closed column bonus */
void init_evaluation_table() {
    for(int index=0;index<13;index++) {
        int count = 0;
        while (count < TABLE_SIZE) {
            int score = 0, u = 1;
            int arr[4];
            arr[0] = count & 0xF;
            arr[1] = (count >> 4) & 0xF;
            arr[2] = (count >> 8) & 0xF;
            arr[3] = (count >> 12) & 0xF;
            score += (u << POTENTIAL_MERGE_BONUS) * _potential_merge_table[0][count];
            if(index <= 4) {
                for(int i=0;i<4;i++) {
                    score += (u << arr[i]);
                    if (arr[i] == 0)
                        score += (u << (OPEN_SQUARE_BONUS));
                }
            }
            if(index < 8) {
                for(int i=0;i<3;i++) {
                    if(arr[i] < arr[i+1]) {
                        int j = i;
                        while (++j <= 3) {
                            score -= (u<<arr[j]) * (NON_MONOTONICITY_PENALTY);
                        }
                        score += (u<<arr[i]) * (NON_MONOTONICITY_PENALTY);
                    }
                }
            }
            if(index >= 8 && index < 11) {
                for(int i=0;i<3;i++) {
                    if(arr[i] > arr[i+1]) {
                        int j = i+1;
                        while (--j >= 0) {
                            score -= (u<<arr[j]) * (NON_MONOTONICITY_PENALTY);
                        }
                        score += (u<<arr[i+1]) * (NON_MONOTONICITY_PENALTY);
                    }
                }
            }
            if(index == 4 || index == 5 || index == 8) {
                if(arr[0]*arr[1]*arr[2]*arr[3] != 0)
                    score += (u << (CLOSED_COLUMN_BONUS));
            }
            if(index >= 11) {
                for(int i=0;i<3;i++) {
                    if(arr[i] < arr[i+1]) {
                        int j = i;
                        while (++j <= 3) {
                            score -= (u<<arr[j]) * (NON_MONOTONICITY_PENALTY);
                        }
                        score += (u<<arr[i]) * (NON_MONOTONICITY_PENALTY);
                    }
                }
            }
            _global_evaluation_table[index][count] = score;
            count++;
        }
    }
}
void init_potential_merge_table() {
    int count = 0;
    while (count < TABLE_SIZE) {
        int arr[4], arr_merged[4], _empty = 0, _empty_merged = 0, _merge_score = 0, u = 0, _square = 0, _square_merged = 0;
        unsigned long int _row_merged = get_global_move_effect_table(0)[count];
        arr[0] = count & 0xF;
        arr[1] = (count >> 4) & 0xF;
        arr[2] = (count >> 8) & 0xF;
        arr[3] = (count >> 12) & 0xF;
        for(int i=0;i<4;i++) {
            _square += arr[i];
            _empty += (arr[i] == 0 ? 1 : 0);
        }
        if(arr[0] != 0 && arr[0] == arr[1] && arr[2] != 0 && arr[2] == arr[3]) {
            _merge_score += ((u << arr[0]) + (u << arr[2])) * 2;
        }
        arr_merged[0] = _row_merged & 0xF;
        arr_merged[1] = (_row_merged >> 4) & 0xF;
        arr_merged[2] = (_row_merged >> 8) & 0xF;
        arr_merged[3] = (_row_merged >> 12) & 0xF;
        for(int i=0;i<4;i++) {
            _square_merged += arr_merged[i];
            _empty_merged += (arr_merged[i] == 0 ? 1 : 0);
        }
        _potential_merge_table[0][count] = _square - _square_merged;
        _potential_merge_table[1][count] = _empty_merged - _empty;
        if(_merge_score == 0) {
            _merge_score += (u << (_global_merge_score_table[count] + 2));
        }
        count++;
    }
}

/*
 This evaluation makes weight distribution:
 +--+--+--+--+
 | 1| 2| 3| 4|
 +--+--+--+--+
 | 2| 6| 8|10|
 +--+--+--+--+
 | 3| 8|12|14|
 +--+--+--+--+
 | 4|10|14|16|
 +--+--+--+--+
 */
/* This function should be fast to compute */
int evaluate_0(unsigned long int game_board) {
    int max_score = MIN_SCORE;
    int score = 0;
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[4][get_column(game_board, 0)];
    score += _global_evaluation_table[5][get_column(game_board, 1)];
    score += _global_evaluation_table[6][get_column(game_board, 2)];
    score += _global_evaluation_table[7][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    rotate(&game_board);
    score = 0;
    
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[4][get_column(game_board, 0)];
    score += _global_evaluation_table[5][get_column(game_board, 1)];
    score += _global_evaluation_table[6][get_column(game_board, 2)];
    score += _global_evaluation_table[7][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    rotate(&game_board);
    score = 0;
    
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[4][get_column(game_board, 0)];
    score += _global_evaluation_table[5][get_column(game_board, 1)];
    score += _global_evaluation_table[6][get_column(game_board, 2)];
    score += _global_evaluation_table[7][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    rotate(&game_board);
    score = 0;
    
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[4][get_column(game_board, 0)];
    score += _global_evaluation_table[5][get_column(game_board, 1)];
    score += _global_evaluation_table[6][get_column(game_board, 2)];
    score += _global_evaluation_table[7][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    
    return max_score;
}

/* This function should be fast to compute */
int evaluate_1(unsigned long int game_board) {
    int max_score = MIN_SCORE;
    int score = 0;
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[4][get_column(game_board, 0)];
    score += _global_evaluation_table[8][get_column(game_board, 1)];
    score += _global_evaluation_table[9][get_column(game_board, 2)];
    score += _global_evaluation_table[10][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    rotate(&game_board);
    score = 0;
    
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[4][get_column(game_board, 0)];
    score += _global_evaluation_table[8][get_column(game_board, 1)];
    score += _global_evaluation_table[9][get_column(game_board, 2)];
    score += _global_evaluation_table[10][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    rotate(&game_board);
    score = 0;
    
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[4][get_column(game_board, 0)];
    score += _global_evaluation_table[8][get_column(game_board, 1)];
    score += _global_evaluation_table[9][get_column(game_board, 2)];
    score += _global_evaluation_table[10][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    rotate(&game_board);
    score = 0;
    
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[4][get_column(game_board, 0)];
    score += _global_evaluation_table[8][get_column(game_board, 1)];
    score += _global_evaluation_table[9][get_column(game_board, 2)];
    score += _global_evaluation_table[10][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    
    return max_score;
}

/*
 This evaluation makes weight distribution:
 +--+--+--+--+
 | 1| 2| 8| 9|
 +--+--+--+--+
 | 2| 4| 9|10|
 +--+--+--+--+
 | 3| 6|12|14|
 +--+--+--+--+
 | 4|10|14|16|
 +--+--+--+--+
 */
/* This function should be fast to compute */
int evaluate_2(unsigned long int game_board) {
    int max_score = MIN_SCORE;
    int score = 0;
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[11][get_column(game_board, 0)];
    score += _global_evaluation_table[12][get_column(game_board, 1)];
    score += _global_evaluation_table[6][get_column(game_board, 2)];
    score += _global_evaluation_table[7][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    rotate(&game_board);
    score = 0;
    
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[11][get_column(game_board, 0)];
    score += _global_evaluation_table[12][get_column(game_board, 1)];
    score += _global_evaluation_table[6][get_column(game_board, 2)];
    score += _global_evaluation_table[7][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    rotate(&game_board);
    score = 0;
    
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[11][get_column(game_board, 0)];
    score += _global_evaluation_table[12][get_column(game_board, 1)];
    score += _global_evaluation_table[6][get_column(game_board, 2)];
    score += _global_evaluation_table[7][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    rotate(&game_board);
    score = 0;
    
    score += _global_evaluation_table[0][get_row(game_board, 0)];
    score += _global_evaluation_table[1][get_row(game_board, 1)];
    score += _global_evaluation_table[2][get_row(game_board, 2)];
    score += _global_evaluation_table[3][get_row(game_board, 3)];
    score += _global_evaluation_table[11][get_column(game_board, 0)];
    score += _global_evaluation_table[12][get_column(game_board, 1)];
    score += _global_evaluation_table[6][get_column(game_board, 2)];
    score += _global_evaluation_table[7][get_column(game_board, 3)];
    max_score = score > max_score ? score : max_score;
    
    return max_score;
}

int open_square(unsigned long int game_board) {
    int ret = 0, board, i = 16;
    unsigned long int _game_board = game_board;
    while (--i >= 0) {
        board = _game_board & 0xF;
        _game_board >>= 4;
        ret += (board == 0 ? 1 : 0);
    }
    /* consider the potential merge squares */
    int row_potential_merge = (_potential_merge_table[1][get_row(game_board, 0)] + _potential_merge_table[1][get_row(game_board, 1)] + _potential_merge_table[1][get_row(game_board, 2)] + _potential_merge_table[1][get_row(game_board, 3)]);
    int column_potential_merge = (_potential_merge_table[1][get_column(game_board, 0)] + _potential_merge_table[1][get_column(game_board, 1)] + _potential_merge_table[1][get_column(game_board, 2)] + _potential_merge_table[1][get_column(game_board, 3)]);
    ret += (row_potential_merge > column_potential_merge ? row_potential_merge : column_potential_merge);
    return ret;
}

int max_square(unsigned long int game_board) {
    int max_square = -1, block;
    while (game_board > 0) {
        block = game_board & 0xF;
        max_square = max_square > block ? max_square : block;
        game_board >>= 4;
    }
    return max_square;
}

int max_square_substract_max(unsigned long int *game_board) {
    int max_square = -1, _i = -1, _j = -1;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(get_block(*game_board, i, j) > max_square) {
                max_square = get_block(*game_board, i, j);
                _i = i;
                _j = j;
            }
    set_block(game_board, _i, _j, 0);
    return max_square;
}

int merge_bonus(unsigned long int cur_game_board, unsigned long int ori_game_board) {
    int cur = 0, ori = 0, u = 1;
    while (cur_game_board > 0) {
        cur += (u << (cur_game_board & 0xF)) * (cur_game_board & 0xF);
        cur_game_board >>= 4;
    }
    while (ori_game_board > 0) {
        ori += (u << (cur_game_board & 0xF)) * (cur_game_board & 0xF);
        ori_game_board >>= 4;
    }
    return cur - ori;
}

int* get_global_evaluation_table(int index) {
    return _global_evaluation_table[index];
}
int* get_global_merge_score_table() {
    return _global_merge_score_table;
}

void set_search_depth(int d) {
    _search_depth = d;
}
int get_search_depth(void) {
    return _search_depth;
}
void reset_search_depth(void) {
    _search_depth = SEARCH_DEPTH;
}
void set_cut_off_depth(int d) {
    _cut_off_depth = d;
}
int get_cut_off_depth(void) {
    return _cut_off_depth;
}
void reset_cut_off_depth(void) {
    _cut_off_depth = CUT_OFF_DEPTH;
}
char expectimax_search(unsigned long int game_board) {
    unsigned long int _game_board = game_board;
    char cmd = 'x';
    int _total_score = 0;
    int _total = 0, _max_expectation_score = DEFAULT_SCORE, ret[4];
    for(int i=0;i<4;i++)
        ret[i] = MIN_SCORE;
    if(checkmove(&game_board, 'd', 0) != -1) {
        _total_score = 0;
        _total = 0;
        ret[0] = _expectimax_search(&game_board, _game_board, &_total_score, &_total, 1);
        if(ret[0] > _max_expectation_score) {
            _max_expectation_score = ret[0];
            cmd = 'd';
        }
    }
    /* reset */
    game_board = _game_board;
    
    if(checkmove(&game_board, 's', 0) != -1) {
        _total_score = 0;
        _total = 0;
        ret[1] = _expectimax_search(&game_board, _game_board, &_total_score, &_total, 1);
        if(ret[1] > _max_expectation_score) {
            _max_expectation_score = ret[1];
            cmd = 's';
        }
    }
    /* reset */
    game_board = _game_board;
    
    if(checkmove(&game_board, 'a', 0) != -1) {
        _total_score = 0;
        _total = 0;
        ret[2] = _expectimax_search(&game_board, _game_board, &_total_score, &_total, 1);
        if(ret[2] > _max_expectation_score) {
            _max_expectation_score = ret[2];
            cmd = 'a';
        }
    }
    /* reset */
    game_board = _game_board;
    
    if(checkmove(&game_board, 'w', 0) != -1) {
        _total_score = 0;
        _total = 0;
        ret[3] = _expectimax_search(&game_board, _game_board, &_total_score, &_total, 1);
        if(ret[3] > _max_expectation_score) {
            _max_expectation_score = ret[3];
            cmd = 'w';
        }
    }
    /* reset */
    game_board = _game_board;
    
    return cmd;
}

int _expectimax_search(unsigned long int* game_board, unsigned long int ori_game_board, int *total_score, int *total, int current_depth) {
    if(current_depth == get_search_depth())
    {
        (*total)++;
        *total_score += evaluate_0(*game_board);
        return RETURN_DEFAULT;
    }
    
    if(current_depth < _cut_off_depth) {
        for(int i=0;i<=3;i++) {
            for(int j=0;j<=3;j++) {
                if(get_block(*game_board, i, j) == 0) {
                    
                    set_block(game_board, i, j, 1);
                    
                    int _total_score_arr[4], _total_arr[4];
                    memset(_total_arr, 0, 4*sizeof(int));
                    memset(_total_score_arr, 0, 4*sizeof(int));
                    
                    unsigned long int _game_board = *game_board;
                    int _total_score = 0, _total = 0;
                    if(checkmove(game_board, 'd', 0) != -1) {
                        _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
                        *game_board = _game_board;
                        _total_score_arr[0] = _total_score;
                        _total_arr[0] = _total;
                        _total_score = 0;
                        _total = 0;
                    }
                    
                    if(checkmove(game_board, 's', 0) != -1) {
                        _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
                        *game_board = _game_board;
                        _total_score_arr[1] = _total_score;
                        _total_arr[1] = _total;
                        _total_score = 0;
                        _total = 0;
                    }
                    
                    if(checkmove(game_board, 'a', 0) != -1) {
                        _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
                        *game_board = _game_board;
                        _total_score_arr[2] = _total_score;
                        _total_arr[2] = _total;
                        _total_score = 0;
                        _total = 0;
                    }
                    
                    if(checkmove(game_board, 'w', 0) != -1) {
                        _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
                        *game_board = _game_board;
                        _total_score_arr[3] = _total_score;
                        _total_arr[3] = _total;
                        _total_score = 0;
                        _total = 0;
                    }
                    int _max_expectation_score = MIN_SCORE;
                    if (_total_arr[0] != 0 && _total_score_arr[0]/_total_arr[0] > _max_expectation_score)
                        _max_expectation_score = _total_score_arr[0]/_total_arr[0];
                    if (_total_arr[1] != 0 && _total_score_arr[1]/_total_arr[1] > _max_expectation_score)
                        _max_expectation_score = _total_score_arr[1]/_total_arr[1];
                    if (_total_arr[2] != 0 && _total_score_arr[2]/_total_arr[2] > _max_expectation_score)
                        _max_expectation_score = _total_score_arr[2]/_total_arr[2];
                    if (_total_arr[3] != 0 && _total_score_arr[3]/_total_arr[3] > _max_expectation_score)
                        _max_expectation_score = _total_score_arr[3]/_total_arr[3];
                    
                    *total_score += _max_expectation_score * .9;
                    
                    set_block(game_board, i, j, 2);
                    memset(_total_arr, 0, 4*sizeof(int));
                    memset(_total_score_arr, 0, 4*sizeof(int));
                    
                    _total_score = 0;
                    _total = 0;
                    if(checkmove(game_board, 'd', 0) != -1) {
                        _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
                        *game_board = _game_board;
                        _total_score_arr[0] = _total_score;
                        _total_arr[0] = _total;
                        _total_score = 0;
                        _total = 0;
                    }
                    
                    if(checkmove(game_board, 's', 0) != -1) {
                        _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
                        *game_board = _game_board;
                        _total_score_arr[1] = _total_score;
                        _total_arr[1] = _total;
                        _total_score = 0;
                        _total = 0;
                    }
                    
                    if(checkmove(game_board, 'a', 0) != -1) {
                        _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
                        *game_board = _game_board;
                        _total_score_arr[2] = _total_score;
                        _total_arr[2] = _total;
                        _total_score = 0;
                        _total = 0;
                    }
                    
                    if(checkmove(game_board, 'w', 0) != -1) {
                        _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
                        *game_board = _game_board;
                        _total_score_arr[3] = _total_score;
                        _total_arr[3] = _total;
                        _total_score = 0;
                        _total = 0;
                    }
                    _max_expectation_score = MIN_SCORE;
                    if (_total_arr[0] != 0 && _total_score_arr[0]/_total_arr[0] > _max_expectation_score)
                        _max_expectation_score = _total_score_arr[0]/_total_arr[0];
                    if (_total_arr[1] != 0 && _total_score_arr[1]/_total_arr[1] > _max_expectation_score)
                        _max_expectation_score = _total_score_arr[1]/_total_arr[1];
                    if (_total_arr[2] != 0 && _total_score_arr[2]/_total_arr[2] > _max_expectation_score)
                        _max_expectation_score = _total_score_arr[2]/_total_arr[2];
                    if (_total_arr[3] != 0 && _total_score_arr[3]/_total_arr[3] > _max_expectation_score)
                        _max_expectation_score = _total_score_arr[3]/_total_arr[3];
                    
                    *total_score += _max_expectation_score * .1;
                    /* update visited state and transposition table */
                    (*total)++;
                    /* reset */
                    set_block(game_board, i, j, 0);
                }
            }
        }
    }
    else {
        unsigned long int _game_board_cp = *game_board;
        newcell(game_board);
        int _total_score_arr[4], _total_arr[4];
        memset(_total_arr, 0, 4*sizeof(int));
        memset(_total_score_arr, 0, 4*sizeof(int));
        
        unsigned long int _game_board = *game_board;
        int _total_score = 0, _total = 1;
        if(checkmove(game_board, 'd', 0) != -1) {
            _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
            *game_board = _game_board;
            _total_score_arr[0] = _total_score;
            _total_arr[0] = _total;
            _total_score = 0;
            _total = 0;
        }
        
        if(checkmove(game_board, 's', 0) != -1) {
            _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
            *game_board = _game_board;
            _total_score_arr[1] = _total_score;
            _total_arr[1] = _total;
            _total_score = 0;
            _total = 0;
        }
        
        if(checkmove(game_board, 'a', 0) != -1) {
            _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
            *game_board = _game_board;
            _total_score_arr[2] = _total_score;
            _total_arr[2] = _total;
            _total_score = 0;
            _total = 0;
        }
        
        if(checkmove(game_board, 'w', 0) != -1) {
            _expectimax_search(game_board, ori_game_board, &_total_score, &_total, current_depth+1);
            *game_board = _game_board;
            _total_score_arr[3] = _total_score;
            _total_arr[3] = _total;
            _total_score = 0;
            _total = 0;
        }
        int _max_expectation_score = MIN_SCORE;
        if (_total_arr[0] != 0 && _total_score_arr[0]/_total_arr[0] > _max_expectation_score)
            _max_expectation_score = _total_score_arr[0]/_total_arr[0];
        if (_total_arr[1] != 0 && _total_score_arr[1]/_total_arr[1] > _max_expectation_score)
            _max_expectation_score = _total_score_arr[1]/_total_arr[1];
        if (_total_arr[2] != 0 && _total_score_arr[2]/_total_arr[2] > _max_expectation_score)
            _max_expectation_score = _total_score_arr[2]/_total_arr[2];
        if (_total_arr[3] != 0 && _total_score_arr[3]/_total_arr[3] > _max_expectation_score)
            _max_expectation_score = _total_score_arr[3]/_total_arr[3];
        
        *total_score += _max_expectation_score;
        (*total)++;
        /* reset */
        *game_board = _game_board_cp;
    }
    return *total_score/(*total);
}

int enable_bruteforce_search(unsigned long int game_board) {
    int score = get_score(game_board);
    char max_block = max_square_substract_max(&game_board);
    while(score > 0 && max_block > 10) {
        int u = 1;
        if(score >= (u<<(max_block+1))-(_search_depth<<1))
            return 1;
        score -= (u<<max_block);
        max_block = max_square_substract_max(&game_board);
    }
    return 0;
}

void _bruteforce_search(unsigned long int* game_board, unsigned long int ori_game_board, int *count, int trigger, int current_depth) {
    
    if(current_depth == get_search_depth())
    {
        if (trigger != -1)
            _merge_score[*count] = merge_bonus(*game_board, ori_game_board);
        (*count)++;
        return;
    }
    /* game over */
    if(checkgame(*game_board) != 0) {
        trigger = -1;
    }
    if(trigger != -1) {
        unsigned long int _game_board_cp = *game_board;
        newcell(game_board);
        unsigned long int _game_board = *game_board;
        _bruteforce_search(game_board, ori_game_board, count, checkmove(game_board, 'd', 0), current_depth+1);
        *game_board = _game_board;
        _bruteforce_search(game_board, ori_game_board, count, checkmove(game_board, 's', 0), current_depth+1);
        *game_board = _game_board;
        _bruteforce_search(game_board, ori_game_board, count, checkmove(game_board, 'a', 0), current_depth+1);
        *game_board = _game_board;
        _bruteforce_search(game_board, ori_game_board, count, checkmove(game_board, 'w', 0), current_depth+1);
        *game_board = _game_board;
        /* reset */
        *game_board = _game_board_cp;
    }
    else {
        _bruteforce_search(game_board, ori_game_board, count, -1, current_depth+1);
        _bruteforce_search(game_board, ori_game_board, count, -1, current_depth+1);
        _bruteforce_search(game_board, ori_game_board, count, -1, current_depth+1);
        _bruteforce_search(game_board, ori_game_board, count, -1, current_depth+1);
    }
}
char bruteforce_search(unsigned long int game_board) {
    memset(_merge_score, 0, BRUTEFORCE_SEARCH_SPACE*sizeof(int));
    
    unsigned long int _game_board = game_board;
    char cmd = 'x';
    int max_merge_score = 2*BRUTEFORCE_SEARCH_DEPTH, max_merge_achieved = 1, merge_achieved[4];
    int count = 0;
    int index = 0, _index = 0;
    double relax_ratio = 1.0;
    memset(merge_achieved, 0, 4*sizeof(int));
    _bruteforce_search(&game_board, _game_board, &count, checkmove(&game_board, 'd', 0), 1);
    game_board = _game_board;
    _bruteforce_search(&game_board, _game_board, &count, checkmove(&game_board, 's', 0), 1);
    game_board = _game_board;
    _bruteforce_search(&game_board, _game_board, &count, checkmove(&game_board, 'a', 0), 1);
    game_board = _game_board;
    _bruteforce_search(&game_board, _game_board, &count, checkmove(&game_board, 'w', 0), 1);
    game_board = _game_board;
    for(int i=0;i<BRUTEFORCE_SEARCH_SPACE;i++)
        if(max_merge_score < _merge_score[i])
            max_merge_score =_merge_score[i];
    max_merge_score *= relax_ratio;
    
    index += BRUTEFORCE_SEARCH_SPACE / 4;
    for(;_index<index;_index++)
        if(_merge_score[_index] >= max_merge_score)
            merge_achieved[0]++;
    index += BRUTEFORCE_SEARCH_SPACE / 4;
    for(;_index<index;_index++)
        if(_merge_score[_index] >= max_merge_score)
            merge_achieved[1]++;
    index += BRUTEFORCE_SEARCH_SPACE / 4;
    for(;_index<index;_index++)
        if(_merge_score[_index] >= max_merge_score)
            merge_achieved[2]++;
    index += BRUTEFORCE_SEARCH_SPACE / 4;
    for(;_index<index;_index++)
        if(_merge_score[_index] >= max_merge_score)
            merge_achieved[3]++;
    
    for(int i = 0; i < 4; i++)
        max_merge_achieved = max_merge_achieved < merge_achieved[i] ? merge_achieved[i] : max_merge_achieved;
    if(max_merge_achieved == merge_achieved[0])
        cmd = 'd';
    else if (max_merge_achieved == merge_achieved[1])
        cmd = 's';
    else if (max_merge_achieved == merge_achieved[2])
        cmd = 'a';
    else if (max_merge_achieved == merge_achieved[3])
        cmd = 'w';
    
    return cmd;
}


void reset_transposition_table() {
    memset(_transposition_table, 0, HASH_TABLE_SIZE*sizeof(int));
}
int* get_transposition_table() {
    return _transposition_table;
}

