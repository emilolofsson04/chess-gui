
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
#include "logs.h"

#include <raylib.h>
#include "Gui.h"
#include "firstframeperturn.h"
#ifdef HAS_ENGINE
#include "../Computer/Engine.h"
#endif

const ChessTheme THEME_CLASSIC = {
    .boardLight = {250, 246, 229, 255},
    .boardDark   = {86, 166, 67, 255}, 
    .panelBg     = {238, 238, 234, 255},
    .panelBorder = {200, 200, 195, 255},
    .textMuted   = {120, 120, 120, 255},
    .accentCheck = {230,  80,  80, 255},
    .WINDOW      = {255, 255, 255, 255} 
};

const ChessTheme THEME_DARK = {
    // .boardLight  = {230, 220, 215, 255}, 
    .boardLight  = {251, 230, 255, 255},
    //.boardDark   = { 59,  61,  80, 255}, 
    .boardDark   = {64,   0,  77, 255},
    .panelBg     = { 34,  34,  49, 255},
    .panelBorder = { 49,  50,  68, 255}, 
    .textMuted   = {166, 173, 200, 255},
    .accentCheck = {243, 139, 168, 255},  
    .WINDOW      = { 17,  17,  27, 255}
};

int main() {
    
    bool game = true; // True while game is active
    bool colour = true; // True while whites turn, false else 

    struct GameState gameState;
    gameState.ctp = 1;

    initialize(&gameState); // Sets up the pieces and board for turn 1


    int iteration = 0;  // Keeps track of amount of turns
    struct move playedMoves[256]; // Logs Playedmoves

    struct move Moves[256];    // Initialize a set of moves
    int totalMoves = 0;             // The total amount of moves
    int *totMov = &totalMoves; 

    struct move legalMoves[256]; // Initialize a set of legal moves that have been filtered for king safety
    int totalLegalMoves = 0;        // Total amount of legal moves
    int *totLegMov = &totalLegalMoves;

    bool *agame = &game; // active game (obsolete in gui) 

    // Board size and Gui design
    struct Gui gui;
    gui.squareSize = 120;
    gui.evalbarSize = 30;
    gui.yoffset = 30;
    gui.xoffset = 30 + gui.evalbarSize;
    gui.boardSize = 8 * gui.squareSize; 
    gui.textboxSize = 300;
    gui.x = gui.boardSize + 2* gui.xoffset + gui.textboxSize;
    gui.y = gui.boardSize + 2* gui.yoffset;

    // Init board gui
    InitWindow(gui.x, gui.y, "Chess Board");
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(60);
    ChessTheme theme = THEME_CLASSIC;
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    struct pngs PiecesPng = ReadPiecePngs(gui.squareSize);
    struct pngs CapturedPiecesPng = ReadPiecePngs(gui.squareSize/3*2);
    struct pngs tinypngs = ReadPiecePngs(35);


    // flags for game logic
    int firstFramePerTurn = 1;
    int Clicked = 0;
    int* pieceChosen = &Clicked;
    int promotionChoice = 0;
    int* pChoice = &promotionChoice;
    int startFile = 0;
    int startRank = 0;
    int* sFile = &startFile;
    int* sRank = &startRank;
    float evaluation = 0;
    float* eval = &evaluation;

    struct move inputMove;
    struct move promotionMove;
    struct Pieces CapturedPieces;


    // flags for text drawing
    int checkMate = 0;
    int staleMate = 0;
    int check = 0;

    // logs
    char playedMovestrings[256][5];

    // Scroll
    int scrollAmount = 0;
    int* scroll = &scrollAmount;


    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_T)) { // Toggle themes
            if (theme.panelBg.r == THEME_DARK.panelBg.r) {
                theme = THEME_CLASSIC;
            } else {
                theme = THEME_DARK;
            }
        }
        BeginDrawing();
        ClearBackground(theme.WINDOW);
        DrawRectangle(gui.xoffset - 3 , gui.yoffset - 3, gui.boardSize+6, gui.boardSize+6, theme.panelBorder);

        DrawBoard(&gui, theme);
        DrawHighlights(&gui, Clicked, startFile, startRank, theme);
        DrawPieces(gameState.Board, PiecesPng, &gui, theme);
        DrawGameHistory(&gui, playedMovestrings, iteration, tinypngs, theme, scroll);
        DrawCapturedPieces(&gui, CapturedPieces, CapturedPiecesPng, theme);
        DrawPromotion(gameState.Board, PiecesPng, &gui, Clicked, startFile, startRank, promotionMove, pChoice, gameState.ctp,  theme);
        DrawEvalBar(&gui, evaluation, theme);
//      DrawTextBox(squareSize, xoffset, yoffset, textboxSize, boardSize);

        DrawGameText(&gui, gameState.ctp, check, checkMate, staleMate, theme);

        if (firstFramePerTurn) {
            // Reset and then find all legal moves in the position
            *totLegMov = 0;
            *totMov = 0;
         
            int k = (iteration > 1) ? iteration - 1 : 0;
            AllMoves(totMov, Moves, &gameState, &playedMoves[k]); 

            for (int i = 0; i < *totMov; i++) {
                verifylegality(legalMoves, totLegMov, Moves[i], &gameState);
            }
                
            printf("I found %d moves, and %d legal moves. \n",*totMov, *totLegMov);

            firstframeperturn(totalLegalMoves, agame, &gameState);

            logMoves(playedMovestrings, iteration, &playedMoves[k]);
            firstFramePerTurn = 0;
        }

        // Prompt user for move
        struct move playedMove;
        
        
        inputMove = InputCheck(pieceChosen, gameState.ctp, sFile, sRank, &gui, promotionMove, pChoice);
             
        #ifdef HAS_ENGINE      
        if (!gameState.ctp && game) {
            int k = (iteration > 1) ? iteration - 1 : 0;
        
            int ArgBestMove = Calc(&gameState, &playedMoves[k], 4, eval);
            printf("the arg of the best move is: %d\n", ArgBestMove);
            printf(" The eval is = %f \n ", evaluation);

            inputMove = legalMoves[ArgBestMove];
             
        }
        if (gameState.ctp && game) {
            int k = (iteration > 1) ? iteration - 1 : 0;
        
            int ArgBestMove = Calc(&gameState, &playedMoves[k], 4, eval);
            printf("the arg of the best move is: %d\n", ArgBestMove);
            printf(" The eval is = %f \n ", evaluation);

            inputMove = legalMoves[ArgBestMove];
             
        }
        #endif
                 
        if (inputMove.colour != -1) { // Checks if the given move was correct
            // Checks if the move is a promotion
            if (promotionCheck(inputMove, gameState.Board, legalMoves, totLegMov) && inputMove.promotionPiece == -1) {
                promotionMove = inputMove;
                promotionChoice = 1;
            }
            else {
                playedMove = isMoveLegal(totLegMov, inputMove, legalMoves);
                if (playedMove.pieceIndex != -1) {
                    playmove(&playedMove, &gameState);        
                    updateboard(&gameState);
                    printBoard(gameState.Board);
                    
                    if (playedMove.capture == -1) {
                        capturedPiecesUpdates(playedMove, &CapturedPieces, &gameState);
                    }
                    // Final clean up and swap color
                    playedMoves[iteration] = playedMove;
                    gameState.ctp = !gameState.ctp;
                    (iteration)++;
                    firstFramePerTurn = 1;
                    startFile = 0;
                    startRank = 0;
                }
            }
        }

        EndDrawing(); 
    }

    CloseWindow();
    return 0;
}
