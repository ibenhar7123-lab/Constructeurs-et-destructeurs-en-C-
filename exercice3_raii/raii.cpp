// Exercice 3 - RAII, unique_ptr et exceptions dans les constructeurs
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

class BufferRAII {
    std::unique_ptr<char[]> data;
    std::size_t taille;
public:
    explicit BufferRAII(std::size_t t) : data(std::make_unique<char[]>(t)), taille(t) {}
    BufferRAII(BufferRAII&& o) noexcept
        : data(std::move(o.data)), taille(std::exchange(o.taille, 0)) {}
    BufferRAII& operator=(BufferRAII&& o) noexcept {
        data = std::move(o.data);
        taille = std::exchange(o.taille, 0);
        return *this;
    }
    char* get() const { return data.get(); }
    std::size_t size() const { return taille; }
};

// Une ressource qui affiche sa construction/destruction
struct Trace {
    int id;
    explicit Trace(int i) : id(i) { std::cout << "  Trace " << id << " acquise\n"; }
    ~Trace() { std::cout << "  Trace " << id << " liberee\n"; }
};

class Fragile {
    Trace a, b;
public:
    Fragile() : a(1), b(2) {
        throw std::runtime_error("echec dans le constructeur");
    }
    ~Fragile() { std::cout << "  ~Fragile (jamais appele ici)\n"; }
};

class CompteSecure {
    double solde;
    static double valider(double s) {
        if (s < 0) throw std::invalid_argument("Solde initial negatif");
        return s;
    }
public:
    explicit CompteSecure(double s) : solde(valider(s)) {}
    double getSolde() const { return solde; }
};

int main() {
    BufferRAII b(256);
    std::cout << "Buffer de " << b.size() << " octets alloue\n";
    BufferRAII b2 = std::move(b);
    std::cout << "Apres deplacement : b.size() = " << b.size()
              << ", b2.size() = " << b2.size() << "\n";

    std::cout << "\nConstruction de Fragile :\n";
    try { Fragile f; }
    catch (const std::exception& e) {
        std::cout << "Exception : " << e.what() << "\n";
    }

    try { CompteSecure c(-5); }
    catch (const std::invalid_argument& e) {
        std::cout << "Exception : " << e.what() << "\n";
    }
}
