/*
 * Nome: Davide
 * Cognome: Molteni
 * Matricola: 909812
 * E-Mail: d.molteni17@campus.unimib.it
 */

#ifndef INC_909812_TREE_H
#define INC_909812_TREE_H

#include <ostream>
#include <stdexcept>

/**
 * Eccezione custom che viene lanciata quando si prova ad inserire un elemento già presente all'interno dell'albero
 */
class DuplicateElementException : public std::logic_error {
public:
    DuplicateElementException() : std::logic_error("Duplicate element in binary search tree.") {
    };
};

/**
 * Classe Tree che rappresenta l'albero binario di ricerca
 * @tparam T Tipo T generico
 * @tparam Compare Metodo utilizzato dall'utente per confrontare due tipi T. Utilizza come default std::less<T>
 */
template<typename T, typename Compare = std::less<T> >
class Tree {
private:
    /**
     * Nodo dell'albero binario di ricerca
     * Contiene il valore memorizzato di tipo T e i collegamenti ai nodi adiacenti
     */
    class Node {
    public:
        T data;
        Node* left;
        Node* right;
        Node* parent;

        /**
         * Costruisce un nuovo nodo
         * @param data Reference all'elemento di tipo T contenuto dal nodo
         * @param parent Puntatore (se presente) al nodo padre
         */
        explicit Node(const T& data, Node* parent = nullptr) :
        data(data),
        left(nullptr),
        right(nullptr),
        parent(parent) {};
    };

    /**
     * Funzione per comparare due tipi T
     */
    Compare compare;

    /**
     * Puntatore alla radice dell'albero
     */
    Node* root;

    /**
     * Numero di elementi contenuti nell'albero
     */
    int numberOfElements;

    /**
     * Metodo privato che verifica l'uguaglianza tra due elementi utilizzando il comparatore dell'albero.
     * Due elementi sono considerati uguali se nessuno dei due risulta minore dell'altro.
     *
     * @param firstElement Primo elemento T
     * @param secondElement Secondo elemento T
     * @return True se i due elementi sono equivalenti, False altrimenti
     */
    bool isEqual(const T& firstElement, const T& secondElement) const {
        return !compare(firstElement, secondElement) && !compare(secondElement, firstElement);
    }

    /**
     * Metodo per trovare un determinato dato all'interno dell'albero
     * @param data Reference al dato che si vuole trovare all'interno dell'albero
     * @return Puntatore al nodo che contiene il dato richiesto, oppure nullptr se il dato non è presente
     */
    const Node* find(const T& data) const {
        const Node* current = root;

        while (current != nullptr) {
            if (isEqual(data, current->data)) return current;

            current = compare(data, current->data) ? current->left : current->right;
        }

        return nullptr;
    }

    /**
     * Metodo ricorsivo che svuota il sottoalbero generato dal nodo specificato
     * @param node Puntatore al nodo da cui si vuole svuotare
     */
    void clear(Node* node) {
        if (node == nullptr) return;

        // Chiamate ricorsive
        clear(node->left);
        clear(node->right);

        // Memoria deallocata
        delete node;
    }

    /**
     * Metodo ricorsivo che inserisce nell'albero tutti gli elementi del sottoalbero generato dal nodo specificato
     *
     * @param node Radice del sottoalbero da copiare
     */
    void recursiveInsert(const Node* node) {
        if (node == nullptr) return;

        insert(node->data);

        recursiveInsert(node->left);
        recursiveInsert(node->right);
    }

public:
    /**
     * Costruisce un albero vuoto
     */
    Tree() : root(nullptr), numberOfElements(0) {
    };

    /**
     * Copy constructor.
     * Costruisce una copia dell'albero specificato
     * @param tree Albero da copiare
     */
    Tree(const Tree& tree) : root(nullptr), numberOfElements(0) {
        recursiveInsert(tree.root);
    }

    /**
     * Operatore di assegnazione
     * Sostituisce il contenuto dell'albero corrente con una copia dell'albero specificato
     * @param other Albero da copiare
     * @return Riferimento all'albero corrente
     */
    Tree& operator=(const Tree& other) {
        if (this == &other) return *this;

        clear();

        recursiveInsert(other.root);
        return *this;
    }

    /**
     * Costruttore da iteratore
     * Costruisce un albero inserendo tutti gli elementi compresi nell'intervallo specificato
     *
     * Se durante la costruzione viene rilevato un elemento duplicato, l'albero viene svuotato e
     * l'eccezione viene lanciata
     *
     * @tparam Iterator Tipo dell'iteratore utilizzato
     * @param begin Iteratore al primo elemento dell'intervallo
     * @param end Iteratore all'ultimo elemento dell'intervallo
     *
     * @throws DuplicateElementException Se l'intervallo contiene elementi duplicati
     */
    template<typename Iterator>
    Tree(Iterator begin, Iterator end) : root(nullptr), numberOfElements(0) {
        try {
            for (Iterator it = begin; it != end; ++it) {
                insert(*it);
            }
        } catch (DuplicateElementException&) {
            clear();
            throw;
        }
    }

    /**
     * Distruttore
     * Dealloca tutti i nodi contenuti nell'albero
     */
    ~Tree() {
        clear();
    }

    /**
     * Restituisce il numero di elementi dell'albero di ricerca
     * @return Numero di elementi all'interno dell'albero di ricerca
     */
    int size() const {
        return numberOfElements;
    }

    /**
     * Metodo pubblico clear, dealloca tutti i nodi contenuti nell'albero e lo riporta alle condizioni iniziali
     */
    void clear() {
        clear(root);
        root = nullptr;
        numberOfElements = 0;
    }

    /**
     * Iteratore a sola lettura
     */
    class const_iterator {
    private:
        /**
         * Puntatore al nodo corrente
         */
        const Node* current;

    public:
        /**
         * Crea un nuovo iteratore a partire da un nodo specificato
         * @param node Nodo a cui l'iteratore deve puntare
         */
        explicit const_iterator(const Node* node) : current(node) {
        }

        /**
         * Operatore di dereferenziazione. Restituisce una reference al dato di tipo T contenuto dal nodo
         * @return Riferimento costante al valore contenuto nel nodo corrente
         */
        const T& operator*() const {
            return current->data;
        }

        /**
         * Operatore di pre-incremento
         * Avanza l'iteratore all'elemento successivo secondo l'ordinamento "inorder" dell'albero
         * @return Riferimento all'iteratore aggiornato.
         */
        const_iterator& operator++() {
            /*
             * Se il nodo in cui ci troviamo possiede un sottonodo destro, ci spostiamo nel nodo più a sinistra di
             * questo sottoalbero destro.
             */
            if (current->right != nullptr) {
                current = current->right;
                while (current->left != nullptr) current = current->left;

                return *this;
            }

            /*
             * Altrimenti risaliamo fino a quando il nodo da cui saliamo è un nodo sinistro: se il nodo da cui saliamo
             * è un nodo sinistro, abbiamo la certezza di non aver esplorato il sottoalbero generato dal nodo destro
             * al quale siamo saliti, per via dell'algoritmo precedente.
             */
            const Node* temp;
            do {
                temp = current;
                current = current->parent;
            } while (current != nullptr && current->right == temp);
            return *this;
        }

        /**
         * Operatore di disuguaglianza
         * @param other Altro iteratore da confrontare
         * @return True se i due iteratori sono diversi, False altrimenti
         */
        bool operator!=(const const_iterator& other) const {
            return current != other.current;
        }
    };

    /**
     * Restituisce un iteratore di sola lettura al primo elemento dell'albero.
     * @return Iteratore al minimo elemento dell'albero oppure end() se l'albero è vuoto
     */
    const_iterator begin() const {
        if (root == nullptr) return const_iterator(nullptr);

        const Node* current = root;
        while (current->left != nullptr) current = current->left;
        return const_iterator(current);
    }

    /**
     * Restituisce un iteratore di sola lettura che rappresenta la fine della sequenza
     * @return Iteratore all'elemento successivo all'ultimo dell'intervallo
     */
    const_iterator end() const {
        return const_iterator(nullptr);
    }

    /**
     * Metodo per verificare se un determinato dato T sia presente all'interno dell'albero
     * @param data Reference al dato che si vuole controllare
     * @return True se il dato è presente all'interno dell'albero, False altrimenti
     */
    bool contains(const T& data) const {
        const Node* current = root;

        while (current != nullptr) {
            if (isEqual(data, current->data)) return true;

            // trovandoci in un albero di ricerca, ci spostiamo nel sottoalbero sinistro se il valore cercato è minore
            // del valore corrente, altrimenti nel sottoalbero destro.
            current = compare(data, current->data) ? current->left : current->right;
        }

        return false;
    }

    /**
     * Inserisce un elemento all'interno dell'albero di ricerca
     * @param data Reference al dato da inserire
     *
     * @throws DuplicateElementException se l'albero contiene già l'elemento specificato
     */
    void insert(const T& data) {
        // Se l'albero è vuoto
        if (root == nullptr) {
            root = new Node(data);
            numberOfElements++;
            return;
        }

        Node* current = root;
        Node* previous = nullptr;

        while (current != nullptr) {
            if (isEqual(current->data, data)) throw DuplicateElementException();

            previous = current;
            current = compare(data, current->data) ? current->left : current->right;
        }

        Node* newNode = new Node(data, previous);

        if (compare(data, previous->data)) previous->left = newNode;
        else previous->right = newNode;
        numberOfElements++;
    }

    /**
     * Crea un nuovo albero contenente il sottoalbero generato nel nodo contenente il valore specificato
     * @param data Reference che identifica la radice del sottoalbero da generare
     * @return Copia del sottoalbero oppure un albero vuoto se il valore non è presente
     */
    Tree subtree(const T& data) const {
        if (!contains(data)) return Tree();

        Tree newTree;
        newTree.recursiveInsert(find(data));

        return newTree;
    }
};

/**
 * Stampa tutti gli elementi dell'albero sullo stream specificato
 * @tparam T Tipo degli elementi contenuti nell'albero
 * @tparam Compare Tipo del comparatore utilizzato dall'albero
 * @param os Stream di output su cui effettuare la stampa
 * @param tree Albero da stampare
 * @return Riferimento allo stream di output
 */
template<typename T, typename Compare>
std::ostream& operator<<(std::ostream& os, const Tree<T, Compare>& tree) {
    os << "[";

    typename Tree<T, Compare>::const_iterator it = tree.begin();

    if (it != tree.end()) {
        os << *it;
        ++it;
    }

    for (; it != tree.end(); ++it) {
        os << ", " << *it;
    }

    os << "]";

    return os;
}
/**
 * Stampa tutti gli elementi dell'albero che soddisfano il predicato specificato
 * @tparam T Tipo degli elementi contenuti nell'albero
 * @tparam Compare Tipo del comparatore utilizzato dall'albero
 * @tparam P Tipo del predicato
 * @param tree Albero da esaminare
 * @param predicate Predicato utilizzato per selezionare gli elementi da stampare
 */
template<typename T, typename Compare, typename P>
void printIF(const Tree<T, Compare>& tree, const P& predicate) {
    typename Tree<T, Compare>::const_iterator it = tree.begin();
    for (; it != tree.end(); ++it) {
        if (predicate(*it)) std::cout << *it << std::endl;
    }
}

#endif //INC_909812_TREE_H
