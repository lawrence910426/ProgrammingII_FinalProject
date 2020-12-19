# Team 7122
- Members
    - 109062302 郭品毅
    - 109062328 吳邦寧
- Name
    - Hiya-Why-You-Use-Reinforcement-Learning-To-Play-Tetris-You-Dont-Know-How-To-Play-It-Yourself-Uncle-Roger-So-Sad-Now

# Summary
The project consist of three major parts, tetris game, A.I. opponent and multiplayer support. 
- The concept of tetris game stems from tetris battle.
- The A.I. opponent should based on Machine-Learning.
- The multiplayer funciton should based on `Tcp` and `Server-Client` architecture. 

# Tetris
Tetris is a game about filling the lines and strategically placing the blocks. The following is our rules.
- Once the blocks are placed to form a line, the line would vanish and send a attack signal to a opponent (if opponent exists).
- Players may only put a block to the hold storage if the hold storage is empty.
- Players may preview 3 pending blocks in advance.

# A.I. Opponent
The artificial intelligence opponent should based on Q-Learning, an algorithm that contributes to the creation of AlphaGo. If the implementation of Q-Learning is too sophisticated or the performance of other methods surpass the performance of Q-Learning, we would alter our architecture.

We would prepare some drinks for whom could beat our A.I. during the demo. 

# Multiplayer support
To communicate between machines, we decided to apply Server-Client architecture on our project. The client would connect to server's 7122 port via TCP.
