/*
author: bain à main, whoever that is...
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "snakeAPI.h"


// liste des cases que snake utilise (avec coordonnées, puis la case précédente (vers la queue) puis la case suivante (vers la tête))
typedef struct snakeCase_{
    struct snakeCase_* prev;
    int x;
    int y;
    struct snakeCase_* next;
} snakeCase;

//structrue snake
//Debut, fin , nbtours ?
typedef struct Snake_{
    snakeCase* debut;
    snakeCase* fin;
    int nbTours;
} Snake;

typedef struct point_{
    int N;
    int S;
    int E;
    int O;
    //rajouter un pointeur vers la case du serpent ? (initialiser à NULL, puis voir s'il y a un serpent dans la case en question)
    snakeCase* snake;
} point;

//Structure des données dont on a besoin dans initGame
typedef struct gameData_{
    int sizeX;
    int sizeY;
    int nbWalls;
    char gameName[50];
    char gameType[150];
    int* walls; //Il faudra l'allouer dynamiquement I guess
                //Problème: si on le fait depuis initGame, l'adresse qu'on utilisera sera dans la fonction, donc inutilisable une fois
                //          qu'on sort de la fonction
                // Créer une variable globale ?
} gameData;


void remplirTab(point**, int*);
void playMove();
void calcNxtMove();
void initGame(gameData*);

// void remplirTab(point** tab, int* walls){
//     //à partir de walls, on remplit tab ?

// }


void main(){
    gameData* data = (gameData*)malloc(sizeof(gameData)); //Données que initGame va utiliser/modifier
    initGame(data);
    //tout ce qu'on utilise dans initGame reste dedans => tous les tableaux, nbWalls etc. sont à mettre en paramètre de la fonction.
    //ainsi elle modifiera des adresses mémoire de variables qui ne sont pas dedans et qu'on pourra réutiliser plus tard
    // -> structure gameData

    // à voir si ça sert plus tard
    int start = 1; //On part du principe qu'on commence car c'est le cas pouir l'instant.
    //Voir dans quel cas on commence plus tard
    t_move move;
    t_move moveAdv;
    while (1){
        printArena();
        //sleep(1.5);

        if (start){
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


void initGame(gameData* data){
    // Faire une structure avec les données dont on a besoin dans initGame ?
    connectToServer("localhost",1234,"bruh");
    strcpy(data->gameType, "TRAINING SUPER_PLAYER difficulty=1 timeout=7 seed=123 start=0");
    waitForSnakeGame(data->gameType, data->gameName, &(data->sizeX), &(data->sizeY), &(data->nbWalls));
    printf("test\n");

    // waitForSnakeGame(gameType, gameName, &sizeX, &sizeY, &nbWalls);
    int temp = data->nbWalls;
    // int walls[(4*temp)];
    printf("%d\n",temp);
    // si getSnakeArena retourne 0, on commence. Par ailleurs, on remplit "walls", un tableau contenant les coordonnées des murs.
    // faire un tableau 4*nbWalls pour avoir des vecteurs de 4 int (pour les coordonnées des murs) ?
    int* walls = (int*)malloc(data->nbWalls * 4 * sizeof(int));
    int start = getSnakeArena(walls);

    int structure[data->nbWalls][4];
    int i = 0;
    int j = 0;
    int k = 0;

    // Complète le tableau (et affiche les coordonnées des murs si on décommente)
    for (k = 0; k<data->nbWalls; k++){
        for (j = 0; j<4; j++){
        structure[k][j] = walls[4*k+j];
        //printf("%d \n",structure[k][j]);
        //if (j==3) printf("-------\n");
        }
    }

    //J'imagine qu'il faut aussi mettre les bordures de l'arène.
    //TODO
    // // DEBUG et compréhension : pour voir l'arène ainsi que les coordonnées des murs
    // // (c'était avant l'élaboration de la structure de données. Ça marche toujours en revanche ça sert moins (voire pas du tout)).
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
}


void playMove(){
    /*Calcule (en principe) et joue le prochain move*/
    printf("Normalement ça joue là\n");
    int move;
    scanf("%d",&move);
    printf("lalalala\n");

    sendMove((t_move) move); //Voir si le return code nous sert. Normal = 0; (moi) Win = 1; (moi) je Lose = -1;

    //ajouter une fonction qui permet d'ajouter le move dans la liste chaînée.
    //La fonction en question calculera les coordonées utilisées par le snake qu'on joue.
}

// TODO
void calcNxtMove(){
    /*Calcule le prochain move en fonction du move retourné par getMove et de la structure de données*/
    t_move moveAdv;
    printf("On attend le prochain move [en théorie. En pratique le move est calculé et renvoyé en une fraction de secondes.]\n");
    getMove(&moveAdv); //Voir si le return code nous sert. Normal = 0; (opposant (donc si je lose)) Win = 1; (opposant) Lose = -1;

    //ajouter une fonction qui permet d'ajouter le move dans la liste chaînée.
    //La fonction en question calculera les coordonées utilisées par le snake adverse.
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



// // Update
// Faire une structure où y'a la tête du snake et on analyse systématiquement ce qu'il y a aux points cardinaux.

// Pour l'analyse, il faut checker les murs. Donc il faut faire une structure où sur chaque coordonnée, on note s'il y a un mur (struct NSEO).
// --> tableau 2D de struct NSEO;
// --> fonction pour remplir le tableau
//      --> soit on check pour chaque x;y du tableau de struct s'il y a un conflit, soit pour chaque x;y du tableau "walls", on remplit le
//          tableau de struct  aux bonnes coordonnées. Cette deuxième hypothèse est probablement plus simple et moins encombrante en mémoire.
//      --> P-ê réorganiser "walls" en créant un tableau de structures x1,y1,x2,y2 et à chaque item dudit tableau, on peut remplir le tableau
//          de struct NSEO.
//              --> Inutile, on ne s'en servira qu'une fois en principe. Autant ne pas gâcher de mémoire.
//          À chaque item du tableau (les 4 coords), on déduit dans un premier temps sur quel point cardinal se trouve le mur à partir des coordonnées.
//          On remplit alors le tableau de struct NSEO. On remplit aussi le mur sur la case de la deuxième coordonnée.
//          On fait ça pour tous les points du tableau.
//          P-ê commencer par remplir les contours de l'arène ? Normalement ils ne sont pas dans "walls" donc ouais, c'est à faire en 1er lieu.

// Ensuite, on regarde toutes les coordonnées utilisées par le snake. S'il y a une coordonée qui "rentre en conflit" avec les coordonnées au NSEO
// de la tête, on "ban" ces coordonées là de la décision (sinon il y a collision sur le snake).
// --> Pour chaque point du snake, on compare ses coordonnées à celle de la tête. On en déduit si oui ou non il y a conflit 
//     (il y en aura au moins un. Peut-être pas au premier tour ? (tour 0 I guess...) À voir.)

// Tout ça, ça nous donne à chaque tour la/les décision(s) qu'on peut prendre.

// Y'a un truc encore pas très clair dans ma tête :
// On fait le tableau 2D NSEO. On le remplit. Ensuite, à chaque tour de snake, on check dans le tableau à la coordonnée correspondante
// (donc x;y de la tête est le x;y qu'on check dans le tableau). On màj les points où la tête peut aller.

// Ensuite, on check pour chaque coordonée que le snake monopolise si c'est à une case près dans la tête. On màj NSEO où on peut aller.
// Cette deuxième partie, on la recommence à chaque tour (contrairement à la premiere où on ne re-remplit pas à chaque tour mais on check simplement)




// // notes CM
// 
// Calculer où peuvent aller les deux serpents ?
// attention à gérer la taille pour stocker la position du serpent (?)
// refaire un printarena à partir de la structure et de ce qu'on a dedans
// dans la structure NSEO, mettre un pointeur vers la case du serpent qui est sur la case (pourquoi faire ?)
// besoin de savoir s'il y a des murs et si y'a un serpent, le tout dans une seule structure
// Voir algorithme de coloriage (pour toutes les cases, on voit à partir de combien de coups ladite case est accessible)
// battre random_player = 12/20
// Tout refaire sur papier avec des schémas et tout pour bien comprendre
// Préciser toutes les variables