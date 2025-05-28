#pragma once

#include "Player.hpp"

namespace coup {

    class Baron : public Player 
    {
    public:
        Baron(Game& game, const std::string& name);

        // הפעולה הייחודית לברון
        void invest();

        // הפעולה המיוחדת כשחוסמים אותו ב-saction
        void on_sanction(); // נניח שהמשחק קורא לה כשהברון נבחר כמטרה ל-saction

        
         void undo(Player& target) override;

         std::string get_role() const  override;
    };

}
