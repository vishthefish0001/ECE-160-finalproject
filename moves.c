#include <stdio.h>
#include <stdlib.h>
#include "moves.h"

/*
 * Helper: checks if direction (dr, dc) from (r, c) is a valid jump.
 * "opponent" is the color of the enemy piece that must occupy the middle square.
 * Returns 1 if the jump is legal, 0 otherwise.
 * A jump requires middle square (r+dr, c+dc) holds an opponent piece and landing square (r+2*dr, c+2*dc) is empty and in bounds
 */
static int can_jump_dir(const GameState *state, int r, int c,
                         int dr, int dc, int opponent) {
    int mid_r = r + dr;
    int mid_c = c + dc;
    int land_r = r + 2 * dr;
    int land_c = c + 2 * dc;

    // Bounds check on both middle and landing squares 
    if (mid_r < 0 || mid_r >= BOARD_SIZE || mid_c < 0 || mid_c >= BOARD_SIZE)
        return 0;
    if (land_r < 0 || land_r >= BOARD_SIZE || land_c < 0 || land_c >= BOARD_SIZE)
        return 0;

    // Middle must be an opponent piece, landing must be empty 
    if (state->board[mid_r][mid_c].color != opponent)
        return 0;
    if (state->board[land_r][land_c].color != EMPTY)
        return 0;

    return 1;
}

/*
 * Returns 1 if the piece at (r, c) has at least one legal jump available.
 * Regular pieces only jump forward; kings jump all four diagonals.
 *
 * Weakness: does NOT check for forced multi-jump chains after landing —
 * that is handled in main.c's game loop after apply_move().
 */
int can_piece_jump(const GameState *state, int r, int c) {
    Piece p = state->board[r][c];
    if (p.color == EMPTY) return 0;

    // Black moves up (row decreases), Red moves down (row increases) 
    int dir = (p.color == BLACK) ? -1 : 1;
    int opponent = (p.color == BLACK) ? RED : BLACK;

    // Forward-left and forward-right jumps 
    if (can_jump_dir(state, r, c, dir, -1, opponent)) return 1;
    if (can_jump_dir(state, r, c, dir,  1, opponent)) return 1;

    // Kings can also jump backward 
    if (p.is_king) {
        if (can_jump_dir(state, r, c, -dir, -1, opponent)) return 1;
        if (can_jump_dir(state, r, c, -dir,  1, opponent)) return 1;
    }

    return 0;
}


 //Returns 1 if ANY piece of player_color has a legal jump.
 // When this is true, that player MUST jump — they cannot make a simple move.
 
int must_player_jump(const GameState *state, int player_color) {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (state->board[r][c].color == player_color) {
                if (can_piece_jump(state, r, c)) return 1;
            }
        }
    }
    return 0;
}


 // Checks if a specific piece has any empty diagonal squares to step into.
 //Regular pieces only move forward; kings move in all four directions.
 
int can_piece_move(const GameState *state, int r, int c) {
    Piece p = state->board[r][c];
    int dir = (p.color == BLACK) ? -1 : 1;

    // Forward-left and forward-right
    if (r + dir >= 0 && r + dir < BOARD_SIZE) {
        if (c - 1 >= 0 && state->board[r + dir][c - 1].color == EMPTY) return 1;
        if (c + 1 < BOARD_SIZE && state->board[r + dir][c + 1].color == EMPTY) return 1;
    }

    // Kings: also check backward diagonals 
    if (p.is_king) {
        int back = -dir;
        if (r + back >= 0 && r + back < BOARD_SIZE) {
            if (c - 1 >= 0 && state->board[r + back][c - 1].color == EMPTY) return 1;
            if (c + 1 < BOARD_SIZE && state->board[r + back][c + 1].color == EMPTY) return 1;
        }
    }

    return 0;
}

/*
 * Validates the destination the player typed and fills in jumped_row/jumped_col.
 * Returns the Move with:
 *   jumped_row == -1  -> valid simple move (no capture)
 *   jumped_row >= 0   -> valid jump; jumped_row/col identify the captured piece
 *   jumped_row == -2  -> illegal move; caller should re-prompt
 *
 * DOES NOT verify that a jump was mandatory when one was available —
 * that enforcement happens in input.c before get_full_move is called.
 */
Move get_full_move(const GameState *state, Move m) {
    Piece p = state->board[m.from_row][m.from_col];
    int dir = (p.color == BLACK) ? -1 : 1;
    int opponent = (p.color == BLACK) ? RED : BLACK;

    int row_diff = m.to_row - m.from_row;
    int col_diff = m.to_col - m.from_col;  // signed; abs used for distance 
    int abs_col  = (col_diff < 0) ? -col_diff : col_diff;

    // Simple 1-step diagonal move 
    if (abs_col == 1 && state->board[m.to_row][m.to_col].color == EMPTY) {
        // Forward for regular pieces; either direction for kings 
        if (row_diff == dir || (p.is_king && row_diff == -dir)) {
            m.jumped_row = -1;
            m.jumped_col = -1;
            return m;
        }
    }

    //2 step diagonal jump
    if (abs_col == 2 && (row_diff == 2 || row_diff == -2)) {
        // The middle square is halfway between from and to 
        int mid_r = (m.from_row + m.to_row) / 2;
        int mid_c = (m.from_col + m.to_col) / 2;

        //Direction of jump must be legal for this piece type 
        int jump_dr = row_diff / 2;   // +1 or -1 
        int valid_dir = (jump_dr == dir) || (p.is_king && jump_dr == -dir);

        if (valid_dir &&
            state->board[mid_r][mid_c].color == opponent &&
            state->board[m.to_row][m.to_col].color == EMPTY) {

            m.jumped_row = mid_r;
            m.jumped_col = mid_c;
            return m;
        }
    }

    // Neither a valid step nor a valid jump 
    m.jumped_row = -2;
    return m;
}

/*
 * Executes a validated move on the board:
 *    Moves the piece from source to destination
 *    If it was a jump, removes the captured piece and updates piece count
 *    Crowns the piece if it reached the back row
 *
 * multi-jump continuation is NOT handled here.
 * After apply_move returns, main.c checks if the landed piece can jump again.
 */
void apply_move(GameState *state, Move m) {
    // Pick up the piece 
    Piece p = state->board[m.from_row][m.from_col];

    // Clear the source square 
    state->board[m.from_row][m.from_col].color  = EMPTY;
    state->board[m.from_row][m.from_col].is_king = 0;

    // Place the piece at the destination 
    state->board[m.to_row][m.to_col] = p;

    // If a jump occurred, remove the captured piece and decrement count 
    if (m.jumped_row >= 0) {
        state->board[m.jumped_row][m.jumped_col].color  = EMPTY;
        state->board[m.jumped_row][m.jumped_col].is_king = 0;

        if (p.color == BLACK)
            state->red_count--;
        else
            state->black_count--;
    }

    
     // Crowning: Black reaches row 0 (top of array = row 8 on screen),
     //Red reaches row 7 (bottom of array = row 1 on screen).
     
    if (p.color == BLACK && m.to_row == 0)
        state->board[m.to_row][m.to_col].is_king = 1;
    if (p.color == RED && m.to_row == BOARD_SIZE - 1)
        state->board[m.to_row][m.to_col].is_king = 1;
}
