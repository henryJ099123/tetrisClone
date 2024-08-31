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
~~- DO NOT HOLD DOWN A BUTTON if using the student machines (this applies to the school where I turned this in)~~
~~- the student machines are too slow compared to a laptop~~
~~- I spent a long time making this program more efficient.~~
~~- My personal laptop can run this thing perfectly fast.~~
~~- Unfortunately, the student machines are much slower at drawing filled-in squares.~~
~~- I made sure to only paste/remove squares when absolutely necessary,~~
~~- but the updates still take a second to load, unfortunately.~~
- Rename the corresponding makefile for your computer to `Makefile` to run `make` command easier
- If you want to download, and are using a mac,
    - in projectFunc.c, make sure to uncomment line 21 so the following runs:
      `return arc4random_uniform(7);`
      (AKA remove `//` from the beginning of the line)
    - however, if you are not using a mac, leave that line commented or make sure it is commented.
    - because for some reason rand() is broken on macs. I ran into this problem when coding this.
