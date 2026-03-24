#pragma once
#include "GameMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Hand.h"
#include "GameDefs.h"
#include <vector>
#include <memory>

class Game {
private:
    GameMap map_;
    std::unique_ptr<Player> player_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    Hand hand_;
    int killCount_;
    bool running_;

    bool canMoveTo(int x, int y) const;
    void spawnEnemy(int x, int y);
    void applyDamageToEnemy(Enemy& enemy, int damage); // общий метод урона
    void attackEnemy(Enemy& enemy);
    void processEnemyTurns();
    void removeDeadEnemies();
    void checkWinCondition();
    void endGame();
    void grantRandomSpell();
    void tryGrantSpell();

public:
    Game();

    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);
    int  getKillCount() const noexcept;

    void processPlayerMove(Direction dir);
    void processCastSpell(int spellIndex, int targetX, int targetY);

    void printMap()    const;
    void printStatus() const;
    void printHand()   const;

    bool isRunning() const noexcept;

    // Открытые методы для заклинаний
    int  getPlayerX() const noexcept;
    int  getPlayerY() const noexcept;
    bool dealDamageAt(int x, int y, int damage);
    void dealDamageInArea(int x, int y, int damage);
};