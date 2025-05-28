#pragma once
#include "Player.hpp"

namespace coup {

    class Merchant : public Player {
    public:
        Merchant(Game& game, const std::string& name);

        // פונקציה שהשחקנים המשתמשים ב-arrest יוכלו לקרוא
        void on_arrested_by(Player& source);
        void on_turn_start();
        void gather() override;
        void tax() override;
        void bribe() override;
        void arrest(Player& target) override;
        void sanction(Player& target) override;
        void coup(Player& target) override;
        void undo(Player& target) override;
        std::string get_role() const  override;
    };

}
