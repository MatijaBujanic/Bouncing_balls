#ifndef __GAME_H__
#define __GAME_H__

#include"includes.hpp"
#include"ball.hpp"

class Game{
private:
//Window variables
sf::RenderWindow *window;
sf::VideoMode videoMode;

//Mouse positions
sf::Vector2i mousePosWindow;
sf::Vector2f mousePosView;    

//Variables
bool mouseHeld;
float cellSize;
int gridRows;
int gridColumns;
std::vector<std::vector<std::vector<int>>>grid;

float fps;

std::vector<Ball> balls;

//inits
void mainInit();
void initVariables();
void initWindow();
void initGrid();


public:
//Constructors
Game();
~Game();

//Primary func
void update();
void render();
void pollEvents();

//trivial for pollEvents
void procesKey(const sf::Event::KeyPressed* keyPressed);

//mousePos
void updateMousePos();

//click func
void checkClick();
bool colides(sf::Vector2f mPosView);
void spawnBall(sf::Vector2f mPosView);


//renders
void renderBalls();

//
void moveBalls();
void updateGrid();
void checkNeighbours(int x, int y);
void checkCollisions();
void bounceBalls(int indexA, int indexB);
void changeColor(int indexA, int indexB);

//accessors
bool running();
float getFPS() const { return fps; }
void setFPS(float value) { fps = value; }

};


#endif // __GAME_H__