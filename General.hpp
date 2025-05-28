#pragma once

#include "Player.hpp"

namespace coup {

    class General : public Player {
    public:
        General(Game& game, const std::string& name);

        // חוסם הפיכה - משלם 5 מטבעות ומונע את ההפיכה
        bool block_coup(Player& target_player, Player& attacker);

        // קריאה מהמשחק במקרה של arrest נגד הגנרל
        void on_arrested();

        void undo(Player& target) override;
        std::string get_role() const  override;
    };

}
