#ifndef map_h
#define map_h
char *getLine(FILE *f);
void guard(char *str);
char lookAtTile(int x , int y);
void replaceTile(int x, int y,char newTile);
void readMap(const char* mapName);
bool setName(char *str);
bool setWin(char *str);
void getUserInput();
void loadMap(FILE *f);
int addRow(char ***array, char *element, size_t *count, size_t *buf);
int allocateArray(char ***array, int n);
char *printMap();
bool startsWith(char *str1, char *str2);
char *lookWindow(int y, int x, int radius);
#endif