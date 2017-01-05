#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Map.h"


// global variables
char *mapName, **map;//the map as a '2D pointer array'.
int totalGoldOnMap = 0;//the total gold on map.
int rows = 0, cols = 0;//the number of rows and collums of the map. 

// headers
char *getLine(FILE *f);
void guard(char *str);
char lookAtTile(int x , int y);
void replaceTile(int x, int y,char newTile);
void readMap(const char *mapName);
bool setName(char *str);
bool setWin(char *str);
void getUserInput();
void loadMap(FILE *f);
int addRow(char ***array, char *element, size_t *count, size_t *buf);
int allocateArray(char ***array, int n);
char *printMap();
bool startsWith(char *str1, char *str2);
char *lookWindow(int y, int x, int radius);

/**
 * A function that takes a file and loads the DoD map 
 * with its attributes from it.
 */
void loadMap(FILE *f) {
    char *line;
    bool error = false;
    int lineCounter = 0;
    size_t i = 0, n = 2;
	
    //Allocating some memory for the array.
    allocateArray(&map,(int) n);
    while (1) {
        line = getLine(f);
        if (!line) {
            break;
        }
        lineCounter++;
        if (lineCounter == 1) {
            error = setName(line);
            if (error)
                break;
        } else if (lineCounter == 2) {
            error = setWin(line);
            if (error)
                break;
        } else {
            cols = (int) strlen(line);
            if (addRow(&map, line, &i, &n))
                rows++;
        }
    }
	//In case some info from the file is incorrect an error message is printed.
    if (error) {
        setName("");
        setWin("");
        perror("map file invalid or wrongly formatted\n");
        exit(EXIT_FAILURE);
    }
    fclose(f);
}

/**
 * Function that copies the content of the map
 * onto a char array(string) and returns it.
 */
char *printMap() {
    char *output = malloc(rows*cols);
    int i;
    for (i = 0; i< rows; i++){
        strcpy(output + i*cols,map[i]);
    }
    return output;
}

/**
 * Function that creates DoD look window
 * given by any position on the map and it's radius.
 * @return char* - returns it as a 'string'.
 */
char *lookWindow(int y, int x, int radius) {
    char *look = calloc((radius * radius) + radius, sizeof(char));
    int i = 0, j = 0;
    for (i = 0; i < radius; i++) {
        for (j = 0; j < radius; j++) {
            int posX = x + j - radius/2;
            int posY = y + i - radius/2;
            if (posX >= 0 && posX < cols &&
                posY >= 0 && posY < rows)
                look[j + i*(radius + 1)] = map[posY][posX];
            else
                look[j + i*(radius + 1)] = '#';
        }
        if (i != 4)
            look[radius + i * (radius + 1)] = '\n';
    }
    look[0] = 'X';
    look[radius-1] = 'X';
    look[(radius * radius) - 1] = 'X';
    look[radius*radius + (radius - 2)] = 'X';
    look[radius*radius + (radius - 1)] = '\0';
    return look;
}

/**
 * Function that allocates enough memory for 
 * every line of the map array. A reference to 
 * the map array and the desired size of it are passed as arguments.
 * @return if the array has been correctly allocated.
 */
int allocateArray(char ***array, int n){
    *array = calloc(n - 1, sizeof(char*));
    if (*array) {
        int i=0;
        for (i=0;i<n-1;i++)
            (*array)[i] = calloc(n, sizeof(char));
    } else {
        return 0;
    }
    return 1;
}
/**
 * Function that reallocates enough memory for a line to store a line from the file
 * of the map while looping through the file lines.
 * @return if the line has been corectly allocated.
 */
int addRow(char ***array, char *element, size_t *count, size_t *buf)
{
    if (*count == *buf)
    {
        char **tmp = realloc(*array, *buf * 2 * sizeof(**array));
        if (tmp)
        {
            *buf *= 2;
            *array = tmp;
        }
        else
        {
            return 0;
        }
    }
    (*array)[(*count)++] = element;
    return 1;
}
/**
 * A function that assigns and checks if the win condition on the map is correct.
 * Message is printed and the program is terminated if not so.
 * @return  - if it is correct or no.
 */
bool setWin(char *str) {
    if (!startsWith("win ", str)) {
        return true;
    }
    int win = 0;
    win = atoi(str + 4);
    if (win <= 0) {
        perror("the map does not contain a valid win criteria!\n");
        exit(EXIT_FAILURE);
    }
    totalGoldOnMap = win;
    return false;
}
/**
 * A function that assigns and checks if map name is correct.
 * Message is printed and the program is terminated if not so.
 * @return  - if it is correct or no.
 */
bool setName(char *str) {
    size_t size;
    size = strlen(str);
    if (!startsWith("name ", str)) {
        return true;
    }
    char *name = malloc(size);
    guard(name);
    strncpy(name, str + 5, strlen(str));
    if (strlen(name) < 1)
        return true;
    
    mapName = name;
    return false;
}

/**
 * Function that checks if a string starts with a set of chars.
 * @return if a string starts with a set of chars .
 */
bool startsWith(char *str1, char *str2) {
    size_t str1len = strlen(str1);
    size_t str2len = strlen(str2);
    
    return str2len < str1len ? false : strncmp(str1, str2, str1len) == 0;
}
/**
 * Function that attemts to load from a file
 * with path 'mapName' and if suck doesnt exist loads the game from the 'example_map.txt'.
 */ 
void readMap(const char *mapName) {
    FILE *f;
	char *nameExtention = malloc(sizeof(mapName) + 8);
	strcpy(nameExtention,mapName); 
	strcat(nameExtention,".txt");
    f = fopen(nameExtention,"r");
    if(f == NULL )
    {
        f = fopen("maps/example_map.txt", "r");
        if (f == NULL) {
            perror("no valid map name given and default file example_map.txt not found.\n");
            exit(EXIT_FAILURE);
        }
    }
    loadMap(f);
}

/**
 * Functions that checks weather there is enough memory.
 */
void guard(char *str) {
    if (str == NULL) {
        perror("Out of memory.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 *
 * @return - the char at certain position on the map.
 */
char lookAtTile(int y , int x){
    return *(*(map+y)+x);
}
/**
 * @param - newTile.
 * Function that replaces the char at certain position on the map with newTile.
 */
void replaceTile(int y, int x,char newTile){
    *(*(map+y)+x) = newTile;
}

/**
 * Function that reads line by line from a file and 
 * @return the line as a char*.
 */
char *getLine(FILE *f) {
    int i = 0, ch, length = 0;
    size_t size = 50;
    
    if(EOF==(ch=fgetc(f))) {
        return NULL;
    }
    ungetc(ch, f);
    
    char *line = malloc(size);
    guard(line);
    //Until it gets to the end of file/line the info from the file line is stored char by char.  
    while((ch != EOF) && (ch != '\n'))
    {
        if(i == size-1)
        {
            // double the size if memory is not enough
            size *= 2;
            line = realloc(line, size);
            guard(line);
        }
        if ((ch = fgetc(f)) == EOF)
            break;
        line[length++] = ch;
        i++;
    }
	//Terminating char is put at the end of each line .
    line[i] = '\0';
    return line;
}