//
//This source file provides the functionality of a DoD 
//player. It uses fields from GameLogic.java 
//accompanied with C functions for additional  
//functionality. The file #includes 2 header files:
// 1. The "GameLogic.h" is the header file for GameLogic.java native methods.
// 2. The "Map.h" is a header file for Map.c which mimics the functiopnality
// of Map.java*(previous courseworks)
//
// @author Konstantin Simeonov
//
#include <jni.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "GameLogic.h"
#include "Map.h"

//Headers
int *initiatePlayer();
bool checkWin(int collectedGold,char lastPosition);
int *initiatePlayer();
bool checkWin(int collectedGold, char lastPosition);
void saveState(const char *pName, int cGold, int p0, int p1,
               char currentTile);
void saveNewPlayer(const char *pName, int cGold, int p0, int p1,
                   char currentTile);
void savePlayer();
void saveMapState();
void loadPlayer();
void loadMapState();

//external variables from the Map.c file.
extern char *mapName, **map;
extern int totalGoldOnMap, rows, cols;
//counters for the number of players/gold collected during a DoD game.
int playerCounter = 0, collectedGoldCounter = 0;

//This defines a structure containing all
//the information held by a DoD player(it will be savet to a "tempFile" file later).
typedef struct player {
    char name[30];
    int gold;
    int positionOnMap[2];
    char lastposition;
} player;

//This defines a structure containing all
//the information held by a DoD game(it will be savet to a "tempMap" file later).
typedef struct tempMap {
    char nameOfMap[30];
    int goldPosition[2];
} tempMap;

//We initialise two pointer arrays to structures
//of types defined above.
struct player *allPlayers;
struct tempMap *mapState;




/* 
 * This function takes a player name as jstring
 * and initialises a DoD player on a random position(if the game is reloaded)
 * or on a random position on the map if the player is new. It also assignes the right 
 * amount of gold  and the tile on which a player had been standing in case he is from
 * the same game, but has left for some reason.
 * Class:     GameLogic
 * Method:    startGame
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_GameLogic_startGame
  (JNIEnv *env, jobject thisObject, jstring playerName){
    //int array variable to store the player position from java 
	int *playerPosition = calloc(2,sizeof(int));
	//Initialising the playerposition,collectedGold,playerName,active
	//and collectedGold fields from java using jni functions;
	jclass logic = (*env) -> GetObjectClass(env,thisObject);
	jfieldID pposition = (*env) -> GetFieldID(env,logic,"playerPosition","[I");
	jfieldID lastPosition = (*env) -> GetFieldID(env,logic,"lastPosition","C");
	jchar currentTile = (*env) -> GetCharField(env,thisObject, lastPosition);
    jfieldID currentPlayer = (*env)->GetFieldID(env, logic, "playerName", "Ljava/lang/String;");
	jfieldID active = (*env) -> GetFieldID(env,logic,"active","Z");
	jintArray player = (*env) -> GetObjectField(env, thisObject, pposition);
	jint *position = (*env) -> GetIntArrayElements(env, player, 0);
	jfieldID gold = (*env) -> GetFieldID(env,logic,"collectedGold","I");
    jint collectedGold = (*env) -> GetIntField(env, thisObject, gold);
    const char *nameOfPlayer  = (char*)(*env)->GetStringUTFChars(env, playerName, 0);
	/*
	*/
	//
	//pointer to a "player" structure to use for iterating through the save file
    struct player *ptr;
	//variable to check if a player "exist" on the save file.
	//and a counter;
    int exists = 0,i = 0;
	//Loop that checks if the player name matches one from a list of
	//saved players and if so assigns all the attributes of that player.
	 for (ptr = allPlayers; i < playerCounter; ptr++) {
        
        if(strcmp(nameOfPlayer, ptr->name) == 0){
		//In case the position of the saved player is being occupied by 
		//another one the player is put on a random position.
            if (lookAtTile(ptr->positionOnMap[0], ptr->positionOnMap[1]) == 'P'
                || lookAtTile(ptr->positionOnMap[0], ptr->positionOnMap[1]) == 'B') {
                playerPosition  = initiatePlayer();
                ptr->positionOnMap[0] = playerPosition[1];
                ptr->positionOnMap[1] = playerPosition[1];
                (*env) -> SetCharField(env,thisObject,lastPosition,lookAtTile(ptr->positionOnMap[0],
                                                                       ptr->positionOnMap[1]));
                
            }
            position[0] = ptr->positionOnMap[0];
            position[1] = ptr->positionOnMap[1];
            (*env) -> SetIntField(env,thisObject,gold,ptr->gold);
            (*env) -> SetCharField(env,thisObject,lastPosition,lookAtTile(position[0], position[1]));
            replaceTile(position[0],position[1], 'P');
            exists = 1;
        }
        i++;
        
    }
	//In case the player is a "new player" he is spawn on a random tile. 
    if (!exists) {
        playerPosition = initiatePlayer();
        position[0] = playerPosition[0];
        position[1] = playerPosition[1];
        (*env) -> SetCharField(env,thisObject,lastPosition,lookAtTile(position[0],position[1]));
        replaceTile(position[0],position[1], 'P');
        saveNewPlayer(nameOfPlayer, collectedGold, position[0],
                      position[1], currentTile);
    }
	//Assigning fields in GameLogic.java and releasing memory.
    (*env) -> ReleaseStringUTFChars(env, playerName, nameOfPlayer);
    (*env) -> SetObjectField(env, thisObject, currentPlayer, playerName);
	(*env) -> SetBooleanField(env,thisObject,active,true);
	(*env) -> ReleaseIntArrayElements(env, player, position, 0);


}


/*
 * This function checks if a saved state of the current game and exists
 * and if so loads from save files.
 * Class:     GameLogic
 * Method:    setMap
 * Signature: (Ljava/io/File;)V
 */
JNIEXPORT void JNICALL Java_GameLogic_setMap
  (JNIEnv *env, jobject thisObject, jstring mapName){
    FILE *file;
	//Initialising the active field from GameLogic.java using jni functions.
	jclass logic = (*env) -> GetObjectClass(env,thisObject);
	jfieldID active = (*env) -> GetFieldID(env,logic,"active","Z");
	const char *name  = (char*)(*env)->GetStringUTFChars(env, mapName, 0);
	//Check if a save file of the current game exist and if so such is loaded.
	file = fopen("tempMap", "r");
    readMap(name);
    mapState = malloc(rows * cols * sizeof(struct tempMap));
    allPlayers  = malloc(rows * cols * sizeof(struct player));
    if (file != NULL) {
        loadMapState();
        int i = 0;
        struct tempMap *ptr;
        if(strcmp(mapState -> nameOfMap, name) == 0){
            for(ptr = mapState; i < collectedGoldCounter;ptr++){
                replaceTile(ptr -> goldPosition[0],ptr -> goldPosition[1],'.');
                i++;
            }
            fclose(file);
            file = fopen("Temp","r");
            if(file != NULL)
            {
                loadPlayer();
            }
        }
    }
    strcpy(mapState -> nameOfMap, name);
    
    
   //Assigning fields in GameLogic.java and releasing memory.
  (*env) -> SetBooleanField(env,thisObject,active,true);
  (*env) -> ReleaseStringUTFChars(env, mapName, name);
  //fclose(f);


}

/*
 * This function returns the gold left for the player to collect
 * in order to win the game as a jstring.
 * Class:     GameLogic
 * Method:    hello
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_GameLogic_hello
  (JNIEnv *env, jobject thisObject){
  //Initialising the collectedGold field from GameLogic.java using jni functions.
	jclass logic = (*env) -> GetObjectClass(env,thisObject);
	jfieldID gold = (*env) -> GetFieldID(env,logic,"collectedGold","I");
	jint collectedGold = (*env) -> GetIntField(env, thisObject, gold);
	char *output = malloc(sizeof("GOLD: ") + 3*sizeof(int));
	sprintf(output, "GOLD: %d", totalGoldOnMap - collectedGold);
	return (*env)->NewStringUTF(env, output);
}

/* This function mimics the functionality of the original DoD move 
 * function but saves the impact of every move as well as performing it.
 * Class:     GameLogic
 * Method:    move
 * Signature: (CZ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_GameLogic_move
  (JNIEnv *env, jobject thisObject, jchar direction, jboolean bot){
	int *newPosition = calloc(2,sizeof(int));
	//Initialising the playerposition,collectedGold,playerName,active
	//and collectedGold fields from java using jni functions;
	jclass logic = (*env) -> GetObjectClass(env,thisObject);
	jfieldID pposition = (*env) -> GetFieldID(env,logic,"playerPosition","[I");
	jfieldID currentPlayer = (*env)->GetFieldID(env, logic, "playerName", "Ljava/lang/String;");
        jstring pName = (*env)->GetObjectField(env, thisObject, currentPlayer);
        const char *playerName = (char*) (*env)->GetStringUTFChars(env, pName, 0);
	jintArray player = (*env) -> GetObjectField(env, thisObject, pposition);
	jfieldID last = (*env) -> GetFieldID(env,logic,"lastPosition","C");
	jfieldID active = (*env) -> GetFieldID(env,logic,"active","Z");
	jfieldID gold = (*env) -> GetFieldID(env,logic,"collectedGold","I");
	jint collectedGold = (*env) -> GetIntField(env, thisObject, gold);
	jchar lastPosition = (*env) -> GetCharField(env, thisObject, last);
	jint *position = (*env) -> GetIntArrayElements(env, player, 0);
	newPosition[0] = position[0];
    newPosition[1] = position[1];
	/*
	 */	 

  switch (direction){
      case 'N':
          newPosition[0] -=1;
          break;
      case 'E':
          newPosition[1] +=1;
          break;
      case 'S':
          newPosition[0] +=1;
          break;
      case 'W':
          newPosition[1] -=1;
          break;
      default:
          break;
  }

  if(lookAtTile(newPosition[0], newPosition[1]) != '#' && lookAtTile(newPosition[0], newPosition[1]) != 'P'
          && lookAtTile(newPosition[0], newPosition[1]) != 'B'){
      //When player moves from one tile to another ,previous one is restored to be '.','G' or 'E' again.
      replaceTile(position[0],position[1],lastPosition);
      (*env) -> SetCharField(env, thisObject, last, lookAtTile(newPosition[0], newPosition[1]));
      char pos = lookAtTile(newPosition[0], newPosition[1]);
      position[0] = newPosition[0];
      position[1] = newPosition[1];

      //Depending on if the method is called from bot/client the tile on which the player is is replaced by 'B' or 'P'.
      if(bot){
          replaceTile(position[0], position[1], 'B');
      }
      else{
          replaceTile(position[0], position[1], 'P');
      }
	  //If some player wins a message is returned.
      if (checkWin(collectedGold,pos)) {
    	  (*env) -> SetBooleanField(env,thisObject,active,false);
          return (*env)->NewStringUTF(env, "Congratulations!!! \n You have escaped the Dungeon of Doom!!!!!! \n"
        		  "Thank you for playing! \n The game will now exit!");;

      }
	  //The impact of the player move is saved to a file.
	  saveState(playerName, collectedGold, newPosition[0], newPosition[1],
                  pos);
	  //Freeing memory.
      (*env)->ReleaseIntArrayElements(env, player, position, 0);
      return (*env)->NewStringUTF(env, "SUCCESS");
  } else {
      return (*env)->NewStringUTF(env, "FAIL");
  }
}


/*
 * This function deletes a player from the map in case he disconnects.
 * Class:     GameLogic
 * Method:    resetPosition
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_GameLogic_resetPosition
  (JNIEnv *env, jobject thisObject){
    //Initialising the playerposition and lastposition
	//fields from GameLogic.java using jni functions;
	jclass logic = (*env) -> GetObjectClass(env,thisObject);
	jfieldID last = (*env) -> GetFieldID(env,logic,"lastPosition","C");
	jchar lastPosition = (*env) -> GetCharField(env, thisObject, last);
	jfieldID pposition = (*env) -> GetFieldID(env,logic,"playerPosition","[I");
	jintArray player = (*env) -> GetObjectField(env, thisObject, pposition);
	jint *playerPosition = (*env) -> GetIntArrayElements(env, player, 0);
	//deletes the player from the map.
	replaceTile(playerPosition[0],playerPosition[1],lastPosition);

}

/*
 * This function mimics the functionality of the original DoD 
 * pickup method, but saves the impact of it.
 * Class:     GameLogic
 * Method:    pickup
 * Signature: (Z)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_GameLogic_pickup
  (JNIEnv *env, jobject thisObject, jboolean bot){
    //Initialising the playerposition,collectedGold,playerName
	//and collectedGold fields from GameLogic.java using jni functions;
	jclass logic = (*env) -> GetObjectClass(env,thisObject);
	jfieldID last = (*env) -> GetFieldID(env,logic,"lastPosition","C");
	jchar lastPosition = (*env) -> GetCharField(env, thisObject, last);
	jfieldID pposition = (*env) -> GetFieldID(env,logic,"playerPosition","[I");
	jfieldID currentPlayer = (*env)->GetFieldID(env, logic, "playerName", "Ljava/lang/String;");
    jstring pName = (*env)->GetObjectField(env, thisObject, currentPlayer);
    const char *playerName = (char*)(*env)->GetStringUTFChars(env, pName, NULL);
	jintArray player = (*env) -> GetObjectField(env, thisObject, pposition);
	jfieldID gold = (*env) -> GetFieldID(env,logic,"collectedGold","I");
	jint collectedGold = (*env) -> GetIntField(env, thisObject, gold);
	jint *playerPosition = (*env) -> GetIntArrayElements(env, player, 0);
	/*
	*/
	 if (lastPosition == 'G') {
		        (*env) -> SetIntField(env,thisObject,gold,++collectedGold);
	            replaceTile(playerPosition[0], playerPosition[1], bot ? 'B' : 'P');
	            (*env) -> SetCharField(env,thisObject,last,(char)'.');
	            char *output = malloc(sizeof("SUCCESS, GOLD COINS: ") + 3*sizeof(int));
	            sprintf(output, "SUCCESS, GOLD COINS: %d", collectedGold);
				//Saves the state of the player.
				saveState(playerName, collectedGold, playerPosition[0], playerPosition[1],
                  '.');
				struct tempMap *ptr = mapState;
				int i = 0;
				// Iterating through the array of structs 
				// in order to put the new position on which the gold 
				// has been taken on the map.
				while (i<collectedGoldCounter) {
				ptr++;
				i++;
				}
				collectedGoldCounter++;
				ptr -> goldPosition[0] = playerPosition[0];
				ptr -> goldPosition[1] = playerPosition[1];
				//The current state of the map is saved.
				saveMapState();
	            return (*env)->NewStringUTF(env, output);
	        }

	        return (*env)->NewStringUTF(env, "FAIL\nThere is nothing to pick up...");

}

/*
 * A function that returns the lookWindow of 
 * the player to a specified radius as a jstring.
 * Class:     GameLogic
 * Method:    look
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_GameLogic_look
  (JNIEnv *env, jobject thisObject){
	jclass logic = (*env) -> GetObjectClass(env,thisObject);
	jfieldID pposition = (*env) -> GetFieldID(env,logic,"playerPosition","[I");
	jintArray player = (*env) -> GetObjectField(env, thisObject, pposition);
	jint *playerPosition =(*env) -> GetIntArrayElements(env, player, 0);
	int radius = 5;
	return (*env)->NewStringUTF(env,lookWindow(playerPosition[0],playerPosition[1],radius));
}

/* 
 * A function that returns the whole map as a jstring.
 * Class:     GameLogic
 * Method:    printMap
 * Signature: ()V
 */
JNIEXPORT jstring JNICALL Java_GameLogic_printMap
  (JNIEnv *env, jobject thisObject){

 return (*env)->NewStringUTF(env, printMap());
}

/* A function that checks if a position on the map i a wall.
 * Class:     GameLogic
 * Method:    isWall
 * Signature: (C)Z
 */
JNIEXPORT jboolean JNICALL Java_GameLogic_isWall
  (JNIEnv *env, jobject thisObject, jchar direction){
    //initialising java fields uusing jni functions.
	jclass logic = (*env) -> GetObjectClass(env,thisObject);
	jfieldID pposition = (*env) -> GetFieldID(env,logic,"playerPosition","[I");
	jintArray player = (*env) -> GetObjectField(env, thisObject, pposition);
	jint *playerPosition = (*env) -> GetIntArrayElements(env, player, 0);
	switch (direction) {
	            case 'N':
	                return lookAtTile(playerPosition[0] - 1, playerPosition[1]) == '#';
	            case 'S':
	                return lookAtTile(playerPosition[0] + 1, playerPosition[1]) == '#';
	            case 'E':
	                return lookAtTile(playerPosition[0], playerPosition[1] + 1) == '#';
	            case 'W':
	                return lookAtTile(playerPosition[0], playerPosition[1] - 1) == '#';
	            default:
	                return false;
	        }
}

/* An accessor for the width of the map.
 * Class:     GameLogic
 * Method:    getMapHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_GameLogic_getMapWidth
 (JNIEnv *env, jobject thisObject){

	return cols;
}

/* An accessor for the height of the map.
 * Class:     GameLogic
 * Method:    getMapWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_GameLogic_getMapHeight
  (JNIEnv *env, jobject thisObject){

	return rows;
}
/**
 * This function returns if the game has been won, and if so deletes the save files.
 */
bool checkWin(int collectedGold,char lastPosition){
    if ((collectedGold >= totalGoldOnMap) &&
        (lastPosition == 'E')) {
        remove("Temp");
        remove("tempMap");
        return true;
    }
    return false;
}


  /**
     * Finds a random position for the player in the map.
     * @return Return null; if no position is found or a position vector [y,x]
     */
int *initiatePlayer(){
        int *pos,r,r1;
        pos = calloc(2, sizeof(int));
        time_t t;
        srand((unsigned)time(&t));
        while((r = rand()%rows)>rows || (r1=rand()%cols)>cols){continue; }
        pos[0] = r;
        pos[1] = r1;

        int counter = 1;
        //Prevents players to spawn on walls,bots or other players.
        while ((lookAtTile(pos[0], pos[1]) == '#' || lookAtTile(pos[0], pos[1]) == 'P' || lookAtTile(pos[0], pos[1]) == 'B')
                && (counter < rows * cols)) {
            pos[1]=  abs((int)(counter * cos(counter)));
            pos[0]= abs((int)(counter * sin(counter)));
            counter++;
        }
        return (lookAtTile(pos[0], pos[1]) == '#') ? NULL : pos;
    }
/**
 * Saves the attributes of a player to its according structure
 * in a struct array if such exist.
 */
void saveState(const char *pName, int cGold, int p0, int p1,
               char currentTile) {
    player *ptr;
    int i = 0;
    for (ptr = allPlayers; i < playerCounter; ptr++) {
        if(strcmp(pName, ptr -> name) == 0){
            ptr->gold = cGold;
            ptr->positionOnMap[0] = p0;
            ptr->positionOnMap[1] = p1;
            ptr->lastposition = currentTile;
        }
        i++;
    }
    savePlayer();
}
/**
 * Saves the attributes of a new player to a structure
 * in a the struct array.
 */
void saveNewPlayer(const char *pName, int cGold, int p0, int p1,
                   char currentTile) {
    player *ptr;
    ptr = allPlayers;
    int i = 0;
    while (i < playerCounter) {
        ptr++;
        i++;
    }
    playerCounter ++;
    strcpy(ptr->name, pName);
    ptr->gold = cGold;
    ptr->positionOnMap[0] = p0;
    ptr->positionOnMap[1] = p1;
    ptr->lastposition = currentTile;
    savePlayer();
}

/**
  * A function that saves the current game players array to a temp file.
  */
void savePlayer() {
    FILE *file;
    file = fopen("Temp", "w+");
    fwrite(allPlayers, sizeof allPlayers[0], playerCounter, file);
    fclose(file);
}

/**
  * A function that saves the current changes to the map to a temp file.
  */
void saveMapState() {
    FILE *file; 
    file = fopen("tempMap", "w+");
    fwrite(mapState, sizeof mapState[0], collectedGoldCounter, file);
    fclose(file);
}
/**
  * A function that loads the players of the last game from a temp file.
  */
void loadPlayer() {
    FILE *file;
    file = fopen("Temp", "rb");
    playerCounter = (int) fread(allPlayers, sizeof allPlayers[0], rows*cols, file);
    fclose(file);
}
/**
  * A function that loads the changes to the map made in the last game from a temp file.
  */
void loadMapState() {
    FILE *file;
    file = fopen("tempMap", "rb");
    collectedGoldCounter = (int) fread(mapState, sizeof mapState[0], rows*cols, file);
    fclose(file);
}