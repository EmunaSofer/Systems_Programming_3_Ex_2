#pragma once
#include "Player.hpp"

namespace coup {

    class Spy : public Player 
    {
    public:
        Spy(Game& game, const std::string& name);

        // פעולה מיוחדת של מרגל: רואה כמה מטבעות יש לשחקן אחר, ומונע ממנו לעשות arrest בתור הבא
        void investigate(Player& target);

        void undo(Player& target) override;
         std::string get_role() const override;
    };

}
