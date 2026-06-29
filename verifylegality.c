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

void verifylegality(struct move strictlylegalMoves[256], int* stotMov, struct move playedMove, struct GameState* gs){

    int pcolour = playedMove.colour;
    
    // Play the move and update the board
    playmove(&playedMove, gs);
    updateboard(gs);
    
    int KingRank = gs->ctp ? gs->whitePieces[12].rank : gs->blackPieces[12].rank;
    int KingFile = gs->ctp ? gs->whitePieces[12].file : gs->blackPieces[12].file;

    // Check if King is safe after move, and if so save it
    if (isKingSafe(gs->Board, KingRank, KingFile, pcolour)) {
        strictlylegalMoves[*stotMov] = playedMove;
        (*stotMov)++;
    }
    
    // Unplay the move and update the board
    unplaymove(playedMove, gs);
    updateboard(gs); 

}
