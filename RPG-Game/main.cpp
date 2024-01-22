#include <SFML/Graphics.hpp>
#include "include/menu.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Earth and Moon", sf::Style::Default, settings);


    Menu menu(window);

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("Assets/background.png");

    // Crie um sprite para o background
    sf::Sprite background(backgroundTexture);



    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else {
                menu.handleEvent(event);  // Chama a fun��o de tratamento de eventos do menu
            }
        }


        // Chama a fun��o de desenho do menu
        menu.draw();

        // Restante do c�digo do loop principal
        // ...

        // window.clear();
        // Desenhe outras coisas se necess�rio
        // window.display();
    }

    return 0;
}
