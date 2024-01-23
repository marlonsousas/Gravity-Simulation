// solar_system_screen.cpp
#include "MenuScreen.hpp"
#include "earth_moon.hpp"
#include "solar_system.hpp"

MenuScreen::MenuScreen(sf::RenderWindow& window) : window(window) {
	if (!font.loadFromFile("Assets/fonts/TechMono.ttf")) {
		// Tratamento de erro se a fonte não puder ser carregada
		std::exit(-1);
	}

	title.setString("Solar System Simulation");
	title.setFont(font);
	title.setCharacterSize(30);
	title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
	title.setPosition(window.getSize().x / 2, window.getSize().y / 2);

	// Inicialize outras variáveis ou funcionalidades conforme necessário
}

void MenuScreen::run() {
	//-------------------------------- INITIALIZE --------------------------------


	sf::Text opcao1;
	sf::Text opcao2;
	sf::Text opcao3;
	sf::Text opcao4;
	sf::Text exit;


	// ----------------------- MENU Text ----------------------------------------
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

	exit.setString("Exit");
	exit.setFont(font);
	exit.setCharacterSize(30);
	exit.setOrigin(exit.getLocalBounds().width / 2, exit.getLocalBounds().height / 2);
	exit.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 200);


	// ---------------------- BACKGROUND --------------------------------------

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	sf::Texture backgroundTexture_;
	sf::Sprite backgroundSprite_;

	backgroundTexture.loadFromFile("Assets/space.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(1, 1);
	backgroundSprite.setColor(sf::Color::White);

	backgroundTexture_.loadFromFile("Assets/background.png");
	backgroundSprite_.setTexture(backgroundTexture_);
	backgroundSprite_.setScale(1, 1);


	const int numPoints = 10; // N�mero de pontos na malha de distor��o
	sf::VertexArray distortion(sf::Quads, numPoints * 4);

	float angleIncrement = 2 * 3.14 / numPoints;
	float distortionAmount = 20.f; // Ajuste conforme necess�rio

	EarthMoonScreen EarthMoonScreen(window);
	SolarSystemScreen solarSystemScreen(window);

	sf::View mainView = window.getDefaultView();



	// Lógica principal da nova tela
	while (window.isOpen())
	{
		//-------------------------------- UPDATE --------------------------------
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

				if (opcao1.getGlobalBounds().contains(mousePos)) {
					opcao1.setCharacterSize(25);
					EarthMoonScreen.run();
					
				}
				else if (opcao2.getGlobalBounds().contains(mousePos)) {
					opcao2.setCharacterSize(25);
					solarSystemScreen.run();
					
				}
				else if (opcao3.getGlobalBounds().contains(mousePos)) {
					EarthMoonScreen.run();
					
				}

				else if (opcao4.getGlobalBounds().contains(mousePos)) {
					EarthMoonScreen.run();

				}
				else if (exit.getGlobalBounds().contains(mousePos)) {
					std::exit(-1);

				}
			}
		}


		window.clear(sf::Color::Black);

		for (float y = 0; y < window.getSize().y; y += backgroundSprite_.getGlobalBounds().height) {
			for (float x = 0; x < window.getSize().x; x += backgroundSprite_.getGlobalBounds().width) {
				backgroundSprite_.setPosition(x, y);
				window.draw(backgroundSprite_);
			}
		}

		for (float y = 0; y < window.getSize().y; y += backgroundSprite.getGlobalBounds().height) {
			for (float x = 0; x < window.getSize().x; x += backgroundSprite.getGlobalBounds().width) {
				backgroundSprite.setPosition(x, y);
				window.draw(backgroundSprite);
			}
		}




		//window.draw(backgroundSprite);

		window.clear();

		window.draw(opcao1);
		window.draw(opcao2);
		window.draw(opcao3);
		window.draw(opcao4);
		window.draw(exit);


		window.display();

		//-------------------------------- DRAW --------------------------------
	}

}