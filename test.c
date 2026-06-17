#include <raylib.h>
#include <stdio.h>
#include <ctype.h>

#include "structs.h"
#include "initialize.h"
#include "board.h"
int Clicked = 0;
int* alreadyClicked = &Clicked;
void DrawBoard(char board[8][8]) {
        for (int i = 0; i < 8; i++) {
            for (int k = 0; k < 8; k++) {
                if ((i%2 == 0 && k%2 == 0) || (i%2 == 1 && k%2 == 1)) {
                    DrawRectangle(i*100, k*100, 100, 100, WHITE);
                }
                else {
                    DrawRectangle(i*100, k*100, 100, 100, GREEN);
                }
            }
        }
        for (int i = 0; i < 8; i++) {
            for (int k = 0; k < 8; k++) {
                if (board[k][i] != '.') {
                    if (isupper(board[k][i])) {
                        DrawCircle(i*100 + 50, 700 -k*100 + 50, 30, BLACK);
                    }
                    else {
                        DrawCircle(i*100 + 50, 700 - k*100 + 50, 30, GRAY);
                    }
                }
            }
        }
} 
void InputCheck(int* alreadyClicked) {
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (*alreadyClicked) {    
                int tcol = mouse.x / 100; 
                int trow = mouse.y / 100;
               
                printf("Move target: rank %d, file %d\n", 8 - trow , tcol + 1);
                *alreadyClicked = 0;
            }
            else {
                int scol = mouse.x / 100;
                int srow = mouse.y / 100; 
                *alreadyClicked = 1;
                printf("Move starting: rank %d, file %d\n", 8 - srow , scol + 1);
            }
        }
} 
int main(){
    struct piece wPieces[16]; // white pieces
    struct piece bPieces[16]; // black pieces
    char board[8][8]; // board

    initialize(wPieces, bPieces, board);
    SetTraceLogLevel(LOG_NONE);
    InitWindow(800,800, "Board");
    
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        InputCheck(alreadyClicked);
        BeginDrawing();
        DrawBoard(board);
        EndDrawing();
    }        
    CloseWindow();
    return 0;
}
