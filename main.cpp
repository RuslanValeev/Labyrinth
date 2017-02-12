#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <iostream>
#include "base.hpp"
#include <string>
#include <sstream>
#include <stdio.h>

using namespace sf;
using namespace std;

RenderWindow window;

class Audio {
private:
    Music track;

public:
    Audio(string path)
    {
        if (!track.openFromFile(path)) {
            return EXIT_FAILURE;
        }
    }

    void on()
    {
        this->track.play();
    }

    void off()
    {
        this->track.stop();
    }
};

class Player {
public:
    float dx, dy;
    FloatRect rect;
    Sprite sprite;
    Texture t_hero;

    Player()
    {
        this->t_hero.loadFromFile(resourcePath() + "metka1.png");
        this->sprite.setTexture(this->t_hero);
        dx = dy = 0;
    }
    void setPosition(int x, int y)
    {
        this->rect = FloatRect(x, y, 16, 16);
    }

    bool update(float time)
    {
        rect.left += dx * time;
        Collision(0);

        rect.top += dy * time;
        Collision(1);

        if (Collision(0)) {
            return true;
        }

        sprite.setPosition(rect.left, rect.top);

        dx = 0;
        dy = 0;

        return false;
    }

private:
    bool Collision(int dir)
    {
        for (int i = rect.top / 23; i < (rect.top + rect.height) / 23; i++)
            for (int j = rect.left / 23; j < (rect.left + rect.width) / 23; j++) {
                if (Map_1[i][j] == '1' || Map_1[i][j] == '3') {
                    if ((dx > 0) && (dir == 0))
                        rect.left = j * 23 - rect.width;
                    if ((dx < 0) && (dir == 0))
                        rect.left = j * 23 + 23;
                    if ((dy > 0) && (dir == 1)) {
                        rect.top = i * 23 - rect.height;
                        dy = 0;
                    }
                    if ((dy < 0) && (dir == 1)) {
                        rect.top = i * 23 + 23;
                        dy = 0;
                    }
                    return 0;
                }
                if (Map_1[i][j] == '2') {
                    if ((rect.top < (i + 1) * 23) && (rect.top > (i - 1) * 23) && (rect.left < (j + 1) * 23) && (rect.left > (j - 1) * 23)) {
                        return true;
                    }
                }
            }
    }
};

class TextMenu {
private:
    Texture t_line;
    Sprite sprite;

public:
    TextMenu(string path, int x, int y)
    {
        this->t_line.loadFromFile(path);
        this->sprite.setTexture(this->t_line);
        this->sprite.setPosition(x, y);
    }

    void SetColor(Color color)
    {
        this->sprite.setColor(color);
    }

    bool Pos(int x, int y)
    {
        return (IntRect(x, y, 600, 100).contains(Mouse::getPosition(window)));
    }

    void Print()
    {
        window.draw(this->sprite);
    }
};

class Play {
private:
    std::string BaseMap[H];
    sf::RectangleShape rectangle;
    Player p;
    Event event;

    friend Clock;

    void initBase(std::string Map[H])
    {
        rectangle.setSize(sf::Vector2f(23, 23));
        for (int i = 0; i < H; i++)
            this->BaseMap[i] = Map[i];
    }

    void print_base()
    {
        for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++) {
                if (this->BaseMap[i][j] == '1')
                    rectangle.setFillColor(Color::Black);
                if (this->BaseMap[i][j] == '2')
                    rectangle.setFillColor(Color::White);
                if (this->BaseMap[i][j] == '3')
                    rectangle.setFillColor(Color::White);
                if (this->BaseMap[i][j] == '0')
                    continue;

                rectangle.setPosition(j * 23, i * 23);
                window.draw(rectangle);
            }
    }

public:
    bool show(Clock clock, std::string Map[H])
    {
        this->p.setPosition(230, 785);
        initBase(Map);

        bool key = true;
        while (window.isOpen() && key) {
            float time = clock.getElapsedTime().asMilliseconds();
            time = time * 2;
            clock.restart();

            while (window.pollEvent(this->event))
                if (this->event.type == Event::Closed) {
                    window.close();
                    return false;
                }

            window.clear(Color(255, 255, 255));

            if (Keyboard::isKeyPressed(Keyboard::Left))
                this->p.dx = -0.1;
            if (Keyboard::isKeyPressed(Keyboard::Right))
                this->p.dx = 0.1;
            if (Keyboard::isKeyPressed(Keyboard::Up))
                this->p.dy = -0.1;
            if (Keyboard::isKeyPressed(Keyboard::Down))
                this->p.dy = 0.1;

            print_base();

            window.draw(this->p.sprite);
            if (this->p.update(time)) {
                window.clear(Color(255, 255, 255));
                key = false;
                //fanfar.off();
                return true;
            }

            window.display();
        }
    }
};

int main()
{
    window.create(VideoMode(2800, 1403), "Labirinth - Ruslan Valeev");

    Audio game(resourcePath() + "game.ogg");
    Audio intro(resourcePath() + "intro.ogg");

    Clock clock;

    TextMenu menu1(resourcePath() + "newGame.png", 950, 400);
    TextMenu menu2(resourcePath() + "aboutProgramm.png", 950, 550);
    TextMenu menu3(resourcePath() + "exitProgramm.png", 950, 700);
    TextMenu about(resourcePath() + "about.png", 850, 450);
    TextMenu level1(resourcePath() + "level1.png", 150, 200);
    TextMenu level2(resourcePath() + "level2.png", 700, 200);
    TextMenu level3(resourcePath() + "level3.png", 1250, 200);
    TextMenu level4(resourcePath() + "level4.png", 1800, 200);

    bool Menu = true;
    int MenuNum1 = 0;
    int MenuNum2 = 0;

    intro.on();

    while (window.isOpen() && Menu) {
        bool Menu1 = true;

        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        menu1.SetColor(Color::White);
        menu2.SetColor(Color::White);
        menu3.SetColor(Color::White);

        MenuNum1 = 0;

        window.clear(Color(0, 0, 0));

        if (menu1.Pos(950, 400)) {
            menu1.SetColor(Color::Yellow);
            MenuNum1 = 1;
        }
        if (menu2.Pos(950, 550)) {
            menu2.SetColor(Color::Yellow);
            MenuNum1 = 2;
        }
        if (menu3.Pos(950, 700)) {
            menu3.SetColor(Color::Yellow);
            MenuNum1 = 3;
        }

        if (Mouse::isButtonPressed(Mouse::Left)) {
            switch (MenuNum1) {
            case 1: {
                while (Menu1) {
                    Event event;

                    while (window.pollEvent(event)) {
                        if (event.type == Event::Closed) {
                            window.close();
                            return 0;
                        }
                        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                            Menu1 = false;
                        }
                    }

                    level1.SetColor(Color::White);
                    level2.SetColor(Color::White);
                    level3.SetColor(Color::White);
                    level4.SetColor(Color::White);

                    MenuNum2 = 0;

                    window.clear(Color(0, 0, 0));

                    if (level1.Pos(150, 200)) {
                        level1.SetColor(Color::Yellow);
                        MenuNum2 = 1;
                    }
                    if (level2.Pos(700, 200)) {
                        level2.SetColor(Color::Yellow);
                        MenuNum2 = 2;
                    }
                    if (level3.Pos(1250, 200)) {
                        level3.SetColor(Color::Yellow);
                        MenuNum2 = 3;
                    }
                    if (level4.Pos(1800, 200)) {
                        level4.SetColor(Color::Yellow);
                        MenuNum2 = 4;
                    }

                    if (Mouse::isButtonPressed(Mouse::Left)) {
                        switch (MenuNum2) {
                        case 1: {
                            intro.off();
                            game.on();
                            Play maze;
                            if (!maze.show(clock, Map_1))
                                return 0;
                        } break;
                        case 2: {
                            intro.off();
                            game.on();
                            Play maze;
                            if (!maze.show(clock, Map_1))
                                return 0;
                        } break;
                        case 3: {
                            intro.off();
                            game.on();
                            Play maze;
                            if (!maze.show(clock, Map_1))
                                return 0;
                        } break;
                        case 4: {
                            intro.off();
                            game.on();
                            Play maze;
                            if (!maze.show(clock, Map_1))
                                return 0;
                        } break;
                        }
                    }
                    level1.Print();
                    level2.Print();
                    level3.Print();
                    level4.Print();
                    window.display();
                }
            } break;
            case 2: {
                about.Print();
                window.display();
                while (!Keyboard::isKeyPressed(Keyboard::Escape))
                    ;
            } break;
            case 3: {
                window.close();
                Menu = false;
            } break;
            }
        }

        menu1.Print();
        menu2.Print();
        menu3.Print();

        window.display();
    }

    return 0;
}