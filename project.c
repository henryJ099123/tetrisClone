//Henry Jochaniewicz
//Fund Comp Final Project (lab11)
//project.c: main driver for tetris

#include "projectFunc.h"

//main function
int main() {
	srand(time(NULL));
	int width = (WIDTH+10)*SIDE, height = (HEIGHT+2)*SIDE;
	gfx_open(width, height, "final project");

	bool proMode = false;
	if(!controls(width, height, &proMode)) return 1;
	gfx_clear();
	gfx_changefont("9x15");	
	//initialize board
	Square board[WIDTH][HEIGHT];
	for(int i = 0; i < WIDTH; i++) {
		for(int j = 0; j < HEIGHT; j++) {
			initSquare(&board[i][j],i,j);
		}
	}

	//initialize gameplay elements: tetrimino (the tetris blocks)
	//game tetrimino location: WIDTH/2 - 1, HEIGHT - 2
	//held tetrimino location: WIDTH+4, HEIGHT-10
	//next tetrimino location, WIDTH+4, HEIGHT-3
	Tetrimino block = spawnTetrimino(randomShape(), WIDTH/2 - 1, HEIGHT-2);
	Tetrimino holdBlock;
	holdBlock.x = WIDTH + 4;
	holdBlock.y = HEIGHT - 10;
	holdBlock.shape = -1; //invalid shape for the first one
	Tetrimino nextBlock = spawnTetrimino(randomShape(), WIDTH+4, HEIGHT-3);

	char input; //user input
	int xShift = 0; //when user wants to move the tetris block
	bool lowerByOne = false; //make the block drop when the user wants
	bool swappedHold = false; //if the user wants to hold a block
	int score = 0; //the score :D
	int numLinesCleared = 0;
	int levelIndex = 0;
	level numMSToWait = updateLevel(numLinesCleared, &levelIndex);

	//hold timing information for the block drop
	clock_t timeStart = clock();
	clock_t currTime = 0;
	clock_t deltaTime = 0;

	//info for frame update? may delete if i feel like it
	clock_t frameStart = clock();
	clock_t deltaFrame = 0;

	//draw the initial stuffs
	dispBorder(score);
	dispRightSide(nextBlock, holdBlock, score, &levelIndex);
	dispGameplayElements(board, block);

	while(1) {
		gfx_flush();
		currTime = clock();
		deltaFrame = currTime - frameStart;
		deltaTime = currTime - timeStart;
		if(clockToMilliseconds(deltaTime) < 16.7) continue;
		frameStart = clock();
		
		//user input
		if(gfx_event_waiting()) {
			input = gfx_wait();
			switch(input) {
				case 'a': //shift the tetrimino one to the left
					xShift = -1;
					break;
				case 'd': //shift tetrimino one to the right
					xShift = 1;
					break;
				case 's': //lower the tetrimino by one
					lowerByOne = true;
					break;
				case 'w': //drop that tetrimino where it stands
					coverTetrimino(block);
					coverShadowTetrimino(block,board);
					while(!updateTetriminoPos(&block, board)); //continue to drop the tetrimino
					drawTetrimino(block);
					numMSToWait = updateScoreAndClearLines(board, &numLinesCleared, &score, &levelIndex, !proMode);
					block = spawnTetrimino(nextBlock.shape, WIDTH/2 - 1, HEIGHT-2);
					nextBlock = spawnTetrimino(randomShape(), WIDTH+4, HEIGHT-3);

					//draw the correct stuff (the board will be redrawn in function above)
					clearRightSide();
					dispRightSide(nextBlock, holdBlock, score, &levelIndex);
					castShadowTetrimino(block, board);
					drawTetrimino(block);

					//restart the timers/flags for the dropped block
					swappedHold = false;
					timeStart = clock();
					deltaTime = 0;
					break;
				case 'e': //swap held tetrimino
					if(!swappedHold) { //if the player has not already swapped their tetrimino
						swappedHold = true;
						coverTetrimino(block);
						coverShadowTetrimino(block, board);
						swapHeldTetrimino(&block, &holdBlock, &nextBlock);

						//redraws the whole board bc the held pieces kept pasting over each other
						clearRightSide();
						dispRightSide(nextBlock, holdBlock, score, &levelIndex);
						castShadowTetrimino(block, board);
						drawTetrimino(block);
					}
					break;
				case 32: //rotate that thang (spacebar)
					coverTetrimino(block);
					coverShadowTetrimino(block, board);
					implementRotateTetrimino(&block, board);
					castShadowTetrimino(block, board);
					drawTetrimino(block);
					break;
				case 27: //escape (quit)
					return 0;
			}
		}
		//move Tetrimino if the user wanted it moved
		if(xShift != 0) {
			coverTetrimino(block);
			coverShadowTetrimino(block,board);
			moveTetriminoLeftRight(&block, xShift, board);
			castShadowTetrimino(block,board);
			drawTetrimino(block);
			xShift = 0;
		}

		//if the time has gone past 1 second, or the user hit s, drop the block
		//(or place it if cannot drop)
		if(lowerByOne || clockToMilliseconds(deltaTime) > (double) numMSToWait) {
			timeStart = clock();
			deltaTime = 0;
			coverTetrimino(block);
			coverShadowTetrimino(block, board);
			if(updateTetriminoPos(&block, board)) {
				drawTetrimino(block);
				numMSToWait = updateScoreAndClearLines(board, &numLinesCleared, &score, &levelIndex, !proMode);
				block = spawnTetrimino(nextBlock.shape, WIDTH/2 - 1, HEIGHT-2);
				nextBlock = spawnTetrimino(randomShape(), WIDTH+4, HEIGHT-3);
				clearRightSide();
				dispRightSide(nextBlock, holdBlock, score, &levelIndex);
				swappedHold = false;
			}
			castShadowTetrimino(block, board);
			drawTetrimino(block);
			lowerByOne = false;
		}
		//GAME OVER
		//if lose condition: the current block's position is no good (the blocks got too high)
		if(!isValidPlacement(block, board)) {
			gfx_color(255,255,255);
			gfx_text(SIDE*2,height/2,"GAME OVER. Press ESC to end.");
			while(input != 27)
				input = gfx_wait();
			return 0;
		}
	}
	return 0;
}