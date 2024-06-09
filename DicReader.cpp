#include <fstream>
#include <sstream>
#include <string>
#include "trie.h"

class DicReader {

public:
    DicReader(const std::string& _filename) {
        filename = _filename;
    }   

    // Método responsável por ler o arquivo .dic e armazenar as palavras na árvore
    void readAndInsert(structures::Trie<std::string>& trie) {
        std::string info; // Variável será usada para armazenar cada linha do arquivo à medida que é lida
        std::ifstream input(filename); // Objeto usado para ler o arquivo
        std::size_t posicao = 0;
        if (input.is_open()) { // Verifica se o arquivo foi aberto com sucesso
            while (!input.eof()) { // Continua no laço enquanto o final do arquivo não for alcançado
                std::getline(input, info); // Lê uma linha do arquivo e salva em info
                std::size_t comprimento = info.size();

                // Salva a posição da primeira ocorrência de abertura "[" a partir da posição específicada
                std::size_t posAbertura = info.find('[', 0); 
                // Salva a posição da primeira ocorrência de fechamento "]"
                std::size_t posFechamento = info.find(']', posAbertura);
                // Caso não encontre os colchetes, significa que não há mais ocorrências de chaves e a string terminou
                if (posAbertura == std::string::npos || posFechamento == std::string::npos) {
                    break; // Sai do laço principal
                }
                // Extrai o conteúdo da string entre as posições de abertura e fechamento dos colchetes
                std::string conteudo = info.substr(posAbertura + 1, posFechamento - posAbertura - 1);

                trie.insert(conteudo, posicao, comprimento); // Insere na árvore

                posicao += info.size()+1; // Próxima posição vai ser logo após o fim da linha atual
            }
        } else {
                std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        }
    }
private:
    std::string filename;
};


