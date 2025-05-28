//emuna002@gmail.com
#include "Governor.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Governor::Governor(Game& game, const std::string& name)
    : Player(game, name) // קריאה תקינה לבנאי של Player
    {}


    void Governor::tax()
    {
        ensure_turn();

        if (coin_count >= 10) {
            throw runtime_error("Must perform coup with 10 or more coins");
        }

        // Governor מקבל 3 מטבעות במקום 2
        coin_count += 3;
        game->end_turn();
    }

    void Governor::undo(Player& target) 
    {

        if (&target == this) {
            throw runtime_error("Cannot undo your own action");
        }

        // נניח שהפעולה שבוצעה על־ידי target הייתה tax, אז מחזירים את המטבעות
        target.decrease_coins(2);


    }

     std::string Governor::get_role() const  
    {
        return "Governor";
    }

}