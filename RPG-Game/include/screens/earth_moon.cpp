// nova_tela.cpp
#include <SFML/Graphics.hpp>
#include "earth_moon.hpp"
#include "MenuScreen.hpp"
#include <Windows.h>
#include <thread>

using namespace std;
using namespace sf;


float massaValue = 5.972;
void setMassa(float newMassa) {
	massaValue = newMassa;
}
float getMassa() {
	return massaValue;
}

float volumeValue = 1.08321;
void setVolume(float newVolume) {
	volumeValue = newVolume;
}
float getVolume() {
	return volumeValue;
}

float raioValue = 6371;
void setRaio(float newRaio) {
	raioValue = newRaio;
}
float getRaio() {
	return raioValue;
}

string moverCaractereParaOFinal(char caractere, std::string& minhaString) {
	size_t pos = minhaString.find(caractere);

	// Se o caractere for encontrado, mova-o para o final
	while (pos != std::string::npos) {
		minhaString.erase(pos, 1);  // Remove o caractere da posição atual
		minhaString.push_back(caractere);  // Adiciona o caractere no final
		pos = minhaString.find(caractere);  // Encontra a próxima ocorrência do caractere
	}

	return minhaString;
}

bool isMouseOverSprite(const sf::Sprite& sprite, sf::Vector2f mousePosition) {
	// Calcula o centro do sprite
	sf::Vector2f spriteCenter = sprite.getPosition() + 0.5f * sf::Vector2f(sprite.getTexture()->getSize());

	// Calcula a distância entre o centro do sprite e a posição do mouse
	float distance = std::sqrt(std::pow(mousePosition.x - spriteCenter.x, 2) + std::pow(mousePosition.y - spriteCenter.y, 2));

	// Verifica se a distância é menor ou igual ao raio do sprite
	return distance <= 0.5f * std::min(sprite.getTexture()->getSize().x, sprite.getTexture()->getSize().y);
}


class NovaJanela {
public:
	NovaJanela() {
		novaJanela.close();
		textos.clear();
		
	}

	void run(float massaFloatiN = 1000.f) {
		novaJanela.create(sf::VideoMode(400, 700), "Nova Janela");
		novaJanela.setPosition(sf::Vector2i(10, 10));

		HWND hwnd = novaJanela.getSystemHandle();

		SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_CAPTION & ~WS_THICKFRAME);

		sf::Text textBack("Apply", font, 25);
		textBack.setPosition(170, 600);
		textBack.setFillColor(sf::Color::Black);

		sf::Text massaOri;
		massaOri.setString("Mass (10e24 Kg) : ");
		massaOri.setFont(font);
		massaOri.setCharacterSize(20);
		massaOri.setFillColor(sf::Color::Black);
		massaOri.setPosition(20, 400);

		sf::Text volumeOri;
		volumeOri.setString("Volume (10e12 Km³): ");
		volumeOri.setFont(font);
		volumeOri.setCharacterSize(20);
		volumeOri.setFillColor(sf::Color::Black);
		volumeOri.setPosition(20, 430);

		sf::Text radiusOri;
		radiusOri.setString("Radius (km): ");
		radiusOri.setFont(font);
		radiusOri.setCharacterSize(20);
		radiusOri.setFillColor(sf::Color::Black);
		radiusOri.setPosition(20, 460);

		sf::Text gravOri;
		gravOri.setString("Gravity (m/s): ");
		gravOri.setFont(font);
		gravOri.setCharacterSize(20);
		gravOri.setFillColor(sf::Color::Black);
		gravOri.setPosition(20, 490);

		sf::Text massaText;
		massaText.setString("Mass: ");
		massaText.setFont(font);
		massaText.setCharacterSize(20);
		massaText.setFillColor(sf::Color::Black);
		massaText.setPosition(230, 400);

		sf::Text volumeText;
		volumeText.setString("");
		volumeText.setFont(font);
		volumeText.setCharacterSize(20);
		volumeText.setFillColor(sf::Color::Black);
		volumeText.setPosition(230, 430);

		sf::Text radiusText;
		radiusText.setString("");
		radiusText.setFont(font);
		radiusText.setCharacterSize(20);
		radiusText.setFillColor(sf::Color::Black);
		radiusText.setPosition(230, 460);

		sf::Text gravText;
		gravText.setString("");
		gravText.setFont(font);
		gravText.setCharacterSize(20);
		gravText.setFillColor(sf::Color::Black);
		gravText.setPosition(230, 490);

		//terraInfo.drawText("Mass: 5.972 × 10^24 kg", 15, 20, 400);
				//terraInfo.drawText("Volume: 1.08321 × 10^12 km³", 15, 20, 430);
				//terraInfo.drawText("Radius: 6,371 km", 15, 20, 460);
				//terraInfo.drawText("Gravity: 9.81 m/s²", 15, 20, 490);


		std::string massa = std::to_string(getMassa());
		std::string volume = std::to_string(getVolume());
		std::string radius = std::to_string(getRaio());
		std::string gravity = "9.81";

		gravText.setString(gravity);

		int escolhido = 0;

		std::string entrada;

		sf::Text pipe("|", font, 24);
		pipe.setPosition(10, 10);

		sf::Clock clock;
		bool isVisible = true;
		float blinkInterval = 0.5f;  // Intervalo de piscar em segundos

		sf::Cursor lineCursor;
		lineCursor.loadFromSystem(sf::Cursor::Text);

		sf::Cursor arrowCursor;
		arrowCursor.loadFromSystem(sf::Cursor::Arrow);

		
		while (novaJanela.isOpen()) {
			sf::Event event;
			while (novaJanela.pollEvent(event)) {
				sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

				if (event.type == sf::Event::Closed) {
					
					novaJanela.close();
				}
				else if (event.type == sf::Event::TextEntered) {
					if (escolhido == 1) {
						if (event.text.unicode >= '0' && event.text.unicode <= '9' || event.text.unicode == '.') {
							// Apenas aceita números
							if (massa.length() <= 20) {
								massa.pop_back();
								massa += static_cast<char>(event.text.unicode);
								massa += "_";
							}
						}else if (!massa.empty() && massa.length() >= 8) {
							massa.pop_back();
						}
					}else if (escolhido == 2) {
						if (event.text.unicode >= '0' && event.text.unicode <= '9' || event.text.unicode == '.') {
							// Apenas aceita números
							if (volume.length() <= 20) {
								volume.pop_back();
								volume += static_cast<char>(event.text.unicode);
								volume += "_";
							}
						}
						else if (!volume.empty() && volume.length() >= 10) {
							volume.pop_back();
						}
					}
					else if (escolhido == 3) {
						if (event.text.unicode >= '0' && event.text.unicode <= '9' || event.text.unicode == '.') {
							// Apenas aceita números
							if (radius.length() <= 20) {
								radius.pop_back();
								radius += static_cast<char>(event.text.unicode);
								radius += "_";
							}
						}
						else if (!radius.empty() && radius.length() >= 10) {
							radius.pop_back();
						}
					}

					
				}

			}

			if (clock.getElapsedTime().asSeconds() >= blinkInterval) {
				isVisible = !isVisible;
				clock.restart();
			}

			// Atualiza a visibilidade do pipe
			


			if (event.type == sf::Event::MouseButtonPressed) {

				sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

				if (textBack.getGlobalBounds().contains(mousePos - sf::Vector2f(0, 40.f))) {

					massa.pop_back();
					volume.pop_back();
					radius.pop_back();
					setMassa(std::stof(massa));
					setVolume(std::stof(volume));
					setRaio(std::stof(radius));

					textos.clear();
					novaJanela.close();
				}
				else if(massaText.getGlobalBounds().contains(mousePos - sf::Vector2f(0, 40.f))) {
					escolhido = 1;
					massa = "_";
					novaJanela.setMouseCursor(lineCursor);
				}
				else if (volumeText.getGlobalBounds().contains(mousePos - sf::Vector2f(50, 40.f))) {
					escolhido = 2;
					volume = "_";
					novaJanela.setMouseCursor(lineCursor);
				}
				else if (radiusText.getGlobalBounds().contains(mousePos - sf::Vector2f(50, 40.f))) {
					escolhido = 3;
					radius = "_";
					novaJanela.setMouseCursor(lineCursor);
				}
				else {
					novaJanela.setMouseCursor(arrowCursor);
				}
				
				
				
			}


			novaJanela.clear(color);


			for (const auto& texto : textos) {


				novaJanela.draw(texto);
			}

			novaJanela.draw(textBack);
			novaJanela.draw(massaText);
			novaJanela.draw(volumeText);
			novaJanela.draw(radiusText);
			novaJanela.draw(gravText);
			novaJanela.draw(massaOri);
			novaJanela.draw(volumeOri);
			novaJanela.draw(radiusOri);
			novaJanela.draw(gravOri);
			// Desenhe os elementos da nova janela aqui, se necessário.
			novaJanela.display();

			massaText.setString(massa);
			volumeText.setString(volume);
			radiusText.setString(radius);

		}
	}

	void setBackgroundColor(sf::Color color_new) {
		color = sf::Color::White;
		novaJanela.clear(color);
	}

	sf::Vector2u getSize() {
		return novaJanela.getSize();
	}

	void drawText(string text_string, float fontSize, float positionx, float positiony) {
		font.loadFromFile("Assets/fonts/TechMono.ttf");
		sf::Text text(text_string, font, fontSize);
		text.setFillColor(sf::Color::Black);

		float limiteLargura = 350;  // 20 pixels de margem

		std::string linhaAtual;
		for (std::size_t i = 0; i < text_string.size(); ++i) {
			char character = text_string[i];
			linhaAtual += character;

			// Configura o texto para a linha atual e obtém a largura
			text.setString(linhaAtual);
			float larguraTexto = text.getLocalBounds().width;

			// Se a largura exceder o limite
			if (larguraTexto > limiteLargura) {
				// Desenha a linha atual e move para a próxima linha
				text.setPosition(positionx, positiony);
				textos.push_back(text);

				// Reinicia a linha atual
				linhaAtual.clear();
				positiony += text.getLocalBounds().height + 10;
			}
		}

		// Desenha a última linha, se houver
		if (!linhaAtual.empty()) {
			text.setString(linhaAtual);
			text.setPosition(positionx, positiony);
			textos.push_back(text);
		}
		
	}

private:
	std::vector<sf::Text> textos;
	sf::Font font;
	float massaFloat;
	float raioFloat;
	float volumeFloat;
	sf::Text text_o;
	sf::Color color = sf::Color::White;
	sf::RenderWindow novaJanela;
	std::thread thread;
};


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

	massaEsferaMaior = 1000 * massaEsferaMaior * getVolume() / 5.972;

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

EarthMoonScreen::EarthMoonScreen(sf::RenderWindow& window) : window(window) {
	// Inicialize membros ou realize configurações necessárias para a nova tela
}

void EarthMoonScreen::run() {

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

	// Obtém as dimensões do texto para calcular a origem
	sf::FloatRect bounds = earthSprite.getLocalBounds();
	earthSprite.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

	// Define a posição inicial no centro da janela
	earthSprite.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);


	sf::CircleShape satelite;
	satelite.setRadius(10);
	satelite.setPosition(window.getSize().x / 2.0f, 100);
	satelite.setFillColor(sf::Color::Color(255, 255, 255, 255));
	sf::Vector2f velocidade(0.f, 0.f); // Velocidade inicial

	velocidade.x = 0.5f;

	sf::View mainView = window.getDefaultView();
	float zoomFactor = 1.f;  // Fator de zoom inicial



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

	MenuScreen MenuScreen(window);

	NovaJanela terraInfo;
	NovaJanela luaInfo;


	sf::Cursor handCursor;
	handCursor.loadFromSystem(sf::Cursor::Hand);

	sf::Cursor arrowCursor;
	arrowCursor.loadFromSystem(sf::Cursor::Arrow);

	
	
	float massaEsferaMenor = 11.f;   // Massa da esfera menor

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
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
				zoomFactor = 1.f;
				mainView.setSize(window.getDefaultView().getSize() / zoomFactor);
				sf::ContextSettings settings;
				setMassa(5.972f);
				setVolume(1.08321f);
				setRaio(6371.f);
				sf::CircleShape satelite;
				satelite.setRadius(10);
				satelite.setPosition(window.getSize().x / 2.0f, 100);
				satelite.setFillColor(sf::Color::Color(255, 255, 255, 255));
				sf::Vector2f velocidade(0.f, 0.f); // Velocidade inicial
				settings.antialiasingLevel = 8;
				window.create(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Earth and Moon", sf::Style::Default, settings);
				HWND hwnd = window.getSystemHandle();

				SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_CAPTION & ~WS_THICKFRAME);
			}

			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				zoomFactor = 1.f;
				mainView.setSize(window.getDefaultView().getSize() / zoomFactor);
				MenuScreen.run();
			}

		}

		earthSprite.setScale(0.1 * getRaio() / 6371, 0.1 * getRaio() / 6371);

		float massaEsferaMaior = getMassa(); // Massa da esfera maior


		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		sf::Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition, mainView);

		

		if (event.type == sf::Event::MouseButtonPressed) {

			sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

			if (backText.getGlobalBounds().contains(mousePos)) {

				MenuScreen.run();
			}
			else if (earthSprite.getGlobalBounds().contains(worldMousePosition + sf::Vector2f(80, 80))) {
				

				terraInfo.setBackgroundColor(sf::Color::White);
				terraInfo.drawText("Earth", 40, 150, 80);
				terraInfo.drawText("Our home planet, Earth is unique in its ability to support life. "
					"It has a diverse range of environments, from deep oceans to high mountains, and is"
					"characterized by a rich atmosphere with essential elements for life.", 14, 20, 200);
				terraInfo.run();
			}

		}

		if (earthSprite.getGlobalBounds().contains(worldMousePosition)) {

			window.setMouseCursor(handCursor);
		}
		else {
			// Restaura o cursor padrão quando fora do sprite
			window.setMouseCursor(arrowCursor);
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


		float velocidadeR = std::sqrt(velocidade.x * velocidade.x + velocidade.y * velocidade.y) / 5;
		if (velocidadeR > 10.05) {
			earthSprite.rotate(0);
		}
		else{
			earthSprite.rotate(velocidadeR);
		}
		


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
		window.draw(satelite);
		window.draw(earthSprite);
		


		window.display();

		tempo += 0.1f;
		//-------------------------------- DRAW --------------------------------
	}

}
