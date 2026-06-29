#include "structs.h"
#include <stdbool.h>


void logMoves(char playedMoves[256][5], int totPlayedMoves, struct move* playedMove) {
        if (totPlayedMoves != 0){ 
            playedMoves[totPlayedMoves-1][0] = playedMove->startFile +'a' -1;
            playedMoves[totPlayedMoves-1][1] = playedMove->startRank + '0';
            playedMoves[totPlayedMoves-1][2] = playedMove->targetFile +'a' -1;
            playedMoves[totPlayedMoves-1][3] = playedMove->targetRank + '0';

            if (playedMove->promotionPiece != -1) {
                char whitePromo[4] = {'q', 'r', 'b', 'n'};
                char blackPromo[4] = {'Q', 'R', 'B', 'N'};
                char* Promo = playedMove->colour ? whitePromo : blackPromo;
     
                playedMoves[totPlayedMoves-1][4] = Promo[playedMove->promotionPiece];
                
            }
            playedMoves[totPlayedMoves-1][4]= '\0';

        
        }

}
