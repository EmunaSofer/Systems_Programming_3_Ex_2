#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <random>
#include <chrono>
#include "Game.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

using namespace coup;

class gui {
private:
    sf::RenderWindow window;
    sf::Font font;

    enum State { WELCOME, ADD_PLAYERS, GAME_PLAY, SELECT_TARGET };
    State currentState = WELCOME;

    std::string inputText;
    std::string errorMessage;
    std::string successMessage;
    std::string pendingAction;

    Game game;
    std::vector<std::unique_ptr<Player>> ownedPlayers;

    sf::Color backgroundColor = sf::Color::White;
    sf::Color textColor = sf::Color::Black;
    sf::Color buttonColor = sf::Color(173, 216, 230); // light blue
    sf::Color errorColor = sf::Color::Red;

    // דגל למניעת הוספת שחקן כפולה
    bool addButtonPressed = false;
    sf::Clock buttonClock; // טיימר למניעת לחיצות כפולות

public:
    gui() : window(sf::VideoMode(1100, 800), "Coup Game GUI") {
        font.loadFromFile("arial.ttf");
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered && currentState == ADD_PLAYERS) {
                if (event.text.unicode < 128 && isalnum(event.text.unicode)) {
                    inputText += static_cast<char>(event.text.unicode);
                } else if (event.text.unicode == 8 && !inputText.empty()) {
                    inputText.pop_back();
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                float x = event.mouseButton.x;
                float y = event.mouseButton.y;

                if (currentState == WELCOME && x >= 400 && x <= 700 && y >= 300 && y <= 380) {
                    currentState = ADD_PLAYERS;
                } else if (currentState == ADD_PLAYERS) {
                    // טיפול בכפתור ADD עם מניעת לחיצה כפולה
                    if (x >= 650 && x <= 770 && y >= 180 && y <= 230 && buttonClock.getElapsedTime().asMilliseconds() > 500) {
                        addPlayer();
                        buttonClock.restart();
                    } else if (x >= 500 && x <= 700 && y >= 600 && y <= 680) {
                        if (game.get_all_players().size() < 2) {
                            errorMessage = "At least 2 players required to start the game.";
                            successMessage.clear();
                        } else {
                            currentState = GAME_PLAY;
                        }
                    }
                } else if (currentState == GAME_PLAY) {
                    std::vector<std::string> actions = {"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup", "End Turn"};
                    for (size_t i = 0; i < actions.size(); ++i) {
                        float ax = 50 + (i % 3) * 200;
                        float ay = 400 + (i / 3) * 100;
                        if (x >= ax && x <= ax + 180 && y >= ay && y <= ay + 80) {
                            std::string action = actions[i];
                            Player* current = game.current_turn();
                            if (!current) return;

                            if (action == "Gather") {
                                try {
                                    current->gather();
                                    game.end_turn();
                                    successMessage = "Gather action completed.";
                                    errorMessage.clear();
                                } catch (const std::exception& e) {
                                    errorMessage = e.what();
                                    successMessage.clear();
                                }
                            } else if (action == "Tax") {
                                try {
                                    current->tax();
                                    game.end_turn();
                                    successMessage = "Tax action completed.";
                                    errorMessage.clear();
                                } catch (const std::exception& e) {
                                    errorMessage = e.what();
                                    successMessage.clear();
                                }
                            } else if (action == "End Turn") {
                                game.end_turn();
                                successMessage = "Turn ended.";
                                errorMessage.clear();
                            } else {
                                // בדיקה מקדימה אם יש מספיק מטבעות לכל הפעולות
                                bool canPerformAction = true;
                                std::string requiredCoinsMessage = "";
                                
                                if (action == "Coup" && current->coins() < 7) {
                                    canPerformAction = false;
                                    requiredCoinsMessage = "Not enough coins for Coup (requires 7 coins).";
                                } else if (action == "Bribe" && current->coins() < 1) {
                                    canPerformAction = false;
                                    requiredCoinsMessage = "Not enough coins for Bribe (requires 1 coin).";
                                } else if (action == "Arrest" && current->coins() < 2) {
                                    canPerformAction = false;
                                    requiredCoinsMessage = "Not enough coins for Arrest (requires 2 coins).";
                                } else if (action == "Sanction" && current->coins() < 3) {
                                    canPerformAction = false;
                                    requiredCoinsMessage = "Not enough coins for Sanction (requires 3 coins).";
                                }
                                
                                if (!canPerformAction) {
                                    errorMessage = requiredCoinsMessage;
                                    successMessage.clear();
                                } else {
                                    pendingAction = action;
                                    currentState = SELECT_TARGET;
                                    errorMessage.clear();
                                }
                            }
                            break;
                        }
                    }
                } else if (currentState == SELECT_TARGET) {
                    const auto& players = game.get_all_players();
                    Player* current = game.current_turn();
                    for (size_t i = 0; i < players.size(); ++i) {
                        if (players[i] != current) {
                            float px = 50 + (i % 3) * 200;
                            float py = 300 + (i / 3) * 100;
                            if (x >= px && x <= px + 180 && y >= py && y <= py + 70) {
                                try {
                                    if (pendingAction == "Bribe") current->bribe();
                                    else if (pendingAction == "Arrest") current->arrest(*players[i]);
                                    else if (pendingAction == "Sanction") current->sanction(*players[i]);
                                    else if (pendingAction == "Coup") current->coup(*players[i]);
                                    
                                    successMessage = pendingAction + " action completed.";
                                    errorMessage.clear();
                                } catch (const std::exception& e) {
                                    errorMessage = e.what();
                                    successMessage.clear();
                                }
                                
                                // בכל מקרה - גם אם הצליח וגם אם נכשל - נגמר התור וחוזרים למשחק
                                game.end_turn();
                                currentState = GAME_PLAY;
                                break;
                            }
                        }
                    }
                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                // כבר לא נחוץ - משתמשים בטיימר במקום
            }
        }
    }

    void addPlayer() {
        if (inputText.empty()) {
            errorMessage = "Please enter a player name.";
            successMessage.clear();
            return;
        }
        if (game.get_all_players().size() >= 6) {
            errorMessage = "Maximum of 6 players reached.";
            successMessage.clear();
            return;
        }

        // בדיקה אם השם כבר קיים
        const auto& existingPlayers = game.get_all_players();
        for (const auto* player : existingPlayers) {
            if (player->get_name() == inputText) {
                errorMessage = "Player name already exists.";
                successMessage.clear();
                return;
            }
        }

        std::vector<std::function<std::unique_ptr<Player>(Game&, const std::string&)>> roles = {
            [](Game& g, const std::string& n) { return std::make_unique<Governor>(g, n); },
            [](Game& g, const std::string& n) { return std::make_unique<Spy>(g, n); },
            [](Game& g, const std::string& n) { return std::make_unique<Baron>(g, n); },
            [](Game& g, const std::string& n) { return std::make_unique<General>(g, n); },
            [](Game& g, const std::string& n) { return std::make_unique<Judge>(g, n); },
            [](Game& g, const std::string& n) { return std::make_unique<Merchant>(g, n); },
        };

        // יצירת מחולל מספרים רנדומאלי חדש בכל פעם
        std::random_device rd;
        std::default_random_engine engine(rd());
        std::uniform_int_distribution<int> dist(0, roles.size() - 1);

        int index = dist(engine);

        try {
            ownedPlayers.push_back(roles[index](game, inputText));

            successMessage = "Player added: " + inputText + " (" + ownedPlayers.back()->get_role() + ")";
            errorMessage.clear();
            inputText.clear();
        } catch (const std::exception& e) {
            errorMessage = "Failed to add player: " + std::string(e.what());
            successMessage.clear();
        }
    }

    sf::Text createText(const std::string& str, unsigned int size, sf::Color color, float x, float y) {
        sf::Text text(str, font, size);
        text.setFillColor(color);
        text.setPosition(x, y);
        return text;
    }

    sf::RectangleShape createButton(float x, float y, float width, float height, sf::Color color = sf::Color(100, 100, 200)) {
        sf::RectangleShape rect(sf::Vector2f(width, height));
        rect.setFillColor(buttonColor);
        rect.setPosition(x, y);
        return rect;
    }

    void render() {
        window.clear(backgroundColor);

        if (currentState == WELCOME) {
            window.draw(createText("Welcome to Coup Game", 48, textColor, 300, 100));
            window.draw(createButton(400, 300, 300, 80));
            window.draw(createText("Start", 28, textColor, 520, 320));
        } else if (currentState == ADD_PLAYERS) {
            window.draw(createText("Add Players", 48, textColor, 250, 50));
            window.draw(createButton(400, 180, 200, 50));
            window.draw(createText("Player Name:", 24, textColor, 250, 190));
            window.draw(createText(inputText, 24, textColor, 410, 190));
            window.draw(createButton(650, 180, 120, 50));
            window.draw(createText("ADD", 24, textColor, 685, 195));

            const auto& players = game.get_all_players();
            window.draw(createText("Players (" + std::to_string(players.size()) + "/6):", 32, textColor, 50, 280));
            for (size_t i = 0; i < players.size(); ++i) {
                window.draw(createButton(50, 330 + i * 60, 500, 50));
                std::string playerInfo = players[i]->get_name() + " (" + players[i]->get_role() + ")";
                window.draw(createText(playerInfo, 20, textColor, 60, 345 + i * 60));
            }

            window.draw(createButton(500, 600, 200, 80));
            window.draw(createText("START GAME", 28, textColor, 530, 625));

            if (!errorMessage.empty()) {
                window.draw(createText(errorMessage, 20, errorColor, 50, 250));
            }
            if (!successMessage.empty()) {
                window.draw(createText(successMessage, 20, sf::Color::Green, 50, 250));
            }
        } else if (currentState == GAME_PLAY || currentState == SELECT_TARGET) {
            auto* current = game.current_turn();
            window.draw(createText("Coup Game", 48, textColor, 350, 20));
            if (current) {
                window.draw(createText("Current Turn: " + current->get_name() + " (" + current->get_role() + ")", 28, textColor, 50, 100));
                window.draw(createText("Coins: " + std::to_string(current->coins()), 24, textColor, 50, 140));
            }
            window.draw(createText("Coins in Treasury: " + std::to_string(game.coins_left()), 24, textColor, 50, 180));

            if (currentState == GAME_PLAY) {
                std::vector<std::string> actions = {"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup", "End Turn"};
                for (size_t i = 0; i < actions.size(); ++i) {
                    float x = 50 + (i % 3) * 200;
                    float y = 400 + (i / 3) * 100;
                    window.draw(createButton(x, y, 180, 80));
                    window.draw(createText(actions[i], 18, textColor, x + 10, y + 20));
                }
            } else if (currentState == SELECT_TARGET) {
                window.draw(createText("Select Target for " + pendingAction, 24, textColor, 50, 250));
                const auto& players = game.get_all_players();
                for (size_t i = 0, j = 0; i < players.size(); ++i) {
                    if (players[i] != current) {
                        float x = 50 + (j % 3) * 200;
                        float y = 300 + (j / 3) * 100;
                        window.draw(createButton(x, y, 180, 70));
                        window.draw(createText(players[i]->get_name(), 18, textColor, x + 10, y + 20));
                        ++j;
                    }
                }
            }

            if (!errorMessage.empty()) {
                window.draw(createText(errorMessage, 18, errorColor, 50, 580));
            }
            if (!successMessage.empty()) {
                window.draw(createText(successMessage, 18, sf::Color::Green, 50, 610));
            }
        }

        window.display();
    }
};

int main() {
    gui g;
    g.run();
    return 0;
}