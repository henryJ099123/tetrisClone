# tetrisClone
Tetris project I made for school

Tetris Game

If you've never played tetris:
- blocks will continue to drop from the top
- fill all the squares in a horizontal line to clear it and get points
- clear more lines at a time to get more points
- if you let the blocks get too high and cannot spawn a new block you lose

Controls:
- a: move block left
- d: move block right
- w: hard drop block where it is
- s: soft drop block
- spacebar: rotate the block
- e: hold block / trade for held block
- esc: quit

Notes:
- This was developed and tested on an apple silicon Mac with an ARM based processor.
    - This should work for other platforms but I apologize if it does not
    - If you are on a mac, use the Terminal for the following instructions
- How to get started (assuming a unix-based shell):
    - Rename the corresponding makefile for your computer to `Makefile` to run `make` command easier
        - aka if you have do not have a mac, rename `Makefile-general` to `Makefile`, or otherwise rename `Makefile-mac` to `Makefile`
    - run `make clean` in shell
    - if you have mac, see below
    - run `make`
    - this should generate an executable file called `ProjectExec`
    - run `ProjectExec` by typing `./ProjectExec` into shell
    - have fun!
- If you want to download, and are using a mac,
    - in projectFunc.c, make sure to uncomment line 21 so the following runs:
      `return arc4random_uniform(7);`
      (AKA remove `//` from the beginning of the line)
    - however, if you are not using a mac, leave that line commented or make sure it is commented.
    - because for some reason rand() is broken on macs. I ran into this problem when coding this.
