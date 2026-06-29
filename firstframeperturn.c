#include "structs.h"
#include <stdbool.h>
#include <stdio.h> 
#include "kingsafety.h"

void firstframeperturn(int totalLegalMoves, bool* agame, struct GameState* gs) {
    if (gs->ctp) {
        if (totalLegalMoves == 0) {
            if (!isKingSafe(gs->Board, gs->whitePieces[12].rank, gs->whitePieces[12].file, 1)) {

                printf("Checkmate - Black wins\n");
            
            }
            else {
                printf("Stalemate - Draw \n");
            }
            *agame = 0;
        }   
         else {
             if (!isKingSafe(gs->Board, gs->whitePieces[12].rank, gs->whitePieces[12].file, 1)) {
                printf("White is in check \n");
            }

        }
    }
    else {
        if (totalLegalMoves == 0) {
            if (!isKingSafe(gs->Board, gs->blackPieces[12].rank, gs->blackPieces[12].file, 0)) {
                printf("Checkmate - White wins\n");
            }
            else {
                printf("Stale mate - Draw \n");
            }
            *agame = 0;
        }
        else {
             if (!isKingSafe(gs->Board, gs->blackPieces[12].rank, gs->blackPieces[12].file, 0)) {
                printf("Black is in check \n");
            }

        }
    }

}
