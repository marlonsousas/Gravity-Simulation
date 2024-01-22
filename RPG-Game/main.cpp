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
                menu.handleEvent(event);  // Chama a função de tratamento de eventos do menu
            }
        }


        // Chama a função de desenho do menu
        menu.draw();

        // Restante do código do loop principal
        // ...

        // window.clear();
        // Desenhe outras coisas se necessário
        // window.display();
    }

    return 0;
}
