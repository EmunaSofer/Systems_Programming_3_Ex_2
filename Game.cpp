//emuna002@gmail.com
#include "Game.hpp"
#include "Player.hpp"
#include <stdexcept>
#include <algorithm>

using namespace std;

namespace coup {

    Game::Game() {}

    // הוספת שחקן
    void Game::add_player(Player* p) {
        if (player_list.size() >= 6) {
            throw runtime_error("Maximum 6 players allowed");
        }
        player_list.push_back(p);
    }

    // הסרת שחקן
    void Game::remove_player(Player* p) 
    {
        if (coup_blocked) {
            coup_blocked = false;  // מאפסים את הדגל אחרי שימוש אחד
            return; // מבטלים את ההפיכה
        }

        auto it = std::find(player_list.begin(), player_list.end(), p);
        if (it == player_list.end()) {
            throw std::runtime_error("Player not found");
        }
        p->eliminate();

        if (player_list.size() == 1) {
            return;
        }

        if (current_index >= player_list.size()) {
            current_index = 0;
        }
    }

    // הדפסת רשימת השחקנים
    std::string Game::turn() const {
        if (player_list.empty()) {
            throw runtime_error("No players in the game");
        }
        return player_list.at(current_index)->get_name();
    }


    std::vector<std::string> Game::players() const {
        std::vector<std::string> names;
        for (auto* p : player_list) {
            names.push_back(p->get_name());
        }
        return names;
    }

    // הדפסת מנצח
    std::string Game::winner() const {
        if (player_list.size() != 1) {
            throw runtime_error("The game is not finished yet");
        }
        return player_list.front()->get_name();
    }

    // מחזיר את השחקן שתורו
    Player* Game::current_turn() const {
        if (player_list.empty()) {
            throw runtime_error("No players");
        }
        return player_list.at(current_index);
    }

    // סיום תור
    void Game::end_turn() {
    if (player_list.empty()) {
        throw std::runtime_error("No players in the game");
    }

    size_t original_index = current_index;

    do {
        current_index = (current_index + 1) % player_list.size();
        if (current_index == original_index) {
            // סבב שלם ואין אף שחקן פעיל
            throw std::runtime_error("No active players remaining");
        }
    } while (!player_list[current_index]->is_active());

    turn_counter++;
}

    // תור של מי
    int Game::get_turn_count() const {
        return turn_counter;
    }

    void Game::cancel_coup(Player* target) {
        // פשוט מוסיף אותו חזרה לרשימת השחקנים אם הוא לא שם
        auto it = std::find(player_list.begin(), player_list.end(), target);
        if (it == player_list.end()) {
            player_list.insert(player_list.begin() + current_index, target);
        } else {
            throw runtime_error("Cannot cancel coup – player already active");
        }
    }

    std::vector<Player*> Game::get_all_players() const 
    {
      return player_list;
    }

    int Game::coins_left() const {
    return this->treasury_coins;
    }

}
