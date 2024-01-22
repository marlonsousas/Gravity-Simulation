// menu.hpp
#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "screens/earth_moon.hpp"

class Menu {
public:
    Menu(sf::RenderWindow& window);

    void draw();
    void handleEvent(sf::Event& event);

    void selecionarOpcao1();

private:
    sf::Font font;
    sf::Text opcao1;
    sf::Text opcao2;
    sf::Text opcao3;
    sf::Text opcao4;
    NovaTela novaTela;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::RenderWindow& window;  // Referência para o objeto RenderWindow da função main
};


#endif // MENU_HPP
