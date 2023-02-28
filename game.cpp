#include"Math.h"
#include"game.h"
Clock gameClock;
 float deltaTime = 0.0f;
int first =0;
int count = 0;
int scoreInteger;
int die=3;


  game::game(int w ,int h ,std::string title){
     window= new RenderWindow(VideoMode(w, h), title);
     
      music.openFromFile("audio/meditate.ogg");
      music.setVolume(70);
      music.play();
      music.setLoop(true);
      font.loadFromFile("Love Casual/Love Casual.ttf");
      score.setFont(font);
      score.setOutlineThickness(2.0f);
      score.setOutlineColor(Color::Black);
      score.setPosition(w-60,h-40);
      score.setString("0");
      life.setFont(font);
      life.setOutlineThickness(2.0f);
      life.setOutlineColor(Color::Black);
      life.setPosition(60,h-40);
      life.setString("3");
     

    
     width = w;
     length = h;
    paddle.setSize(Vector2f(100.0f,20.0f));
    paddle.setFillColor(Color(80,10,10));
    paddle.setOutlineThickness(1.0f);
    paddle.setOutlineColor(Color(255,255,255));
    paddle.setPosition(Vector2f(width/2-paddle.getSize().x/2,length-40));

    ball.setRadius(8.0f);
    ball.setFillColor(Color(255,255,255));
    ball.setOutlineThickness(1.0f);
    ball.setOutlineColor(Color(255,255,255));
  
    ds=400.0f;
    angle=0.0f;
   
    
    block.setSize(Vector2f(40,30));
    block.setOutlineThickness(1.0f);
    
    int totalblocks=(width/block.getSize().x)*(length/block.getSize().y);
    isblock = new bool[totalblocks];
    hp = new int[totalblocks];
    
    for(int i = 0;i<totalblocks;i++){
      isblock[i]=false;
      hp[i]=-1;
    }
   
   reset();

   

  }
     game::~game(){
      delete[] isblock;
      delete window;
     }
     void game:: reset(){
      scoreInteger=0;
      die = 3;
      life.setString("3");

      score.setString("0");
      first=0;
       speed = Vector2f(0.0f,0.0f);
        for(int i = 0;i<160;i++){
            isblock[i]=true;
            hp[i]=rand()%3;
            }
         }
    void game:: event(){
         
         if (e.type == Event::Closed)
                window->close();
        else if(e.type==Event::MouseMoved){
            paddle.setPosition(Vector2f(Mouse::getPosition(*window).x-(paddle.getSize().x/2),length-40));
        if(paddle.getPosition().x<0)
	           paddle.setPosition(Vector2f(0,paddle.getPosition().y));
        else if(paddle.getPosition().x>width-paddle.getSize().x)
	          paddle.setPosition(Vector2f(width-paddle.getSize().x,paddle.getPosition().y));
      }
      else if(e.type==Event::MouseButtonPressed){
        if(first==0){
              ball.setPosition(Vector2f(paddle.getPosition().x+(paddle.getSize().x/2)-ball.getRadius(),paddle.getPosition().y-paddle.getSize().y));
              angle = (-20-(rand()%140)*(M_PI/180.0));
              
              speed.x=cos(angle)*ds;

              speed.y=sin(angle)*ds;
              first++;

        }
      

      }

    }

    void game:: update(float dt){
      if(speed.y==0)
        ball.setPosition(Vector2f(paddle.getPosition().x+(paddle.getSize().x/2)-ball.getRadius(),paddle.getPosition().y-paddle.getSize().y));
      else{
        ball.setPosition(Vector2f(ball.getPosition().x+(speed.x*dt),ball.getPosition().y+(speed.y*dt)));
      }
      if(ball.getPosition().x<=0.0f)
        {
          ball.setPosition(Vector2f(0.0f,ball.getPosition().y));
          speed.x=abs(speed.x);
        }
        else if(ball.getPosition().x+(ball.getRadius()*2.0f)>=width)
        {
          ball.setPosition(Vector2f(width-(ball.getRadius()*2.0f),ball.getPosition().y));
          speed.x=-abs(speed.x);
        }

        if(ball.getPosition().y<=0.0f)
        {
          ball.setPosition(Vector2f(ball.getPosition().x,0.0f));
          speed.y=abs(speed.y);
        }
        else if(ball.getPosition().y+(ball.getRadius()*2.0f)>=length)
        {
          
            die =  std::stoi(((std::string)(life.getString())).c_str());
           die-=1;
           
            life.setString((std::to_string(die)));
            ball.setPosition(Vector2f(paddle.getPosition().x+(paddle.getSize().x/2)-ball.getRadius(),paddle.getPosition().y-paddle.getSize().y));
            first=0;
            if(die==0)
              reset();
          
        }
        if(ball.getPosition().x+(ball.getRadius()*2.0f)>=paddle.getPosition().x
          &&ball.getPosition().y+(ball.getRadius()*2.0f)>=paddle.getPosition().y
          &&ball.getPosition().x<paddle.getPosition().x+paddle.getSize().x
          &&ball.getPosition().y<paddle.getPosition().y+paddle.getSize().y)
        {
            speedFactor = 4.0f;
            float ratio = abs(speed.x)/abs(speed.y);
            if(speed.x<0.0f)
              speed.x-=speedFactor;
            else
              speed.x+=speedFactor;
            if(speed.y<0.0f)
              speed.y-=speedFactor*ratio;
            else
              speed.y+=speedFactor*ratio;

              float paddleCentre = paddle.getPosition().x+(paddle.getSize().x/2.0f);
              float ballCentre = ball.getPosition().x+ball.getRadius();
              speed.x += (ballCentre-paddleCentre)*2.0f;
              ball.setPosition(Vector2f(ball.getPosition().x,paddle.getPosition().y-(ball.getRadius()*2.0f)));
              speed.y=-abs(speed.y);
        }
        for(int y = 0;y<length/block.getSize().y;y++)
        {
	        for(int x = 0;x<width/block.getSize().x;x++)
	         {
		      if(isblock[(int)(x+(y*(width/block.getSize().x)))])
             {
                if(ball.getPosition().x+(ball.getRadius()*2.0f)>=x*block.getSize().x
                  &&ball.getPosition().x+(ball.getRadius()*2.0f)>=x*block.getSize().x
                  &&ball.getPosition().x<(x+1)*block.getSize().x
                  &&ball.getPosition().y<(y+1)*block.getSize().y)
                  {
                    if(hp[(int)(x+(y*(width/block.getSize().x)))]==0){
                       scoreInteger =  std::stoi(((std::string)(score.getString())).c_str());
                      scoreInteger+=10;
                      score.setString(std::to_string(scoreInteger));
                      count++;
                      if(count==160){
                        reset();

                      }
                    isblock[(int)(x+(y*(width/block.getSize().x)))]=false;
                    }
                    else if(hp[(int)(x+(y*(width/block.getSize().x)))]==1){
                      block.setFillColor(Color(255,100,50));
                      block.setOutlineColor(Color(255,255,255));
                      hp[(int)(x+(y*(width/block.getSize().x)))]-=1;
                    }
                    else if(hp[(int)(x+(y*(width/block.getSize().x)))]==2){
                      block.setFillColor(Color(255,255,0));
                      block.setOutlineColor(Color(255,255,255));
                      hp[(int)(x+(y*(width/block.getSize().x)))]-=1;
                    }

                    speed.y=abs(speed.y);
                    ball.setPosition(Vector2f(ball.getPosition().x,(y+1)*block.getSize().y));

                  }
             }
           }
        }
       
            
    }
    void game::render(){
       
      
        for(int y = 0;y<length;y+=block.getSize().y)
        {
	        for(int x = 0;x<width;x+=block.getSize().x)
	         {
		       if(isblock[(int)((x/block.getSize().x)+((y/block.getSize().y)*(width/block.getSize().x)))])
             {
                if(hp[(int)((x/block.getSize().x)+((y/block.getSize().y)*(width/block.getSize().x)))]==0){
                block.setFillColor(Color(255,100,50));
                block.setOutlineColor(Color(255,255,255));
                }
                else if(hp[(int)((x/block.getSize().x)+((y/block.getSize().y)*(width/block.getSize().x)))]==2){
                  block.setFillColor(Color(255,0,0));
                  block.setOutlineColor(Color(255,255,255));
                }
                else if(hp[(int)((x/block.getSize().x)+((y/block.getSize().y)*(width/block.getSize().x)))]==1){
                  block.setFillColor(Color(255,255,0));
                  block.setOutlineColor(Color(255,255,255));
                }
            
             }
             

         else
          {
            
            block.setFillColor(Color(rand()%(10+y/16),rand()%(10+((length-y))%16),rand()%10));
            block.setOutlineColor(Color(40,40,40));
            
          }


          block.setPosition(Vector2f(x,y));
            window->draw(block);
	      }
      }
     


        
        window->draw(ball);
        window->draw(paddle);
        window->draw(life);
        window->draw(score);
      
      

      
    }
    void game:: run(){
         
         while (window->isOpen()){
            
          gameClock.restart();
        
        while(window->pollEvent(e)){
            event();
        }
        
        update(deltaTime);
        window->clear();
       // window->draw(imageSprite);
        
        render();
        window->display();
        deltaTime=gameClock.getElapsedTime().asSeconds();
         }
         

    }
int main()
{
    game  game(800,600,"Block Breaker");
    game.run();
    return 0;
}
