#include "Player.hpp"
#include "Game.hpp"
#include "Judge.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

namespace coup {

    Player::Player(Game& game, const string& name)
    {
        this->game=&game;
        this->name=name;
        game.add_player(this);
    }

    const std::string& Player::get_name() const
    {
        return name;
    }

    int Player::coins() const 
    {
        return coin_count;
    }

    void Player::set_last_target(Player* p)
    {
    last_target = p;
    }
    
    // לקיחת מטבע 1
    void Player::gather() 
    {
        ensure_turn(); 
        if (coin_count >= 10) 
        {
            throw runtime_error("Must perform coup with 10 or more coins");
        }
        if (blocked_until > game->get_turn_count()) 
        {
            throw runtime_error("Action 'gather' is blocked due to sanction");
        }
        coin_count += 1;
        game->end_turn();
    }

    // לקיחת 2 מטבעות
    void Player::tax()
    {
        ensure_turn();
        if (coin_count >= 10) 
        {
            throw runtime_error("Must perform coup with 10 or more coins");
        }
        if (blocked_until > game->get_turn_count()) 
        {
            throw runtime_error("Action 'tax' is blocked due to sanction");
        }
        coin_count += 2;
        game->end_turn();
    }

    // הדחה 
    void Player::coup(Player& target) 
    {
        ensure_turn();
        if (coin_count < 7) 
        {
            throw runtime_error("Not enough coins to perform a coup");
        }
        coin_count -= 7;
        target.eliminate();
        game->end_turn();

    }

    // היא בודקת אם התור הנוכחי במשחק שייך לשחקן הזה (this).
    void Player::ensure_turn() const 
    {
        if (game->current_turn() != this) 
        {
            throw runtime_error("Not your turn");
        }
    }

    // סימון שחקן שלא יוכל לקחת כלפי בסיס
    void Player::sanction(Player& target) 
    {
        ensure_turn();
        
        // בדיקה אם target הוא Judge באמצעות dynamic_cast
        Judge* judge_target = dynamic_cast<Judge*>(&target);
        
        if(judge_target != nullptr) // אם target הוא Judge
        {
            if (coin_count < 4) 
            {
                throw runtime_error("Not enough coins to sanction");
            }
            coin_count -= 4;
        }
        else // אם target אינו Judge
        {
            if (coin_count < 3) 
            {
                throw runtime_error("Not enough coins to sanction");
            }
            coin_count -= 3;
        }
        
        target.blocked_until = game->get_turn_count();
        last_target = &target;
        game->end_turn();
    }
        // העברת מטבע משחקן אחד לאחר 
    void Player::arrest(Player& target) 
    {
        ensure_turn();
        if (&target == last_target) 
        {
            throw runtime_error("Cannot arrest the same player twice in a row");
        }
        if (target.coin_count == 0) 
        {
            throw runtime_error("Target has no coins to arrest");
        }
        target.coin_count -= 1;
        coin_count += 1;
        last_target = &target;
        game->end_turn();
    }

    // תור נוסף
    void Player::bribe() 
    {
        ensure_turn();
        if (coin_count < 4) 
        {
            throw runtime_error("Not enough coins for bribe");
        }
        coin_count -= 4;
        // פעולה נוספת תתבצע ע"י השחקן מחוץ לפונקציה זו
    }
    
    // הורדת מטבעות בצורה בטוחה
    void Player::decrease_coins(int amount) 
    {
    if (coin_count < amount)
    {
        throw std::runtime_error("Not enough coins to remove");
    }
    coin_count -= amount;
    }

    bool Player::is_active() const 
    {
    return alive;
    }

    void Player::eliminate() 
    {
        alive = false;
    }
}
