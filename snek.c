/*
author: bain à main, whoever that is...
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "snakeAPI.h"
#include "snek.h"

void main(){
    // gameData* data = (gameData*)malloc(sizeof(gameData)); //Données que initGame va utiliser/modifier
    gameData data;
    initGame(&data);

    //srand(time(NULL));
    srand(1337);

    //printf("%d\n",data.tabMurs[1][1]);
    //tout ce qu'on utilise dans initGame reste dedans => tous les tableaux, nbWalls etc. sont à mettre en paramètre de la fonction.
    //ainsi elle modifiera des adresses mémoire de variables qui ne sont pas dedans et qu'on pourra réutiliser plus tard
    // -> structure gameData


    // arene* Arena = (arene*)malloc(sizeof(arene));
    remplirTab(&data);

    // DEBUG : voir si le tableau est rempli correctement aux coordonnées (x;y) ; à comparer avec printArena.
    // -> remplirTab FONCTIONNE
    // int x = 0;
    // int y = 0;
    // printf("(%d;%d) N:%d S:%d E:%d O:%d \n",x,y, data.arene[x][y].N,data.arene[x][y].S,data.arene[x][y].E,data.arene[x][y].O);

    // x = 1;
    // printf("(%d;%d) N:%d S:%d E:%d O:%d \n",x,y, data.arene[x][y].N,data.arene[x][y].S,data.arene[x][y].E,data.arene[x][y].O);

    // x = data.sizeX - 1;
    // y = data.sizeY - 1;
    // printf("(%d;%d) N:%d S:%d E:%d O:%d \n",x,y, data.arene[x][y].N,data.arene[x][y].S,data.arene[x][y].E,data.arene[x][y].O);

    // x = 4;
    // y = 1;
    // printf("(%d;%d) N:%d S:%d E:%d O:%d \n",x,y, data.arene[x][y].N,data.arene[x][y].S,data.arene[x][y].E,data.arene[x][y].O);

    // x = 18;
    // y = 8;  
    // printf("(%d;%d) N:%d S:%d E:%d O:%d \n",x,y, data.arene[x][y].N,data.arene[x][y].S,data.arene[x][y].E,data.arene[x][y].O);


    printf(" mySnake(tête) : (%d,%d)\n",data.joueurs[0].Tete->x,data.joueurs[0].Tete->y);
    printf(" mySnake(queue) : (%d,%d)\n",data.joueurs[0].Queue->x,data.joueurs[0].Queue->y);
    printf(" mySnake(nbTours) : %d\n",data.joueurs[0].nbTours);
    printf(" opSnake(tête) : (%d,%d)\n",data.joueurs[1].Tete->x,data.joueurs[1].Tete->y);
    printf(" opSnake(queue) : (%d,%d)\n",data.joueurs[1].Queue->x,data.joueurs[1].Queue->y);
    printf(" opSnake(nbTours) : %d\n",data.joueurs[1].nbTours);
    printf("dim arenes : %dx%d\n", data.sizeX, data.sizeY);

    //tester nbTours des snakes
    //tester les coords des snakes ?



    // t_move move;
    // t_move moveAdv;
    //etat de la game. Si on est a 1, ça joue. Si on est à 0, c'est que la game est terminée.
    int state = 1;
    while (state){
        printArena();
        //sleep(1.5);

        //si c'est à moi de jouer, je joue puis j'update l'arene avec le coup que je veins de faire
        // Si c'est à lui, on getmove (aka takeMove ici) puis on update l'arène avec le coup qui vient d'être joué
        // -> fonction qui, quand on lui donne un coup, màj l'arène (càd les serpents pcq les murs sont statiques)
        //      -> moi ou opposant (même algo)
       
        if (!data.start){
            state = playMove(&data);
            state = takeMove(&data);
            compteurUpdate(&data);
        }
        else{
            state = playMove(&data);
            state = takeMove(&data);
            compteurUpdate(&data);
        }
    }

    
};


void initGame(gameData* p_data){

    connectToServer("localhost",1234,"bruh");
    waitForSnakeGame("TRAINING RANDOM_PLAYER difficulty=1 timeout=7 seed=123 start=0", p_data->gameName, &(p_data->sizeX), &(p_data->sizeY), &(p_data->nbWalls));
    // waitForSnakeGame(gameType, gameName, &sizeX, &sizeY, &nbWalls);
    // int temp = p_data->nbWalls;
    // // int walls[(4*temp)];
    // printf("%d\n",temp);


    // si getSnakeArena retourne 0, on commence. Par ailleurs, on remplit "walls", un tableau contenant les coordonnées des murs.
    // faire un tableau 4*nbWalls pour avoir des vecteurs de 4 int (pour les coordonnées des murs) ?
    int* wallsInitial = (int*)malloc(p_data->nbWalls * 4 * sizeof(int));
    p_data->start = getSnakeArena(wallsInitial);

    //Il ne s'agit pas de la structure avec laquelle on va travailler par la suite, mais d'une "pré-structure" dans laquelle on va
    //mettre chaque set de 4 coordonnées
    p_data->tabMurs = (int**) malloc(p_data->nbWalls*sizeof(int*));
    for (int i = 0; i<p_data->nbWalls; i++){
        p_data->tabMurs[i] = (int*)malloc(4*sizeof(int));
    }


    // Complète le tableau (et affiche les coordonnées des murs si on décommente)
    for (int k = 0; k<p_data->nbWalls; k++){
        for (int l = 0; l<4; l++){
        p_data->tabMurs[k][l] = wallsInitial[4*k+l];
        // printf("%d \n",p_data->tabMurs[k][l]);
        // if (l==3) printf("-------\n");
        }
    }
    free(wallsInitial);


    //Tableau des deux snakes
    p_data->joueurs = (Snake*)malloc(2*sizeof(Snake));

    // //Probablement inutile, c'est pas un tableau 2D qu'on manipule ici...
    // //On n'a et on n'aura toujours que deux snakes, pas besoin de for loop
    // for (int j = 0; j < 2; j++){
    //     p_data->joueurs[j] = (Snake*)malloc(sizeof(Snake));
    // }

    // //Initialisation des deux snakes

    //initialisation dynamique d'un snake
    Snake mySnake;
    snakeCase* myCase = (snakeCase*)malloc(sizeof(snakeCase));
    myCase->prev = NULL;
    myCase->next = NULL;
    myCase->x = 2;
    myCase->y = (p_data->sizeY)/2;

    mySnake.Tete = myCase;
    mySnake.Queue = myCase;
    mySnake.nbTours = 0;

    //Opponant snake
    Snake opSnake; // = (Snake*)malloc(sizeof(Snake));
    snakeCase* opCase = (snakeCase*)malloc(sizeof(snakeCase));
    opCase->prev = NULL;
    opCase->next = NULL;
    opCase->x = (p_data->sizeX) - 3; 
    opCase->y = (p_data->sizeY)/2;

    opSnake.Tete = opCase;
    opSnake.Queue = opCase;
    opSnake.nbTours = 0;

    p_data->joueurs[0] = mySnake;
    p_data->joueurs[1] = opSnake;

    p_data->tour = 1;

}



int playMove(gameData* p_data){
    /*Calcule (en principe) et joue le prochain move*/

    // printf("Normalement ça joue là\n");

    // // jouer manuellement
    // int move;
    // scanf("%d",&move);
    // sendMove((t_move) move); //Voir si le return code nous sert. Normal = 0; je Win = 1; je Lose = -1;

    t_move move = playRandom(p_data);
    t_return_code x = sendMove((t_move) move);
    if (x != 0){
        
        if (x == 1){
            printf("Vous avez gagné. L'opposant a perdu.");
            return 0;
        }
        else if (x == -1){
            printf("Vous avez perdu. L'opposant a gagné.");
            return 0;
        }
    }


    // Si on est à t0, on ajoute une case de tête mais on ne retire pas la case de queue; 
    // Sinon, on rajoute une tête et on enlève la queue.

    //ajout d'une nouvelle tête
    snakeCase* next = (snakeCase*)malloc(sizeof(snakeCase));

    //le précédent du snake est le précédent de la case créée ; son next est vide.
    next->prev = p_data->joueurs[0].Tete;
    next->next = NULL;
    //ajouter les coords d'où se trouve le point en question

    //Test ; ces deux lignes font une erreur de segmentation (donc quasi tout ce qu'il y a dans le switch aussi...)
        // next->y = p_data->joueurs[0].Tete->next->y - 1;
        // next->x = p_data->joueurs[0].Tete->next->x;
            // Evidemment, y'a PAS de next à la tête...

    switch((int)move){
        case 0: //Nord
        // printf("%d\n",p_data->joueurs[0].Tete->y);
        // printf("%d\n",p_data->joueurs[0].Tete->x);
        next->y = p_data->joueurs[0].Tete->y - 1;
        next->x = p_data->joueurs[0].Tete->x;
        break;
        
        case 1: //Est
        next->y = p_data->joueurs[0].Tete->y;
        next->x = p_data->joueurs[0].Tete->x + 1;
        break;
        
        case 2: //Sud
        next->y = p_data->joueurs[0].Tete->y + 1;
        next->x = p_data->joueurs[0].Tete->x;
        break;

        case 3: //Ouest
        next->y = p_data->joueurs[0].Tete->y;
        next->x = p_data->joueurs[0].Tete->x - 1;
        break;

        default:
        // printf("Erreur, ce n'est pas un move. Normalement, ça va planter juste après ce mess. \n");
    }
    //repointer vers la tête du serpent
    p_data->joueurs[0].Tete->next = next;
    p_data->joueurs[0].Tete = next;
    // p_data->joueurs[0].nbTours += 1;

    // Si on est à T0, on ne change pas la queue
    
    if (p_data->tour == 1){
        // printf(" mySnake(nbTours) : %d\n",p_data->joueurs[0].nbTours);
        p_data->joueurs[0].nbTours = p_data->tour;
        
        // printf("---------------------------ajout\n");
    }
    //Sinon (donc si nbTours <= 9)
    else{
        // printf("pas d'ajout---------------------------\n");
        snakeCase* temp = p_data->joueurs[0].Queue->next;
        free(p_data->joueurs[0].Queue);
        p_data->joueurs[0].Queue = temp;
        p_data->joueurs[0].Queue->prev = NULL;
        p_data->joueurs[0].nbTours = p_data->tour;
    }

    // printf(" mySnake(tête) : (%d,%d)\n",p_data->joueurs[0].Tete->x,p_data->joueurs[0].Tete->y);
    // printf(" mySnake(queue) : (%d,%d)\n",p_data->joueurs[0].Queue->x,p_data->joueurs[0].Queue->y);
    // printf(" mySnake(nbTours) : %d\n",p_data->joueurs[0].nbTours);
    //ajouter une fonction qui permet d'ajouter le move dans la liste chaînée.
    //La fonction en question calculera les coordonées utilisées par le snake qu'on joue.
    return 1;
}

void compteurUpdate(gameData* p_data){
    /*Update le compteur. à utiliser à chaque fin de tour (donc dans la boucle infinie du main)*/
    if (p_data->tour == 9){
        p_data->tour = 0;
    }
    else{
        p_data->tour += 1;
    }
}

//checker si je mets bien à jour les pointeurs next ET prev...
//maj la tête et la queue
//surtout, next de l'ancienne tete et temp->prev = NULL
int takeMove(gameData* p_data){
    /*On reprend quasi le même code que pour playMove (au moins la base càd comment le snake (adverse ici) bouge.)*/
    //voir le code et les commentaires dans playMove (juste au-dessus) pour en savoir plus
    t_move moveAdv;
    // printf("On attend le prochain move [en théorie. En pratique le move est calculé et renvoyé en une fraction de secondes.]\n");


    t_return_code x = getMove(&moveAdv);
    if (x != 0){
        
        if (x == 1){
            printf("Vous avez perdu. L'opposant a gagné.\n");
            return 0;
        }
        else if (x == -1){
            printf("Vous avez gagné. L'opposant a perdu.\n");
            return 0;
        }
    }
     //Voir si le return code nous sert. Normal = 0; (opposant (donc si je lose)) Win = 1; (opposant) Lose = -1;
    // printf("move Adverse (NESO) : %d\n",(int)moveAdv);
    snakeCase* nextAdv = (snakeCase*)malloc(sizeof(snakeCase));
    nextAdv->prev = p_data->joueurs[1].Tete;
    nextAdv->prev->next = nextAdv;
    nextAdv->next = NULL;

    switch((int)moveAdv){
        case 0: //Nord
        nextAdv->y = p_data->joueurs[1].Tete->y - 1;
        nextAdv->x = p_data->joueurs[1].Tete->x;
        break;

        case 1: //Est
        nextAdv->y = p_data->joueurs[1].Tete->y;
        nextAdv->x = p_data->joueurs[1].Tete->x + 1;
        break;

        case 2: //Sud
        nextAdv->y = p_data->joueurs[1].Tete->y + 1;
        nextAdv->x = p_data->joueurs[1].Tete->x;
        break;

        case 3: //Ouest
        nextAdv->y = p_data->joueurs[1].Tete->y;
        nextAdv->x = p_data->joueurs[1].Tete->x - 1;
        break;

        default:
        // printf("Erreur, ce n'est pas un move. Normalement, ça n'arrivera pas. \n");
    }

    p_data->joueurs[1].Tete = nextAdv;

    
    // Si on est à T0, on ne change pas la queue
    if (p_data->tour == 1){
        // printf(" opSnake(nbTours) : %d\n",p_data->joueurs[1].nbTours);
        p_data->joueurs[1].nbTours = p_data->tour;
        
        // printf("-----------------------OP--ajout\n");
    }
    //Sinon (donc si nbTours <= 9)
    else{
        // printf("pas d'ajout--OP-----------------------\n");
        snakeCase* tempAdv = p_data->joueurs[1].Queue->next;
        free(p_data->joueurs[1].Queue);
        p_data->joueurs[1].Queue = tempAdv;
        p_data->joueurs[1].Queue->prev = NULL;
        p_data->joueurs[1].nbTours = p_data->tour;
    }


    // printf(" opSnake(tête) : (%d,%d)\n",p_data->joueurs[1].Tete->x,p_data->joueurs[1].Tete->y);
    // printf(" opSnake(queue) : (%d,%d)\n",p_data->joueurs[1].Queue->x,p_data->joueurs[1].Queue->y);
    // printf(" opSnake(nbTours) : %d\n",p_data->joueurs[1].nbTours);
    return 1;
}




void remplirTab(gameData* p_data){ 
    /*
    Remplit un tableau "p_data" de points à partir des données reçues par le jeu 
    (depuis la fonction getSnakeArena dans initGame qui a écrit dans p_data->tabMurs).
    */

    //allocation de l'arene en points càd point[sizeX][sizeY];
    p_data->arene = (point**)malloc((p_data->sizeX+1)*sizeof(point*));
    for (int x = 0; x<p_data->sizeX+1; x++){
        p_data->arene[x] = (point*)malloc((p_data->sizeY+1)*sizeof(point));
    }

    // Initialiser NSEO à false
    //remplacer i;j par x;y
    for (int i = 0; i < p_data->sizeX+1; i++){
        for (int j = 0; j < p_data->sizeY+1; j++){
            p_data->arene[i][j].N = false;
            p_data->arene[i][j].S = false;
            p_data->arene[i][j].E = false;
            p_data->arene[i][j].O = false;
        }
    }

    //remplir les bordures de l'arène
    for (int i = 0; i<p_data->sizeX+1; i++){
        p_data->arene[i][0].N = true;
        p_data->arene[i][p_data->sizeY-1].S = true;
    }
    for (int j = 0; j<p_data->sizeY+1; j++){
        p_data->arene[0][j].O = true;
        p_data->arene[p_data->sizeX-1][j].E = true;
    }

    //remplir les données des murs
    int x1,y1,x2,y2;
   
    //Pour chaque point, on parcourt toutes les coordonnées des murs.
    for (int k = 0; k<p_data->nbWalls; k++){
        x1 = p_data->tabMurs[k][0];
        y1 = p_data->tabMurs[k][1];
        x2 = p_data->tabMurs[k][2];
        y2 = p_data->tabMurs[k][3];

        // Si (x;y) et (x1;y1) matchent, on check où se trouve le mur. Quand on l'a trouvé, on modifie le flag 
        // (N, S, E ou O) du point de l'arène.
        // On refait la même chose avec (x2;y2) juste après.
        // if ((x == x1) && (y == y1)){
        if (y1 == y2){
            if (x1 < x2){
                p_data->arene[x1][y1].E = true;
                p_data->arene[x2][y2].O = true;
            }
            else{
                p_data->arene[x1][y1].O = true;
                p_data->arene[x2][y2].E = true;
            }
        }
        if (x1 == x2){
            if (y1 < y2){
                p_data->arene[x1][y1].S = true;
                p_data->arene[x2][y2].N = true;
            }
            else{
                p_data->arene[x1][y1].N = true;
                p_data->arene[x2][y2].S = true;
            }
        }
    }
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
//              --> oups... trop tard.
//              --> y'a peut-être une redondance de données dans Data, à voir
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
//  ->  la taille du serpent augmente. Donc la place qu'il prend en mémoire aussi, donc faut gérer tout ça
//      (normalement pas trop compliqué vu que c'est une liste (doublement) chaînée)
// refaire un printarena à partir de la structure et de ce qu'on a dedans
// dans la structure NSEO, mettre un pointeur vers la case du serpent qui est sur la case (pourquoi faire ?)
// besoin de savoir s'il y a des murs et si y'a un serpent, le tout dans une seule structure
// Voir algorithme de coloriage (pour toutes les cases, on voit à partir de combien de coups ladite case est accessible)
// battre random_player = 12/20
// Tout refaire sur papier avec des schémas et tout pour bien comprendre
// Préciser toutes les variables


// dans initgame, positionner les serpents càd créer la tête, la queue et les cases
// 

// Ajout d'une case entre tour 0 et 1
// On commence à tour 0
// les tours sont modulo 10 (donc t10 = t0 => une case en plus au tour suivant)

// Faire une fonction qui joue aléatoirement (sans toucher de murs) :
//  -> En gros, dès qu'on veut jouer un move, on check les murs (et le snake) et on les évite.

t_move playRandom(gameData* p_data){
    //erreur de segmentation quand on va à l'index 0...


    // On prend les coords de la tete du snake, on voit s'il y a un mur à proximité. Une fois qu'on a les directions fermées, on en prend une au hasard.
    int x_snake = p_data->joueurs[0].Tete->x;
    int y_snake = p_data->joueurs[0].Tete->y;

    p_data->arene[x_snake][y_snake];
    //

    // Tant qu'on n'est pas à NULL, on parcourt tous les previous et si on match sur les autres, ??
    // 

    //faire une copie de l'arène puis manipuler cette copie pour déterminer où aller.
    // -> array 2D de point
    point** areneTemp = (point**)malloc((p_data->sizeX+1)*sizeof(point*));
    for (int x = 0; x < p_data->sizeX+1; x++){
        areneTemp[x] = (point*)malloc((p_data->sizeY+1)*sizeof(point));
    }
    
    // pour chaque membre de l'array, on copie les valeurs qui sont dans p_data -> arene
    for (int x = 0; x < p_data->sizeX+1; x++){
        for (int y = 0; y < p_data->sizeY+1; y++){
            areneTemp[x][y].N = p_data->arene[x][y].N;
            areneTemp[x][y].S = p_data->arene[x][y].S;
            areneTemp[x][y].E = p_data->arene[x][y].E;
            areneTemp[x][y].O = p_data->arene[x][y].O;
        }
    }


    //Erreur de segmentation ici

    //une fois que c'est fait, pour chaque case que prend le snake, faut mettre S de la case au nord, N de la case au sud, O de la case à l'est et E de la case à l'ouest
    //on commence par le snake adverse:
    int xAdv,yAdv;
    snakeCase* pSnakeAdv = p_data->joueurs[1].Tete;
    while (!(pSnakeAdv == NULL)){
        xAdv = pSnakeAdv->x;
        yAdv = pSnakeAdv->y;
        //condition pour qu'on ne soit jamais à -1 ?
        // printf("op(%d,%d)\n",xAdv,yAdv);

        //Condition pour ne pas avoir d'indice -1 (tentative désespérée de résoudre la segmentation fault...)
        if (xAdv == 0){
            //case au nord de l'actuelle
            areneTemp[xAdv][yAdv - 1].S = true;
            //sud
            areneTemp[xAdv][yAdv + 1].N = true;
            //est
            areneTemp[xAdv + 1][yAdv].O = true;
        }
        else if (yAdv == 0){
            //sud
            areneTemp[xAdv][yAdv + 1].N = true;
            //est
            areneTemp[xAdv + 1][yAdv].O = true;
            //ouest
            areneTemp[xAdv - 1][yAdv].E = true; 
        }
        else{
            //case au nord de l'actuelle
            areneTemp[xAdv][yAdv - 1].S = true;
            //sud
            areneTemp[xAdv][yAdv + 1].N = true;
            //est
            areneTemp[xAdv + 1][yAdv].O = true;
            //ouest
            areneTemp[xAdv - 1][yAdv].E = true;
        }




        pSnakeAdv = pSnakeAdv->prev;
        
    } 
    free(pSnakeAdv);

    int xMe,yMe;
    snakeCase* pSnakeMe = p_data->joueurs[0].Tete;
    while (!(pSnakeMe == NULL)){
        xMe = pSnakeMe->x;
        yMe = pSnakeMe->y;
        //condition pour qu'on ne soit jamais à -1 ?
        // printf("me(%d,%d)\n",xMe,yMe);
        //case au nord de l'actuelle
        if (xMe == 0){
            //case au nord de l'actuelle
            areneTemp[xMe][yMe - 1].S = true;
            //sud
            areneTemp[xMe][yMe + 1].N = true;
            //est
            areneTemp[xMe + 1][yMe].O = true;
        }
        else if (yMe == 0){
            //sud
            areneTemp[xMe][yMe + 1].N = true;
            //est
            areneTemp[xMe + 1][yMe].O = true;
            //ouest
            areneTemp[xMe - 1][yMe].E = true; 
        }
        else{
            //case au nord de l'actuelle
            areneTemp[xMe][yMe - 1].S = true;
            //sud
            areneTemp[xMe][yMe + 1].N = true;
            //est
            areneTemp[xMe + 1][yMe].O = true;
            //ouest
            areneTemp[xMe - 1][yMe].E = true;
        }

        pSnakeMe = pSnakeMe->prev;
    } 
    free(pSnakeMe);
    

    // int x = 26;
    // int y = 5;
    // printf("ARENE TEMP: (%d;%d) N:%d S:%d E:%d O:%d \n",x,y, areneTemp[x][y].N,areneTemp[x][y].S,areneTemp[x][y].E,areneTemp[x][y].O);


    // x = 3;
    // y = 6;
    // printf("ARENE TEMP: (%d;%d) N:%d S:%d E:%d O:%d \n",x,y, areneTemp[x][y].N,areneTemp[x][y].S,areneTemp[x][y].E,areneTemp[x][y].O);

    // x = p_data->joueurs[0].Tete->x;
    // y = p_data->joueurs[0].Tete->y;
    // printf("ARENE TEMP: (%d;%d) N:%d S:%d E:%d O:%d \n",x,y, areneTemp[x][y].N,areneTemp[x][y].S,areneTemp[x][y].E,areneTemp[x][y].O);

    // x = 18;
    // y = 8;  
    // printf("ARENE TEMP: (%d;%d) N:%d S:%d E:%d O:%d \n",x,y, areneTemp[x][y].N,areneTemp[x][y].S,areneTemp[x][y].E,areneTemp[x][y].O);




    //À ce moment-là, on a une arène avec tous les endroits où on ne peut pas aller.
    //On peut alors jouer un coup random à chaque fois tant qu'on ne tombe pas dans un cul de sac. (pcq on ne peut prédire qu'un coup à l'avance...)

    //générer un coup aléatoire (entre 0 et 3.)
    //checker si ce coup est valide
    //S'il ne l'est pas, on régénère
    // -> avec un flag ?
    int coup;
    bool flag = false;
    while (flag == false){
        //générer coup aléatoirement
        coup = rand() % 4;

        //vérifier les murs autour de la tête du snake qu'on joue, adapter le flag en conséquence.
        switch (coup){
            //N
            case 0: 
                //vérifier le mur au nord de la tête
                if (areneTemp[p_data->joueurs[0].Tete->x][p_data->joueurs[0].Tete->y].N == 0){
                    flag = true;
                }
                // printf("FLAG : %d ; COUP : %d\n", flag,coup);
                break;
            //E
            case 1:
                if (areneTemp[p_data->joueurs[0].Tete->x][p_data->joueurs[0].Tete->y].E == 0){
                    flag = true;
                }
                // printf("FLAG : %d ; COUP : %d\n", flag,coup);
                break;
            //S
            case 2:
                if (areneTemp[p_data->joueurs[0].Tete->x][p_data->joueurs[0].Tete->y].S == 0){
                    flag = true;
                }
                // printf("FLAG : %d ; COUP : %d\n", flag,coup);
                break;
            //O
            case 3:
                if (areneTemp[p_data->joueurs[0].Tete->x][p_data->joueurs[0].Tete->y].O == 0){
                    flag = true;
                }
                // printf("FLAG : %d ; COUP : %d\n", flag,coup);
                break;
            //pas un coup
            default:
                break;
        }

    }

    // printf("FLAG : %d ; COUP : %d\n", flag,coup);
    for (int x = 0; x < p_data->sizeX+1; x++){
        free(areneTemp[x]);
    }
    free(areneTemp);

    return (t_move)coup;
}

