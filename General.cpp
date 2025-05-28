//emuna002@gmail.com
#include "General.hpp"
#include "stdexcept"

using namespace std;

namespace coup 
{
    General::General(Game& game, const std::string& name)
    : Player(game, name)
    {}

     // מונע הפיכה שנעשתה נגד שחקן אחר או נגד עצמו
   bool General::block_coup(Player& target_player, Player& attacker)
    {
    if (coin_count < 5) {
        throw std::runtime_error("General doesn't have enough coins to block the coup");
    }

    coin_count -= 5;
    game->cancel_coup(&target_player);  // מבטל את ההפיכה בפועל

    return true; // הפיכה בוטלה
    }


    void General::on_arrested() {
        // מקבל את המטבע חזרה
        coin_count += 1;
    }

    void General::undo(Player& /*target*/) {
    throw std::runtime_error("This player type cannot undo actions");
}

    std::string General::get_role() const 
    {
        return "General";
    }
}