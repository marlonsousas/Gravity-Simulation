// solar_system_screen.cpp
#include "solar_system.hpp"
#include "MenuScreen.hpp"

using namespace std;
using namespace sf;

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
	void rotate(float deltaTime, float raio, static sf::Vector2u windowSize, float velocidadeAngular=0.01f) {
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




struct Planetas {
	sf::Sprite sprite;
	sf::Vector2f velocidade;

	// Construtor que inicializa o sprite com uma determinada posição, textura e escala
	Planetas(float x, float y, const sf::Texture& texture, float escala = 1.0f) {
		sprite.setTexture(texture);
		sprite.setScale(escala, escala);
		sprite.setPosition(x, y);
		velocidade = sf::Vector2f(0.f, 0.f);
	}
};

class Asteroides {
public:
	// Construtor
	Asteroides(float raio, float angulo, sf::Texture& texture, float scale, static sf::Vector2u windowSize)
		: scale(scale), angulo(angulo) {
		// Calcular as coordenadas polares para a posição inicial em relação ao centro da tela
		float x = windowSize.x / 2.f + raio * std::cos(angulo);
		float y = windowSize.y / 2.f + raio * std::sin(angulo);

		// Configurar o sprite
		sprite.setTexture(texture);
		sprite.setScale(scale, scale);
		sprite.setPosition(x, y);
	}

	// Função para atualizar a posição e desenhar o asteroide
	void update(float deltaTime, float raio, static sf::Vector2u windowSize) {
		// Atualizar o ângulo para girar em círculo
		angulo += velocidadeAngular * deltaTime;

		// Calcular as novas coordenadas polares para a posição
		float x = windowSize.x / 2.f + raio * std::cos(angulo) * -1;
		float y = windowSize.y / 2.f + raio * std::sin(angulo) * -1;

		// Atualizar a posição do sprite usando setPosition
		sprite.setPosition(x, y);
	}

	void draw(sf::RenderWindow& window) const {
		window.draw(sprite);
	}

private:
	float scale;
	sf::Sprite sprite;
	float angulo; // Ângulo em coordenadas polares
	float velocidadeAngular = 0.01f; // Velocidade angular para controlar a rotação
};


SolarSystemScreen::SolarSystemScreen(sf::RenderWindow& window) : window(window) {
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

void SolarSystemScreen::run() {

	sf::View mainView = window.getDefaultView();
	float zoomFactor = 1.f;  // Fator de zoom inicial
	sf::Vector2f lastMousePosition;


	//-------------------------------- INITIALIZE --------------------------------
	
	sf::Text backText;
	backText.setString("Back");
	backText.setFont(font);
	backText.setCharacterSize(30);
	backText.setOrigin(backText.getLocalBounds().width / 2, backText.getLocalBounds().height / 2);
	backText.setPosition(100, 100);


	// ---------------------- BACKGROUND --------------------------------------

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	sf::Texture backgroundTexture_;
	sf::Sprite backgroundSprite_;

	sf::Texture sunTexture;
	sf::Sprite sunSprite;

	backgroundTexture.loadFromFile("Assets/space.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(1, 1);
	backgroundSprite.setColor(sf::Color::White);

	backgroundTexture_.loadFromFile("Assets/background.png");
	backgroundSprite_.setTexture(backgroundTexture_);
	backgroundSprite_.setScale(1, 1);


	// Define a posição inicial no centro da janela
	sunSprite.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);



	const int numPoints = 10; // N�mero de pontos na malha de distor��o
	sf::VertexArray distortion(sf::Quads, numPoints * 4);

	float angleIncrement = 2 * 3.14 / numPoints;
	float distortionAmount = 20.f; // Ajuste conforme necess�rio

	MenuScreen MenuScreen(window);

	// -------------------------------------- PLANETS ---------------------------------------

	std::vector<Planetas> planetas;
	std::vector<Asteroides> asteroides1;
	std::vector<Asteroides> asteroides2;
	std::vector<Asteroides> asteroides3;
	std::vector<Asteroides> asteroides4;
	std::vector<Asteroides> asteroides5;


	std::vector<Asteroides> asteroides6;
	std::vector<Asteroides> asteroides7;
	std::vector<Asteroides> asteroides8;
	std::vector<Asteroides> asteroides9;
	std::vector<Asteroides> asteroides10;
	std::vector<Asteroides> asteroides11;
	std::vector<Asteroides> asteroides12;
	std::vector<Asteroides> asteroides13;

	// Número de frames na imagem
	int numFrames = 60;
	// Duração de cada frame em segundos
	float frameDuration = 1.f;
	// Cria um sprite animado com os frames e a duração especificados



	// Asteroide
	sf::Texture asteroideTexture;
	sf::Sprite asteroideSprite;
	asteroideTexture.loadFromFile("Assets/asteroid.png");

	// Calcular o ângulo entre cada asteroide
	float anguloEntreAsteroides = 2 * static_cast<float>(3.1415f) / 50;


	for (int i = 0; i < 50; i++) {
		float anguloEntreAsteroides = 2 * static_cast<float>(3.1415f) / 50;
		float angulo = i * anguloEntreAsteroides;
		asteroides1.emplace_back(700.f, angulo, asteroideTexture, 0.2f, window.getSize());
		asteroides2.emplace_back(720.f, angulo - 111, asteroideTexture, 0.2f, window.getSize());
		asteroides3.emplace_back(740.f, angulo - 222, asteroideTexture, 0.2f, window.getSize());
		asteroides4.emplace_back(760.f, angulo - 233, asteroideTexture, 0.2f, window.getSize());
		asteroides5.emplace_back(780.f, angulo - 644, asteroideTexture, 0.2f, window.getSize());


	}

	for (int i = 0; i < 100; i++) {
		float anguloEntreAsteroides = 2 * static_cast<float>(3.1415f) / 100;
		float angulo = i * anguloEntreAsteroides;
		asteroides6.emplace_back(1600.f, angulo, asteroideTexture, 0.2f, window.getSize());
		asteroides7.emplace_back(1620.f, angulo - 111, asteroideTexture, 0.2f, window.getSize());
		asteroides8.emplace_back(1640.f, angulo - 222, asteroideTexture, 0.2f, window.getSize());
		asteroides9.emplace_back(1660.f, angulo - 233, asteroideTexture, 0.2f, window.getSize());
		asteroides10.emplace_back(1680.f, angulo - 644, asteroideTexture, 0.2f, window.getSize());
		asteroides11.emplace_back(1640.f, angulo - 222, asteroideTexture, 0.2f, window.getSize());
		asteroides12.emplace_back(1660.f, angulo - 233, asteroideTexture, 0.2f, window.getSize());
		asteroides13.emplace_back(1680.f, angulo - 644, asteroideTexture, 0.2f, window.getSize());

	}

	


	// Sol
	sf::Texture solTexture;
	solTexture.loadFromFile("Assets/sol2.png");
	AnimatedSprite sol(solTexture, 81, 2.0f);
	sol.setScale(3, 3);
	sf::FloatRect bounds = sol.getLocalBounds();
	sol.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	sol.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	// Mercúrio
	sf::Texture mercurioTexture;
	mercurioTexture.loadFromFile("Assets/mercurio_animate.png");
	AnimatedSprite mercurio(mercurioTexture, numFrames, frameDuration);
	mercurio.setScale(0.2, 0.2);
	mercurio.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	mercurio.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 300);

	// Venus
	sf::Texture venusTexture;
	venusTexture.loadFromFile("Assets/venus_animate.png");
	AnimatedSprite venus(venusTexture, numFrames, frameDuration);
	venus.setScale(0.27, 0.27);
	venus.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	venus.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 400);

	// Terra
	sf::Texture terraTexture;
	terraTexture.loadFromFile("Assets/terra_animate.png");
	AnimatedSprite terra(terraTexture, numFrames, frameDuration);
	terra.setScale(0.3, 0.3);
	terra.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	terra.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 500);

	// Marte
	sf::Texture marteTexture;
	marteTexture.loadFromFile("Assets/marte_animate.png");
	AnimatedSprite marte(marteTexture, numFrames, frameDuration);
	marte.setScale(0.20, 0.20);
	marte.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	marte.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 600);

	// Jupiter
	sf::Texture jupiterTexture;
	jupiterTexture.loadFromFile("Assets/jupiter_animate.png");
	AnimatedSprite jupiter(jupiterTexture, numFrames, frameDuration);
	jupiter.setScale(1, 1);
	jupiter.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	jupiter.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 900);

	// saturno
	sf::Texture saturnoTexture;
	saturnoTexture.loadFromFile("Assets/saturn_animate.png");
	AnimatedSprite saturno(saturnoTexture, 50, frameDuration);
	saturno.setScale(0.8, 0.8);
	saturno.girar(45);
	saturno.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	saturno.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 1200);


	// Urano
	sf::Texture uranoTexture;
	uranoTexture.loadFromFile("Assets/urano_animate.png");
	AnimatedSprite urano(uranoTexture, numFrames, frameDuration);
	urano.setScale(0.6, 0.6);
	urano.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	urano.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 1400);


	// Netuno
	sf::Texture neturnoTexture;
	neturnoTexture.loadFromFile("Assets/netuno_animate.png");
	AnimatedSprite netuno(neturnoTexture, numFrames, frameDuration);
	netuno.setScale(0.5, 0.5);
	netuno.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	netuno.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 1600);
	
	sf::Clock clock;
	sf::Time lastFrameTime = clock.getElapsedTime();


	int count = 0;

	// Lógica principal da nova tela
	while (window.isOpen())
	{
		//-------------------------------- UPDATE --------------------------------
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			else if (event.type == sf::Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.delta < 0) {
					zoomFactor *= 0.9f;
				}
				else if (event.mouseWheelScroll.delta > 0) {
					zoomFactor *= 1.1f;
				}

				mainView.setSize(window.getDefaultView().getSize() / zoomFactor);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				MenuScreen.run();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W) {
				// Armazena a posição inicial do clique do mouse
				mainView.move(0, -10);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
				// Armazena a posição inicial do clique do mouse
				mainView.move(0, 10);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
				// Armazena a posição inicial do clique do mouse
				mainView.move(-10, 0);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {
				// Armazena a posição inicial do clique do mouse
				mainView.move(10, 0);
			}


		}
		if (event.type == sf::Event::MouseButtonPressed) {

			sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

			if (backText.getGlobalBounds().contains(mousePos)) {

				MenuScreen.run();
				exit(-1);
			}

		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W) {
			window.close();
		}


		sol.update(clock.getElapsedTime().asSeconds());
		mercurio.update(clock.getElapsedTime().asSeconds());
		venus.update(clock.getElapsedTime().asSeconds());
		terra.update(clock.getElapsedTime().asSeconds());
		marte.update(clock.getElapsedTime().asSeconds());
		jupiter.update(clock.getElapsedTime().asSeconds());
		saturno.update(clock.getElapsedTime().asSeconds());
		urano.update(clock.getElapsedTime().asSeconds());
		netuno.update(clock.getElapsedTime().asSeconds());

		if (count >= 100) {
			count = 0;
			clock.restart().asSeconds();
		}

		mercurio.rotate(clock.getElapsedTime().asSeconds(), 300.f, window.getSize(), 49.0/1000.f);
		venus.rotate(clock.getElapsedTime().asSeconds(), 400.f, window.getSize(), 1.62/1000);
		terra.rotate(clock.getElapsedTime().asSeconds(), 500.f, window.getSize(), 1/1000.f);
		marte.rotate(clock.getElapsedTime().asSeconds(), 600.f, window.getSize(), 0.85/1000);

		jupiter.rotate(clock.getElapsedTime().asSeconds(), 900.f, window.getSize(), 0.44/1000);
		saturno.rotate(clock.getElapsedTime().asSeconds(), 1200.f, window.getSize(), 0.33/1000);
		urano.rotate(clock.getElapsedTime().asSeconds(), 1400.f, window.getSize(), 0.23/1000);
		netuno.rotate(clock.getElapsedTime().asSeconds(), 1600.f, window.getSize(), 0.18/1000);


		// Atualizar as posições dos asteroides e girar em círculo
		for (Asteroides& asteroide : asteroides1) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 700.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides2) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 720.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides3) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 740.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides4) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 760.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides5) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 780.f, window.getSize());
		}

		for (Asteroides& asteroide : asteroides6) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 1800.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides7) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 1820.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides8) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 1840.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides9) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 1860.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides10) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 1880.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides11) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 1900.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides12) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 1920.f, window.getSize());
		}
		for (Asteroides& asteroide : asteroides13) {
			asteroide.update(clock.getElapsedTime().asSeconds(), 1940.f, window.getSize());
		}


		window.clear();

		sf::View backgroundView = window.getDefaultView();
		window.setView(backgroundView);

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


		window.setView(mainView);

		//window.draw(backgroundSprite);

		sol.draw(window);
		mercurio.draw(window);
		venus.draw(window);
		terra.draw(window);
		marte.draw(window);
		jupiter.draw(window);
		saturno.draw(window);
		urano.draw(window);
		netuno.draw(window);

		window.draw(sunSprite);
		for (const Asteroides& asteroide : asteroides1) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides2) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides3) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides4) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides5) {
			asteroide.draw(window);
		}

		for (const Asteroides& asteroide : asteroides6) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides7) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides8) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides9) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides10) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides11) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides12) {
			asteroide.draw(window);
		}
		for (const Asteroides& asteroide : asteroides13) {
			asteroide.draw(window);
		}
		window.display();

		count += 1;
		//-------------------------------- DRAW --------------------------------
	}

}