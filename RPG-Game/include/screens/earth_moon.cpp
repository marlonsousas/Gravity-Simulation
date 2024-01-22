// nova_tela.cpp
#include <SFML/Graphics.hpp>
#include "earth_moon.hpp"
using namespace std;
using namespace sf;


// Estrutura para representar uma esfera menor
struct EsferaMenor {
	sf::CircleShape shape;
	sf::Vector2f velocidade;

	// Construtor que inicializa a esfera menor com uma determinada posição e raio
	EsferaMenor(float x, float y, float raio) {
		shape.setRadius(raio);
		shape.setFillColor(sf::Color::White);
		shape.setPosition(x, y);
		velocidade = sf::Vector2f(0.f, 0.f);
	}
};

// Função para calcular a força gravitacional entre duas esferas
sf::Vector2f calcularForcaGravitacional(const sf::Vector2f& posEsferaMenor, const sf::Vector2f& posEsferaMaior, float massaEsferaMenor, float massaEsferaMaior) {
	const float G = 0.1f; // Constante gravitacional (ajuste conforme necessário)

	sf::Vector2f direcao = posEsferaMaior - posEsferaMenor;
	float distanciaAoQuadrado = direcao.x * direcao.x + direcao.y * direcao.y;
	float distancia = std::sqrt(distanciaAoQuadrado);

	// Evita divisão por zero
	if (distancia < 1.0f) {
		distancia = 1.0f;
		distanciaAoQuadrado = 1.0f;
	}

	float forca = (G * massaEsferaMenor * massaEsferaMaior) / distanciaAoQuadrado;

	// Normaliza a direção e multiplica pela força
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

NovaTela::NovaTela(sf::RenderWindow& window) : window(window) {
	// Inicialize membros ou realize configurações necessárias para a nova tela
}

void NovaTela::run() {

	//-------------------------------- INITIALIZE --------------------------------
	sf::Font font2;
	if (!font2.loadFromFile("Assets/fonts/TechMono.ttf")) {
		std::exit(-1);
	}

	std::vector<EsferaMenor> esferasMenores;

	sf::Clock temporizador;
	float tempoPassado = 0.0f;
	float tempoPassadoScore = 0.0f;
	const float intervaloMensagem = 3.0f; // 5 segundos

	// Configurações da simulação
	float gravidade = 9.8f;   // Aceleração devido à gravidade (m/s^2)
	float alturaInicial = 10.f; // Altura inicial da esfera
	float velocidadeInicial = 0.2f; // Velocidade inicial da esfera

	float tempo = 0.f;

	// Configurações da queda
	float raioOrbita = 500.f; // Raio inicial da órbita
	float angulo = 0.001f;       // Ângulo inicial
	float velocidadeQueda = 1.f; // Velocidade de queda
	float raioEntradaCampoGravitacional = 200.f; // Raio para entrar

	float massaEsferaMaior = 1000.f; // Massa da esfera maior
	float massaEsferaMenor = 1.f;   // Massa da esfera menor

	float fatorAmortecimento = 0.8f;

	// Configuração da caixa
	sf::RectangleShape backBox(sf::Vector2f(100, 50)); // Largura x Altura
	backBox.setPosition(100, 100); // Posição da caixa
	backBox.setFillColor(sf::Color::Transparent); // Cor de preenchimento da caixa

	backBox.setOutlineColor(sf::Color::White);
	backBox.setOutlineThickness(5.0f);

	sf::Text backText;
	backText.setString("Back");
	backText.setFont(font2);
	backText.setCharacterSize(30);
	backText.setOrigin(backText.getLocalBounds().width / 2, backText.getLocalBounds().height / 2);
	backText.setPosition(100, 100);



	//Declare a Font object
	Font font;

	//Load and check the availability of the font file
	font.loadFromFile("Assets/fonts/Arial.ttf");

	sf::Texture earthTexture;
	sf::Sprite earthSprite;

	earthTexture.loadFromFile("Assets/planet.png");
	earthSprite.setTexture(earthTexture);
	earthSprite.setScale(0.1, 0.1);

	// Obtém as dimensões do texto para calcular a origem
	sf::FloatRect bounds = earthSprite.getLocalBounds();
	earthSprite.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

	// Define a posição inicial no centro da janela
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

			sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

			if (backText.getGlobalBounds().contains(mousePos)) {
				printf("Voltar");
				exit(-1);
			}

			if (event.mouseButton.button == sf::Mouse::Left) {
				// Cria uma nova esfera
				satelite.setPosition(event.mouseButton.x, event.mouseButton.y);
				satelite.setFillColor(sf::Color::White);
				sf::Vector2f velocidade(0.f, 0.1f); // Velocidade inicial
			}
		}

		// ------------------------------ Program -----------------------------
		// Atualiza a posição da esfera menor na órbita

		//raioOrbita -= 0.01f;

		sf::Vector2f forcaGravitacional = calcularForcaGravitacional(satelite.getPosition(), earthSprite.getPosition(), massaEsferaMenor, massaEsferaMaior);
		sf::Vector2f aceleracao = forcaGravitacional / massaEsferaMenor;

		velocidade += aceleracao;


		satelite.move(velocidade);


		sf::Vector2f direcaoColisao = earthSprite.getPosition() - satelite.getPosition();

		float distanciaColisao = std::sqrt(direcaoColisao.x * direcaoColisao.x + direcaoColisao.y * direcaoColisao.y);

		earthSprite.rotate(std::sqrt(velocidade.x * velocidade.x + velocidade.y * velocidade.y) * 1.37);


		if (verificarColisao(satelite, earthSprite)) {
			// Normaliza a direção da colisão
			direcaoColisao /= distanciaColisao;

			// Calcula a componente da velocidade na direção da colisão
			float velocidadeColisao = velocidade.x * direcaoColisao.x + velocidade.y * direcaoColisao.y;

			// Inverte a componente da velocidade na direção da colisão (quica para frente)
			velocidade.y = -velocidade.y + velocidade.y / 70;
			velocidade.x = -velocidade.x + velocidade.x / 70;

		}





		//-------------------------------- DRAW --------------------------------
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
		window.draw(satelite);
		window.draw(earthSprite);
		window.draw(backText);
		


		window.display();

		tempo += 0.1f;
		//-------------------------------- DRAW --------------------------------
	}

}
