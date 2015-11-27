#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>

using namespace std;
using namespace sf;

void DrawPlayfield(sf::RenderWindow* window, Color back_color, Color front_color);
void DrawRackets(sf::RenderWindow* window,Color front_color,float pos_pl1, float pos_pl2, float length);

// Konfiguration //

//#define BACK_COLOR  0x353535ff
//#define FRONT_COLOR 0xffffffff

#define BACK_COLOR  0x001300ff
#define FRONT_COLOR 0x20a020ff

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define PLAYFIELD_SCALE_X 0.9
#define PLAYFIELD_SCALE_Y 0.8

#define MIDDLELINE_SCALE 0.013
#define HORLINE_SCALE 0.013

#define RACKET_SCALE_X 0.013
#define RACKET_SCALE_Y 0.125

#define RACKET_SPEED 2000

int main()
{
    sf::RenderWindow window;
    sf::VideoMode vm;
    sf::Event event;
    sf::Clock clock;

    sf::Time frame_time;

    cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << endl;
    cout << "Das Game Fester wird erstellt" << endl;
    window.create(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT),"POOOOOONG !!!!", sf::Style::Close /*| sf::Style::Fullscreen*/);

    float racket_pl1 = SCREEN_HEIGHT / 2;
    float racket_pl2 = SCREEN_HEIGHT / 2;

    float racket_length = SCREEN_HEIGHT * RACKET_SCALE_Y;
    float  racket_speed = RACKET_SPEED * (768.0 / window.getSize().y);

    float racket_pl1_add_pos = 0.0f;
    float racket_pl2_add_pos = 0.0f;

    unsigned char racket_keys = 0;
    unsigned char racket_keys_old = 0;


    float racket_min_y = (window.getSize().y - window.getSize().y * PLAYFIELD_SCALE_Y) / 2;
    float racket_max_y = racket_min_y + window.getSize().y * PLAYFIELD_SCALE_Y;

    racket_min_y += racket_length/2;
    racket_max_y -= racket_length/2;

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

        // Bildschirmaufbau
        DrawPlayfield(&window,sf::Color(BACK_COLOR),sf::Color(FRONT_COLOR));
        DrawRackets(&window,sf::Color(FRONT_COLOR),racket_pl1,racket_pl2,racket_length);
        window.display();

        sf::sleep(sf::milliseconds(10));
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
