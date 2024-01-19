#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib> 
#include <time.h>
#include <random>

using namespace std;
using namespace sf;


// Estrutura para representar uma esfera menor
struct EsferaMenor {
	sf::CircleShape shape;
	sf::Vector2f velocidade;

	// Construtor que inicializa a esfera menor com uma determinada posi��o e raio
	EsferaMenor(float x, float y, float raio) {
		shape.setRadius(raio);
		shape.setFillColor(sf::Color::Green);
		shape.setPosition(x, y);
		velocidade = sf::Vector2f(0.f, 0.f);
	}
};

// Fun��o para calcular a for�a gravitacional entre duas esferas
sf::Vector2f calcularForcaGravitacional(const sf::Vector2f& posEsferaMenor, const sf::Vector2f& posEsferaMaior, float massaEsferaMenor, float massaEsferaMaior) {
	const float G = 0.1f; // Constante gravitacional (ajuste conforme necess�rio)

	sf::Vector2f direcao = posEsferaMaior - posEsferaMenor;
	float distanciaAoQuadrado = direcao.x * direcao.x + direcao.y * direcao.y;
	float distancia = std::sqrt(distanciaAoQuadrado);

	// Evita divis�o por zero
	if (distancia < 1.0f) {
		distancia = 1.0f;
		distanciaAoQuadrado = 1.0f;
	}

	float forca = (G * massaEsferaMenor * massaEsferaMaior) / distanciaAoQuadrado;

	// Normaliza a dire��o e multiplica pela for�a
	direcao /= distancia;
	sf::Vector2f forcaResultante = direcao * forca;

	return forcaResultante;
};

bool verificarColisao(const sf::CircleShape& esfera1, const sf::Sprite& esfera2) {
	sf::Vector2f direcaoColisao = esfera2.getPosition() - esfera1.getPosition();
	float distanciaColisao = std::sqrt(direcaoColisao.x * direcaoColisao.x + direcaoColisao.y * direcaoColisao.y);

	return distanciaColisao <= esfera1.getRadius() + esfera2.getGlobalBounds().width / 2.0f;
};

void corrigirPosicao(CircleShape& esferaMenor, const sf::Sprite& esferaMaior) {
	sf::Vector2f direcaoColisao = esferaMaior.getPosition() - esferaMenor.getPosition();
	float distanciaColisao = std::sqrt(direcaoColisao.x * direcaoColisao.x + direcaoColisao.y * direcaoColisao.y);
	float sobreposicao = (esferaMenor.getRadius() + esferaMaior.getGlobalBounds().width / 2.f) - distanciaColisao;

	sf::Vector2f normalColisao = direcaoColisao / distanciaColisao;
	sf::Vector2f correcao = normalColisao * sobreposicao;

	esferaMenor.move(correcao);
};

int main()
{
	//-------------------------------- INITIALIZE --------------------------------

	std::vector<EsferaMenor> esferasMenores;

	sf::Clock temporizador;
	float tempoPassado = 0.0f;
	float tempoPassadoScore = 0.0f;
	const float intervaloMensagem = 3.0f; // 5 segundos

	// Configura��es da simula��o
	float gravidade = 9.8f;   // Acelera��o devido � gravidade (m/s^2)
	float alturaInicial = 10.f; // Altura inicial da esfera
	float velocidadeInicial = 0.2f; // Velocidade inicial da esfera

	float tempo = 0.f;

	// Configura��es da queda
	float raioOrbita = 500.f; // Raio inicial da �rbita
	float angulo = 0.001f;       // �ngulo inicial
	float velocidadeQueda = 1.f; // Velocidade de queda
	float raioEntradaCampoGravitacional = 200.f; // Raio para entrar

	float massaEsferaMaior = 1000.f; // Massa da esfera maior
	float massaEsferaMenor = 1.f;   // Massa da esfera menor

	float fatorAmortecimento = 0.8f;



	//Declare a Font object
	Font font;

	//Load and check the availability of the font file
	if (!font.loadFromFile("Assets/fonts/Arial.ttf"))
	{
		cout << "can't load font" << endl;
		return 1;
	}

	Text text("Score: 0", font);
	text.setCharacterSize(40);
	text.setFillColor(Color::White);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "RPG Game", sf::Style::Default, settings);

	
	


	sf::Texture earthTexture;
	sf::Sprite earthSprite;

	earthTexture.loadFromFile("Assets/planet.png");
	earthSprite.setTexture(earthTexture);
	earthSprite.setScale(0.2, 0.2);

	// Obt�m as dimens�es do texto para calcular a origem
	sf::FloatRect bounds = earthSprite.getLocalBounds();
	earthSprite.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

	// Define a posi��o inicial no centro da janela
	earthSprite.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);


	sf::CircleShape satelite;
	satelite.setRadius(10);
	satelite.setPosition(window.getSize().x / 2.0f, 200);
	satelite.setFillColor(sf::Color::Color(255, 255, 255, 255));
	sf::Vector2f velocidade(0.f, 0.f); // Velocidade inicial

	velocidade.x = 0.5f;
	


	// ---------------------- BACKGROUND --------------------------------------

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	backgroundTexture.loadFromFile("Assets/background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(1, 1);
	

	//-------------------------------- INITIALIZE --------------------------------

	//main game loop
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
				// Cria uma nova esfera
				satelite.setPosition(event.mouseButton.x, event.mouseButton.y);
				satelite.setFillColor(sf::Color::Green);
				sf::Vector2f velocidade(0.f, 0.1f); // Velocidade inicial
			}
		}

		// ------------------------------ Program -----------------------------
		// Atualiza a posi��o da esfera menor na �rbita

		//raioOrbita -= 0.01f;

		sf::Vector2f forcaGravitacional = calcularForcaGravitacional(satelite.getPosition(), earthSprite.getPosition(), massaEsferaMenor, massaEsferaMaior);
		sf::Vector2f aceleracao = forcaGravitacional / massaEsferaMenor;

		velocidade += aceleracao;
	

		satelite.move(velocidade);


		sf::Vector2f direcaoColisao = earthSprite.getPosition() - satelite.getPosition();

		float distanciaColisao = std::sqrt(direcaoColisao.x * direcaoColisao.x + direcaoColisao.y * direcaoColisao.y);


		if (verificarColisao(satelite, earthSprite)) {
			// Normaliza a dire��o da colis�o
			direcaoColisao /= distanciaColisao;

			// Calcula a componente da velocidade na dire��o da colis�o
			float velocidadeColisao = velocidade.x * direcaoColisao.x + velocidade.y * direcaoColisao.y;

			// Inverte a componente da velocidade na dire��o da colis�o (quica para frente)
			velocidade.y = -velocidade.y + velocidade.y/70;
			velocidade.x = -velocidade.x + velocidade.x/70;
			
		}





		//-------------------------------- DRAW --------------------------------
		window.clear(sf::Color::Black);

		for (float y = 0; y < window.getSize().y; y += backgroundSprite.getGlobalBounds().height) {
			for (float x = 0; x < window.getSize().x; x += backgroundSprite.getGlobalBounds().width) {
				backgroundSprite.setPosition(x, y);
				window.draw(backgroundSprite);
			}
		}


		//window.draw(backgroundSprite);
		window.draw(satelite);
		window.draw(earthSprite);
		
		window.display();

		tempo += 0.1f;
		//-------------------------------- DRAW --------------------------------
	}

	return 0;
}