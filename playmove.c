

#include "structs.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "playmove.h"

struct move translate(char move[6], bool* ctp) {
    /*
     Converts the input move string into a move struct
     */
    struct move inputMove;
    inputMove.colour = -1;
    if(strlen(move) >= 4) {
        
        inputMove.startFile = move[0]-'a'+1;
        inputMove.targetFile = move[2]-'a'+1;
        inputMove.startRank = move[1]-'0';
        inputMove.targetRank = move[3]-'0';
        
        
        inputMove.promotionPiece = -1;
        if (isalpha(move[4])) {
           char whitePromo[4] = {'q', 'r', 'b', 'n'};
           char blackPromo[4] = {'Q', 'R', 'B', 'N'};
           char* Promo = *ctp ? whitePromo : blackPromo;
            

            for (int i = 0; i < 4; i++) {
                if (move[4] == Promo[i]) {
                    move[4] = i;
                    break;
                }
                
            }
            inputMove.promotionPiece = move[4]; // number from 0-7;
            printf("Promotion piece is %d", move[4]);

        }
        
        inputMove.colour = *ctp;
    }   
    return inputMove;
}

void moveupdates(struct move legalMoves[256], int* totMov, int rank, int file, int pInd, int pcolour, int targetRank, int targetFile, int capture, char type, int movetype) {
    /*
     Updates the list of legal moves
     */
    if (movetype !=4) {
    legalMoves[*totMov].startRank = rank;
    legalMoves[*totMov].startFile = file;
    legalMoves[*totMov].targetRank = targetRank;
    legalMoves[*totMov].targetFile = targetFile; 
    legalMoves[*totMov].colour = pcolour;
    legalMoves[*totMov].pieceType = type;
    legalMoves[*totMov].pieceIndex = pInd;
    legalMoves[*totMov].capturedIndex = -1; // Changed later if a piece was captured
    legalMoves[*totMov].moveType = movetype;
    legalMoves[*totMov].capture = -capture;
    legalMoves[*totMov].promotionPiece = -1;
    (*totMov)++;
    }
    
    else {
        for (int i = 0; i < 4; i++) {
            legalMoves[*totMov].startRank = rank;
            legalMoves[*totMov].startFile = file;
            legalMoves[*totMov].targetRank = targetRank;
            legalMoves[*totMov].targetFile = targetFile; 
            legalMoves[*totMov].colour = pcolour;
            legalMoves[*totMov].pieceType = type;
            legalMoves[*totMov].pieceIndex = pInd;
            legalMoves[*totMov].capturedIndex = -1; // Changed later if a piece was captured
            legalMoves[*totMov].moveType = movetype;
            legalMoves[*totMov].capture = -capture;
            legalMoves[*totMov].promotionPiece = i; 
            (*totMov)++;
    


        }

    }
    
}

void playmove(bool* ctp, struct move playedMove[1], struct piece wPieces[16], struct piece bPieces[16]){

    /*
     Executes the move, by changing postion of the moved piece,
     and setting a captured piece to dead

     *  *ctp: colour to play
     *  move playedMove: struct containing move information
     *  piece wPieces: set of white pieces
     *  piece bPieces: set of black pieces
     */

    struct piece* aPieces = *ctp ? wPieces : bPieces;
    struct piece* ePieces = *ctp ? bPieces : wPieces;

    int captureRank = 0;
    int targetFile = playedMove[0].targetFile;
    int startFile;
    int pInd = playedMove[0].pieceIndex;

    char whitePromo[4] = {'q', 'r', 'b', 'n'};
    char blackPromo[4] = {'Q', 'R', 'B', 'N'};

    // Switch case for handling normal captures (case 1) and special moves, en passant (case 1)
    // castling (case 2and 3), and promotion (case 4).
    switch (playedMove[0].moveType) {
    case 0:
        captureRank = playedMove[0].targetRank; // In a standard capture, the piece is on the target square
        break;
    case 1:
        captureRank = playedMove[0].startRank; // In en passant, the piece is on the starting Rank
        break;
    case 2:
        targetFile = playedMove[0].targetFile - 1;  // IN short castle, the the king ends om the square before the rook
        startFile = playedMove[0].startFile + 1;    // and the rook ends one before the king start file.
        break;
    case 3: 
        targetFile = playedMove[0].targetFile + 2;  // In long castle, the king ends two squares infront of the rook
        startFile = playedMove[0].startFile - 1;    // and the rook ends one before the king
        break; 
    case 4:
        captureRank = playedMove[0].targetRank; // Standard in promotion (not garanteed to be a capture)

        // Sets the piece type of the pawn to whatever was input
        if (*ctp) {
            aPieces[pInd].type = whitePromo[playedMove[0].promotionPiece]; 
        }
        else {
            aPieces[pInd].type = blackPromo[playedMove[0].promotionPiece];
        }
        break;
    }

    // Kills captured piece
    if (playedMove[0].capture == -1) {
                for (int i = 0; i < 16; i++) {
                    if (ePieces[i].alive && ePieces[i].rank == captureRank && ePieces[i].file == playedMove[0].targetFile) {
                        ePieces[i].alive = 0;
                        ePieces[i].rank = -1;
                        ePieces[i].file = -1;
                        playedMove[0].capturedIndex = i;
                        break;
                    }
                }
        }

    // Changing position of moved piece
    aPieces[playedMove[0].pieceIndex].rank = playedMove[0].targetRank;
    aPieces[playedMove[0].pieceIndex].file = targetFile;
    aPieces[playedMove[0].pieceIndex].hasBeenMoved++;
    
    // Move rook in the case of castling
    if (playedMove[0].moveType == 2 || playedMove[0].moveType == 3) {
                for (int i = 0; i < 16; i++) {
                    if (aPieces[i].alive && aPieces[i].rank == playedMove[0].targetRank && aPieces[i].file == playedMove[0].targetFile) {
                        aPieces[i].rank = playedMove[0].startRank;
                        aPieces[i].file = startFile;
                        aPieces[i].hasBeenMoved++;
                        playedMove[0].capturedIndex = i; // Set the capture index in order to easy unmove (capture is still = -1)
                        break;
                    }
            }

    }   
}

