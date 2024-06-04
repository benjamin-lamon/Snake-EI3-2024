#include <stdbool.h>

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
    bool N;
    bool S;
    bool E;
    bool O;

    //rajouter un pointeur vers la case du serpent ? (initialiser à NULL, puis voir s'il y a un serpent dans la case en question)
    snakeCase* snake;
} point;

// J'imagine qu'il faut que l'arène pointe vers le point (0;0). Faudra allouer data->sizeX * data->sizeY points (c'est plutôt des cases mais "case" est
// un mot clé déjà défini...)
typedef struct arene_{
	point** arene;
} arene;

//Structure des données dont on a besoin dans initGame
typedef struct gameData_{
    int sizeX;
    int sizeY;
    int nbWalls;
	int start;
    char gameName[50];
    char gameType[150];
    int* walls; //Il faudra l'allouer dynamiquement I guess
                //Problème: si on le fait depuis initGame, l'adresse qu'on utilisera sera dans la fonction, donc inutilisable une fois
                //          qu'on sort de la fonction
                // Créer une variable globale ?
    int** tabMurs; //tableau de nbWalls*4 càd nbWalls*(x1,y1,x2,y2)
} gameData;


void remplirTab(arene*, gameData*, int*);
void playMove();
void calcNxtMove();
void initGame(gameData*);
