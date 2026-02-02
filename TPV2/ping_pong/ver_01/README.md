# PingPong v1

This version uses the classical GameObject architecture with methods handleInput(SDL_Event&), update() and render(). 

We have 3 kinds of GameObjcts:

 1. Ball -- representing the ball
 2. Paddle -- Used for the two paddles
 3. GameManager -- Used for starting the game, showing the score, etc.
 
The logic (checking for collisions, etc) is coded in Game -- we could move the code to GameManger as well, or move the game control from GameManager to Game, but this is not important for our needs for now.

