#include"game.hpp"

Game::Game(){
    mainInit();

}


Game::~Game(){
    delete window;
}


void Game::update()
{
    pollEvents();
    updateMousePos();
    updateGrid();
    checkClick();
    moveBalls();
}

void Game::render()
{
    this->window->clear();

    renderBalls();

    this->window->display();
}

void Game::pollEvents()
{
     while (const std::optional event = this->window->pollEvent())
        {
            if (event->is<sf::Event::Closed>()){
                this->window->close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                procesKey(keyPressed);
            }
        }
}

void Game::procesKey(const sf::Event::KeyPressed* keyPressed)
{
    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        this->window->close();

}

void Game::updateMousePos()
{
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::checkClick()
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){

        if(mouseHeld!=false) return;


        this->mouseHeld=true;
        if(colides(this->mousePosView)) return;

        spawnBall(this->mousePosView);
        
        
    }
    else
    {
        mouseHeld=false;
    }
}



bool Game::colides(sf::Vector2f mPosView)
{
    for(auto &b : this->balls)
    {
        if(b.ball.getGlobalBounds().contains(mPosView)){
            return true;
        }
    }
    return false;
}

void Game::spawnBall(sf::Vector2f mPosView)
{
    float velX=rand()%5 -2;
    float velY=rand()%5 -2;
        
    if (velX==0) rand()%2==0 ? velX+=1 : velX-=1; 
    if (velY==0) rand()%2==0 ? velY+=1 : velY-=1; 

    float offset = rand()%10 -5;
    Ball newBall(20.f + offset,mPosView,{velX,velY});
    balls.push_back(newBall);
}

void Game::renderBalls()
{
    for(auto &b:this->balls)
    {
        this->window->draw(b.ball);
        //std::cout<<"Dosel sam do tud"<<"\n";
    }
}

void Game::moveBalls()
{
    for(auto &b : balls){
        b.checkEdge(videoMode);
        checkCollisions();
        b.updatePos();
    }
}

void Game::updateGrid()
{
    //grid.clear();
    for(size_t i=0; i<grid.size();i++){
        for(size_t j=0;j<grid[i].size();j++){
            grid[i][j].clear();
        }
    }
    for (size_t i= 0; i < balls.size(); i++) {
        int cellX = balls[i].ball.getPosition().x / cellSize;
        int cellY = balls[i].ball.getPosition().y / cellSize;
        grid[cellX][cellY].push_back(i);
    
    }
}


void Game::checkNeighbours(int x, int y)
{
    int dxy[9][2]={{0,0},{1,0},{0,1},{-1,0},{0,-1}
                  ,{1,1},{-1,1},{-1,1},{-1,-1}};
    for(size_t m_i=0; m_i<grid[x][y].size(); m_i++){
        int mainBallIndex=grid[x][y][m_i];
        for(int i=0;i<9;i++){
            int nx=x+dxy[i][0],ny=y+dxy[i][1];
            if(nx<0 or nx>gridRows or ny<0 or ny>gridColumns)continue;
            
            for(size_t s_i=0;s_i<grid[nx][ny].size();s_i++){
                int secondaryBallIndex=grid[nx][ny][s_i];
                if(mainBallIndex==secondaryBallIndex)continue;

                if(checkColiding(balls[mainBallIndex],balls[secondaryBallIndex])){
                    bounceBalls(mainBallIndex,secondaryBallIndex);
                } 
            }
        }
    }
}

void Game::checkCollisions(){
    for(size_t i=0;i<grid.size(); i++){
        for(size_t j=0;j<grid[i].size();j++){

            checkNeighbours(i,j);

        }
    }
}

void Game::bounceBalls(int indexA, int indexB)
{
    //std::swap(balls[indexA].velocity.x, balls[indexB].velocity.x);
    //std::swap(balls[indexA].velocity.y, balls[indexB].velocity.y);

    sf::Vector2f delta = balls[indexA].cords - balls[indexB].cords;
    float distance = std::sqrt(delta.x*delta.x + delta.y*delta.y);
    
    if(distance==0)return;

    sf::Vector2f normal = delta/distance;

    sf::Vector2f relVel = balls[indexA].velocity - balls[indexB].velocity;
    float velAlongNormal = relVel.x * normal.x + relVel.y * normal.y;

    if(velAlongNormal>0) return;

    balls[indexA].velocity-=velAlongNormal * normal;
    balls[indexB].velocity+=velAlongNormal * normal;

}

bool Game::running()
{
    return this->window->isOpen();
}

void Game::initWindow()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    this->videoMode.size={1000,800};
    this->window = new sf::RenderWindow(sf::VideoMode(this->videoMode),"Kruzici2",sf::Style::Titlebar | sf::Style::Close);
    
    this->window->setPosition({
        (int)((desktop.size.x - this->videoMode.size.x) / 2),
        (int)((desktop.size.y - this->videoMode.size.y) / 2)
    });

    this->window->setFramerateLimit(144);
}

void Game::initGrid()
{
    for(int i=0;i<gridRows;i++)
    {   
        grid.push_back({});
        for(int j=0;j<gridColumns;j++)
        {
            grid[i].push_back({});
        }
    }
}

void Game::initVariables()
{
    mouseHeld=false;
    cellSize=50.f;
    gridRows=videoMode.size.x/cellSize +1;
    gridColumns=videoMode.size.y/cellSize +1;
}


void Game::mainInit()
{
    initWindow();
    initVariables();
    initGrid();
}
