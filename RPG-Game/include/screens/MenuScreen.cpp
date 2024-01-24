// solar_system_screen.cpp
#include "MenuScreen.hpp"
#include "earth_moon.hpp"
#include "solar_system.hpp"
#include <Windows.h>

class AnimatedSprite {
public:
	AnimatedSprite(sf::Texture& texture, int numFrames, float frameDuration)
		: texture(texture), numFrames(numFrames), frameDuration(frameDuration) {
		currentFrame = 0;
		elapsedDuration = 0.f;
		frameSize.x = texture.getSize().x / numFrames;
		frameSize.y = texture.getSize().y;
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
	}

	void update(float deltaTime) {
		elapsedDuration += deltaTime;
		if (elapsedDuration >= frameDuration) {
			// Atualiza o retângulo da textura para o próximo frame
			currentFrame = (currentFrame + 1) % numFrames;
			sprite.setTextureRect(sf::IntRect(currentFrame * frameSize.x, 0, frameSize.x, frameSize.y));
			elapsedDuration = 0.f;
		}
	}

	// Função para atualizar a posição e desenhar o asteroide
	void rotate(float deltaTime, float raio, static sf::Vector2u windowSize, float velocidadeAngular = 0.01f) {
		// Atualizar o ângulo para girar em círculo
		angulo += velocidadeAngular * deltaTime;

		// Calcular as novas coordenadas polares para a posição
		float x = windowSize.x / 2.f + raio * std::cos(angulo) * -1;
		float y = windowSize.y / 2.f + raio * std::sin(angulo) * -1;

		// Atualizar a posição do sprite usando setPosition
		sprite.setPosition(x, y);
	}

	void girar(float a) {
		sprite.rotate(a);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}

	void setPosition(float x, float y) {
		sprite.setPosition(x, y);
	}

	void move(float x, float y) {
		sprite.move(x, y);
	}
	void setScale(float x, float y) {
		sprite.setScale(x, y);
	}
	sf::FloatRect getLocalBounds() {
		return sprite.getLocalBounds();
	}
	void setOrigin(float x, float y) {
		sprite.setOrigin(x, y);
	}

private:
	sf::Texture& texture;
	sf::Vector2u frameSize;
	int numFrames;
	float frameDuration;
	int currentFrame;
	float elapsedDuration;
	sf::Sprite sprite;
	sf::Vector2f velocidade;
	float scale;
	float angulo; // Ângulo em coordenadas polares
};

void resetZoom(float& zoomLevel, sf::View& view) {
	zoomLevel = 1.0f;
	view.setSize(800.f, 600.f); // Tamanho padrão da janela
	view.setCenter(400.f, 300.f); // Centraliza na janela
}


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

	HWND hwnd = window.getSystemHandle();

	SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_CAPTION & ~WS_THICKFRAME);


	sf::Text opcao1;
	sf::Text opcao2;
	sf::Text opcao3;
	sf::Text opcao4;
	sf::Text opcao5;
	sf::Text exit;


	// ----------------------- MENU Text ----------------------------------------
	opcao1.setString("Earth and Moon");
	opcao1.setFont(font);
	opcao1.setCharacterSize(30);
	opcao1.setOrigin(opcao1.getLocalBounds().width / 2, opcao1.getLocalBounds().height / 2);
	opcao1.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 150);

	opcao2.setString("Solar System");
	opcao2.setFont(font);
	opcao2.setCharacterSize(30);
	opcao2.setOrigin(opcao2.getLocalBounds().width / 2, opcao2.getLocalBounds().height / 2);
	opcao2.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 75);

	opcao3.setString("Galaxy Simulation");
	opcao3.setFont(font);
	opcao3.setCharacterSize(30);
	opcao3.setOrigin(opcao3.getLocalBounds().width / 2, opcao3.getLocalBounds().height / 2);
	opcao3.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 0);

	opcao4.setString("Options");
	opcao4.setFont(font);
	opcao4.setCharacterSize(30);
	opcao4.setOrigin(opcao4.getLocalBounds().width / 2, opcao4.getLocalBounds().height / 2);
	opcao4.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 75);

	opcao5.setString("Extras");
	opcao5.setFont(font);
	opcao5.setCharacterSize(30);
	opcao5.setOrigin(opcao5.getLocalBounds().width / 2, opcao5.getLocalBounds().height / 2);
	opcao5.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 150);

	exit.setString("Exit");
	exit.setFont(font);
	exit.setCharacterSize(30);
	exit.setOrigin(exit.getLocalBounds().width / 2, exit.getLocalBounds().height / 2);
	exit.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 150 + 75);


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


	// ------------------------- KEY HELP -----------------------------

	sf::Texture keyRTexture;
	sf::Sprite keyRSprite;
	sf::Text keyRText("Reset Simulation", font, 20);
	keyRText.setFillColor(sf::Color::White);

	keyRTexture.loadFromFile("Assets/keyR.png");
	keyRSprite.setTexture(keyRTexture);
	keyRSprite.setScale(0.5, 0.5);
	keyRSprite.setPosition(50, window.getSize().y / 1.4);
	keyRText.setPosition(110, window.getSize().y / 1.38);


	sf::Texture keyEscTexture;
	sf::Sprite keyEscSprite;
	sf::Text keyEscText("Back to Menu", font, 20);
	keyEscText.setFillColor(sf::Color::White);

	keyEscTexture.loadFromFile("Assets/keyEsc.png");
	keyEscSprite.setTexture(keyEscTexture);
	keyEscSprite.setScale(0.5, 0.5);
	keyEscSprite.setPosition(50, window.getSize().y / 1.5);
	keyEscText.setPosition(110, window.getSize().y / 1.48);

	sf::Texture keyMouseTexture;
	sf::Sprite keyMouseSprite;
	sf::Text keyMouseText("Scroll: Zoom", font, 20);
	keyMouseText.setFillColor(sf::Color::White);

	keyMouseTexture.loadFromFile("Assets/keyMouse.png");
	keyMouseSprite.setTexture(keyMouseTexture);
	keyMouseSprite.setScale(0.5, 0.5);
	keyMouseSprite.setPosition(50, window.getSize().y / 1.3);
	keyMouseText.setPosition(110, window.getSize().y / 1.29);


	const int numPoints = 10; // N�mero de pontos na malha de distor��o
	sf::VertexArray distortion(sf::Quads, numPoints * 4);

	float angleIncrement = 2 * 3.14 / numPoints;
	float distortionAmount = 20.f; // Ajuste conforme necess�rio

	EarthMoonScreen EarthMoonScreen(window);
	SolarSystemScreen solarSystemScreen(window);

	sf::View mainView = window.getDefaultView();

	float zoomLevel = 1.0f;
	sf::View view = window.getView();

	sf::Texture blackHole;
	blackHole.loadFromFile("Assets/logo.png");
	AnimatedSprite blackHoleAnime(blackHole, 60, 0.04f);
	blackHoleAnime.setScale(2.5, 2.5);
	sf::FloatRect bounds = blackHoleAnime.getLocalBounds();
	blackHoleAnime.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	blackHoleAnime.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 380);


	sf::Clock clock;

	int count = 0;

	sf::Cursor arrowCursor;
	arrowCursor.loadFromSystem(sf::Cursor::Arrow);

	window.setMouseCursor(arrowCursor);

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

				if (opcao1.getGlobalBounds().contains(mousePos - sf::Vector2f(0, 40.f))) {
					opcao1.setCharacterSize(25);
					EarthMoonScreen.run();
					
				}
				else if (opcao2.getGlobalBounds().contains(mousePos - sf::Vector2f(0, 40.f))) {
					opcao2.setCharacterSize(25);
					solarSystemScreen.run();
					
				}
				else if (opcao3.getGlobalBounds().contains(mousePos - sf::Vector2f(0, 40.f))) {
					EarthMoonScreen.run();
					
				}

				else if (opcao4.getGlobalBounds().contains(mousePos - sf::Vector2f(0, 40.f))) {
					EarthMoonScreen.run();

				}
				else if (exit.getGlobalBounds().contains(mousePos - sf::Vector2f(0, 40.f))) {
					std::exit(-1);

				}
			}
		}

		mainView.setSize(window.getDefaultView().getSize() / 1.f);

		window.clear(sf::Color::Black);

		blackHoleAnime.update(clock.restart().asSeconds());

		view.setSize(window.getSize().x * zoomLevel, window.getSize().y * zoomLevel);


		window.clear();

		window.setView(mainView);

		for (float y = 0; y < window.getSize().y; y += backgroundSprite_.getGlobalBounds().height) {
			for (float x = 0; x < window.getSize().x; x += backgroundSprite_.getGlobalBounds().width) {
				backgroundSprite_.setPosition(x, y);
				window.draw(backgroundSprite_);
			}
		}


		blackHoleAnime.draw(window);

		window.draw(opcao1);
		window.draw(opcao2);
		window.draw(opcao3);
		window.draw(opcao4);
		window.draw(opcao5);

		window.draw(keyRSprite);
		window.draw(keyRText);
		window.draw(keyEscSprite);
		window.draw(keyEscText);
		window.draw(keyMouseSprite);
		window.draw(keyMouseText);
		
		window.draw(exit);


		window.display();

		count += 1;

		//-------------------------------- DRAW --------------------------------
	}

}