// Tetris Game using SFML in C++ developed by Marius

// compile: g++ -g /home/marius/Desktop/C++/Tetris-Game-using-SFML-in-C-Plus-Plus/game.cpp -o /home/marius/Desktop/C++/Tetris-Game-using-SFML-in-C-Plus-Plus/game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

// to run: ./game

// libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
using namespace sf;

// global variables
const int M = 33;
const int N = 16;
const int tileSize = 18; // Size of each tile

// matrix with M rows and N cols representing the field of tiles
int field[M][N] = {0};

// coordinates
struct Point {
    int x, y;
} a[4], b[4];

// score counter
int score = 0;

// tiles shapes coordinates
int figures[7][4] = {
    1, 3, 5, 7, // shape: I
    2, 4, 5, 7, // Z shape
    3, 5, 4, 6, // S shape
    3, 5, 4, 7, // T shape
    2, 3, 5, 7, // L shape
    3, 5, 7, 6, // J shape
    2, 3, 4, 5  // O shape
};

// current tetromino color
int currentColor;

// work with field limit
bool check() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 1 || a[i].x >= N || a[i].y >= M) {
            return false; // the tiles are out of bounds
        }  
        else if (field[a[i].y][a[i].x]) {
            return false; // collides with an occupied cell in the field
        }
    }
    return true;
}

// initialize a new tetromino
void newTetromino() {
    int n = rand() % 7; // randomly select a shape
    currentColor = 1 + rand() % 7; // randomly select a color

     // starting position for the tetromino
    int startX = N / 2; // center horizontally
    int startY = 0;     // start at the top of the field

    for (int i = 0; i < 4; i++) {
        a[i].x = (figures[n][i] % 2) + startX;
        a[i].y = (figures[n][i] / 2) + startY;
    }
}

// check and clear full lines
void checkLines(sf::Text& text_score_counter) {
    int k = M - 1; // position to move the line down to

    for (int i = M - 1; i >= 0; i--) {
        bool fullLine = true;
        for (int j = 1; j < N; j++) {
            if (field[i][j] == 0) {
                fullLine = false;
                break;
            }
        }

        if (fullLine) {
            score += 10;
            text_score_counter.setString(std::to_string(score));
            // clear the line by moving the lines above it down
            for (int l = i; l > 0; l--) {
                for (int j = 1; j < N; j++) {
                    field[l][j] = field[l - 1][j];
                }
            }

            // clear the top line
            for (int j = 1; j < N; j++) {
                field[0][j] = 0;
            }

            i++; // check the same line again, as it might be full after the move
        }
    }
}


// gameover state
bool gameOver = false;

int main()
{
    // clock for timer
    Clock clock_timer;

    // seed random number generator
    srand(time(0));

    // window settings
    RenderWindow window(VideoMode(400, 600), "Tetris");

    // load font
    sf::Font font;
    if (!font.loadFromFile("./fonts/G_ari_bd.ttf")) {
        return -1;
    }

    // score text
    sf::Text text_score;
    text_score.setFont(font);
    text_score.setString("SCORE");
    text_score.setCharacterSize(20);
    text_score.setFillColor(sf::Color::White);
    text_score.setPosition(310, 220);

    // score counter text
    sf::Text text_score_counter;
    text_score_counter.setFont(font);
    text_score_counter.setString(std::to_string(score));
    text_score_counter.setCharacterSize(20);
    text_score_counter.setFillColor(sf::Color::Green);
    text_score_counter.setPosition(335, 260);

    // time text
    sf::Text text_time;
    text_time.setFont(font);
    text_time.setString("TIME");
    text_time.setCharacterSize(20);
    text_time.setFillColor(sf::Color::White);
    text_time.setPosition(320, 345);

    // time counter text
    sf::Text text_time_counter;
    text_time_counter.setFont(font);
    text_time_counter.setCharacterSize(20);
    text_time_counter.setFillColor(sf::Color::Blue);
    text_time_counter.setPosition(335, 380);

    // game Textures
    Texture t, bg, rr;
    t.loadFromFile("sprites/tiles.png");
    bg.loadFromFile("sprites/background.png");
    rr.loadFromFile("sprites/restart.png");

    // sprites
    Sprite s(t);
    Sprite background(bg);
    Sprite restartSprite(rr);

    // draw restart image
    restartSprite.setPosition(320, 10);
    
    // play background music
    sf::Music music;
    if (!music.openFromFile("sounds/soundtrack.mp3")) {
        // Error handling
        return -1;
    }

    music.setLoop(true); // Loop the music
    music.play();

    // movement variables
    int dx = 0; // horizontal
    bool rotate = false;
    float timer = 0, delay = 0.3;

    Clock clock;

    // initialize the first tetromino
    newTetromino();

    // keep program running
    while (window.isOpen()) {

        if (gameOver) {
            // Draw game over message
            window.clear();
            window.draw(background);

            sf::Text text_game_over;
            text_game_over.setFont(font);
            text_game_over.setString("GAME OVER!\nRESTARTING...");
            text_game_over.setCharacterSize(30);
            text_game_over.setFillColor(sf::Color::Red);
            text_game_over.setPosition(55, 230);
            window.draw(text_game_over);

            window.display();

            // restart the game after a delay
            sf::sleep(sf::seconds(2)); // wait 2 seconds before restarting

            // reset game state
            score = 0;
            text_score_counter.setString(std::to_string(score));
            gameOver = false;

            // clear the field
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    field[i][j] = 0;
                }
            }

            // initialize the first tetromino
            newTetromino();

            // reset the game clock
            clock_timer.restart();
        } 
        else 
        {
            // get current time
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;
            
            Event e;
            while (window.pollEvent(e)) {

                // close the application
                if (e.type == Event::Closed) {
                    window.close();
                }

                // handle mouse click event for restart button
                 if (e.type == sf::Event::MouseButtonPressed) {
                    if (e.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::FloatRect buttonBounds = restartSprite.getGlobalBounds();

                        if (buttonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
                            // restart the game
                            score = 0;
                            text_score_counter.setString(std::to_string(score));
                            gameOver = false;

                            // clear the field
                            for (int i = 0; i < M; i++) {
                                for (int j = 0; j < N; j++) {
                                    field[i][j] = 0;
                                }
                            }

                            // initialize the first tetromino
                            newTetromino();

                            // Reset the game clock
                            clock_timer.restart();
                        }
                    }
                 }

                // movement
                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::Up) 
                        rotate = true;
                    else if (e.key.code == Keyboard::Left) 
                        dx = -1;
                    else if (e.key.code == Keyboard::Right) 
                        dx = 1;
                }
            }



            // get elapsed time
            float clock_seconds = clock_timer.getElapsedTime().asSeconds();
            text_time_counter.setString(std::to_string(static_cast<int>(clock_seconds)) + "s");
            // move down the tile
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                delay = 0.05;
            }

            /// Move ///
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].x += dx;
            }

            // save current shape state and move shape horizontally
            if (!check()) {
                for (int i = 0; i < 4; i++) {
                    a[i] = b[i]; // revert to previous state if move is invalid
                }
            }

            /// Rotate ///
            if (rotate) {
                Point p = a[1]; // center of rotation
                for (int i = 0; i < 4; i++) {
                    int x = a[i].y - p.y;
                    int y = a[i].x - p.x;
                    a[i].x = p.x - x;
                    a[i].y = p.y + y;
                }

                if (!check()) {
                    for (int i = 0; i < 4; i++) {
                        a[i] = b[i];
                    }
                }
            }

            /// Tick ///
            if (timer > delay) {
                for (int i = 0; i < 4; i++) {
                    b[i] = a[i];
                    a[i].y += 1;
                }
                if (!check()) {
                    for (int i = 0; i < 4; i++) {
                        field[b[i].y][b[i].x] = currentColor; // place shape on the field
                        
                        // exceed limit tiles on top
                        if (b[i].y <= 0) {
                            gameOver = true; // Set game over
                            break;
                        }
                    }
                    if (!gameOver) {
                        newTetromino(); // initialize a new tetromino
                        checkLines(text_score_counter); // check and clear completed lines
                    }
                }
                timer = 0;
            }

            // reset direction and rotation
            dx = 0;
            rotate = false;
            delay = 0.3;

            /// Draw ///

            // game background color
            window.clear(Color::White);
            window.draw(background);

            // draw field
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (field[i][j] == 0) continue;
                    s.setTextureRect(IntRect(field[i][j] * tileSize, 0, tileSize, tileSize));
                    s.setPosition(j * tileSize, i * tileSize);
                    window.draw(s);
                }
            }
            
            // draw falling tetromino
            for (int i = 0; i < 4; i++) {
                s.setTextureRect(IntRect(currentColor * tileSize, 0, tileSize, tileSize));
                s.setPosition(a[i].x * tileSize, a[i].y * tileSize);
                window.draw(s);
            }
            
            window.draw(text_score);
            window.draw(text_score_counter);
            window.draw(text_time);
            window.draw(text_time_counter);
            window.draw(restartSprite);
            window.display();
        }
    }
    return 0;
}
