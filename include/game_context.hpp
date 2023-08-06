#pragma once

#include "controller.hpp"
#include "map.hpp"
#include "monster.hpp"
#include "pch.hpp"

class GameContext final : public Singlton<GameContext> {
   public:
    bool ShouldClose() const { return shouldClose_; }
    void Exit() { shouldClose_ = true; }

    SDL_Event& GetEvent() { return event_; }

    GameContext();

    void HandleEvent() {
        if (SDL_KEYDOWN == event_.type) {
            auto key = event_.key.keysym.scancode;
            if (SDL_SCANCODE_R == key) {
                newGame();
            }
        }
        controller->HandleEvent(event_);
    }

    std::unique_ptr<Map> gameMap;
    std::vector<std::unique_ptr<Monster>> monsters;
    std::unique_ptr<Controller> controller;

    void Update();

   private:
    bool shouldClose_ = false;
    SDL_Event event_;

    void newGame();
    void dealCollideWithMap(Monster& Monster);
};