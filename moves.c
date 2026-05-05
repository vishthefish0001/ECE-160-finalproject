#include <stdio.h>
#include <stdlib.h>
#include "moves.h"

//checks one diagonal direction for valid jumps
//r,c are starting squares. dr,dc is the direction to check where they are either plus/minus 1
//opponent is the color of the enemy (0 or 1)
//static cause its only needed for moves.c
static int can_jump_dir(const GameState *state, int r, int c, int dr, int dc, int opponent) {
    int mid_r = r + dr; //piece being jumped over
    int mid_c = c + dc; //its column
    int land_r = r + 2 * dr;// landing square after jump
    int land_c = c + 2 * dc;//its column

    // Bounds check on both middle and landing squares, is it invalid basically
    if (mid_r < 0 || mid_r >= BOARD_SIZE || mid_c < 0 || mid_c >= BOARD_SIZE){
        return 0;
    }
    if (land_r < 0 || land_r >= BOARD_SIZE || land_c < 0 || land_c >= BOARD_SIZE){
        return 0;
    }

    // In order to actually jump, though, the piece being jumped over HAS to be an opponent piece 
    if (state->board[mid_r][mid_c].color != opponent){
        return 0;
    }
    if (state->board[land_r][land_c].color != EMPTY){ //landing sqaure HAS to be empty (availability)
        return 0;
    }

    return 1;
}


 //Can_piece_jump checks if the piece at r,c had jumps available
 //Regular pieces only jump forward diagonals, whereas kings jump all four diagonals.
 //returns 1 when valid jump directions are found
 
int can_piece_jump(const GameState *state, int r, int c) {
    Piece p = state->board[r][c];
    if (p.color == EMPTY){
         return 0; //empty pieces cant jump
    }

     
    int dir = (p.color == BLACK) ? -1 : 1; //forward direction for color
    int opponent = (p.color == BLACK) ? RED : BLACK; //what piece color the jump is happening over

    // Forward-left and forward-right jumps 
    if (can_jump_dir(state, r, c, dir, -1, opponent)){
         return 1;
    }
    if (can_jump_dir(state, r, c, dir,  1, opponent)){
         return 1;
    }

    // Kings can also jump backward diagonally 
    if (p.is_king) {
        if (can_jump_dir(state, r, c, -dir, -1, opponent)){
             return 1;
        }
        if (can_jump_dir(state, r, c, -dir,  1, opponent)){
             return 1;
        }
    }

    return 0; //only if no legal jumps are found
}


 //players are forced to jump if there is one
//must_player_jump scans the board and calls can_piece_jump for every piece of a player
 
int must_player_jump(const GameState *state, int player_color) {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (state->board[r][c].color == player_color) {
                if (can_piece_jump(state, r, c)){
                     return 1; //if  jump is found, force it
                }
            }
        }
    }
    return 0; //no jump available
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

//get full move makes sure the destination is valid and fills the jump ingo
// so is a legal simple or jump move or is it illegal

Move get_full_move(const GameState *state, Move m) {
    Piece p = state->board[m.from_row][m.from_col];
    int dir = (p.color == BLACK) ? -1 : 1;
    int opponent = (p.color == BLACK) ? RED : BLACK;

    int row_diff = m.to_row - m.from_row; //negatives mean moving up, positives mean moving down
    int col_diff = m.to_col - m.from_col;   // signed col diff
    int abs_col  = (col_diff < 0) ? -col_diff : col_diff; //absolute col diff

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
        // The middle square is halfway between from and to coordinates
        int mid_r = (m.from_row + m.to_row) / 2;
        int mid_c = (m.from_col + m.to_col) / 2;

        //Direction of jump must be legal for this piece type 
        int jump_dr = row_diff / 2;   // +1 or -1 is direction of jump
        int valid_dir = (jump_dr == dir) || (p.is_king && jump_dr == -dir);

        if (valid_dir && state->board[mid_r][mid_c].color == opponent && state->board[m.to_row][m.to_col].color == EMPTY) {
     //opponent must be in the middle, and the landing has to be empty
            m.jumped_row = mid_r;
            m.jumped_col = mid_c;
            return m;
        }
    }

    // Neither a valid step nor a valid jump 
    m.jumped_row = -2; //-2 is the signal that an illegal move has been done
    return m;
}

//
void apply_move(GameState *state, Move m) { //executes the valid move, changing the board state
    // Pick up the piece 
    Piece p = state->board[m.from_row][m.from_col]; //saves the place before clearing 

    // Clear the source square 
    state->board[m.from_row][m.from_col].color  = EMPTY;
    state->board[m.from_row][m.from_col].is_king = 0;

    // Place the piece at the destination 
    state->board[m.to_row][m.to_col] = p;

    // If a jump occurred, remove the captured piece and decrement count 
    if (m.jumped_row >= 0) {
        state->board[m.jumped_row][m.jumped_col].color  = EMPTY;
        state->board[m.jumped_row][m.jumped_col].is_king = 0;

        if (p.color == BLACK){
            state->red_count--;
        }
        else{
            state->black_count--;
        }
    }

    
     // Crowning: Black reaches row 0 (top of array = row 8 on screen),
     //Red reaches row 7 (bottom of array = row 1 on screen).
     
    if (p.color == BLACK && m.to_row == 0){
        state->board[m.to_row][m.to_col].is_king = 1;
    }
    if (p.color == RED && m.to_row == BOARD_SIZE - 1){
        state->board[m.to_row][m.to_col].is_king = 1;
    }
}
