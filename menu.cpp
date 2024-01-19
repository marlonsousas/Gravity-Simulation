class TelaMenu {
public:
    TelaMenu(sf::RenderWindow& window) : window(window) {
        if (!font.loadFromFile("caminho/para/sua/fonte.ttf")) {
            std::cerr << "Erro ao carregar a fonte." << std::endl;
        }

        // Configuração do texto do botão
        textoBotao.setFont(font);
        textoBotao.setCharacterSize(24);
        textoBotao.setFillColor(sf::Color::White);
        textoBotao.setString("Iniciar Jogo");
        textoBotao.setPosition(300.f, 300.f);

        // Configuração da área do botão (retângulo)
        areaBotao.left = textoBotao.getPosition().x;
        areaBotao.top = textoBotao.getPosition().y;
        areaBotao.width = textoBotao.getGlobalBounds().width;
        areaBotao.height = textoBotao.getGlobalBounds().height;
    }

    void handleInput(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            // Verifica se o clique do mouse ocorreu sobre a área do botão
            if (areaBotao.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                // Iniciar o jogo (ou fazer a transição para a próxima tela)
                jogoIniciado = true;
            }
        }
    }

    void update(float deltaTime) {
        // Lógica de atualização da tela inicial (se necessário)
    }

    void render() {
        // Renderiza o botão
        window.draw(textoBotao);
    }

    bool isJogoIniciado() const {
        return jogoIniciado;
    }

private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text textoBotao;
    sf::FloatRect areaBotao;
    bool jogoIniciado = false;
};