#include <stdio.h>
#include <stdlib.h> 
#include "moves.h"

int must_player_jump(const GameState *state, int player_color) {
    // We will fill this in later when we add jump logic!
    return 0; 
}

int can_piece_jump(const GameState *state, int r, int c) {
    // We will fill this in later when we add jump logic!
    return 0; 
}

// Checks if a specific piece has any empty diagonal squares to move into
int can_piece_move(const GameState *state, int r, int c) {
    Piece p = state->board[r][c];
    // Black moves UP the board (-1 row), Red moves DOWN (+1 row)
    int dir = (p.color == BLACK) ? -1 : 1; 

    // Check forward-left diagonal
    if (r + dir >= 0 && r + dir < BOARD_SIZE && c - 1 >= 0 && c - 1 < BOARD_SIZE) {
        if (state->board[r + dir][c - 1].color == EMPTY) return 1;
    }
    // Check forward-right diagonal
    if (r + dir >= 0 && r + dir < BOARD_SIZE && c + 1 >= 0 && c + 1 < BOARD_SIZE) {
        if (state->board[r + dir][c + 1].color == EMPTY) return 1;
    }
    
    // Kings can also move backwards
    if (p.is_king) {
        int back_dir = -dir;
        if (r + back_dir >= 0 && r + back_dir < BOARD_SIZE && c - 1 >= 0 && c - 1 < BOARD_SIZE) {
            if (state->board[r + back_dir][c - 1].color == EMPTY) return 1;
        }
        if (r + back_dir >= 0 && r + back_dir < BOARD_SIZE && c + 1 >= 0 && c + 1 < BOARD_SIZE) {
            if (state->board[r + back_dir][c + 1].color == EMPTY) return 1;
        }
    }
    return 0; // Piece is completely blocked
}

// Validates the destination typed by the player
Move get_full_move(const GameState *state, Move m) {
    Piece p = state->board[m.from_row][m.from_col];
    int dir = (p.color == BLACK) ? -1 : 1;

    int row_diff = m.to_row - m.from_row;
    int col_diff = abs(m.to_col - m.from_col);

    // Standard 1-step diagonal move forward
    if (row_diff == dir && col_diff == 1 && state->board[m.to_row][m.to_col].color == EMPTY) {
        m.jumped_row = -1;
        m.jumped_col = -1;
        return m;
    }

    // King 1-step diagonal move backward
    if (p.is_king && row_diff == -dir && col_diff == 1 && state->board[m.to_row][m.to_col].color == EMPTY) {
        m.jumped_row = -1;
        m.jumped_col = -1;
        return m;
    }

    // If it's not a valid move, return -2 to trigger the illegal move error in input.c
    m.jumped_row = -2;
    return m;
}

// Actually moves the piece on the board
void apply_move(GameState *state, Move m) {
    // Pick up the piece from the starting square
    Piece p = state->board[m.from_row][m.from_col];
    
    // Empty the starting square
    state->board[m.from_row][m.from_col].color = EMPTY;
    state->board[m.from_row][m.from_col].is_king = 0;
    
    // Place the piece on the destination square
    state->board[m.to_row][m.to_col] = p;

    // TODO: We will add logic to remove jumped pieces and crown kings here later!
}