// menu.cpp
#include "menu.hpp"

Menu::Menu(sf::RenderWindow& window) : window(window), novaTela(window) {
    if (!font.loadFromFile("Assets/fonts/TechMono.ttf")) {
        std::cerr << "Erro ao carregar a fonte.\n";
        std::exit(-1);
    }

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Assets/background.png")) {
        std::cerr << "Erro ao carregar a textura do background.\n";
        std::exit(-1);
    }

    // Crie um sprite para o background
    sf::Sprite background(backgroundTexture);

    opcao1.setString("Earth and Moon");
    opcao1.setFont(font);
    opcao1.setCharacterSize(30);
    opcao1.setOrigin(opcao1.getLocalBounds().width / 2, opcao1.getLocalBounds().height / 2);
    opcao1.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 200);

    opcao2.setString("Solar System");
    opcao2.setFont(font);
    opcao2.setCharacterSize(30);
    opcao2.setOrigin(opcao2.getLocalBounds().width / 2, opcao2.getLocalBounds().height / 2);
    opcao2.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 100);

    opcao3.setString("Galaxy Simulation");
    opcao3.setFont(font);
    opcao3.setCharacterSize(30);
    opcao3.setOrigin(opcao3.getLocalBounds().width / 2, opcao3.getLocalBounds().height / 2);
    opcao3.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    opcao4.setString("Extras");
    opcao4.setFont(font);
    opcao4.setCharacterSize(30);
    opcao4.setOrigin(opcao4.getLocalBounds().width / 2, opcao4.getLocalBounds().height / 2);
    opcao4.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 100);

    
}

void Menu::selecionarOpcao1() {
    novaTela.run();
}

void Menu::draw() {
    window.clear();
    
    window.draw(background);
    window.draw(opcao1);
    window.draw(opcao2);
    window.draw(opcao3);
    window.draw(opcao4);
    
    window.display();
}

void Menu::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (opcao1.getGlobalBounds().contains(mousePos)) {
                opcao1.setCharacterSize(25);
                selecionarOpcao1();
            }
            else if (opcao2.getGlobalBounds().contains(mousePos)) {
                opcao1.setCharacterSize(25);
                selecionarOpcao1();
            }
            else if (opcao3.getGlobalBounds().contains(mousePos)) {
                opcao1.setCharacterSize(25);
                selecionarOpcao1();
            }
        }
    }
}
