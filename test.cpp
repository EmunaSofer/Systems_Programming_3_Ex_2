#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Game.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

using namespace coup;

TEST_CASE("Game setup and player order") {
    Game game;

    Governor p1(game, "Alice");
    Spy p2(game, "Bob");
    Baron p3(game, "Charlie");

    std::vector<std::string> names = game.players();
    CHECK(names.size() == 3);
    CHECK(names[0] == "Alice");
    CHECK(names[1] == "Bob");
    CHECK(names[2] == "Charlie");
}

TEST_CASE("Player turn order and gather") {
    Game game;
    Governor alice(game, "Alice");
    Spy bob(game, "Bob");

    CHECK(game.turn() == "Alice");
    alice.gather();  // +1 coin
    CHECK(alice.coins() == 1);

    CHECK(game.turn() == "Bob");
    bob.gather();  // +1 coin
    CHECK(bob.coins() == 1);
}

TEST_CASE("Enforcing turn-based system") {
    Game game;
    Governor alice(game, "Alice");
    Spy bob(game, "Bob");

    CHECK_THROWS(bob.gather());  // Not Bob's turn yet
}

TEST_CASE("Bribe gives another turn") {
    Game game;
    General alice(game, "Alice");
    Spy bob(game, "Bob");

    // Accumulate coins
    for (int i = 0; i < 4; ++i) {
        alice.gather();
        bob.gather();
    }

 
   // Alice should have 4, it's her turn
    CHECK(game.turn() == "Alice");
    alice.bribe();  // Should allow second action
    CHECK(game.turn() == "Alice");
    alice.gather();
    CHECK(alice.coins() == 1);  // 4 - 4 + 1
}




TEST_CASE("Maximum number of players") {
    Game game;
    Governor p1(game, "P1");
    Spy p2(game, "P2");
    Baron p3(game, "P3");
    General p4(game, "P4");
    Judge p5(game, "P5");
    Merchant p6(game, "P6");

    CHECK_THROWS(Governor(game, "Extra")); // 7th player
}
