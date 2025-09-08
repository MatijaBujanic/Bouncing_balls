#include"ball.hpp"


Ball::Ball(float radius, sf::Vector2f cords, sf::Vector2f velocity)
    : radius(radius), cords(cords), velocity(velocity){


    initBall();
}

Ball::~Ball(){
    
}
void Ball::initBall()
{
    ball.setRadius(this->radius);
    ball.setPosition(this->cords);

    uint8_t rValue = rand() % 256;
    uint8_t gValue = rand() % 256;
    uint8_t bValue = rand() % 256;

    ball.setFillColor(sf::Color{rValue, gValue, bValue});
}

void Ball::updatePos()
{
    
    this->cords.x += this->velocity.x;
    this->cords.y += this->velocity.y;
    

    ball.move(velocity);
}

void Ball::checkEdge(sf::VideoMode videoMode)
{
    if(this->cords.x<=0 or this->cords.x + this->radius*2 > videoMode.size.x ){
        this->velocity.x*=-1;
    }
    if(this->cords.y<=0 or this->cords.y + this->radius*2 > videoMode.size.y ){
        this->velocity.y*=-1;
    }
}

bool checkColiding(Ball &b1, Ball &b2)
{
    sf::Vector2f centerB1 = {b1.cords.x+b1.radius, b1.cords.y+b1.radius};
    sf::Vector2f centerB2 = {b2.cords.x+b2.radius, b2.cords.y+b2.radius};

    float dx=centerB1.x-centerB2.x;
    float dy=centerB1.y-centerB2.y;
    float distanceSquared = dx * dx + dy * dy;

    float radiusSum = b1.radius+b2.radius;

    return distanceSquared <= (radiusSum * radiusSum);
}




