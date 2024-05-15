//Henry Jochaniewicz
//Fund Comp Final Project (lab11)
//projectFunc.h: function definitions

#include "projectFunc.h"

//converts the clock to ms for measuring time differences
double clockToMilliseconds(clock_t ticks){
    // units/(units/time) => time (seconds) * 1000 = milliseconds
    return 1000.0 * (ticks/(double)CLOCKS_PER_SEC);
}

//whenever the UI must be updated, just pastes a black rectangle over the right
void clearRightSide() {
	gfx_color(0,0,0);
	gfx_fill_rectangle(SIDE*(WIDTH+2),0,SIDE*8,SIDE*(HEIGHT+2));
}

//returns a random number 0,1,2,4,5,6
shape randomShape() {
	return arc4random_uniform(7); //delete this line if it cannot be compiled
	return rand() % 7;
}

//controls screen
bool controls(int width, int height, bool *proModeOn) {
	char proModeString[30] = "PRO MODE: OFF";

	while(1) {
		gfx_changefont("12x24");
		gfx_text(width/2 - 3*SIDE, SIDE, "TETRIS (clone)");
		gfx_changefont("9x15");
		gfx_text(width/2 - 3*SIDE, SIDE * 2, "by Henry Jochaniewicz");
		gfx_text(width/2 - 2*SIDE, SIDE*5, "CONTROLS");
		gfx_text(width/2 - 2*SIDE, SIDE*6, "d: move block right");
		gfx_text(width/2 - 2*SIDE, SIDE*7, "w: hard drop block");
		gfx_text(width/2 - 2*SIDE, SIDE*8, "s: soft drop block");
		gfx_text(width/2 - 2*SIDE, SIDE*9, "spacebar: hold block");
		gfx_text(width/2 - 2*SIDE, SIDE*10, "esc: quit");
		gfx_text(SIDE*2, SIDE*13, "press 'p' for pro mode (removes line clear animation):");
		gfx_text(width/2-2*SIDE , SIDE*14, proModeString);
		gfx_text(width/5 , SIDE*15, "press spacebar to continue (or esc to quit)");
		
		char input;
		input = gfx_wait();
		if(input == 32) return true;
		if(input == 27) return false;
		if(input == 'p') *proModeOn = !(*proModeOn);
		if(*proModeOn) strcpy(proModeString, "PRO MODE: ON");
		else strcpy(proModeString, "PRO MODE: OFF");
		gfx_clear();
	}
}

//sets a tetrimino's points based on its shape
void setShape(Tetrimino *t) {
	Color col;
	switch(t->shape) {
		case I:
			col = (Color) {0,255, 255};
			t->points[1] = (XPoint) {t->x - 1, t->y};
			t->points[2] = (XPoint) {t->x + 1, t->y};
			t->points[3] = (XPoint) {t->x + 2, t->y};
			break;
		case O:
			col = (Color) {255,255,0};
			t->points[1] = (XPoint) {t->x + 1, t->y};
			t->points[2] = (XPoint) {t->x, t->y - 1};
			t->points[3] = (XPoint) {t->x + 1, t->y - 1};
			break;
		case T:
			col = (Color) {255,0,255};
			t->points[1] = (XPoint) {t->x - 1, t->y};
			t->points[2] = (XPoint) {t->x + 1, t->y};
			t->points[3] = (XPoint) {t->x, t->y + 1};
			break;
		case J:
			col = (Color) {0,0,255};
			t->points[1] = (XPoint) {t->x + 1, t->y};
			t->points[2] = (XPoint) {t->x - 1, t->y};
			t->points[3] = (XPoint) {t->x - 1, t->y + 1};
			break;
		case L:
			col = (Color) {255, 125, 0};
			t->points[1] = (XPoint) {t->x + 1, t->y};
			t->points[2] = (XPoint) {t->x - 1, t->y};
			t->points[3] = (XPoint) {t->x + 1, t->y + 1};
			break;
		case S:
			col = (Color) {0, 255, 0};
			t->points[1] = (XPoint) {t->x, t->y + 1};
			t->points[2] = (XPoint) {t->x + 1, t->y + 1};
			t->points[3] = (XPoint) {t->x - 1, t->y};
			break;
		case Z:
			col = (Color) {255, 0, 0};
			t->points[1] = (XPoint) {t->x - 1, t->y + 1};
			t->points[2] = (XPoint) {t->x, t->y + 1};
			t->points[3] = (XPoint) {t->x + 1, t->y};
			break;
	}
	t->color = col;
}

//create a new tetrimino of a shape and location
//will be more useful if I decide to implement 1-bags for randomness
Tetrimino spawnTetrimino(shape input, int x, int y) {
	Tetrimino t;
	t.shape = input;
	t.x = x;
	t.y = y;
	t.points[0] = (XPoint) {t.x,t.y};
	setShape(&t);
	return t;
}

//draws the tetrimino on the screen
void drawTetrimino(Tetrimino t) {
	for(int i = 0; i < 4; i++) {
		Square temp = {t.points[i].x, t.points[i].y,t.color,true};
		drawSquare(temp);
	}
}

//covers the tetrimino at the current spot on the screen
//in order to draw the next one
void coverTetrimino(Tetrimino t) {
	for(int i = 0; i < 4; i++) {
		Square temp = {t.points[i].x, t.points[i].y,(Color) {0,0,0},true};
		drawSquare(temp);
	}
}

//moving the tetrimino up or down or left or right some amount
void shiftTetrimino(Tetrimino *t, int xShift, int yShift) {
	t->x += xShift;
	t->y += yShift;
	for(int i = 0; i < 4; i++) {
		t->points[i].x += xShift;
		t->points[i].y += yShift;
	}
}

//returns whether the passed tetrimino's position will work with the current board
bool isValidPlacement(Tetrimino temp, Square arr[WIDTH][HEIGHT]) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < WIDTH; j++) {
			for(int k = 0; k < HEIGHT; k++) {
				if((temp.points[i].x >= WIDTH || temp.points[i].x < 0) || 
							(temp.points[i].y < 0 || temp.points[i].y >= HEIGHT) ||
							(arr[j][k].isFilled && k == temp.points[i].y && j == temp.points[i].x)) {
					return false;
				} } } }
	return true;
}

//moving the tetrimino left and right in response to user input
void moveTetriminoLeftRight(Tetrimino *t, int xShift, Square arr[WIDTH][HEIGHT]) {
	Tetrimino temp = *t;
	shiftTetrimino(&temp, xShift, 0);
	if(isValidPlacement(temp,arr))
		shiftTetrimino(t, xShift, 0);
}

//updates the tetriminos position by dropping it by one 
bool updateTetriminoPos(Tetrimino *t, Square arr[WIDTH][HEIGHT]) {
	Tetrimino temp = *t;
	shiftTetrimino(&temp, 0, -1); //shift down by 1
	if(!isValidPlacement(temp, arr)) {
		for(int i = 0; i < 4; i++) {
			arr[t->points[i].x][t->points[i].y].isFilled = true;
			arr[t->points[i].x][t->points[i].y].color = t->color;
		}
	}
	else
		shiftTetrimino(t, 0, -1);
	return !isValidPlacement(temp, arr);
}

//rotates the tetrimino
void rotateTetrimino(Tetrimino *t, bool rotateCC) {
	short int xVal;
	short int yVal;
	short int tempVal;
	int rotateDir = (rotateCC) ? -1 : 1;
	//rotation algorithm
	for(int i = 0; i < 4; i++) {
		xVal = t->points[i].x - t->x;
		yVal = t->points[i].y - t->y;
		tempVal = xVal;
		xVal = -rotateDir * yVal;
		yVal = rotateDir * tempVal;
		t->points[i].x = xVal + t->x;
		t->points[i].y = yVal + t->y;
	}
}

//tests whether the tetrimino can be rotated, and does so if it can
//also attempts a wall-kick
void implementRotateTetrimino(Tetrimino *t, Square arr[WIDTH][HEIGHT]) {
	if(t->shape == O) return; //do not spin the O block
	bool rotate = false;
	int shift = 0;
	Tetrimino temp = *t;
	rotateTetrimino(&temp, true);
	if(isValidPlacement(temp,arr))
		rotate = true;
	else { //attempt a wall-kick
		shiftTetrimino(&temp, -1, 0);
		if(isValidPlacement(temp, arr)) {
			rotate = true;
			shift = -1;
		}
		else {
			shiftTetrimino(&temp, 2, 0); //counteract the shift from before
			if(isValidPlacement(temp, arr)) {
				rotate = true;
				shift = 1;
			}
		}
	}
	if(rotate) {
		rotateTetrimino(t, true);
		shiftTetrimino(t, shift, 0);
	}
}

//swap the current tetrimino with the held tetrimino
void swapHeldTetrimino(Tetrimino *main, Tetrimino *hold, Tetrimino *next) {
	if(hold->shape == -1) {//first time?
		*hold = spawnTetrimino(main->shape, WIDTH+4, HEIGHT-10);
		*main = spawnTetrimino(next->shape, WIDTH/2 - 1, HEIGHT-2);
		*next = spawnTetrimino(randomShape(), WIDTH+4, HEIGHT-3);
	}
	else { //if not first time
		shape currShape = main->shape;
		*main = spawnTetrimino(hold->shape, WIDTH/2 - 1, HEIGHT-2);
		*hold = spawnTetrimino(currShape, WIDTH+4, HEIGHT-10);
	}
}

//casts a shadow tetrimino for placement purposes
void castShadowTetrimino(Tetrimino shadow, Square arr[WIDTH][HEIGHT]) {
	while(isValidPlacement(shadow, arr)) {
		shiftTetrimino(&shadow, 0, -1);
	}
	shiftTetrimino(&shadow, 0, 1); //means I shot it down one too low
	for(int i = 0; i < 4; i++) {
		Square temp = {shadow.points[i].x, shadow.points[i].y,shadow.color,true};
		drawShadowSquare(temp);
	}
}

//covers the tetrimino's shadow in order to draw the next shadow right
void coverShadowTetrimino(Tetrimino shadow, Square arr[WIDTH][HEIGHT]) {
	while(isValidPlacement(shadow, arr)) {
		shiftTetrimino(&shadow, 0, -1);
	}
	shiftTetrimino(&shadow, 0, 1); //means I shot it down one too low
	for(int i = 0; i < 4; i++) {
		Square temp = {shadow.points[i].x, shadow.points[i].y,(Color) {0,0,0},true};
		drawShadowSquare(temp);
	}
}


//initialize each square to be blank
void initSquare(Square *s, int xCo, int yCo) {
	Color black = {0,0,0};
	s->x = xCo;
	s->y = yCo;
	s->color = black;
	s->isFilled = false;
}

//draw a square to the screen (for tetrimino, board, etc.)
void drawSquare(Square s) {
	int height = (HEIGHT+2)*SIDE;
	gfx_color(s.color.r, s.color.g, s.color.b);
	gfx_fill_rectangle(SIDE + SIDE * s.x, height - 2*SIDE - SIDE * s.y, SIDE, SIDE);
	gfx_color(0,0,0);
	gfx_rectangle(SIDE + SIDE * s.x, height - 2*SIDE - SIDE * s.y, SIDE, SIDE);
}

//shadow rectangle (for the tetrimino shadow)
void drawShadowSquare(Square s) {
	int height = (HEIGHT+2)*SIDE;
	gfx_color(s.color.r, s.color.g, s.color.b);
	gfx_rectangle(SIDE + SIDE * s.x, height - 2*SIDE - SIDE * s.y, SIDE, SIDE);
}


//display the grey-box border
void dispBorder(int score) {
	int width = (WIDTH+2)*SIDE;
	int height = (HEIGHT+2)*SIDE;
	Color grey = {150,150,150};
	for(int i = 0; i < WIDTH + 2; i++) {
		gfx_color(grey.r,grey.g,grey.b);
		gfx_fill_rectangle(i*SIDE, height - SIDE, SIDE, SIDE);
		gfx_fill_rectangle(i*SIDE, 0, SIDE, SIDE);
		gfx_color(0,0,0);
		gfx_rectangle(i*SIDE, height - SIDE, SIDE, SIDE);
		gfx_rectangle(i*SIDE, 0, SIDE, SIDE);
	}
	for(int i = 0; i < HEIGHT + 2; i++) {
		gfx_color(grey.r,grey.g,grey.b);
		gfx_fill_rectangle(0, height - SIDE * i, SIDE, SIDE);
		gfx_fill_rectangle(width - SIDE, height - SIDE * i, SIDE, SIDE);
		gfx_color(0,0,0);
		gfx_rectangle(0, height - SIDE * i, SIDE, SIDE);
		gfx_rectangle(width - SIDE, height - SIDE * i, SIDE, SIDE);
	}
}

//function to display the score
void dispScore(int score) {
	char scoreString[30];
	sprintf(scoreString, "SCORE: %d", score);
	gfx_color(255,255,255);
	gfx_text(SIDE*(WIDTH+3), (HEIGHT+1)*SIDE, scoreString);
}

//number of points to return
int calcPoints(int lines) {
	switch(lines) {
		case 0: return 0;
		case 1: return 1000;
		case 2: return 3000;
		case 3: return 5000;
		case 4: return 8000;
	}
	return 0;
}

//updates score, clears lines, and returns the new speed to drop the blocks
level updateScoreAndClearLines(Square arr[WIDTH][HEIGHT], int *numLines, int *score, int *levelIndex, bool showAnim) {
	int linesCleared = clearLines(arr, showAnim);
	*numLines += linesCleared;
	*score += calcPoints(linesCleared);
	if(linesCleared > 0)
		dispBoard(arr);
	return updateLevel(*numLines, levelIndex);
}

//function to display the current level
void dispLevel(int *index) {
	char levelString[30];
	sprintf(levelString, "LEVEL: %d", *index);
	gfx_color(255,255,255);
	gfx_text(SIDE*(WIDTH+3), (HEIGHT)*SIDE, levelString);
}

//updates the level based on number of lines cleared
//these get really hard really fast...
level updateLevel(int lines, int* index) {
	level levels[10] = {levelOne, levelTwo, levelThree, levelFour, levelFive,
						levelSix, levelSeven, levelEight, levelNine, levelTen};
	*index = lines / 10;
	if(*index > 9) return levels[9];
	return levels[*index];
}

//function to display everything on the right hand of the screen
void dispRightSide(Tetrimino next, Tetrimino held, int score, int *index) {
	gfx_changefont("9x15");
	dispScore(score);
	dispLevel(index);
	int width = (WIDTH+2)*SIDE;
	gfx_text(width + 3*SIDE, 8*SIDE, "HOLD");
	gfx_text(width+3*SIDE, SIDE, "NEXT");
	drawTetrimino(next);
	if(held.shape != -1)
		drawTetrimino(held);
}

//display the current squares
void dispBoard(Square arr[WIDTH][HEIGHT]) {
	gfx_color(0,0,0);
	gfx_rectangle(SIDE, SIDE, SIDE * WIDTH, SIDE * HEIGHT);
	gfx_fill_rectangle(SIDE, SIDE, SIDE * WIDTH, SIDE * HEIGHT);
	for(int i = 0; i < WIDTH; i++) {
		for(int j = 0; j < HEIGHT; j++) {
			if(arr[i][j].isFilled)
				drawSquare(arr[i][j]);
		}
	}
}

//displays the board, block, and shadowed tetrimino
void dispGameplayElements(Square arr[WIDTH][HEIGHT], Tetrimino t) {
	dispBoard(arr);
	castShadowTetrimino(t, arr);
	drawTetrimino(t);
}

//shift elements down from a line clear
void shiftElementsDown(Square arr[WIDTH][HEIGHT], int index) {
	for(int i = 0; i < WIDTH; i++) {
		for(int j = index; j < HEIGHT - 1; j++) {
			arr[i][j].color = arr[i][j+1].color;
			arr[i][j].isFilled = arr[i][j+1].isFilled;
		}
		arr[i][HEIGHT-1] = (Square) {i,HEIGHT-1, (Color) {0,0,0}, false};
	}
}

//clear lines function
int clearLines(Square arr[WIDTH][HEIGHT], bool showAnim) {
	int numLinesCleared = 0;
	bool lines[HEIGHT];
	for(int i = 0; i < HEIGHT; i++) lines[i] = true;

	for(int j = 0; j < HEIGHT; j++) {
		for(int i = 0; i < WIDTH; i++) {
			if(!arr[i][j].isFilled) {
				lines[j] = false;
				break;
			}
		}
		if(lines[j]) numLinesCleared++;
	}
	if(numLinesCleared > 0 && showAnim) clearLinesAnim(arr, lines, numLinesCleared);
	for(int j = HEIGHT; j >= 0; j--) {
		if(lines[j]) shiftElementsDown(arr, j);
	}
	return numLinesCleared;
}

void clearLinesAnim(Square arr[WIDTH][HEIGHT], bool linesCleared[], int numLines) {
	/*
	char stringOfLines[10];
	switch(numLines) {
		case 1: strcpy(stringOfLines, "single"); break;
		case 2: strcpy(stringOfLines, "double"); break;
		case 3: strcpy(stringOfLines, "triple"); break;
		case 4: strcpy(stringOfLines, "TETRIS"); break;
	} */
	if(numLines == 4) {
		gfx_color(255,255,255);
		gfx_changefont("10x20");
		gfx_text(SIDE*5, SIDE*4, "TETRIS");
	}
	gfx_flush();
	int height = (HEIGHT+2)*SIDE;
	for(int n = 0; n < 2; n++) {
		gfx_flush();
		if(n == 1) usleep(250000);
		for(int i = 0; i < HEIGHT; i++) {
			if(linesCleared[i]) {
				gfx_color(255,255,255);
				gfx_fill_rectangle(SIDE, height - 2*SIDE - SIDE * i, WIDTH * SIDE, SIDE);
			}
		}
		gfx_flush();
		usleep(250000);
		for(int i = 0; i < HEIGHT; i++) {
			if(linesCleared[i]) {
				for(int j = 0; j < WIDTH; j++) {
					drawSquare(arr[j][i]);
				}
			}
		}
	}
}