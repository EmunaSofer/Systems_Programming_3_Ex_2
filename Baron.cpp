//emuna002@gmail.com
#include "Baron.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Baron::Baron(Game& game, const std::string& name)
    : Player(game, name) // קריאה תקינה לבנאי של Player
    {}

    void Baron::invest()
    {
         ensure_turn();

        if (coin_count < 3) 
        {
            throw std::runtime_error("Not enough coins to invest");
        }

        coin_count -= 3;
        coin_count += 6;
        game->end_turn();
    }

    void Baron::on_sanction() 
     {
        // נקראת כשברון נבחר כיעד ל-sanction
        coin_count += 1;
    }

    void Baron::undo(Player& /*target*/) {
    throw std::runtime_error("This player type cannot undo actions");
}

   std::string Baron::get_role() const
    {
        return "Baron";
    }

}
