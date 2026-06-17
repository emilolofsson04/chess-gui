
#include "structs.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "playmove.h"
#include "unplaymove.h"

void unplaymove(bool* ctp, struct move playedMove, struct piece wPieces[16], struct piece bPieces[16]){

    /*
     unExecutes the move, by changing postion of the moved piece,
     and setting a captured piece to alive

     *  *ctp: colour to play
     *  move playedMove: struct containing move information
     *  piece wPieces: set of white pieces
     *  piece bPieces: set of black pieces
     */
    struct piece* aPieces = *ctp ? wPieces : bPieces;
    struct piece* ePieces = *ctp ? bPieces : wPieces;
    
    int i = playedMove.capturedIndex;
    int pInd = playedMove.pieceIndex;
    int capturedRank = -1;
  
    // Switch case for handling normal and special moves
    switch (playedMove.moveType) {
        case 0:
            capturedRank = playedMove.targetRank; // Normally the captured piece is on the target square
            break;
        case 1:
            capturedRank = playedMove.startRank; // In en passant, the captured piece is on the starting rank
            break;

        case 2: 
        case 3:
            aPieces[i].rank = playedMove.targetRank;
            aPieces[i].file = playedMove.targetFile;
            aPieces[i].hasBeenMoved--;

            break;
        case 4:
            capturedRank = playedMove.targetRank;

            if (*ctp) {
                aPieces[pInd].type = 'p';
            }
            else {
                aPieces[pInd].type = 'P';
            }
            break;
    }


    // Alives captured piece
    if (playedMove.capture == -1) {
            ePieces[i].alive = 1;
            ePieces[i].rank = capturedRank;
            ePieces[i].file = playedMove.targetFile;
    }

    // Changing position of moved piece
        aPieces[playedMove.pieceIndex].rank = playedMove.startRank;
        aPieces[playedMove.pieceIndex].file = playedMove.startFile;
        aPieces[playedMove.pieceIndex].hasBeenMoved--;
   
}
