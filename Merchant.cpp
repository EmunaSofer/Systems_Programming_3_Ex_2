//emuna002@gmail.com
#include "Merchant.hpp"
#include <stdexcept>

using namespace std;

namespace coup 
{
    Merchant::Merchant(Game& game, const std::string& name)
    :Player(game,name)
    {}

    void Merchant::on_arrested_by(Player& source)
    {
        if (coin_count < 2) {
            throw std::runtime_error("Merchant doesn't have enough coins for penalty");
        }

        coin_count -= 2;
        
    }

    void Merchant::on_turn_start() 
    {
        if (coin_count>=3)
        {
            coin_count +=1;
        } 
    }
    
    void Merchant::gather() 
    {
        on_turn_start();
        Player::gather();
    }

    void Merchant::tax() 
    {
        on_turn_start();
        Player::tax();
    }

    void Merchant::bribe() 
    {
        on_turn_start();
        Player::bribe();
    }

    void Merchant::arrest(Player& target) 
    {
        on_turn_start();
        Player::arrest(target);
    }

    void Merchant::sanction(Player& target) 
    {
        on_turn_start();
        Player::sanction(target);
    }

    void Merchant::coup(Player& target) 
    {
        on_turn_start();
        Player::coup(target);
    }

    void Merchant::undo(Player& target) {
    throw std::runtime_error("This player type cannot undo actions");
    }
    std::string Merchant::get_role() const  
    {
        return "Merchant";
    }
}