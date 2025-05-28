//emuna002@gmail.com
#include "Spy.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

namespace coup {

    Spy::Spy(Game& game, const std::string& name)
        : Player(game, name) {}

    void Spy::investigate(Player& target)
    {
        // הפעולה לא נחשבת כתור ולכן לא נבדוק ensure_turn
        cout << "Spy " << this->name << " sees that " << target.get_name()
             << " has " << target.coins() << " coins." << endl;

        // מונע מהשחקן לבצע arrest בתור הבא
        target.set_last_target(&target);  // טריק: סימון כאילו נעשתה עליו arrest
    }
    void Spy::undo(Player& target) {
    throw std::runtime_error("This player type cannot undo actions");
    }

    std::string Spy::get_role() const 
    {
        return "Spy";
    }
}
