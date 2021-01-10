# Tetris Game Specs

# UI

![](https://i.imgur.com/xslrNEt.png)


### Background
rgb(57, 57, 57)

### Gameplay area 
Background: rgb(57, 57, 57)
Gird: rgb(47, 47, 47)

### Borders
Outer rgb(58, 219, 209)
Inner rgb(36, 133, 122)

# Classes

## TetrisController

### Methods
- Next
    - Called by Game every FALL_TIME milliseconds
    - Fall the block if state is FALLING
        - If fall test success:
            - Fall
            - If second fall test fails, set the state to LANDING
        - Else set the state to LANDING
    - Place the block if state is LANDING and set the state to LANDED or LOSE
    - Generate new block if state is LANDED and set the state to FALLING
    - Returns current game state

- Draw
    - Called by Game when update needed
    - Draw current block state
        - Call Tetromino->Draw

- Hold

- Rotate(bool ccw)

### Members

- rows[25]
    - Current terrain
    - 0 at bottom
    - Use deque

- state
    - block state
    - FALLING, LANDING, **LANDED**, LOSE

- falling_block
    - Current falling block
    - nullptr if no falling block

- next_blocks
    - Next blocks
    - Use queue

- hold_block
    - Current hold block
    - nullptr if no hold

- last_hold
    - If player holds the block last time
    - By the rule, player cannot perform the hold operation twice in a row


## Tetromino

## Methods

- DrawActive(pos_x, pos_y)
    - Draw at gameplay area

- DrawPreview(pos)
    - Draw at preview area

- DrawHold()
    - Draw at hold area

- Rotate(bool ccw?)
    - Rotate the block
    - ccw is false: rotate cw (orientation + 1)
    - ccw is true: rotate ccw (orientation - 1)

## Members

- block[4][4]
    - The shape of the block

- color
    - Color of the block

- orientation
    - 0 to 3

# [Super Rotating System (SRS)](https://tetris.fandom.com/wiki/SRS)

![](https://i.imgur.com/JW16nyL.png)

---
 J, L, S, T, Z Tetromino Wall Kick Data
|Orientation|Test 1 |Test 2 |Test 3|Test 4 |Test 5|
|-|-|-|-|-|-|
|0>>1 |	( 0, 0) |	(-1, 0) |	(-1, 1) |	( 0,-2) |	(-1,-2)|
|1>>2 |	( 0, 0) |	( 1, 0) |	( 1,-1) |	( 0, 2) |	( 1, 2)|
|2>>3 |	( 0, 0) |	( 1, 0) |	( 1, 1) |	( 0,-2) |	( 1,-2)|
|3>>0 |	( 0, 0) |	(-1, 0) |	(-1,-1) |	( 0, 2) |	(-1, 2)|

---

I Tetromino Wall Kick Data
|Orientation|Test 1 |Test 2 |Test 3|Test 4 |Test 5|
|-|-|-|-|-|-|
|0>>1 |( 0, 0) |	(-2, 0) |	( 1, 0) |	(-2,-1) |	( 1, 2)|
|1>>2 	|( 0, 0) |	(-1, 0) |	( 2, 0) |	(-1, 2) |	( 2,-1)|
|2>>3 |	( 0, 0) |	( 2, 0) |	(-1, 0) |	( 2, 1) 	|(-1,-2)|
|3>>0 |	( 0, 0) |	( 1, 0) |	(-2, 0) |	( 1,-2) |	(-2, 1)|

(x, y) means move x blocks right and move y blocks up
For rotating ccw, the offset is the negative of the original offset

---

1. Perform the rotation
    - Rotate the whole 4x4 or 3x3 or 2x2 block
3. Tests the 5 offsets until one succeed
4. If none of the test succeed, the turn fails


# Multiplayer API Specs- The Hiya Protocal
> Port 7122

## Client > Server
|Operation: 1 Byte| Operand| Operand|
|-|-|-|-|-|
|0x01 Register| 1B NameLength | Name
|0x04 Update Board| 200B: BoardEncoding|
|0x05 Attack| 1B: Target | 1B: Lines
|0x06 Dead|

## Server > Client
|Operation: 1 Byte| Operand| Operand| Operand | |
|-|-|-|-|-|-|
|0x01 Register| 1B: Player ID | 1B Player Count | Players ([ID][Length][Name])
|0x02 Players| 1B Player Count | Players ([ID][Length][Name])
|0x03 Game Started
|0x04 UpdateBoard| 1B: Player | 200B: BoardEncoding|
|0x05 Attack| 1B: Attacker | 1B: Target | 1B: Lines |
|0x06 Player Dead| 1B: Player |

## Board Encoding

|Row 0|Row 1|Row2...
|-|-|-|-|-|
|10 Bytes | 10 Bytes | 10 Bytes
Total: 200 Bytes

|Tile| Byte|
|-|-|
|NONE| 0x01|
|RED| 0x02|
|ORANGE| 0x03|
|YELLOW| 0x04|
|GREEN| 0x05|
|BLUE| 0x06|
|SKY| 0x07|
|PURPLE| 0x08|
