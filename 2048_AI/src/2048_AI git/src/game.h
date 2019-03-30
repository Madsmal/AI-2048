//
//  game.h
//  2048_AI
//
//  Created by 张世凡 on 11/03/2019.
//  Copyright © 2019 张世凡. All rights reserved.
//

#ifndef game_h
#define game_h

#include <stdio.h>
/* params */
#define ENABLE_HASHING 0
#define SEARCH_DEPTH 6 //6
#define CUT_OFF_DEPTH 3  //3
#define BRUTEFORCE_SEARCH_DEPTH (9)
#define BRUTEFORCE_SEARCH_SPACE (262144)
#define BRUTEFORCE_REPEATED 12
#define TEST_LOOP 20
/* heuristics */
#define OPEN_SQUARE_BONUS 8  //9
#define POTENTIAL_MERGE_BONUS 2
#define CLOSED_COLUMN_BONUS 0  //8
#define NON_MONOTONICITY_PENALTY 42
/* fixed */
#define MIN_SCORE -1000000
#define DEFAULT_SCORE MIN_SCORE+1
#define TABLE_SIZE 65536
#define HASH_TABLE_SIZE (65536 << 1)
/* return values */
#define RETURN_DEFAULT 0
#define GAME_OVER 1
#define MOVE_NOT_VALID_ERROR 10

/* Make a move specified by cmd. If the move is not valid, return -1. Given sum = -1, return immediately with -1. Otherwise, return the score gain. */
int checkmove(unsigned long int *game_board, char cmd, int sum);
/* Generate a new number. 2: 90% ; 4: 10% */
void newcell(unsigned long int *game_board);
/* Check if the game is over. If not, return 0. */
int checkgame(unsigned long int game_board);
/* Print the gameboard. */
void printboard(unsigned long int game_board);
/* Initialize gameboard. */
void initboard(unsigned long int* game_board);
/* Precompute move effect table. */
void init_move_effect_table(void);
/* Return the game score. */
int get_score(unsigned long int game_board);
/* Return the move effect table */
unsigned long int* get_global_move_effect_table(int index);
/* Set a tile of value 2 to the power @code pow */
void set_block(unsigned long int* game_board, unsigned long int i, unsigned long int j, unsigned long int pow);
/* Rotate the gameboard in anti-clock manner */
void rotate(unsigned long int *game_board);
/* Reverse a row or column */
void reverse(int *value);
/* Return the row of index */
unsigned int get_row(unsigned long int game_board, unsigned long int index);
/* Return the column of index */
unsigned int get_column(unsigned long int game_board, unsigned long int index);
/* Return the block of row i and column j */
unsigned char get_block(unsigned long int game_board, unsigned long int i, unsigned long int j);

char expectimax_search(unsigned long int game_board);

int enable_bruteforce_search(unsigned long int game_board);
char bruteforce_search(unsigned long int game_board);

void init_transposition_table(void);
void reset_transposition_table(void);
void update_transposition_table(unsigned long int game_board);
int* get_transposition_table(void);

void set_search_depth(int d);
int get_search_depth(void);
void reset_search_depth(void);

void set_cut_off_depth(int d);
int get_cut_off_depth(void);
void reset_cut_off_depth(void);

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
int evaluate_0(unsigned long int game_board);
/*
 This evaluation makes weight distribution:
 +--+--+--+--+
 | 4| 8| 9|10|
 +--+--+--+--+
 | 3| 7| 8|12|
 +--+--+--+--+
 | 2| 6| 7|14|
 +--+--+--+--+
 | 1| 3| 5|16|
 +--+--+--+--+
 May be helpful for trying to get very high score, but not stable
 compared to @code evaluate();
 */
int evaluate_1(unsigned long int game_board);
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
 Useful for merging the largest tile.
 */
int evaluate_2(unsigned long int game_board);

int* get_global_evaluation_table(int index);
int* get_global_merge_score_table(void);

/*  Initialize potential merge table and merge score table.
 Should be called after global move effect table is initialized */
void init_potential_merge_table(void);
/*  Initialize evaluation table.
 Should be called after potential merge table is initialized */
void init_evaluation_table(void);
/* Return the number of empty tiles */
int open_square(unsigned long int game_board);
/* Return the max square (as power of 2) and substract it from gameboard */
int max_square_substract_max(unsigned long int *game_board);
/* Return the max square (as power of 2) */
int max_square(unsigned long int game_board);
/* Return the bonus score for merged square */
int merge_bonus(unsigned long int cur_game_board, unsigned long int ori_game_board);

char computer_player(unsigned long int game_board);
char try_random_move(unsigned long int game_board);
int try_move(unsigned long int game_board, char cmd);
#endif /* game_h */

