#include <stdio.h>
#include "board.h"
#include "input.h"
#include "moves.h" 


//renders board after every move, checks for legal moves, 
int main() {
    GameState state;
    init_board(&state);
    
    state.current_player = BLACK;

    printf("Welcome to Checkers!\n");

    //games end when a player runs out of pieces or has no legal moves to make
    while (state.red_count > 0 && state.black_count > 0) { 
        render_board(&state);

        //have to check for illegal moves to avoid calling get_player_move since nothing to do.

        int has_jump = must_player_jump(&state, state.current_player);
        int has_move = 0;

        if (!has_jump) {
            // Check if any piece can take a simple step 
            for (int r = 0; r < BOARD_SIZE && !has_move; r++){
                for (int c = 0; c < BOARD_SIZE && !has_move; c++){
                    if (state.board[r][c].color == state.current_player){
                        if (can_piece_move(&state, r, c)){
                            has_move = 1;
                        }
                    }
                }
            }
        }
        

        if (!has_jump && !has_move) {
            // Current player is completely blocked — they lose 
            const char *loser  = (state.current_player == BLACK) ? "Black" : "Red";
            const char *winner = (state.current_player == BLACK) ? "Red"   : "Black";
            printf("%s has no legal moves. %s wins!\n", loser, winner);
            break;
        }

        // Get and apply the player's chosen move 
        Move m = get_player_move(&state);
        apply_move(&state, m);

        
         // Multi-jump loops after a capture to check if the same piece can jump again
         //The rules require it to continue jumping in the same turn.
         //Keep prompting the same player until no further jump is possible.
         
        
        while (m.jumped_row >= 0 && can_piece_jump(&state, m.to_row, m.to_col)) {
            render_board(&state);
            printf("Multi-jump available! You must keep jumping.\n");
            m = get_continuation_jump(&state, m.to_row, m.to_col);
            apply_move(&state, m);
        }

        // Switch players after the full turn is done 
        state.current_player = (state.current_player == BLACK) ? RED : BLACK;
    }

    // Print final board and winner (piece-count win condition) 
    render_board(&state);
    if (state.red_count == 0){
        printf("Black wins!\n");
    }
    else if (state.black_count == 0){
        printf("Red wins!\n");
    }

    return 0;
}
