#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include "Game.hpp"

namespace coup {

    class Game; // Forward declaration
    // מחלקה אבסטרקטית
    class Player {
    protected:
        std::string name; // שם השחקן
        Game* game; // איזה משחק אנחנו מדברים 
        int coin_count=0; // מספר המטבעות שח השחקן
        int blocked_until = 0;      // מספר התור שבו השחקן מפסיק להיות חסום (אם 0 או קטן מהתור הנוכחי - לא חסום)
        Player* last_target = nullptr; // מצביע לשחקן שהיה יעד בפעולה האחרונה (או nullptr אם אין)
        bool alive = true;
        // כדי למנוע פעולות לא חוקיות על תור
        void ensure_turn() const;

    public:
        Player(Game& game, const std::string& name); // בנאי
        virtual ~Player() = default; // מה שמציין לי שזה מחלקה אבסטרקטית

        const std::string& get_name() const; // החזרת שם השחקן ואי אפשר לשנות את השם
        int coins() const; // החזרת מספר המטבעות
         void set_last_target(Player* p);
         
        // פונקציות פעולה בסיסיות:
        virtual void gather();
        virtual void tax();
        virtual void bribe();
        virtual void arrest(Player& target);
        virtual void sanction(Player& target);
        virtual void coup(Player& target);

        // פעולות שחוסמות / מיוחדות – ימומשו לפי תפקיד
        virtual void undo(Player& target) = 0;
        virtual std::string get_role() const =0;
        void decrease_coins(int amount); // מוריד מטבעות באופן בטוח
        bool is_active() const;
        void eliminate();

    };

}
