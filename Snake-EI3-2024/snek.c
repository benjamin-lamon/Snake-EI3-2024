/*
author: bain a main, whoever that is...
*/
#include <stdio.h>
#include <stdlib.h>
#include "snakeAPI.h"

void main(){
    connectToServer("localhost",1234,"bruh");
    char gameName[50];
    int sizeX;
    int sizeY;
    int nbWalls;
    char gameType[150] = "TRAINING SUPER_PLAYER";
    waitForSnakeGame(gameType, gameName, &sizeX, &sizeY, &nbWalls);
};