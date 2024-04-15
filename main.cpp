#include "Board.hpp"
#include "Screen.hpp"
#include "Other.hpp"
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include "Server.hpp"
#include <thread>

int main()
{
    srand(time(0));
    RenderWindow window(VideoMode(900, 1020), L"2048", Style::Close);
    Board board;
    board.new_step();
    board.new_step();
    Screen screen;
    screen.set_screen(board.tiles, board.score);
    bool go = false;
    bool gameover = false;
    //std::thread server_thread([&board]() { start_server(board); });;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed: window.close(); break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Up) {
                    make_move(board, 0);
                    board.new_step();
                }
                if (event.key.code == sf::Keyboard::Down) {
                    make_move(board, 1);
                    board.new_step();
                }
                if (event.key.code == sf::Keyboard::Left) {
                    make_move(board, 2);
                    board.new_step();
                }
                if (event.key.code == sf::Keyboard::Right) {
                    make_move(board, 3);
                    board.new_step();
                }
                if (event.key.code == sf::Keyboard::F2) {
                    start_server(board, window, screen);
                    /*gameover = true;*/
                    
                }
                break;
            }

        }
        window.clear(Color(187, 173, 160));
        screen.set_screen(board.tiles, board.score);
        screen.display(&window);
        if (gameover)
            break;
    }

    if (gameover) {
        screen.game_over(&window);
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }
    }

    return 0;
}