#include "Game.h"
#include <pplinterface.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <numbers>
#include <string>
#include "Components.h"
#include "Entity.h"
#include "Vec2.h"

Game::Game(const std::string& config) {
  init(config);
};

void Game::init(const std::string& path) {
  std::ifstream fin(path);
  if (!fin.is_open()) {
    std::cout << "Error opening file - GAME INIT\n";
  }
  std::string window;
  // WINDOW
  fin >> window >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FR >> m_windowConfig.FS;
  // FONT
  std::string font;
  fin >> font >> m_fontConfig.F >> m_fontConfig.S >> m_fontConfig.R >> m_fontConfig.G >> m_fontConfig.B;
  // PLAYER
  std::string player;
  fin >> player >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >>
      m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >>
      m_playerConfig.OT >> m_playerConfig.V;
  // ENEMY
  std::string enemy;
  fin >> enemy >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >>
      m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >>
      m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;

  // BULLET
  std::string bullet;
  fin >> bullet >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >>
      m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >>
      m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;

  // // TODO read in config file here
  if (m_windowConfig.FS == 1) {
    m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "Geometric War", sf::Style::Fullscreen);
  } else {
    m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "Geometric War");
  }
  m_window.setFramerateLimit(m_windowConfig.FR);
  m_font.loadFromFile(m_fontConfig.F);
  m_text.setFont(m_font);
  m_text.setCharacterSize(m_fontConfig.S);
  m_text.setString("0");
  spawnPlayer();
}

void Game::run() {
  // add pause here
  // SOME SYSTEM SHOULD FUNCTION WHILE PAUSED
  while (m_running) {
    m_entities.update();
    sEnemySpawner();
    sMovement();
    sCollision();
    sUserInput();
    sRender();
    m_currentFrame++;
  }
  std::cout << "Game exited!";
}

void Game::setPaused(bool paused) {
  m_paused = paused;
}

// ENTITIES CREATORS
void Game::spawnPlayer() {
  auto entity = m_entities.addEntity("player");
  entity->cTransform =
      std::make_shared<CTransform>(Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2), Vec2(5.0f, 5.0f), 0.0f);
  entity->cShape = std::make_shared<CShape>(m_playerConfig.SR,
                                            m_playerConfig.V,
                                            sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
                                            sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
                                            m_playerConfig.OT);
  entity->cInput = std::make_shared<CInput>();
  m_player = entity;
}

void Game::spawnEnemy() {
  auto entity = m_entities.addEntity("enemy");
  float ex = rand() % m_window.getSize().x;
  float ey = rand() % m_window.getSize().y;
  Vec2 enemyPos = Vec2(ex, ey);
  Vec2 dist = m_player->cTransform->pos - enemyPos;
  float magnitude = std::sqrt((dist.x * dist.x) + (dist.y * dist.y));
  int sideCount = m_enemyConfig.VMIN + (rand() % m_enemyConfig.VMAX);
  entity->cTransform = std::make_shared<CTransform>(enemyPos, Vec2(dist.x / magnitude, dist.y / magnitude), 0.0f);
  entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR,
                                            sideCount,
                                            sf::Color(0, 0, 0),
                                            sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
                                            m_enemyConfig.OT);
  m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity>& e) {
  const int VERTICIES = e->cShape->circle.getPointCount();
  float angleStep = 360 / VERTICIES;
  float angle = 0;
  float r = e->cShape->circle.getRadius();
  for (size_t i = 0; i < VERTICIES; i++) {
    angle += angleStep;
    float sin = std::sin(angle * std::numbers::pi / 180);
    float cos = std::cos(angle * std::numbers::pi / 180);
    auto entity = m_entities.addEntity("smallEnemy");
    entity->cTransform = std::make_shared<CTransform>(e->cTransform->pos, Vec2(cos, sin), 0);
    entity->cShape = std::make_shared<CShape>(e->cShape->circle.getRadius() / 2,
                                              e->cShape->circle.getPointCount(),
                                              e->cShape->circle.getFillColor(),
                                              e->cShape->circle.getOutlineColor(),
                                              e->cShape->circle.getOutlineThickness());
  }
}

void Game::spawnBullet(std::shared_ptr<Entity>& e, const Vec2& mousePos) {
  auto entity = m_entities.addEntity("bullet");
  auto playerPos = &m_player->cTransform->pos;
  entity->cTransform = std::make_shared<CTransform>(*playerPos, playerPos->normalized(mousePos), 0.0f);
  entity->cShape = std::make_shared<CShape>(m_bulletConfig.SR,
                                            m_bulletConfig.V,
                                            sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
                                            sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
                                            m_bulletConfig.OT);
}

// SYSTEMS

// INPUT
void Game::sUserInput() {
  sf::Event event;
  while (m_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      m_running = false;
    }
    if (event.type == sf::Event::KeyPressed) {
      switch (event.key.code) {
        case sf::Keyboard::W:
          m_player->cInput->up = true;
          break;
        case sf::Keyboard::S:
          m_player->cInput->down = true;
          break;
          break;
        case sf::Keyboard::D:
          m_player->cInput->right = true;
          break;
          break;
        case sf::Keyboard::A:
          m_player->cInput->left = true;
          break;
          break;
        default:
          break;
      }
    }
    if (event.type == sf::Event::KeyReleased) {
      switch (event.key.code) {
        case sf::Keyboard::W:
          m_player->cInput->up = false;
          break;
        case sf::Keyboard::S:
          m_player->cInput->down = false;
          break;
        case sf::Keyboard::D:
          m_player->cInput->right = false;
          break;
        case sf::Keyboard::A:
          m_player->cInput->left = false;
          break;
        default:
          break;
      }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
      Vec2 mousePos(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
      spawnBullet(m_player, mousePos);
    }
  }
}

// COLLISION
void Game::sCollision() {
  enforceBoundaries(m_player);
  checkPlayerCollisionWithAllTargetsByTag("enemy");
  checkPlayerCollisionWithAllTargetsByTag("smallEnemy");

  for (auto& b : m_entities.getEntities("bullet")) {
    checkBulletCollisionWithAllTargetsByTag(b, "enemy");
    checkBulletCollisionWithAllTargetsByTag(b, "smallEnemy");
  }
  for (auto& e : m_entities.getEntities()) {
    destroyEntityOutOfScreen(e);
  }
}

void Game::enforceBoundaries(std::shared_ptr<Entity>& e) {
  // screen borders

  if ((e->cTransform->pos.x - e->cShape->circle.getRadius()) < 0) {
    e->cTransform->pos.x = 0 + e->cShape->circle.getRadius();
  }
  if ((e->cTransform->pos.y - e->cShape->circle.getRadius()) < 0) {
    e->cTransform->pos.y = 0 + e->cShape->circle.getRadius();
  }
  if ((e->cTransform->pos.x + e->cShape->circle.getRadius()) > m_window.getSize().x) {
    e->cTransform->pos.x = m_window.getSize().x - e->cShape->circle.getRadius();
  }
  if ((e->cTransform->pos.y + e->cShape->circle.getRadius()) > m_window.getSize().y) {
    e->cTransform->pos.y = m_window.getSize().y - e->cShape->circle.getRadius();
  }
};

void Game::bulletCollisionWithEnemy(std::shared_ptr<Entity>& bullet, std::shared_ptr<Entity>& target) {
  // screen border
  auto dist = bullet->cTransform->pos.size(target->cTransform->pos);
  auto rSum = bullet->cShape->circle.getRadius() + target->cShape->circle.getRadius();
  if (dist < rSum) {
    if (target->tag() == "enemy") {
      spawnSmallEnemies(target);
      m_score += 10;
    }
    if (target->tag() == "smallEnemy") {
      m_score += 5;
    }
    target->destroy();
    bullet->destroy();
  }
}

void Game::checkBulletCollisionWithAllTargetsByTag(std::shared_ptr<Entity>& bullet, std::string tag) {
  for (auto& e : m_entities.getEntities(tag)) {
    bulletCollisionWithEnemy(bullet, e);
  }
}

void Game::destroyEntityOutOfScreen(std::shared_ptr<Entity>& e) {
  if ((e->cTransform->pos.x - e->cShape->circle.getRadius()) < 0) {
    e->destroy();
  }
  if ((e->cTransform->pos.y - e->cShape->circle.getRadius()) < 0) {
    e->destroy();
  }
  if ((e->cTransform->pos.x + e->cShape->circle.getRadius()) > m_window.getSize().x) {
    e->destroy();
  }
  if ((e->cTransform->pos.y + e->cShape->circle.getRadius()) > m_window.getSize().y) {
    e->destroy();
  }
}

void Game::checkPlayerCollisionWithAllTargetsByTag(std::string targetTag) {
  for (auto& target : m_entities.getEntities(targetTag)) {
    auto dist = m_player->cTransform->pos.size(target->cTransform->pos);
    auto rSum = m_player->cShape->circle.getRadius() + target->cShape->circle.getRadius();
    if (dist < rSum) {
      target->destroy();
      m_player->cTransform->pos.x = m_window.getSize().x / 2;
      m_player->cTransform->pos.y = m_window.getSize().y / 2;
    }
  }
}

// MOVEMENT
void Game::sMovement() {
  const int SPEED = 5;
  const int BULLET_SPEED = 10;
  m_player->cTransform->velocity = {0.0f, 0.0f};
  if (m_player->cInput->up) {
    m_player->cTransform->velocity.y = -SPEED;
  }
  if (m_player->cInput->down) {
    m_player->cTransform->velocity.y = SPEED;
  }
  if (m_player->cInput->right) {
    m_player->cTransform->velocity.x = SPEED;
  }
  if (m_player->cInput->left) {
    m_player->cTransform->velocity.x = -SPEED;
  }
  m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
  m_player->cTransform->pos.y += m_player->cTransform->velocity.y;

  for (auto& b : m_entities.getEntities("bullet")) {
    b->cTransform->pos.x += b->cTransform->velocity.x * BULLET_SPEED;
    b->cTransform->pos.y += b->cTransform->velocity.y * BULLET_SPEED;
  }
  for (auto& e : m_entities.getEntities("enemy")) {
    e->cTransform->pos.x += e->cTransform->velocity.x;
    e->cTransform->pos.y += e->cTransform->velocity.y;
  }
  for (auto& e : m_entities.getEntities("smallEnemy")) {
    e->cTransform->pos.x += e->cTransform->velocity.x;
    e->cTransform->pos.y += e->cTransform->velocity.y;
  }
}

// ENEMY SPAWNER
void Game::sEnemySpawner() {
  if (m_currentFrame - m_lastEnemySpawnTime >= 120) {
    spawnEnemy();
  }
}

// RENDER
void Game::sRender() {
  m_window.clear();
  std::string score_str = std::format("{}", m_score);
  for (auto& e : m_entities.getEntities()) {
    e->cShape->circle.setPosition(e->cTransform->pos);
    e->cShape->circle.rotate(2);
    m_window.draw(e->cShape->circle);
  }
  // std::cout << "ENTITY: " << m_entities.getEntities().size() << " | ";
  // std::cout << "ENEMY: " << m_entities.getEntities("enemy").size() << " | ";
  // std::cout << "S_ENEMY: " << m_entities.getEntities("smallEnemy").size() << " | ";
  // std::cout << "BULLET : " << m_entities.getEntities("bullet").size() << "\n";
  m_text.setString(score_str);
  m_window.draw(m_text);
  m_window.display();
}
