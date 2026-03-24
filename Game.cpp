#include "Game.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>
#include "GameExceptions.h"
#include <sstream>

Game::Game()
    : map_(kDefaultMapSize, kDefaultMapSize),
      running_(true),
      hand_(kHandMaxSize),
      killCount_(0)
{
    int cx = map_.getWidth()  / 2;
    int cy = map_.getHeight() / 2;
    player_ = std::make_unique<Player>(cx, cy);

    spawnEnemy(1, 1);
    spawnEnemy(map_.getWidth() - 2, 1);
    spawnEnemy(1, map_.getHeight() - 2);
    spawnEnemy(map_.getWidth() - 2, map_.getHeight() - 2);

    grantRandomSpell(); // начальное случайное заклинание

    std::cout << "=== Game started! Map: "
              << map_.getWidth() << "x" << map_.getHeight()
              << " | Enemies: " << enemies_.size() << " ===\n";
    std::cout << "You start with 1 random spell!\n";
}

bool Game::isRunning() const noexcept { return running_; }
int  Game::getPlayerX() const noexcept { return player_->getX(); }
int  Game::getPlayerY() const noexcept { return player_->getY(); }

bool Game::canMoveTo(int x, int y) const {
    return map_.isInsideBounds(x, y) && map_.isTileWalkable(x, y);
}

void Game::spawnEnemy(int x, int y) {
    if (canMoveTo(x, y)) {
        enemies_.push_back(std::make_unique<Enemy>(x, y));
    }
}

// Единый метод урона — используется и при атаке, и при заклинаниях
void Game::applyDamageToEnemy(Enemy& enemy, int damage) {
    enemy.receiveDamage(damage);
    if (!enemy.isAlive()) {
        std::cout << "Enemy down! +" << kScorePerKill << " score\n";
        player_->addScore(kScorePerKill);
        killCount_++;
        tryGrantSpell();
    }
}

void Game::attackEnemy(Enemy& enemy) {
    int dmg = player_->getDamage();
    applyDamageToEnemy(enemy, dmg);
    std::cout << "You hit the enemy for " << dmg
              << " dmg! Enemy HP: " << enemy.getHp() << "\n";
}

bool Game::dealDamageAt(int x, int y, int damage) {
    for (auto& enemy : enemies_) {
        if (enemy->isAlive() && enemy->getX() == x && enemy->getY() == y) {
            applyDamageToEnemy(*enemy, damage);
            return true;
        }
    }
    return false;
}

void Game::dealDamageInArea(int x, int y, int damage) {
    for (int dy = 0; dy < 2; dy++) {
        for (int dx = 0; dx < 2; dx++) {
            dealDamageAt(x + dx, y + dy, damage);
        }
    }
}

void Game::grantRandomSpell() {
    if (hand_.isFull()) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);

    if (dist(gen) == 0) {
        hand_.addSpell(std::make_unique<DirectDamageSpell>());
        std::cout << "New spell acquired: Direct Damage!\n";
    } else {
        hand_.addSpell(std::make_unique<AreaDamageSpell>());
        std::cout << "New spell acquired: Area Damage!\n";
    }
}

void Game::tryGrantSpell() {
    if (killCount_ % kKillsPerSpell == 0) {
        grantRandomSpell();
    }
}

void Game::processEnemyTurns() {
    int px = player_->getX();
    int py = player_->getY();

    for (auto& enemy : enemies_) {
        if (!enemy->isAlive()) continue;

        auto step = enemy->nextStepToward(px, py);
        int nx = step.first;
        int ny = step.second;

        if (nx == px && ny == py) {
            player_->receiveDamage(enemy->getDamage());
            std::cout << "Enemy hits you for " << enemy->getDamage()
                      << " dmg! Your HP: " << player_->getHp() << "\n";
            if (!player_->isAlive()) {
                endGame();
                return;
            }
        } else if (canMoveTo(nx, ny)) {
            bool blocked = false;
            for (const auto& other : enemies_) {
                if (other.get() == enemy.get()) continue;
                if (other->isAlive() && other->getX() == nx && other->getY() == ny) {
                    blocked = true;
                    break;
                }
            }
            if (!blocked) enemy->moveTo(nx, ny);
        }
    }
}

void Game::removeDeadEnemies() {
    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
            [](const auto& e) { return !e->isAlive(); }),
        enemies_.end()
    );
}

void Game::checkWinCondition() {
    if (enemies_.empty() && player_->isAlive()) {
        std::cout << "\n=== YOU WIN! Score: " << player_->getScore() << " ===\n";
        running_ = false;
    }
}

void Game::endGame() {
    running_ = false;
    std::cout << "\n=== GAME OVER! Score: " << player_->getScore() << " ===\n";
}

void Game::processPlayerMove(Direction dir) {
    if (!running_) return;

    int px = player_->getX();
    int py = player_->getY();
    int nx = px, ny = py;

    switch (dir) {
        case Direction::Up:    ny--; break;
        case Direction::Down:  ny++; break;
        case Direction::Left:  nx--; break;
        case Direction::Right: nx++; break;
    }

    for (auto& enemy : enemies_) {
        if (enemy->isAlive() && enemy->getX() == nx && enemy->getY() == ny) {
            attackEnemy(*enemy);
            removeDeadEnemies();
            processEnemyTurns();
            removeDeadEnemies();
            checkWinCondition();
            return;
        }
    }

    if (canMoveTo(nx, ny)) {
        player_->moveTo(nx, ny);
        std::cout << "Moved to (" << nx << ", " << ny << ")\n";
    } else {
        std::cout << "Can't move there!\n";
    }

    processEnemyTurns();
    removeDeadEnemies();
    checkWinCondition();
}

void Game::processCastSpell(int spellIndex, int targetX, int targetY) {
    if (!running_) return;

    if (hand_.isEmpty()) {
        std::cout << "No spells in hand!\n";
        return;
    }

    Spell* spell = hand_.getSpell(spellIndex);
    if (!spell) {
        std::cout << "Invalid spell number!\n";
        return;
    }

    bool used = spell->cast(*this, targetX, targetY);
    if (used) {
        hand_.removeSpell(spellIndex);
        removeDeadEnemies();
        checkWinCondition();
        if (running_) {
            processEnemyTurns(); // заклинание тратит ход
            removeDeadEnemies();
            checkWinCondition();
        }
    }
}

void Game::printMap() const {
    std::cout << "\n";
    for (int y = 0; y < map_.getHeight(); ++y) {
        for (int x = 0; x < map_.getWidth(); ++x) {
            if (player_->getX() == x && player_->getY() == y) {
                std::cout << "P ";
                continue;
            }
            bool drawn = false;
            for (const auto& e : enemies_) {
                if (e->isAlive() && e->getX() == x && e->getY() == y) {
                    std::cout << "E ";
                    drawn = true;
                    break;
                }
            }
            if (!drawn) std::cout << ". ";
        }
        std::cout << "\n";
    }
    std::cout << "  P=You  E=Enemy  .=Empty\n";
}

void Game::printStatus() const {
    std::cout << "[HP: " << player_->getHp()
              << " | Score: " << player_->getScore()
              << " | Enemies: " << enemies_.size()
              << " | Spells: " << hand_.getSize() << "/" << hand_.getMaxSize() << "]\n";
}

void Game::printHand() const {
    std::cout << "--- Your Hand ("
              << hand_.getSize() << "/" << hand_.getMaxSize() << ") ---\n";
    hand_.print();
}

int Game::getKillCount() const noexcept { return killCount_; }

void Game::serialize(std::ostream& out) const {
    out << "PLAYER " << player_->getX() << " " << player_->getY()
        << " " << player_->getHp() << " " << player_->getScore() << "\n";
    out << "KILLCOUNT " << killCount_ << "\n";
    out << "ENEMIES " << enemies_.size() << "\n";
    for (const auto& e : enemies_)
        out << "ENEMY " << e->getX() << " " << e->getY()
            << " " << e->getHp() << "\n";
    auto ids = hand_.getSpellTypeIds();
    out << "HAND " << ids.size() << "\n";
    for (int id : ids)
        out << "SPELL " << id << "\n";
}

void Game::deserialize(std::istream& in) {
    std::string token;
    int px, py, php, pscore;

    if (!(in >> token) || token != "PLAYER")
        throw CorruptedSaveException("Game::deserialize", "expected PLAYER");
    if (!(in >> px >> py >> php >> pscore))
        throw CorruptedSaveException("Game::deserialize", "invalid player data");

    if (!(in >> token) || token != "KILLCOUNT")
        throw CorruptedSaveException("Game::deserialize", "expected KILLCOUNT");
    if (!(in >> killCount_))
        throw CorruptedSaveException("Game::deserialize", "invalid killcount");

    int enemyCount;
    if (!(in >> token) || token != "ENEMIES")
        throw CorruptedSaveException("Game::deserialize", "expected ENEMIES");
    if (!(in >> enemyCount))
        throw CorruptedSaveException("Game::deserialize", "invalid enemy count");

    player_   = std::make_unique<Player>(px, py, php, pscore);
    enemies_.clear();

    for (int i = 0; i < enemyCount; i++) {
        int ex, ey, ehp;
        if (!(in >> token) || token != "ENEMY")
            throw CorruptedSaveException("Game::deserialize", "expected ENEMY");
        if (!(in >> ex >> ey >> ehp))
            throw CorruptedSaveException("Game::deserialize", "invalid enemy data");
        enemies_.push_back(std::make_unique<Enemy>(ex, ey, ehp));
    }

    int handSize;
    if (!(in >> token) || token != "HAND")
        throw CorruptedSaveException("Game::deserialize", "expected HAND");
    if (!(in >> handSize))
        throw CorruptedSaveException("Game::deserialize", "invalid hand size");

    hand_ = Hand(kHandMaxSize);
    for (int i = 0; i < handSize; i++) {
        int typeId;
        if (!(in >> token) || token != "SPELL")
            throw CorruptedSaveException("Game::deserialize", "expected SPELL");
        if (!(in >> typeId))
            throw CorruptedSaveException("Game::deserialize", "invalid spell type");
        if (typeId == 0)
            hand_.addSpell(std::make_unique<DirectDamageSpell>());
        else if (typeId == 1)
            hand_.addSpell(std::make_unique<AreaDamageSpell>());
        else
            throw CorruptedSaveException("Game::deserialize",
                "unknown spell typeId: " + std::to_string(typeId));
    }

    running_ = true;
}