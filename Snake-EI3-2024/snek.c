/*
author: bain à main, whoever that is...
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "snakeAPI.h"

void main(){
    connectToServer("localhost",1234,"bruh");
    char gameName[50];
    int sizeX;
    int sizeY;
    int nbWalls;
    char gameType[150] = "TRAINING SUPER_PLAYER difficulty=1 timeout=5 seed=123 start=0";
    waitForSnakeGame(gameType, gameName, &sizeX, &sizeY, &nbWalls);
    // waitForSnakeGame(gameType, gameName, &sizeX, &sizeY, &nbWalls);
    int walls[nbWalls];
    getSnakeArena(&walls);
    int test = 1;
    t_move move;
    t_move moveAdv;
    while (test == 1){
        printArena();
        sleep(1.5);
        scanf("%d",&move);
        sendMove(move);
        getMove(&moveAdv);
    }
    printArena();
};