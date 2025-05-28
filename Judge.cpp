#include "Judge.hpp"
#include "stdexcept"

using namespace std;

namespace coup 
{
    Judge::Judge(Game& game, const std::string& name)
    :Player(game,name)
    {}


    void Judge::undo(Player& target)
    {
        if (target.coins() < 4) {
            throw std::runtime_error("Target does not have enough coins to undo bribe");
        }
    }

     std::string Judge::get_role() const 
    {
        return "Judge";
    }
}