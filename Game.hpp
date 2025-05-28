//emuna002@gmail.com
#pragma once
#include <string>
#include <vector>

namespace coup {

    class Player; // הצהרה קדמית כדי לא לכלול את כל Player כאן

    class Game {
    private:
        std::vector<Player*> player_list; // רשימת שחקנים
        size_t current_index = 0; // מספר מטבעות
        int turn_counter = 0;
        bool coup_blocked = false; 
        int treasury_coins;


    public:
        Game();

        // מחזירה את שם השחקן שבתור
        std::string turn() const;

        // מחזירה את שמות השחקנים הפעילים
        std::vector<std::string> players() const;

        // מחזירה את שם המנצח (אם יש רק שחקן אחד)
        std::string winner() const;

        // מחזירה את השחקן שבתור (כדי לבדוק אם מותר לפעול)
        Player* current_turn() const;

        // התקדמות לתור הבא
        void end_turn();

        // מספר תור (משמש בין השאר לחישוב חסימות זמניות)
        int get_turn_count() const;

        // מוסיפה שחקן למשחק (נקרא מתוך Player)
        void add_player(Player* p);

        // מסירה שחקן מהמשחק (בהפיכה)
        void remove_player(Player* p);

        // לבטל הפיכה
        void cancel_coup(Player* target);

        std::vector<Player*> get_all_players() const;
        
        int coins_left() const;
    };

}
