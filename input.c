#include <stdio.h>
#include <stdlib.h>
#include "input.h" 
#include "moves.h"

//flush_stdin — clears leftover characters from the input buffer.
//discard everything inside scanfs since scanf reads "abc" from "abc/n" so next read is clean
static void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF){
        
    }
}

//read_destination — reads and converts a coordinate like "C4" into array indices. It also accpets lowercase inputs
//col index = letter - 'A'          (so A=0, B=1, ... H=7)
//row index = 8 - display_row       (so row 8 = array 0, row 1 = array 7)
static int read_destination(Move *m) {
    char dest_col;
    int dest_row;

    printf("Enter Destination, ex: C4 ");
    if (scanf(" %c%d", &dest_col, &dest_row) !=2) {
        if(feof(stdin)){ //EOF means no more input so exit cleanly
             exit(1);
        }
        flush_stdin();
        return 0;
    }
    flush_stdin();

    if (dest_col >= 'a' && dest_col <= 'z'){ //lowercase to uppercase
        dest_col = dest_col - 'a' + 'A';
    }

    m->to_col = dest_col - 'A'; //converting board cooridnates to array indices 
    m->to_row = 8 - dest_row;

        //if anything is entered outside the board, reject it
    if (m->to_col < 0 || m->to_col >= BOARD_SIZE ||
        m->to_row < 0 || m->to_row >= BOARD_SIZE) {
        printf("  Error: coordinates out of bounds.\n\n");
        return 0;
    }
    return 1;
}

//This will build the menu of movable pieces for the current player, enforcing the jump rule
//Will return  the valid pieces, so when its 0 a winner is delcared
//if must_jump is true, the only jump-able pieces are included in the menu

static int build_piece_menu(const GameState *state, int valid_pieces[][2], int must_jump){
    int num = 0;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (state->board[r][c].color != state->current_player){
                 continue;
            }
            //check if jump ability then step ability
            int ok = must_jump ? can_piece_jump(state, r, c)
                               : can_piece_move(state, r, c);
            if (ok) {
                valid_pieces[num][0] = r;
                valid_pieces[num][1] = c;
                num++;
            }
        }
    }
    return num;
}

//The player will pick a piece from menu and then its destination, 
//loppong until a legal move is entered
Move get_player_move(const GameState *state) {
    Move m;
    const char *name  = (state->current_player == RED) ? "Red" : "Black";
    int must_jump = must_player_jump(state, state->current_player);
    int valid_pieces[64][2]; //max 64 pieces possible
    int num_pieces = build_piece_menu(state, valid_pieces, must_jump);

    while (1) {
        //print numbered list of movable pieces 
        printf("%s's turn. Available pieces:\n", name);
        for (int i = 0; i < num_pieces; i++) {
            int pr = valid_pieces[i][0];
            int pc = valid_pieces[i][1];
            printf("  %d) %s at %c%d\n",
                   i + 1,
                   state->board[pr][pc].is_king ? "King" : "Piece",
                   'A' + pc, 8 - pr); //converting array cooridnates back to display format
        }

        // Pick piece 
        int choice;
        printf("Select piece number: ");
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > num_pieces) {
            if (feof(stdin)){ 
                exit(1);
            }
            printf("  Error: invalid choice.\n\n");
            flush_stdin();
            continue; //re prompts
        }
        flush_stdin();

        //stores the selected pieces position as move source
        m.from_row = valid_pieces[choice - 1][0];
        m.from_col = valid_pieces[choice - 1][1];

        // Pick destination and validate it
        if (!read_destination(&m)) {
            printf("  Error: invalid format.\n\n");
            continue;
        }

        Move full = get_full_move(state, m);
        if (full.jumped_row == -2) {
            printf("  Error: illegal destination.\n\n");
            continue;
        }

        
        // Enforce forced-jump: if a jump was available but the player
        // entered a simple move, reject it.
        if (must_jump && full.jumped_row == -1) {
            printf("  Error: you must jump.\n\n");
            continue;
        }

        return full;
    }
}
//input is the piece fixed at (r,c)
//used by multi jump in main.c, player pick the jump point
Move get_continuation_jump(const GameState *state, int r, int c) {
    Move m;
    m.from_row = r;
    m.from_col = c;

    while (1) {
        printf("Continue jumping with piece at %c%d.\n",
               'A' + c, 8 - r);

        if (!read_destination(&m)) {
            printf("  Error: invalid format.\n\n");
            continue;
        }

        Move full = get_full_move(state, m);

        // Must be a jump (not a simple step) 
        if (full.jumped_row < 0) {
            printf("  Error: must continue jumping.\n\n");
            continue;
        }

        return full;
    }
}
