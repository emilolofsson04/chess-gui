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
#include "unplaymove.h"

void verifylegality(struct move strictlylegalMoves[256], int* stotMov, bool* ctp, struct move playedMove, struct piece wPieces[16], struct piece bPieces[16], char board[8][8]){

    int rank = playedMove.startRank;
    int file = playedMove.startFile;
    int pInd = playedMove.pieceIndex;
    int pcolour = playedMove.colour;
    int targetRank = playedMove.targetRank;
    int targetFile = playedMove.targetFile;
    int capture = playedMove.capture;
    int type = playedMove.pieceType; 
    int movetype = playedMove.moveType;
    
    // Play the move and update the board
    playmove(ctp, &playedMove, wPieces, bPieces);
    updateboard(board, wPieces, bPieces);
    
    int KingRank = *ctp ? wPieces[12].rank : bPieces[12].rank;
    int KingFile = *ctp ? wPieces[12].file : bPieces[12].file;

    // Check if King is safe after move, and if so save it
    if (isKingSafe(board, KingRank, KingFile, pcolour)) {
        strictlylegalMoves[*stotMov] = playedMove;
        (*stotMov)++;
    }
    
    // Unplay the move and update the board
    unplaymove(ctp, playedMove, wPieces, bPieces);
    updateboard(board, wPieces, bPieces); 

}
