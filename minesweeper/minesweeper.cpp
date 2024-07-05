#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
using namespace sf;
using namespace std;
const int MAX = 20;
const int DIRS = 8;
const int STALA = 10;
const int w = 32;
bool O[MAX][MAX];
int board[MAX][MAX];
int dx[DIRS] = {-1,-1,-1,0,0,1,1,1};
int dy[DIRS] = { -1,0,1,-1,1,-1,0,1 };
int display[MAX][MAX];
int uncovered = 0;

void finish_game(RenderWindow& window,string finish,Color c) {
    window.clear(Color::White);
    Font font;
    if (!font.loadFromFile("fonts/Arial.ttf")) {
        return;
    }
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString(finish);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(c);
    FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.width / 2, textRect.height / 2);
    gameOverText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    window.draw(gameOverText);
    window.display();
}

void bfs(int i, int j) {
    queue<pair<int,int>>q;
    for (int i = 1; i <= STALA; i++) {
        for (int j = 1; j <= STALA; j++) {
            O[i][j] = false;
        }
    }
    q.push(make_pair(i, j));
    O[i][j] = true;
    while (!q.empty()) {
        pair<int,int> u = q.front();
        int x = u.first;
        int y = u.second;
        uncovered++;
        display[x][y] = board[x][y];
        q.pop();
        for (int k = 0; k < DIRS; k++) {
            int tox = dx[k] + x;
            int toy = dy[k] + y;
            if (tox>=1 && tox<=STALA && toy>=1 && toy<=STALA && !O[tox][toy] && board[tox][toy] == 0) {
                O[tox][toy] = true;
                q.push(make_pair(tox, toy));
            }
        }
    }
}
int main()
{
    srand(time(0));
    RenderWindow window(VideoMode(400, 400), "Minesweeper!");
    int w = 32;
    Texture t;
    t.loadFromFile("images/tiles.jpg");
    Sprite s(t);
    for (int i = 1; i <= STALA; i++) {
        for (int j = 1; j <= STALA; j++) {
            board[i][j] = STALA;
            display[i][j] = STALA;
        }
    }

    for (int i = 1; i <= STALA; i++) {
        for (int j = 1; j <= STALA; j++) {
            int isBomb = rand() % 10;
            if (isBomb == 0) {
                board[i][j] = 9;
            }
        }
    }

    for (int i = 1; i <= STALA; i++) {
        for (int j = 1; j <= STALA; j++) {
            if (board[i][j] != 9) {
                board[i][j] = 0;
                for (int k = 0; k < DIRS; k++) {
                    int tox = i + dx[k];
                    int toy = j + dy[k];
                    if (tox >= 1 && tox <= STALA && toy >= 1 && toy <= STALA && board[tox][toy] == 9) {
                        board[i][j]++;
                    }
                }
            }
        }
    }
    bool suspended = false;
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
            if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left){
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    int i = mousePosition.x / w;
                    int j = mousePosition.y / w;
                    if (i >= 1 && i <= STALA && j >= 1 && j <= STALA && display[i][j] >= STALA) {
                        uncovered++;
                        display[i][j] = board[i][j];
                        if (board[i][j] == 0) {
                            bfs(i, j);
                            uncovered--;
                        }
                        if (display[i][j] == 9) { // bomba
                            sf::Time delay = sf::seconds(3);
                            finish_game(window,"GAME OVER",Color::Red);
                            suspended = true;
                            sleep(delay);
                            return 0;
                        }
                        else if (uncovered == STALA * STALA) {
                            sf::Time delay = sf::seconds(3);
                            finish_game(window, "CONGRATS",Color::Green);
                            suspended = true;
                            sleep(delay);
                            return 0;
                        }
                    }
                }
            }

            if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    int i = mousePosition.x / w;
                    int j = mousePosition.y / w;
                    if (i >= 1 && i <= STALA && j >= 1 && j <= STALA && display[i][j] >= STALA) {
                        display[i][j] = STALA+1;
                    }
                }
            }
        }
        window.clear(Color::White);

       

        for (int i = 1; i <= STALA; i++) {
            for (int j = 1; j <= STALA; j++) {
                s.setTextureRect(IntRect(display[i][j] * w, 0, w, w));
                s.setPosition(i * w, j * w);
                window.draw(s);
            }
        }

        window.display();
    }
    return 0;
}