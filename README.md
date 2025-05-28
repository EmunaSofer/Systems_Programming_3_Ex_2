# Systems_Programming_2_Ex_3
#  Coup - Strategy Board Game in C++

##  Project Overview

This project is an implementation of a simplified version of the board game **Coup**, developed in C++ as part of a Systems Programming course.

The game simulates a strategic battle between 2–6 players, each with a hidden role and unique abilities, aiming to eliminate opponents and be the last remaining player.

A full graphical interface (GUI) using **SFML** is provided for playing the game interactively, along with a comprehensive **unit testing suite** using **doctest**.

---

##  Features

- Support for 2–6 players with random role assignment
- Six different roles, each with special abilities
- Core actions (gather, tax, bribe, arrest, sanction, coup)
- Block and counteraction logic implemented per role
- Turn management, game state tracking, and victory detection
- Full GUI implementation with:
  - Opening screen
  - Add player screen
  - Turn-based gameplay screen
  - Blocking/target selection screen
- Exception handling for all invalid actions
- Memory-leak free (verified with `valgrind`)

---

##  Roles and Abilities

| Role       | Special Abilities                                                                 |
|------------|------------------------------------------------------------------------------------|
| Governor   | Gains 3 coins from tax (instead of 2). Can block others' tax.                      |
| Spy        | Views coin count of a player and blocks their next arrest. Free action.           |
| Baron      | Pays 3 coins to gain 6. Gains 1 coin when sanctioned.                             |
| General    | Pays 5 coins to block coup against any player. Recovers coin when arrested.       |
| Judge      | Cancels bribe actions (offender loses 4 coins). Sanctioner pays 1 extra coin.     |
| Merchant   | Starts turn with ≥3 coins? Gains 1 extra. If arrested, loses 2 coins to bank.     |

---

##  Actions

| Action   | Description                                                                              |
|----------|------------------------------------------------------------------------------------------|
| gather   | Gain 1 coin. Can be blocked with `sanction`.                                             |
| tax      | Gain 2 coins (or 3 for Governor). Can be blocked.                                       |
| bribe    | Pay 4 coins to perform an additional action. Can be blocked by Judge.                   |
| arrest   | Take 1 coin from another player. Cannot target the same player twice in a row.          |
| sanction | Pay 3 coins to prevent another from gathering/taxing until their next turn.             |
| coup     | Pay 7 coins to eliminate another player. Must be played if player has ≥10 coins.        |

---

##  Build Instructions

### Prerequisites
- C++17 compiler (e.g. g++)
- SFML library
- Doctest header (included or via package)

### Compile and Run

```bash
make Main         # Builds and runs the game GUI
make test         # Compiles and runs unit tests
make valgrind     # Checks for memory leaks
make clean        # Cleans build artifacts


###  Example Commands
./coup_game         # Run the game GUI
./test_runner       # Run all unit tests
valgrind ./test_runner  # Check for memory leaks

Author:
Emuna Sofer
emuna002@gmail.com

קצו
