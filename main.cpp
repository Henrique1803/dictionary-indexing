#include <iostream>
#include "DicReader.cpp"

int main() {

    /*
      Grupo:
      -> Henrique Mateus Teodoro (23100472)
      -> Rodrigo Schwartz (23100471)
    */
    
    using namespace std;

    string filename;
    string word;

    cin >> filename;  // entrada

    structures::Trie<string> trie;           // Árvore utilizada na aplicação
    DicReader reader = DicReader(filename);  // Objeto responsável por ler o arquivo .dic. É passado como parâmetro do construtor o path do arquivo

    try {
        reader.readAndInsert(trie);          // Lê o arquivo .dic e salva as informações sobre cada palavra na árvore
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;            // Caso ocorra algum erro
        return 1;
    }

    trie.draw();                            // Printa na tela a estrutura da árvore (somente para testes)
    trie.words();                           // Printa na tela as palavras presentes na árvore (somente para testes)
    //trie.printAllNodes();                 // Printa todos os nós da árvore e seus dados (somente para testes)

    // é necessário implementar as funcionalidades de print, de acordo com o que foi pedido no enunciado
    /*while (1) {  // leitura das palavras ate' encontrar "0"
        cin >> word;
        if (word.compare("0") == 0) {
            break;
        }
        cout << word << endl;
    }*/

    return 0;
}
