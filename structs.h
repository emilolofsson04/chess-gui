#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <raylib.h>

struct piece {
    char type;
    bool colour;
    int rank;
    int file;
    int alive;
    int hasBeenMoved;
};

struct move {
    int startRank;
    int startFile;
    int targetRank;
    int targetFile;
    int pieceIndex;
    int pieceType;
    int colour;
    int capture;
    int capturedIndex;
    int moveType;
    int promotionPiece;
};     

struct pngs {
    Texture2D wPawn;
    Texture2D wKnight;
    Texture2D wBishop;
    Texture2D wRook;
    Texture2D wQueen;
    Texture2D wKing;

    Texture2D bPawn;
    Texture2D bKnight;
    Texture2D bBishop;
    Texture2D bRook;
    Texture2D bQueen;
    Texture2D bKing;
};

#endif
