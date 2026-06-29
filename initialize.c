
#include <stdbool.h>

#include "structs.h"
#include "initialize.h"
#include "board.h"

void initialize(struct GameState* gameState) {
    /*
     Initializes all pieces, and the board
     */
    for(int i = 0; i < 8; i++) {
        gameState->whitePieces[i].type = 'p';
        gameState->whitePieces[i].rank = 2;
        gameState->whitePieces[i].file = i+1;
        gameState->whitePieces[i].value = 1;
    }
    for(int i = 0; i < 16; i++) {
        gameState->whitePieces[i].colour = true;
        gameState->whitePieces[i].alive = 1;
        gameState->whitePieces[i].hasBeenMoved = 0;
    }
     
    for(int i = 0; i < 8; i++) {
        gameState->blackPieces[i].type = 'P';
        gameState->blackPieces[i].rank = 7;
        gameState->blackPieces[i].file = i+1;
        gameState->blackPieces[i].value = 1;
    }
    for(int i = 0; i < 16; i++) {
        gameState->blackPieces[i].colour = false;
        gameState->blackPieces[i].alive = 1;
        gameState->blackPieces[i].hasBeenMoved = 0;
    }
    
    // Rooks
    for(int i = 8; i < 16; i = i+7){
        gameState->whitePieces[i].type = 'r';
        gameState->blackPieces[i].type = 'R';
        gameState->whitePieces[i].rank = 1;
        gameState->blackPieces[i].rank = 8;
        gameState->whitePieces[i].file = i+1-8;
        gameState->blackPieces[i].file = i+1-8;
        gameState->whitePieces[i].value = 5;
        gameState->blackPieces[i].value = 5;
    }
    // Knights
    for(int i = 9; i < 16; i = i+5){
        gameState->whitePieces[i].type = 'n';
        gameState->blackPieces[i].type = 'N';
        gameState->whitePieces[i].rank = 1;
        gameState->blackPieces[i].rank = 8;
        gameState->whitePieces[i].file = i+1-8;
        gameState->blackPieces[i].file = i+1-8;
        gameState->whitePieces[i].value = 3;
        gameState->blackPieces[i].value = 3;
    }
    // Bishops
    for(int i = 10; i < 14; i = i+3){
        gameState->whitePieces[i].type = 'b';
        gameState->blackPieces[i].type = 'B';
        gameState->whitePieces[i].rank = 1;
        gameState->blackPieces[i].rank = 8;
        gameState->whitePieces[i].file = i+1-8;
        gameState->blackPieces[i].file = i+1-8;
        gameState->whitePieces[i].value = 3;
        gameState->blackPieces[i].value = 3;
    }
    // Queens
    gameState->whitePieces[11].type = 'q';
    gameState->blackPieces[11].type = 'Q';
    gameState->whitePieces[11].rank = 1;
    gameState->blackPieces[11].rank = 8;
    gameState->whitePieces[11].file = 4;
    gameState->blackPieces[11].file = 4;
    gameState->whitePieces[11].value = 9;
    gameState->blackPieces[11].value = 9;
    
    // Kings 
    gameState->whitePieces[12].type = 'k';
    gameState->blackPieces[12].type = 'K';
    gameState->whitePieces[12].rank = 1;
    gameState->blackPieces[12].rank = 8;
    gameState->whitePieces[12].file = 5;
    gameState->blackPieces[12].file = 5;
    gameState->whitePieces[12].value = 50;
    gameState->blackPieces[12].value = 50;
    
    updateboard(gameState);
}
