// solar_system_screen.hpp
#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include <SFML/Graphics.hpp>

class MenuScreen {
public:
    MenuScreen(sf::RenderWindow& window);

    void run();  // Adicione a declaração da função run aqui

private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text title;

    // Adicione outras variáveis ou funcionalidades conforme necessário
};

#endif // SOLAR_SYSTEM_SCREEN_HPP
