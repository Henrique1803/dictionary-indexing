#ifndef TRIE_H
#define TRIE_H

#include <cstddef>
#include <string>
#include <iostream>

namespace structures {

template<typename T>
class Trie {
public:
    Trie(); // Construtor

    ~Trie(); // Destrutor

    void insert(const T& data, unsigned long posicao, unsigned long comprimento); // Insere a palavra na árvore, criando nós conforme o número de caracteres

    bool contains(const T& data) const; // Verifica se a árvore já possui uma palavra

    bool empty() const; // Verifica se a árvore está vazia

    std::size_t size() const; // Retorna o tamanho da árvore

    void words() const; // Printa na tela as palavras presentes na árvore

    void draw() const; // Desenha na tela a representação da árvore

    void printAllNodes() const; // Método para imprimir todos os nós

private:
    struct NoTrie {
        // Inicializa letra, posição e comprimento em 0, além dos ponteiros filhos para nulo
        NoTrie() : letra(0), posicao(0), comprimento(0) {
            for (int i = 0; i < 26; ++i) {
                filhos[i] = nullptr;
            }
        }

        // Inicializa com a letra passada como parâmetro e seta posição e comprimento em zero, além dos ponteiros filhos para nulo
        explicit NoTrie(char letra_) : letra(letra_), posicao(0), comprimento(0) {
            for (int i = 0; i < 26; ++i) {
                filhos[i] = nullptr;
            }
        }

        char           letra;        
        NoTrie        *filhos[26]; 
        unsigned long  posicao;
        unsigned long  comprimento; 

        bool contains(const T& data, std::size_t index) const {
            // Se isso for verdade, significa que percorremos todos os caracteres
            // e estamos no nó que deveria marcar o final da palavra 
            if (index == data.size()) {
                return comprimento > 0;  // Se comprimento > 0 se este nó realmente representa o final da palavra
            }
            char ch = data[index]; // Próximo caractere da palavra
            int child_index = ch - 'a'; // Cálculo para saber o índice do filho correspondente à letra
            if (filhos[child_index] == nullptr) { // Se não tiver filho com essa letra, a palavra não está presente na árvore
                return false;
            }
            // Chamada recursiva pra verificar se o filho possui o pŕoximo caractere da palavra
            return filhos[child_index]->contains(data, index + 1);
        }

        void printAllNodes(NoTrie* node, const std::string& prefix) const {
            if (node != nullptr) {
                if (node->letra != 0) { // Caso não seja o nó raiz
                    // Printa as informações do nó
                    std::cout << "Letra: " << node->letra
                            << ", Posição: " << node->posicao
                            << ", Comprimento: " << node->comprimento
                            << std::endl;
                }
                for (int i = 0; i < 26; ++i) {
                    if (node->filhos[i] != nullptr) { // Caso tenha um filho nesse caractere
                        printAllNodes(node->filhos[i], prefix + node->filhos[i]->letra); // Chama recursivamente a função para o filho
                    }
                }
            }
        }
    };

    void words(NoTrie* node, std::string prefix) const; // Método utilizado para printar as palavras presentes na árvore

    void destroy(NoTrie* node); // Método utilizado para desalocar os nós da árvore

    void draw(NoTrie* node, std::string prefix, bool isTail) const; // Método utilizado para desenhar a árvore no terminal

    NoTrie* root;
    std::size_t size_;
};

}  // namespace structures

//-------------------------------------

template<typename T>
structures::Trie<T>::Trie() { // Construtor
    root = new NoTrie(); // A raiz é sempre inicializada no construtor, pois não representa um caractere
    size_ = 1; // Tamanho começa em 1
}

template<typename T>
structures::Trie<T>::~Trie() {
    destroy(root); // Destrói e desaloca os nós recursivamente a partir da raiz
}

template<typename T>
void structures::Trie<T>::insert(const T& data, unsigned long posicao, unsigned long comprimento) {
    NoTrie* current = root; // Começa pela raiz
    for (std::size_t i = 0; i < data.size(); ++i) { // Iteração sobre os caracteres da palavra
        char ch = data[i]; // Caractere dessa iteração
        int index = ch - 'a'; // Calcula a posição do caractere no vetor
        if (current->filhos[index] == nullptr) { // Se não tiver filho com esse caractere
            current->filhos[index] = new NoTrie(ch); // Cria o nó filho que representa o caractere
            size_++; // Aumenta o tamanho da árvore
        }
        current = current->filhos[index]; // Nó atual passa a ser o filho, correspondente ao caractere dessa iteração
    }
    // current vai ser o nó que representa o último caractere da palavra, por isso seu comprimento e posição são alterados com o que foi informado
    if (current->comprimento == 0) {
        current->comprimento = comprimento;
        current->posicao = posicao;
    }
}

template<typename T>
bool structures::Trie<T>::contains(const T& data) const { // Verifica se contém uma palavra, iniciando pelo nó raiz
    if (root != nullptr) {
        return root->contains(data, 0);
    } else {
        return false;
    }
}

template<typename T>
bool structures::Trie<T>::empty() const { // Verifica se a árvore está vazia
    return size() == 0;
}

template<typename T>
std::size_t structures::Trie<T>::size() const { // Retorna o tamanho da árvore (quantidade de nós)
    return size_;
}

template<typename T>
void structures::Trie<T>::destroy(NoTrie* node) {
    if (node != nullptr) {
        for (int i = 0; i < 26; ++i) { // Chama o método recursivamente para todos os filhos do nó
            if (node->filhos[i] != nullptr) {
                destroy(node->filhos[i]);
            }
        }
        delete node; // Desaloca o nó da chamada atual
    }
}

template<typename T>
void structures::Trie<T>::words() const { // Printa na tela as palavras presentes na árvore, iniciando a busca pelo nó raiz
    words(root, "");
}

template<typename T>
void structures::Trie<T>::words(NoTrie* node, std::string prefix) const {
    if (node != nullptr) {
        if (node->comprimento > 0) { // Se comprimento do nó atual for maior que zero, significa que representa o final de uma palavra
            std::cout << prefix << std::endl; // Nessa situação, a palavra é printada
        }
        for (int i = 0; i < 26; ++i) { // Percorre todos os filhos do nó
            if (node->filhos[i] != nullptr) { // Se tiver filho nessa posição
                // Chama recursivamente a função com seu filho, passando como parâmetro o nó dele e
                // o prefixo como o que foi passado anteriormente + a letra desse nó,
                // assim, as letras vão sendo concatenadas até formar uma palavra
                words(node->filhos[i], prefix + node->filhos[i]->letra);
            }
        }
    }
}

template<typename T>
void structures::Trie<T>::draw() const { // Printa na o desenho da árvore, iniciando a busca pelo nó raiz
    draw(root, "", true);
}

template<typename T>
void structures::Trie<T>::draw(NoTrie* node, std::string prefix, bool isTail) const { // Printa o desenho da árvore recursivamente
    if (node != nullptr) {
        std::cout << prefix << (isTail ? "└── " : "├── ") << (node->letra == 0 ? '*' : node->letra) << std::endl;
        for (int i = 0; i < 26; ++i) {
            if (node->filhos[i] != nullptr) {
                // Verifica se é o último filho não-nulo
                bool isLast = true;
                for (int j = i + 1; j < 26; ++j) {
                    if (node->filhos[j] != nullptr) {
                        isLast = false;
                        break;
                    }
                }
                draw(node->filhos[i], prefix + (isTail ? "    " : "│   "), isLast);
            }
        }
    }
}

template<typename T>
void structures::Trie<T>::printAllNodes() const { // Printa as informações de todos os nós, iniciando a busca dos nós pela raiz
    root->printAllNodes(root, "");
}

#endif // TRIE_H
