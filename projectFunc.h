//Henry Jochaniewicz
//Fund Comp Final Project (lab11)
//projectFunc.h: file header for tetris game

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "gfx2.h"

//these either define array sizes OR how big a square in pixels. please do not take off
#define WIDTH 10
#define HEIGHT 20
#define SIDE 30

//enum for the different tetrimino shapes...makes the code way easier to read
typedef enum{I=0, O=1, T=2, J=3, L=4, S=5, Z=6} shape;

//number of milliseconds to wait before each drop; useful for not seeing a bunch of random-looking ints
typedef enum{levelOne=1000,levelTwo=793,levelThree=617,levelFour=473,levelFive=355,
			 levelSix=262,levelSeven=190,levelEight=135,levelNine=94,levelTen=64} level;

//struct for color so it can stop annoying me
typedef struct {
	int r;
	int g;
	int b;
} Color;

//x, y here are col, row from the BOTTOM of the board to the TOP
typedef struct {
	short int x;
	short int y;
	Color color;
	bool isFilled;
} Square;

//tetrimino struct, where points are the x,y of each of the four squares
typedef struct {
	int x, y;
	XPoint points[4];
	Color color;
	shape shape;
} Tetrimino;

//function definitions
//there looks like a lot but many of these are rather short and sweet
double clockToMilliseconds(clock_t);
bool controls(int, int, bool*);
void clearRightSide();
void dispBoard(Square [][HEIGHT]);
void dispBorder(int);
void dispScore(int);
int calcPoints(int);
void dispLevel(int*);
level updateLevel(int, int *);
level updateScoreAndClearLines(Square [WIDTH][HEIGHT], int *lines, int *score, int *index, bool showAnim);
void dispRightSide(Tetrimino next, Tetrimino held, int score, int *levelIndex);
void dispGameplayElements(Square [][HEIGHT], Tetrimino main);
void drawSquare(Square);
void drawShadowSquare(Square);
void initSquare(Square *, int, int);
void setShape(Tetrimino *);
Tetrimino spawnTetrimino(shape, int, int);
shape randomShape();
void rotateTetrimino(Tetrimino *, bool);
void implementRotateTetrimino(Tetrimino *, Square [][HEIGHT]);
bool updateTetriminoPos(Tetrimino *, Square [][HEIGHT]);
void drawTetrimino(Tetrimino);
void coverTetrimino(Tetrimino);
void castShadowTetrimino(Tetrimino, Square [][HEIGHT]);
void coverShadowTetrimino(Tetrimino, Square [][HEIGHT]);
void shiftTetrimino(Tetrimino *, int,int);
void moveTetriminoLeftRight(Tetrimino *, int, Square [][HEIGHT]);
bool isValidPlacement(Tetrimino, Square [][HEIGHT]);
int clearLines(Square [][HEIGHT], bool showAnim);
void clearLinesAnim(Square arr[WIDTH][HEIGHT], bool linesCleared[], int numLines);
void swapHeldTetrimino(Tetrimino *main, Tetrimino *hold, Tetrimino *next);
