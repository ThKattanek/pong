#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <math.h>

using namespace std;
using namespace sf;

void DrawPlayfield(sf::RenderWindow* window, Color back_color, Color front_color);
void DrawRackets(sf::RenderWindow* window,Color front_color,float pos_pl1, float pos_pl2, float length);
void DrawBall(sf::RenderWindow* window, Color front_color, float x, float y);

// Konfiguration //

#define BACK_COLOR  sf::Color(0x35,0x35,0x35,0xff)
#define FRONT_COLOR sf::Color(0xff,0xff,0xff,0xff)

//#define BACK_COLOR  sf::Color(0x00,0x13,0x00,0xff)
//#define FRONT_COLOR sf::Color(0x20,0xa0,0x20,0xff)

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define PLAYFIELD_SCALE_X 0.9
#define PLAYFIELD_SCALE_Y 0.8

#define MIDDLELINE_SCALE 0.013
#define HORLINE_SCALE 0.013

#define RACKET_SCALE_X 0.013
#define RACKET_SCALE_Y 0.125

#define BALL_SCALE_X 0.013
#define BALL_SCALE_Y 0.013

#define RACKET_SPEED 2000

#define BALL_SPEED 2000

int main()
{
    sf::RenderWindow window;
    sf::VideoMode vm;
    sf::Event event;
    sf::Clock clock;
    sf::Time frame_time;

    srand(time(NULL));

    char sound1_file[1024];
    char sound2_file[1024];
    char numbers_file[1024];
    
    sprintf(sound1_file,"%s/sound1.ogg",DATA_PATH);
    sprintf(sound2_file,"%s/sound2.ogg",DATA_PATH);
    sprintf(numbers_file,"%s/num.png",DATA_PATH);
    
    cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << endl;
    cout << "Das Game Fester wird erstellt" << endl;
    window.create(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT),"POOOOOONG !!!!", sf::Style::Close /*| sf::Style::Fullscreen*/);

    float racket_pl1 = SCREEN_HEIGHT / 2;
    float racket_pl2 = SCREEN_HEIGHT / 2;

    float racket_length = SCREEN_HEIGHT * RACKET_SCALE_Y;
    float  racket_speed = RACKET_SPEED * ((float)SCREEN_HEIGHT / window.getSize().y);

    float racket_pl1_add_pos = 0.0f;
    float racket_pl2_add_pos = 0.0f;

    unsigned char racket_keys = 0;
    unsigned char racket_keys_old = 0;

    float racket_min_y = (window.getSize().y - window.getSize().y * PLAYFIELD_SCALE_Y) / 2;
    float racket_max_y = racket_min_y + window.getSize().y * PLAYFIELD_SCALE_Y;

    racket_min_y += racket_length/2;
    racket_max_y -= racket_length/2;

    bool ball_aktive = false;
    float ball_speed = BALL_SPEED * ((float)SCREEN_WIDTH / window.getSize().x);

    float angle = (rand() + 1.0)/(RAND_MAX + 2)*60 + 240;
    // Vector aus Winkel errechen auf Eiheitskreis
    sf::Vector2f ball_vector;
    ball_vector.x = cos((M_PI/180)*angle);   // Winkel für COS in Bogenmaß umrechnen
    ball_vector.y = sin((M_PI/180)*angle);   // Winkel für SIN in Bogenmaß umrechnen

    float line_length = SCREEN_WIDTH * PLAYFIELD_SCALE_X;
    float ball_max_left = ((SCREEN_WIDTH - line_length) / 2);
    float ball_max_rigth = ball_max_left + line_length;
    ball_max_left += SCREEN_WIDTH * BALL_SCALE_X;
    ball_max_rigth -= SCREEN_WIDTH * BALL_SCALE_X;

    float ball_max_top = ((SCREEN_HEIGHT - SCREEN_HEIGHT * PLAYFIELD_SCALE_Y) / 2) + (BALL_SCALE_Y * SCREEN_HEIGHT);
    float ball_max_bottom = (ball_max_top + SCREEN_HEIGHT * PLAYFIELD_SCALE_Y) - (BALL_SCALE_Y * SCREEN_HEIGHT * 2);

    float ball_x,ball_y;

    int akt_player = 0;
    int scores_pl1 = 0, scores_pl2 = 0;

    sf::Texture texture;
    texture.loadFromFile(numbers_file);

    sf::Sprite score_pl1_out(texture), score_pl2_out(texture);

    score_pl1_out.setScale(sf::Vector2f(11,11));
    score_pl1_out.setOrigin(Vector2f(0,0));
    score_pl1_out.setPosition(SCREEN_WIDTH / 2 - 80,10);

    IntRect sc1_rec(0,0,4,5);
    score_pl1_out.setTextureRect(sc1_rec);

    score_pl2_out.setScale(sf::Vector2f(11,11));
    score_pl2_out.setOrigin(Vector2f(0,0));
    score_pl2_out.setPosition(SCREEN_WIDTH / 2 + 30,10);

    IntRect sc2_rec(0,0,4,5);
    score_pl2_out.setTextureRect(sc2_rec);    

    sf::SoundBuffer buffer1;
    if (!buffer1.loadFromFile(sound1_file))
            return -1;

    sf::SoundBuffer buffer2;
    if (!buffer2.loadFromFile(sound2_file))
            return -1;

    sf::Sound sound1(buffer1);
    sf::Sound sound2(buffer2);

    frame_time = clock.getElapsedTime();

    while(window.isOpen())
    {

        frame_time = clock.restart();

        while(window.pollEvent(event))
        {
            switch(event.type)
            {
            case (sf::Event::Closed):
                window.close();
                break;

            case sf::Event::KeyPressed:
                switch(event.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                break;

                case sf::Keyboard::Q:
                    // Player 1 UP
                    racket_keys |= 1 << 0;
                    break;

                case sf::Keyboard::A:
                    // Player 1 DOWN
                    racket_keys |= 1 << 1;
                    break;

                case sf::Keyboard::Up:
                    // Player 2 UP
                    racket_keys |= 1 << 4;
                    break;

                case sf::Keyboard::Down:
                    // Player 2 DOWN
                    racket_keys |= 1 << 5;
                    break;

                default:
                    break;
                }
                break;

            case sf::Event::KeyReleased:
                switch(event.key.code)
                {
                case sf::Keyboard::Q:
                    // Player 1 UP
                    racket_keys &= ~(1 << 0);
                    break;

                case sf::Keyboard::A:
                    racket_keys &= ~(1 << 1);
                    // Player 1 DOWN
                    break;

                case sf::Keyboard::Up:
                    // Player 2 UP
                    racket_keys &= ~(1 << 4);
                    break;

                case sf::Keyboard::Down:
                    // Player 2 DOWN
                    racket_keys &= ~(1 << 5);
                    break;
                case sf::Keyboard::Space:
                    if(scores_pl1 < 9 && scores_pl2 <9)
                        ball_aktive = true;
                    break;
                case sf::Keyboard::R:
                    ball_aktive = false;
                    akt_player = 0;
                    scores_pl1 = 0, scores_pl2 = 0;
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }
        }


        unsigned char temp = racket_keys & 0x03 | (racket_keys_old & 0x03) << 2;

        switch (temp)
        {
        case 1: case 9: case 11: case 13:
            // UP
            racket_pl1_add_pos = -frame_time.asMicroseconds() * (1 / racket_speed);
            break;
        case 2: case 6: case 7: case 14:
            // DOWN
            racket_pl1_add_pos = frame_time.asMicroseconds() * (1 / racket_speed);
            break;
        case 4: case 8: case 12:
            // HALT
            racket_pl1_add_pos = 0;
            break;
        default:
            break;
        }

        temp = racket_keys>>4 & 0x03 | (racket_keys_old>>4 & 0x03) << 2;

        switch (temp)
        {
        case 1: case 9: case 11: case 13:
            // UP
            racket_pl2_add_pos = -frame_time.asMicroseconds() * (1 / racket_speed);
            break;
        case 2: case 6: case 7: case 14:
            // DOWN
            racket_pl2_add_pos = frame_time.asMicroseconds() * (1 / racket_speed);
            break;
        case 4: case 8: case 12:
            // HALT
            racket_pl2_add_pos = 0;
            break;
        default:
            break;
        }

        racket_keys_old = racket_keys;

        if(((racket_pl1 + racket_pl1_add_pos) > racket_min_y) && ((racket_pl1 + racket_pl1_add_pos) < racket_max_y))
            racket_pl1 += racket_pl1_add_pos;

        if(((racket_pl2 + racket_pl2_add_pos) > racket_min_y) && ((racket_pl2 + racket_pl2_add_pos) < racket_max_y))
            racket_pl2 += racket_pl2_add_pos;

        // BallPhysics //
        if(ball_aktive)
        {
            ball_x = ball_vector.x * frame_time.asMicroseconds() * (1 / ball_speed) + ball_x;
            ball_y = ball_vector.y * frame_time.asMicroseconds() * (1 / ball_speed) + ball_y;

            if(ball_x < ball_max_left)
            {
                if(ball_y < (racket_pl1-racket_length/2 - SCREEN_HEIGHT*BALL_SCALE_Y))
                {
                    // Punkt für Player 2
                    scores_pl2++;
                    akt_player = 0;
                    ball_aktive = false;
                }

                if(ball_y > (racket_pl1+racket_length/2 + SCREEN_HEIGHT*BALL_SCALE_Y))
                {
                    // Punkt für Player 2
                    scores_pl2++;
                    akt_player = 0;
                    ball_aktive = false;
                }

                if(ball_aktive)
                {
                    sound1.setPlayingOffset(sf::milliseconds(100));
                    sound1.play();
                }
                ball_vector.x *= -1;
            }

            if(ball_x > ball_max_rigth)
            {
                if(ball_y < (racket_pl2-racket_length/2 - SCREEN_HEIGHT*BALL_SCALE_Y))
                {
                    // Punkt für Player 1
                    scores_pl1++;
                    akt_player = 1;
                    ball_aktive = false;
                }

                if(ball_y > (racket_pl2+racket_length/2 + SCREEN_HEIGHT*BALL_SCALE_Y))
                {
                    // Punkt für Player 1
                    scores_pl1++;
                    akt_player = 1;
                    ball_aktive = false;
                }

                if(ball_aktive)
                {
                    sound1.setPlayingOffset(sf::milliseconds(100));
                    sound1.play();
                }

                ball_vector.x *= -1;
            }

            if(ball_y <= ball_max_top)
            {
                sound2.setPlayingOffset(sf::milliseconds(100));
                sound2.play();
                ball_vector.y *= -1;
            }

            if(ball_y >= ball_max_bottom)
            {
                sound2.setPlayingOffset(sf::milliseconds(100));
                sound2.play();
                ball_vector.y *= -1;
            }
        }
        else
        {
            float angle;

            if(akt_player == 0)
            {
                angle = (rand() + 1.0)/(RAND_MAX + 2)*60 + 60;
                ball_x = ball_max_left;
                ball_y = racket_pl1;
            }
            else
            {
                angle = (rand() + 1.0)/(RAND_MAX + 2)*60 + 240;
                ball_x = ball_max_rigth;
                ball_y = racket_pl2;
            }

            ball_vector.x = cos((M_PI/180)*angle);   // Winkel für COS in Bogenmaß umrechnen
            ball_vector.y = sin((M_PI/180)*angle);   // Winkel für SIN in Bogenmaß umrechnen
        }

        // Bildschirmaufbau
        DrawPlayfield(&window,BACK_COLOR,FRONT_COLOR);
        DrawRackets(&window,FRONT_COLOR,racket_pl1,racket_pl2,racket_length);
        DrawBall(&window,FRONT_COLOR,ball_x,ball_y);

        sc1_rec.left=scores_pl1*4;
        sc2_rec.left=scores_pl2*4;
        score_pl1_out.setTextureRect(sc1_rec);
        score_pl2_out.setTextureRect(sc2_rec);
        window.draw(score_pl1_out);
        window.draw(score_pl2_out);

        window.display();

        sf::sleep(sf::milliseconds(1));
    }

    cout << "Und tschüss .." << endl;
    return 0;
}

void DrawPlayfield(RenderWindow *window, sf::Color back_color, sf::Color front_color)
{
    // Mittel Linie //

    float window_xw = window->getSize().x;
    float window_yw = window->getSize().y;
    float middle_line_xw = window_xw * MIDDLELINE_SCALE;
    float middle_line_yw = window_yw * PLAYFIELD_SCALE_Y;

    sf::RectangleShape middle_line;

    window->clear(back_color);

    middle_line.setFillColor(front_color);
    middle_line.setOrigin(middle_line_xw / 2,middle_line_yw / 2);
    middle_line.setPosition(window_xw / 2, window_yw / 2);
    middle_line.setSize(sf::Vector2f(middle_line_xw,middle_line_yw));

    // Ober- und Unterbegrenzung //

    float line_length = window_xw * PLAYFIELD_SCALE_X;
    float start_x = (window_xw - line_length) / 2;
    float point_xw = window_xw * HORLINE_SCALE;
    float point_yw = window_yw * HORLINE_SCALE;

    float oline_y = (window_yw - middle_line_yw) / 2;
    float uline_y = oline_y + middle_line_yw;

    float mul_wert = line_length / 34;

    sf::RectangleShape point;
    point.setSize(sf::Vector2f(point_xw,point_yw));
    point.setOrigin(point_xw / 2,point_yw / 2);
    point.setFillColor(front_color);

    for(int i = 0; i<35; i++)
    {
        point.setPosition(i*mul_wert + start_x,oline_y);
        window->draw(point);

        point.setPosition(i*mul_wert + start_x,uline_y);
        window->draw(point);
    }

    window->draw(middle_line);
}

void DrawRackets(sf::RenderWindow* window, Color front_color, float pos_pl1, float pos_pl2, float length)
{
    float window_xw = window->getSize().x;
    float width =window_xw * RACKET_SCALE_X;
    float line_length = window_xw * PLAYFIELD_SCALE_X;
    float start_x = (window_xw - line_length) / 2;
    float end_x = start_x + line_length;

    sf::RectangleShape racket;
    racket.setSize(sf::Vector2f(width,length));
    racket.setOrigin(width / 2,length / 2);
    racket.setFillColor(front_color);

    racket.setPosition(start_x,pos_pl1);
    window->draw(racket);

    racket.setPosition(end_x,pos_pl2);
    window->draw(racket);
}

void DrawBall(sf::RenderWindow* window,Color front_color, float x, float y)
{
    float window_xw = window->getSize().x;
    float window_yw = window->getSize().y;
    float width =window_xw * BALL_SCALE_X;
    float high =window_yw * BALL_SCALE_Y;

    sf::RectangleShape ball;
    ball.setSize(sf::Vector2f(width,high));
    ball.setOrigin(width/2,high/2);
    ball.setFillColor(front_color);
    ball.setPosition(x,y);

    window->draw(ball);
}
