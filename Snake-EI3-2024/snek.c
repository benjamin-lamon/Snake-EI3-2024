/*
author: bain à main, whoever that is...
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "snakeAPI.h"


void playMove();
void calcNxtMove();


void main(){
    connectToServer("localhost",1234,"bruh");
    char gameName[50];
    int sizeX;
    int sizeY;
    int nbWalls;
    char gameType[150] = "TRAINING SUPER_PLAYER difficulty=1 timeout=5 seed=123 start=0";
    waitForSnakeGame(gameType, gameName, &sizeX, &sizeY, &nbWalls);
    // waitForSnakeGame(gameType, gameName, &sizeX, &sizeY, &nbWalls);
    int walls[4*nbWalls];
    printf("%d\n",nbWalls);
    //si getSnakeArena retourne 0, on commence
    int start = getSnakeArena(walls);


    // // DEBUG et compréhension : pour voir l'arène ainsi que les coordonnées des murs
    // printArena();
    // int j = 0;
    // for (int i = 0; i < 4*nbWalls; i++){
    //     printf("%d \n",walls[i]);
    //     j++;
    //     if (j == 4){
    //         printf("----------\n");
    //         j = 0;
    //     }
    // }

    // à voir si ça sert plus tard
    int test = 1;
    t_move move;
    t_move moveAdv;
    while (test){
        printArena();
        sleep(1.5);
        
        if (!start){
            playMove();
            calcNxtMove();
        }
        else{
            calcNxtMove();
            playMove();
        }
    }
    closeConnection();
    
};

void playMove(){
    printf("Normalement ça joue là");
    t_move move;
    scanf("%d",&move);
    sendMove(move); //Voir si le return code nous sert. Normal = 0; (moi) Win = 1; (moi) je Lose = -1;
}

// TODO
void calcNxtMove(){
    t_move moveAdv;
    printf("On attend le prochain move [en théorie. En pratique le move est calculé et renvoyé en une fraction de secondes.]");
    getMove(&moveAdv); //Voir si le return code nous sert. Normal = 0; (opposant (donc si je lose)) Win = 1; (opposant) Lose = -1;
}


// // Idée pour la structure de données
// Déjà, regarder si on est en statique ou en dynamique

// Si on est en statique, donc avec des éléments qui ne bougent pas, on utilisera probablement des éléments comme des tableaux.
// Pour optimiser la mémoire, on pourra utiliser seulement les coordonnées utiles, donc celles où il y a des murs.

// Pour ce qui est dynamique, on utilisera probablement des structures comme des listes chaînées où figurent prédécesseur, coordonées, successeur.
// Peut-être faudra-t-il utiliser des compteurs en fonction des tours (car le snake grandit lorsque les tours augmentent).

// Enfin, on différencie ce qui est statique de ce qui est dynamique par ce qui bouge.
// Donc les murs sont typiquement statiques là où le snake est purement dynamique.

// Voir algorithmes de Dijkstra et A* pour la suite (ou au moins ceux qui résolvent le problème du plus court chemin.
// Y'a sûrement plus de choses à faire que le plus court chemin mais ça sera déjà un bon "début".)