#include <iostream>
#include "trie.h"        // Supondo que trie.h contenha a implementação da Trie fornecida

int main() {
    structures::Trie<std::string> trie;

    // Teste de inserção e verificação
    trie.insert("hello");
    trie.insert("world");
    trie.insert("hi");

    // Testar se as palavras estão na Trie
    std::cout << "Contém 'hello': " << trie.contains("hello") << std::endl;  // Deve imprimir 1 (true)
    std::cout << "Contém 'world': " << trie.contains("world") << std::endl;  // Deve imprimir 1 (true)
    std::cout << "Contém 'hi': " << trie.contains("hi") << std::endl;        // Deve imprimir 1 (true)
    std::cout << "Contém 'hell': " << trie.contains("hell") << std::endl;    // Deve imprimir 0 (false)
    std::cout << "Contém 'helloo': " << trie.contains("helloo") << std::endl; // Deve imprimir 0 (false)
    std::cout << "Contém 'h': " << trie.contains("h") << std::endl;          // Deve imprimir 0 (false)

    // Teste de tamanho e vazio
    std::cout << "Tamanho da Trie: " << trie.size() << std::endl;  // Deve imprimir 3
    std::cout << "Trie está vazia: " << trie.empty() << std::endl;  // Deve imprimir 0 (false)

    // Inserir uma nova palavra
    trie.insert("hero");

    // Testar se a nova palavra está na Trie
    std::cout << "Contém 'hero': " << trie.contains("hero") << std::endl;  // Deve imprimir 1 (true)

    // Teste de tamanho atualizado
    std::cout << "Tamanho da Trie atualizado: " << trie.size() << std::endl;  // Deve imprimir 4

    return 0;
}
