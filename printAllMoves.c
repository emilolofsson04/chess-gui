

#include "structs.h"
#include <stdio.h>
#include <string.h>

#include "printAllMoves.h"

void printAllMoves(struct move legalMoves[256], int* totMov, bool* ctp) {
    /*
      Prints all possible moves if prompted
     */
    char newmove[5]; 
    for (int i = 0; i < *totMov; i++) {
        newmove[0] = legalMoves[i].startFile +'a' -1;
        newmove[1] = legalMoves[i].startRank + '0';
        newmove[2] = legalMoves[i].targetFile +'a' -1;
        newmove[3] = legalMoves[i].targetRank + '0';
        if (legalMoves[i].promotionPiece != -1) {
            char whitePromo[4] = {'q', 'r', 'b', 'n'};
            char blackPromo[4] = {'Q', 'R', 'B', 'N'};
            char* Promo = *ctp ? whitePromo : blackPromo;
 
            newmove[4] = Promo[legalMoves[i].promotionPiece];
            
        }
        else {
        newmove[4] = '\0';
        }
        printf("%s, ", newmove);
    }
}
