// nova_tela.hpp
#ifndef NOVA_TELA_HPP
#define NOVA_TELA_HPP

#include <SFML/Graphics.hpp>

class NovaTela {
public:
    NovaTela(sf::RenderWindow& window);

    void run();  // Função principal da nova tela

private:
    sf::RenderWindow& window;
    // Adicione membros ou funcionalidades necessários para a nova tela
};

#endif // NOVA_TELA_HPP
