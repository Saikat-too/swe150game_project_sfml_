#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
using namespace sf;
class game
{
    public:
    game(int w ,int h ,std::string title);
    ~game();
    void reset();
    void event();
    void update(float dt);
    void render();
    void run();
    
    
    private:
    RenderWindow*window=NULL;
    int width;
    int length;
    Event e;
    CircleShape ball;
    RectangleShape paddle;
    float ds;
    float angle;
    Vector2f speed;
    RectangleShape block;
    bool* isblock;
    float speedFactor;
    Music music;
    Font font;
    Texture imageSource;
    Sprite imageSprite;
    bool* isbrick;
    RectangleShape brick;
    int* hp;
    Text score;
    Text life;


};