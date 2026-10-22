// Exercice 2 - Buffer dynamique (regle des cinq)
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <utility>

class Buffer {
private:
    char* data;
    std::size_t taille;
public:
    Buffer() : data(nullptr), taille(0) {}

    explicit Buffer(std::size_t t) : data(new char[t]()), taille(t) { // () => zero-initialise
        std::cout << "Buffer de " << taille << " octets cree\n";
    }

    Buffer(const Buffer& o) : data(new char[o.taille]), taille(o.taille) {
        std::copy(o.data, o.data + taille, data);
        std::cout << "Constructeur de copie du buffer\n";
    }

    Buffer(Buffer&& o) noexcept : data(o.data), taille(o.taille) {
        o.data = nullptr;
        o.taille = 0;
        std::cout << "Constructeur de deplacement du buffer\n";
    }

    // Assignation de copie : on alloue AVANT de liberer (sur si new leve)
    Buffer& operator=(const Buffer& o) {
        if (this != &o) {
            char* nouveau = new char[o.taille];
            std::copy(o.data, o.data + o.taille, nouveau);
            delete[] data;
            data = nouveau;
            taille = o.taille;
        }
        std::cout << "Assignation de copie\n";
        return *this;
    }

    Buffer& operator=(Buffer&& o) noexcept {
        if (this != &o) {
            delete[] data;
            data = std::exchange(o.data, nullptr);
            taille = std::exchange(o.taille, 0);
        }
        std::cout << "Assignation de deplacement\n";
        return *this;
    }

    ~Buffer() {
        delete[] data;
        std::cout << "Destruction du buffer (" << taille << " octets)\n";
    }

    char* getData() const { return data; }
    std::size_t getTaille() const { return taille; }
};

int main() {
    Buffer b1(128);
    Buffer b2 = b1;            // copie
    Buffer b3 = std::move(b1); // deplacement
    b2 = b3;                   // assignation copie
    b3 = Buffer(64);           // assignation deplacement
    std::cout << "b1 vide ? " << (b1.getData() == nullptr) << "\n";
}
