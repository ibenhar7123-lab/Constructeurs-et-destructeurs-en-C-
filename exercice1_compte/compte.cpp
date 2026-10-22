// Exercice 1 - CompteBancaire (regle des cinq, version corrigee)
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

class CompteBancaire {
private:
    // L'ordre de declaration = l'ordre d'initialisation (evite -Wreorder)
    std::string numero;
    double solde;
    std::string* historique;
    std::size_t capacite;
    std::size_t nbOps;

    void agrandirHistorique() {
        std::size_t nouvelleCap = capacite ? capacite * 2 : 10; // gere l'objet "moved-from"
        std::string* nouveau = new std::string[nouvelleCap];
        std::copy(historique, historique + nbOps, nouveau);
        delete[] historique;
        historique = nouveau;
        capacite = nouvelleCap;
    }
    void ajouterOperation(const std::string& op) {
        if (nbOps == capacite) agrandirHistorique();
        historique[nbOps++] = op;
    }

public:
    // 1. Constructeur par defaut
    CompteBancaire()
        : numero("NON_DEFINI"), solde(0.0), historique(new std::string[10]),
          capacite(10), nbOps(0) {}

    // 2. Constructeur parametre (passage par valeur + std::move)
    CompteBancaire(std::string num, double s)
        : numero(std::move(num)), solde(s < 0 ? 0.0 : s),
          historique(new std::string[10]), capacite(10), nbOps(0) {
        if (s < 0) std::cerr << "Erreur : solde initial negatif, mis a 0\n";
        ajouterOperation("Creation du compte avec solde " + std::to_string(solde));
    }

    // 3. Constructeur de copie (copie profonde)
    CompteBancaire(const CompteBancaire& o)
        : numero(o.numero), solde(o.solde),
          historique(new std::string[o.capacite ? o.capacite : 10]),
          capacite(o.capacite ? o.capacite : 10), nbOps(o.nbOps) {
        std::copy(o.historique, o.historique + nbOps, historique);
    }

    // 4. Constructeur de deplacement
    CompteBancaire(CompteBancaire&& o) noexcept
        : numero(std::move(o.numero)), solde(o.solde), historique(o.historique),
          capacite(o.capacite), nbOps(o.nbOps) {
        o.historique = nullptr;
        o.capacite = o.nbOps = 0;
        o.solde = 0.0;
    }

    // 5. Assignation (copy-and-swap) : couvre copie ET deplacement
    CompteBancaire& operator=(CompteBancaire other) noexcept {
        swap(other);
        return *this;
    }

    void swap(CompteBancaire& o) noexcept {
        std::swap(numero, o.numero);
        std::swap(solde, o.solde);
        std::swap(historique, o.historique);
        std::swap(capacite, o.capacite);
        std::swap(nbOps, o.nbOps);
    }

    // 6. Destructeur
    ~CompteBancaire() {
        delete[] historique;
        std::cout << "Destruction du compte " << numero << "\n";
    }

    void deposer(double montant) {
        if (montant <= 0) { std::cerr << "Montant de depot invalide\n"; return; }
        solde += montant;
        ajouterOperation("Depot de " + std::to_string(montant));
    }
    void retirer(double montant) {
        if (montant <= 0 || montant > solde) { std::cerr << "Retrait impossible\n"; return; }
        solde -= montant;
        ajouterOperation("Retrait de " + std::to_string(montant));
    }
    void afficher() const {
        std::cout << "Compte " << numero << " - Solde : " << solde << "\n";
        std::cout << "Historique des operations :\n";
        for (std::size_t i = 0; i < nbOps; ++i)
            std::cout << "  " << historique[i] << "\n";
    }
};

int main() {
    CompteBancaire c1("FR123456", 500.0);
    c1.deposer(200);
    c1.retirer(150);
    c1.afficher();

    std::cout << "\n--- Copie ---\n";
    CompteBancaire c2 = c1;
    c2.afficher();

    std::cout << "\n--- Deplacement ---\n";
    CompteBancaire c3 = std::move(c1);
    c3.afficher();

    std::cout << "\n--- c1 (moved-from) reste utilisable ---\n";
    c1.deposer(10);   // sur grace a la correction de agrandirHistorique
    c1.afficher();

    std::cout << "\n--- Assignations ---\n";
    CompteBancaire c4;
    c4 = c2;               // copie
    c4 = CompteBancaire("MA999", 42.0); // deplacement
    c4.afficher();
    std::cout << "\n--- Fin du main ---\n";
}
