#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE  8
#define MAX_MOVES   64
#define MAX_HISTORY 256

#define RED   0
#define BLACK 1
#define EMPTY -1  // used so every square has a color, even empty ones

typedef struct {
    int color;   // RED, BLACK, or EMPTY
    int is_king; // 1 if promoted, 0 otherwise
} Piece;

typedef struct {
    int from_row, from_col;
    int to_row,   to_col;
    int jumped_row, jumped_col; // -1 if no capture happened
} Move;

typedef struct {
    Move *moves; // heap allocated, must call free_move_list() after use
    int   count;
} MoveList;

typedef struct {
    Piece board[BOARD_SIZE][BOARD_SIZE]; // 2D array, every square is a Piece
    int   current_player;                // RED or BLACK
    int   red_count;
    int   black_count;
    Move  history[MAX_HISTORY]; // every move played so far, good for debugging
    int   history_count;
} GameState;

void init_board(GameState *state);
void render_board(const GameState *state);

#endif // BOARD_H
