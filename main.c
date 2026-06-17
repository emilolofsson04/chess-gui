#include <stdlib.h>
#include <time.h>
// provosorisk
#include "structs.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "turn.h"
#include "printAllMoves.h"
#include "movegen.h"
#include "kingsafety.h"
#include "board.h"
#include "playmove.h"
#include "verifylegality.h"
#include "initialize.h"

#include <raylib.h>
#include "Gui.h"


int main() {
    
    bool game = true; // True while game is active
    bool colour = true; // True while whites turn, false else 
    char move[7]; // input move string
    struct piece wPieces[16]; // white pieces
    struct piece bPieces[16]; // black pieces
    char board[8][8]; // board

    initialize(wPieces,bPieces, board); // Sets up the pieces and board for turn 1
    
    int iteration = 0;  // Keeps track of amount of turns
    
    struct move playedMoves[256]; // Logs Playedmoves

    struct move Moves[256];    // Initialize a set of legal moves
    int totalMoves = 0;             // The total amount of moves
    int *totMov = &totalMoves; 

    struct move legalMoves[256]; // Initialize a set of legal moves that have been filtered for king safety
    int totalLegalMoves = 0;        // Total amount of legal moves
    int *totLegMov = &totalLegalMoves;

    bool *ctp = &colour; // "color to play"
    bool *agame = &game; // active game

    // Init board gui
    InitWindow(960,960, "Chess Board");
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);

    struct pngs PiecesPng = ReadPiecePngs();


    int firstFramePerTurn = 1;
    int moveInput = 0;
    int Clicked = 0;
    int* pieceChosen = &Clicked;
    int startFile = 0;
    int startRank = 0;
    int* sFile = &startFile;
    int* sRank = &startRank;
    while (!WindowShouldClose() && game) {
        BeginDrawing();
        DrawBoard(board, PiecesPng);
        if (firstFramePerTurn) {
            // Prompt for whoevers turn it is
            if(*ctp) {
                printf("White to move\n");         
            }
            else {
                printf("Black to move\n");
            }
            
            // Reset and then find all legal moves in the position
            *totLegMov = 0;
            *totMov = 0;
         
            int k = (iteration > 1) ? iteration - 1 : 0;
            AllMoves(ctp, totMov, Moves, wPieces, bPieces, board, &playedMoves[k]); 

            for (int i = 0; i < *totMov; i++) {
                verifylegality(legalMoves, totLegMov, ctp, Moves[i], wPieces, bPieces, board);
            }
                
            printf("I found %d moves, and %d legal moves. \n",*totMov, *totLegMov);
            //

            if (*ctp) {
                if (*totLegMov == 0) {
                    if (!isKingSafe(board, wPieces[12].rank, wPieces[12].file, 1)) {

                        printf("Checkmate - Black wins\n");
                    
                    }
                    else {
                        printf("Stalemate - Draw \n");
                    }
                    *agame = 0;
                }   
                 else {
                     if (!isKingSafe(board, wPieces[12].rank, wPieces[12].file, 1)) {
                        printf("White is in check \n");
                    }

                }
            }
            else {
                if (*totLegMov == 0) {
                    if (!isKingSafe(board, bPieces[12].rank, bPieces[12].file, 0)) {
                        printf("Checkmate - White wins\n");
                    }
                    else {
                        printf("Stale mate - Draw \n");
                    }
                    *agame = 0;
                }
                else {
                     if (!isKingSafe(board, bPieces[12].rank, bPieces[12].file, 0)) {
                        printf("Black is in check \n");
                    }

                }
            }
            firstFramePerTurn = 0;
        }

        // Prompt user for move
        struct move playedMove;
        
        
        struct move inputMove;
        inputMove = InputCheck(pieceChosen, ctp, sFile, sRank);
             
            
                 
        if (inputMove.colour != -1  ) { // Checks if the given move was correct
            printf("We got a move\n");
            printf("startRank/File = %d/%d, and targetRank/File = %d/%d\n", inputMove.startRank, inputMove.startFile, inputMove.targetRank, inputMove.targetFile);
            playedMove = isMoveLegal(totLegMov, inputMove, legalMoves);
            if (playedMove.pieceIndex != -1) {
                printf("Its legal\n");
                playmove(ctp, &playedMove, wPieces, bPieces);        
                updateboard(board, wPieces, bPieces);
                printBoard(board);

                // Final clean up and swap color
                playedMoves[iteration] = playedMove;
                *ctp = !*ctp;
                (iteration)++;
                firstFramePerTurn = 1;
                startFile = 0;
                startRank = 0;
            }
        }




        EndDrawing(); 
    }
    CloseWindow();
    return 0;
}
