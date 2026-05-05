#include <stdio.h>
#include "board.h"
#include "moves.h"

void init_board(GameState *state) {
    int r, c;

    for (r = 0; r < BOARD_SIZE; r++) {
        for (c = 0; c < BOARD_SIZE; c++) {
            state->board[r][c].color   = EMPTY; // clear every square before placing pieces
            state->board[r][c].is_king = 0; //no kings on the board when game starts
        }
    }

    for (r = 0; r < 3; r++) { //this initializes the red pieces, so red always at the top
        for (c = 0; c < BOARD_SIZE; c++) {
            if ((r + c) % 2 == 1) { // pieces only go on dark squares where row+col is odd
                state->board[r][c].color   = RED;
                state->board[r][c].is_king = 0;
            }
        }
    }

    for (r = 5; r < BOARD_SIZE; r++) { //this one initializes the black pieces
        for (c = 0; c < BOARD_SIZE; c++) {
            if ((r + c) % 2 == 1) {
                state->board[r][c].color   = BLACK;
                state->board[r][c].is_king = 0; //again no kings when the board initializes
            }
        }
    }
    //all players start with 12 pieces (3 rows and 4 pieces per row)
    state->red_count     = 12; 
    state->black_count   = 12;
    state->history_count = 0; //no moves have been played 
}

//checks if the current player has a forced jump on the turn, and if so, those moves and pieces 
//get shown, also bolds the pieces the player can move
// r: red pawn R: red king b: black pawn B: black king
//rows 0-7 are shown as rows 8-1
void render_board(const GameState *state) { //prints the board after each move 
    int r, c;

    int must_jump = must_player_jump(state, state->current_player);
    //checks if a jump is forced
    
    printf("\n      A   B   C   D   E   F   G   H\n"); //pritning column labels

    for (r = 0; r < BOARD_SIZE; r++) {
        printf("    +---+---+---+---+---+---+---+---+\n");
        printf("  %d |", 8 - r); // prints row number given by 8-(array index)

        for (c = 0; c < BOARD_SIZE; c++) {
            Piece p = state->board[r][c];
            if (p.color == EMPTY) {
                printf("   |"); //empty squares are blank spaces
            } 
            else { // should a piece be selected for the current player
                int movable = 0;
                if (p.color == state->current_player) {
                    if (must_jump) {
                        if (can_piece_jump(state, r, c)){
                             movable = 1; //jump piecesare selected
                        }
                    } 
                    else {
                        if (can_piece_move(state, r, c)){
                             movable = 1; //movale pieces are selected
                        }
                    }
                }

                char piece_char; //difference between kings and pawn for each color
                if (p.color == RED) {
                    piece_char = p.is_king ? 'R' : 'r'; //for red
                } else {
                    piece_char = p.is_king ? 'B' : 'b'; //for black
                }

                if (movable) {
                    printf(" \033[1m%c\033[0m |", piece_char); //searched this up, it turns on/off bolding in the terminal
                } else {
                    printf(" %c |", piece_char);
                }
            }
        }
        printf("\n");
    }
    printf("    +---+---+---+---+---+---+---+---+\n");//the last boundary at the bottom of the board
    printf("\n  Red: %d pieces   Black: %d pieces\n\n", state->red_count, state->black_count); //piece count per player
}
