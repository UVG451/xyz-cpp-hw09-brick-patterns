#pragma once
#include "IGameState.h"
#include "Menu.h"
#include "SFML/Graphics.hpp"

namespace ArkanoidGame
{
    // Pause menu state
    class PauseMenuState : public IGameState
    {
    private:
        Menu menu;
        sf::Font font;
        sf::RectangleShape background;
        sf::Text titleText;

    public:
        PauseMenuState() = default;
        ~PauseMenuState() = default;

        void onEnter() override;
        void onExit() override;
        void handleEvent(const sf::Event& event) override;
        void update(float timeDelta) override;
        void draw(sf::RenderWindow& window) override;
        const char* getStateName() const override { return "PauseMenuState"; }
    };
}