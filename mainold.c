#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>
#include <time.h>
#include "structs.h"
#include "initialize.h"
#include "turn.h"


int main() {
    
    bool game = true; // True while game is active
    bool colour = true; // True while whites turn, false else 
    char move[7]; // input move string
    struct piece whitePieces[16]; // white pieces
    struct piece blackPieces[16]; // black pieces
    char board[8][8]; // board

    initialize(whitePieces,blackPieces, board); // Sets up the pieces and board for turn 1
    
    int iteration = 0;  // Keeps track of amount of turns
    int *it = &iteration; 
    
    struct move playedMoves[256]; // Logs Playedmoves

    struct move Moves[256];    // Initialize a set of legal moves
    int totalMoves = 0;             // The total amount of moves
    int *totMov = &totalMoves; 

    struct move legalMoves[256]; // Initialize a set of legal moves that have been filtered for king safety
    int totalLegalMoves = 0;        // Total amount of legal moves
    int *totLegMov = &totalLegalMoves;

    bool *ctp = &colour; // "color to play"
    bool *agame = &game; // active game


    while(game) {
        clock_t t;
        t = clock();
        turn(ctp, agame, move, board, whitePieces, blackPieces, it, playedMoves, Moves, totMov, legalMoves, totLegMov); // One turn
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;  // in seconds
        printf(" Time taken last turn: %fs \n", time_taken);
    }
    return 0;
}
