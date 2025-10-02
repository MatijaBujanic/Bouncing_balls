#include"game.hpp"

Game::Game(){
    mainInit();

}


Game::~Game(){
    delete window;
}


void Game::update()
{
    dt = dtClock.restart().asSeconds();  

    pollEvents();
    updateMousePos();
    checkClick();

    if(!paused){
        moveBalls();
        updateGrid();
        checkCollisions();
    }
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

    if (keyPressed->scancode == sf::Keyboard::Scancode::F)
        std::cout<<"FPS: "<<fps<<"\n";
     if (keyPressed->scancode == sf::Keyboard::Scancode::G){
        if(gravity!=0)gravity=0;
        else gravity=75.f;
        updateGrav();
     }
     if (keyPressed->scancode == sf::Keyboard::Scancode::P)
        paused=!paused;
        

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
    Ball newBall(20.f + offset,mPosView,{velX*100,velY*100},this->gravity);
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
        b.updatePos(dt);
        
        //checkCollisions();
    }
}

void Game::updateGrid()
{
    //grid.clear();
    for(auto& row : grid) {
        for(auto& cell : row) {
            cell.clear();
        }
    }

    for (size_t i= 0; i < balls.size(); i++) {
        int cellX = balls[i].ball.getPosition().x / cellSize;
        int cellY = balls[i].ball.getPosition().y / cellSize;

        if (cellX >= 0 && cellX < gridRows && cellY >= 0 && cellY < gridColumns) {
            grid[cellX][cellY].push_back(i);
        }
    
    }
}

void Game::updateGrav()
{
    for(size_t i=0;i<balls.size();i++){
        balls[i].individualGravity=gravity;
    }
}

void Game::checkNeighbours(int x, int y)
{
    int dxy[9][2]={{0,0},{1,0},{0,1},{-1,0},{0,-1}
                  ,{1,1},{1,-1},{-1,1},{-1,-1}};
    
    //int dxy[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}};

    for(size_t m_i=0; m_i<grid[x][y].size(); m_i++)
    {
        int mainBallIndex=grid[x][y][m_i];

        for(int i=0;i<9;i++)
        {
            int nx=x+dxy[i][0],ny=y+dxy[i][1];
             if(nx < 0 || nx >= gridRows || ny < 0 || ny >= gridColumns) continue;
            
            for(size_t s_i=0;s_i<grid[nx][ny].size();s_i++)
            {
                int secondaryBallIndex=grid[nx][ny][s_i];

                if(mainBallIndex==secondaryBallIndex)continue;

                if(checkColiding(balls[mainBallIndex],balls[secondaryBallIndex]))
                {
                    bounceBalls(mainBallIndex,secondaryBallIndex);
                } 
            }
        }
    }
}

void Game::checkCollisions(){
    for(size_t i=0;i<grid.size(); i++){
        for(size_t j=0;j<grid[i].size();j++){

            if(!grid[i][j].empty())
            {
                checkNeighbours(i, j);
            }

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

    changeColor(indexA,indexB);
}

void Game::changeColor(int indexA, int indexB)
{
    uint8_t r=balls[indexA].rValue,
            g=balls[indexA].gValue,
            b=balls[indexA].bValue;

    balls[indexA].rValue-=balls[indexB].rValue;
    balls[indexA].gValue-=balls[indexB].gValue;
    balls[indexA].bValue-=balls[indexB].bValue;

    balls[indexB].rValue+=r;balls[indexB].gValue+=g;balls[indexB].bValue+=b;

    balls[indexA].ball.setFillColor(
        sf::Color{balls[indexA].rValue, balls[indexA].gValue, balls[indexA].bValue});
    balls[indexB].ball.setFillColor(
        sf::Color{balls[indexB].rValue, balls[indexB].gValue, balls[indexB].bValue});
}


bool Game::running()
{
    return this->window->isOpen();
}

void Game::initWindow()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    this->videoMode.size={1000,800};
    this->window = new sf::RenderWindow(sf::VideoMode(this->videoMode),"Bouncing_balls",sf::Style::Titlebar | sf::Style::Close);
    
    this->window->setPosition({
        (int)((desktop.size.x - this->videoMode.size.x) / 2),
        (int)((desktop.size.y - this->videoMode.size.y) / 2)
    });

    this->window->setFramerateLimit(144);
}

void Game::initGrid()
{
    grid.clear(); 
    grid.resize(gridRows); 
    
    for(int i = 0; i < gridRows; i++) {   
        grid[i].resize(gridColumns); 
    }
}

void Game::initVariables()
{
    mouseHeld=false;
    cellSize=50.f;
    this->paused=false;
    this->gravity=75.f;
    gridRows=videoMode.size.x/cellSize +2;
    gridColumns=videoMode.size.y/cellSize +2;
    balls.reserve(1000);
}


void Game::mainInit()
{
    initWindow();
    initVariables();
    initGrid();
}
