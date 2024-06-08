#include <cstddef>  // Para std::size_t
#include <string>   // Para std::string


namespace structures {

template<typename T>
class Trie {
public:
    Trie();

    ~Trie();

    void insert(const T& data);

    bool contains(const T& data) const;

    bool empty() const;

    std::size_t size() const;

    void draw() const;

private:
    struct NoTrie {
        NoTrie() : posicao(0), comprimento(0) {
            for (int i = 0; i < 26; ++i) {
                filhos[i] = nullptr;
            }
        }

        explicit NoTrie(char letra_) : letra(letra_), posicao(0), comprimento(0) {
            for (int i = 0; i < 26; ++i) {
                filhos[i] = nullptr;
            }
        }

        char           letra;        //opcional
        NoTrie        *filhos[26]; // ponteiros como nulos
        unsigned long  posicao;
        unsigned long  comprimento;  //se maior que zero, indica último caracter de uma palavra

        bool contains(const T& data, std::size_t index) const {
            if (index == data.size()) {
                return comprimento > 0;
            }
            char ch = data[index];
            int child_index = ch - 'a';
            if (filhos[child_index] == nullptr) {
                return false;
            }
            return filhos[child_index]->contains(data, index + 1);
        }

    };

    void draw(NoTrie* node, std::string prefix) const;

    void destroy(NoTrie* node);

    NoTrie* root;
    std::size_t size_;
};

}  // namespace structures

//-------------------------------------

template<typename T>
structures::Trie<T>::Trie() {
    root = new NoTrie();
    size_ = 1;
}

template<typename T>
structures::Trie<T>::~Trie() {
    destroy(root);
}

template<typename T>
void structures::Trie<T>::insert(const T& data) {
    NoTrie* current = root;
    for (std::size_t i = 0; i < data.size(); ++i) {
        char ch = data[i];
        int index = ch - 'a';
        if (current->filhos[index] == nullptr) {
            current->filhos[index] = new NoTrie(ch);
            size_++;
        }
        current = current->filhos[index];
    }
    if (current->comprimento == 0) {
        current->comprimento = data.size();
        //size_++; // se size_ for o número de palavras na árvore, deve ser incrementado aqui
    }
}

template<typename T>
bool structures::Trie<T>::contains(const T& data) const {
    if (root != nullptr) {
        return root->contains(data, 0);
    } else {
        return false;
    }
}

template<typename T>
bool structures::Trie<T>::empty() const {
    return size() == 0;
}

template<typename T>
std::size_t structures::Trie<T>::size() const {
    return size_;
}

template<typename T>
void structures::Trie<T>::destroy(NoTrie* node) {
    if (node != nullptr) {
        for (int i = 0; i < 26; ++i) {
            if (node->filhos[i] != nullptr) {
                destroy(node->filhos[i]);
            }
        }
        delete node;
    }
}

template<typename T>
void structures::Trie<T>::draw() const {
    draw(root, "");
}

template<typename T>
void structures::Trie<T>::draw(NoTrie* node, std::string prefix) const {
    if (node != nullptr) {
        if (node->comprimento > 0) {
            std::cout << prefix << std::endl;
        }
        for (int i = 0; i < 26; ++i) {
            if (node->filhos[i] != nullptr) {
                draw(node->filhos[i], prefix + node->filhos[i]->letra);
            }
        }
    }
}
