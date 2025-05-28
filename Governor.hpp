#pragma once
#include "Player.hpp"

namespace coup {

    class Governor : public Player // ירושה 
    {
    public:
        Governor(Game& game, const std::string& name);

        // override לפעולת tax, מקבל 3 מטבעות במקום 2
        void tax() override;

        // override לפעולת הביטול (undo) שמבטלת tax של שחקן אחר
        void undo(Player& target) override;
         std::string get_role() const  override;
    };

}
