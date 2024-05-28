#include "Game.h"
#include <pplinterface.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include "Components.h"
#include "Entity.h"
#include "Vec2.h"

Game::Game(const std::string& config) {
  init(config);
};

void Game::init(const std::string& path) {
  // std::ifstream fin(path);
  // // PLAYER
  // fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >>
  //     m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >>
  //     m_playerConfig.V;
  // // ENEMY
  // fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >>
  //     m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.OT >> m_enemyConfig.VMIN >>
  //     m_enemyConfig.VMAX;

  // // BULLET
  // fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >>
  //     m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >>
  //     m_bulletConfig.V >> m_bulletConfig.L;

  // // TODO read in config file here
  m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
  m_window.setFramerateLimit(60);
  spawnPlayer();
}

void Game::run() {
  // add pause here
  // SOME SYSTEM SHOULD FUNCTION WHILE PAUSED
  while (m_running) {
    m_entities.update();
    // sEnemySpawner();
    sMovement();
    // sCollision();
    sUserInput();
    sRender();
    m_currentFrame++;
  }
  std::cout << "Game exited!";
}

void Game::setPaused(bool paused) {
  m_paused = paused;
}

void Game::spawnPlayer() {
  auto entity = m_entities.addEntity("player");
  entity->cTransform = std::make_shared<CTransform>(Vec2(400.0f, 400.0f), Vec2(3.0f, 3.0f), 0.0f);
  entity->cShape = std::make_shared<CShape>(32.0f, 3, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
  entity->cInput = std::make_shared<CInput>();
  m_player = entity;
}

void Game::spawnEnemy() {
  // TODO spawn enemies, enemies must span within the bounds of the window
  auto entity = m_entities.addEntity("enemy");
  float ex = rand() % m_window.getSize().x;
  float ey = rand() % m_window.getSize().y;
  entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(1.0f, 1.0f), 0.0f);
  entity->cShape = std::make_shared<CShape>(16.0f, 3, sf::Color(0, 0, 255), sf::Color(255, 255, 255), 4.0f);
  // record when the most recenet enemy was spawned
  m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e) {
  // spawn small enemies at the location of the enemy death
  //
}

// SYSTEMS
void Game::sUserInput() {
  // TODO
  // note that you should only be setting the player input component variable here
  // you shouldn't implement the player movement logic here
  // the movement system will read the variables you set in this function.
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
  }
}

void Game::sMovement() {
  const int SPEED = 5;

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
}

void Game::sEnemySpawner() {
  m_entities.getEntities();
}

void Game::sRender() {
  m_window.clear();
  for (auto& e : m_entities.getEntities()) {
    e->cShape->circle.setPosition(e->cTransform->pos);
    m_window.draw(e->cShape->circle);
  }
  m_window.display();
}