#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE  8 
//8x8 board as normal
#define MAX_MOVES   64 
//maximum moves for each turn/person is 64
#define MAX_HISTORY 256
//can hold information for 264 moves

#define RED   0
#define BLACK 1
#define EMPTY -1  
//every possible state of a square has some sort of value

typedef struct {
    int color;   // RED, BLACK, or EMPTY
    int is_king; // 1 if promoted, 0 otherwise
} Piece;

typedef struct {
    int from_row, from_col; //source square
    int to_row,   to_col; //destination sqaure after moves
    int jumped_row, jumped_col; // -1 if no capture happened
} Move;

typedef struct {
    Move *moves; // points to array of moves, calls free_move_list() after use
    int   count; //current number of moves stored
} MoveList;

typedef struct {
    Piece board[BOARD_SIZE][BOARD_SIZE]; // 2D array, every index is a swuare
    int   current_player;                // RED or BLACK
    int   red_count;  //how many red pieces there are
    int   black_count; //how many black pieces
    Move  history[MAX_HISTORY]; // every move played so far
    int   history_count; //how many moves have been made so far
} GameState;

void init_board(GameState *state);
void render_board(const GameState *state);
//any files containing board.h can use these
#endif // BOARD_H
