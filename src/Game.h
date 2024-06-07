#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <string>
#include "Entity.h"
#include "EntityManager.h"

struct WindowConfig {
  int W, H, FR, FS;
};

struct FontConfig {
  std::string F;
  int S, R, G, B;
};

struct PlayerConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
  float S;
};

struct EnemyConfig {
  int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
  float SMIN, SMAX;
};

struct BulletConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
  float S;
};

class Game {
 public:
  Game(const std::string& config);
  void run();

 private:
  // Member Variables
  sf::RenderWindow m_window;
  EntityManager m_entities;
  sf::Font m_font;
  sf::Text m_text;
  WindowConfig m_windowConfig;
  FontConfig m_fontConfig;
  PlayerConfig m_playerConfig;
  EnemyConfig m_enemyConfig;
  BulletConfig m_bulletConfig;
  int m_score = 0;
  int m_currentFrame = 0;
  int m_lastEnemySpawnTime = 0;
  bool m_paused = false;
  bool m_running = true;

  std::shared_ptr<Entity> m_player;
  // Game State
  void init(const std::string& config);
  void setPaused(bool paused);
  // Systems
  void sMovement();
  void sUserInput();
  void sLifesapn();
  void sEnemySpawner();
  void sCollision();
  void sRender();
  // Spawns
  void spawnPlayer();
  void spawnEnemy();
  void spawnSmallEnemies(std::shared_ptr<Entity>& entity);
  void spawnBullet(std::shared_ptr<Entity>& entity, const Vec2& mousePos);
  void spawnSpecialWeapon(std::shared_ptr<Entity>& entity);

  // Misc
  void enforceBoundaries(std::shared_ptr<Entity>& e);
  void checkBulletCollisionWithAllTargetsByTag(std::shared_ptr<Entity>& b, std::string tag);
  void bulletCollisionWithEnemy(std::shared_ptr<Entity>& e, std::shared_ptr<Entity>& target);
  void destroyEntityOutOfScreen(std::shared_ptr<Entity>& e);
  void checkPlayerCollisionWithAllTargetsByTag(std::string target);
};
