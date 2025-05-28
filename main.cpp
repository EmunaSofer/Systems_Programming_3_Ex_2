#include <iostream>
#include "Game.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

using namespace coup;

int main() {
    try {
        Game game;

        // יצירת שחקנים
        Governor g(game, "Alice");
        Spy s(game, "Bob");

        // הוספת שחקנים למשחק
        game.add_player(&g);
        game.add_player(&s);

        // עכשיו אפשר להשתמש במשחק
        std::cout << "It's " << game.turn() << "'s turn." << std::endl;

        // המשך הדמיית המשחק
        g.gather();
        std::cout << "Alice performed gather. Coins: " << g.coins() << std::endl;

        game.end_turn();
        std::cout << "It's " << game.turn() << "'s turn." << std::endl;

        s.tax();
        std::cout << "Bob performed tax. Coins: " << s.coins() << std::endl;

        // וכן הלאה...

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
