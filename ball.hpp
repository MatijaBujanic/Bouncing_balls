#ifndef __BALL_H__
#define __BALL_H__

#include"includes.hpp"




class Ball{

private:
//inits
void initBall();
    
public:
sf::CircleShape ball;
float radius;    
sf::Vector2f cords;
sf::Vector2f velocity;



//Constructors
Ball(float radius, sf::Vector2f cords, sf::Vector2f velocity);
~Ball();

//movement
void updatePos();
void checkEdge(sf::VideoMode videoMode);


};

bool checkColiding(Ball &b1, Ball &b2);





#endif // __BALL_H__