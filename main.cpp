/*
 * Nome: Davide
 * Cognome: Molteni
 * Matricola: 909812
 * E-Mail: d.molteni17@campus.unimib.it
 */

#include <iostream>
#include <vector>

#include "tree.h"

struct Person {
    int id;

    Person(int id = 0) : id(id) {}

    bool operator==(const Person& other) const
    {
        return id == other.id;
    }
};

// Funtore di comparazione
struct PersonCompare {
    bool operator()(const Person& first, const Person& second) const {
        return first.id < second.id;
    }
};

std::ostream& operator<<(std::ostream& os, const Person& person) {
    os << "Person(" << person.id << ")";
    return os;
}

void printEvenPeople(const Tree<Person, PersonCompare>& tree) {
    struct IsEven {
        bool operator()(const Person& p) const {
            return p.id % 2 == 0;
        }
    };

    printIF(tree, IsEven());
}

// Funzione di appoggio per fare alcuni test
// Richiede come argomenti il nome del test, il valore ottenuto dal test in questione e il valore atteso.
// Stampa [PASS] o [FAIL] a seconda dell'uguaglianza tra il valore atteso e il valore ottenuto
template<typename T>
void test(const std::string& testName, const T& obtained, const T& expected)
{
    std::cout << (obtained == expected ? "[PASS] " : "[FAIL] ")
        << testName
        << " | Ottenuto: "
        << obtained
        << " | Atteso: "
        << expected
        << std::endl;
}

void testAlberoVuotoInt() {
    std::cout << "=== Test albero vuoto ===" << std::endl;
    Tree<int> emptyTree;

    test("Size albero vuoto", emptyTree.size(), 0);
    test("Contains su albero vuoto", emptyTree.contains(10), false);
    test<bool>("Iteratori su albero vuoto",emptyTree.begin() != emptyTree.end(), false);

    Tree<int> copiedEmptyTree(emptyTree);

    test<int>("Copia albero vuoto",copiedEmptyTree.size(), 0);
    test<bool>("Iteratori sulla copia albero vuoto",copiedEmptyTree.begin() != copiedEmptyTree.end(), false);

    Tree<int> assignedEmptyTree;
    assignedEmptyTree = emptyTree;

    test<int>("Operatore di assegnazione vuoto",assignedEmptyTree.size(), 0);
    test<bool>("Iteratori su albero vuoto assegnato",assignedEmptyTree.begin() != assignedEmptyTree.end(), false);

    Tree<int> subtree = emptyTree.subtree(10);

    test<int>("Subtree albero vuoto",subtree.size(), 0);
    test<bool>("Iteratori subtree vuoto",subtree.begin() != subtree.end(), false);

    std::cout << "Stampa albero vuoto: " << emptyTree << std::endl;
}

void testSingoloElementoInt() {
    std::cout << "=== Test albero con un elemento ===" << std::endl;

    Tree<int> tree;

    tree.insert(10);

    test("Size dopo insert", tree.size(), 1);
    test("Contains elemento presente", tree.contains(10), true);
    test("Contains elemento assente", tree.contains(5), false);

    std::cout << "Stampa albero singolo elemento: " << tree << std::endl;
}

void testInserimentiMultipliInt() {
    std::cout << "=== Test inserimenti multipli ===" << std::endl;

    Tree<int> tree;

    // testa sia rami destri che rami sinistri
    for (int i = 0; i < 100; i++) {
        if (i % 2 == 0) {
            tree.insert(-i);
            continue;
        }

        tree.insert(i);
    }

    test("Size", tree.size(), 100);

    std::cout << "Stampa albero multipli elementi: " << tree << std::endl;
}

void testInserimentoDuplicatoInt() {
    std::cout << "=== Test inserimento duplicato ===" << std::endl;
    Tree<int> tree;
    tree.insert(10);

    try {
        tree.insert(10);

        std::cout << "[FAIL] Nessuna eccezione lanciata" << std::endl;
    }
    catch (const DuplicateElementException&) {
        std::cout << "[PASS] Eccezione correttamente lanciata" << std::endl;
    }

    test("Size invariata", tree.size(), 1);
}

void testCostruttoreDaIntervalloInt() {
    std::cout << "=== Test costruttore da intervallo ===" << std::endl;
    std::vector<int> values;

    values.push_back(10);
    values.push_back(5);
    values.push_back(20);
    values.push_back(3);

    Tree<int> tree(values.begin(), values.end());

    test("Size: ", tree.size(), 4);

    std::cout << "Stampa: " << tree << std::endl;

}

void testEccezioneCostruttoreDaIntervalloInt() {
    std::cout << "=== Test eccezione costruttore da intervallo ===" << std::endl;
    std::vector<int> values;

    values.push_back(10);
    values.push_back(5);
    values.push_back(20);
    values.push_back(3);
    values.push_back(10);

    try {
        Tree<int> tree(values.begin(), values.end());

        std::cout << "[FAIL] Nessuna eccezione lanciata" << std::endl;
    }
    catch (const DuplicateElementException&) {
        std::cout << "[PASS] Eccezione correttamente lanciata" << std::endl;
    }
}

void testCopyConstructorInt() {
    std::cout << "=== Test copy constructor ===" << std::endl;

    Tree<int> tree;
    tree.insert(1);
    tree.insert(2);

    Tree copiedTree(tree);

    test("Size copy", copiedTree.size(), tree.size());
    test("Contains 1", copiedTree.contains(1), true);
    test("Contains 2", copiedTree.contains(2), true);
    test("Contains 3", copiedTree.contains(3), false);
}

void testSubtreeInt() {
    std::cout << "=== Test subtree ===" << std::endl;

    Tree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(3);
    tree.insert(8);

    Tree<int> subtree = tree.subtree(5);

    test("Size subtree", subtree.size(), 3);

    std::cout << "Subtree ottenuto: " << subtree << std::endl;
    std::cout << "Valore atteso: [3, 5, 8]" << std::endl;

    Tree<int> emptySubtree = tree.subtree(999);
    test("Size subtree inesistente", emptySubtree.size(), 0);
}

void testIteratorInt() {
    std::cout << "=== Test iteratore ===" << std::endl;

    Tree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(3);
    tree.insert(8);

    std::cout << "Valore ottenuto: ";

    for (Tree<int>::const_iterator it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << " ";
    }

    std::cout << std::endl;
    std::cout << "Valore atteso: 3 5 8 10 20" << std::endl;
}

void testClearInt() {
    std::cout << "=== Test clear ===" << std::endl;

    Tree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);

    tree.clear();

    test("Size dopo clear", tree.size(), 0);
    test("Contains dopo clear", tree.contains(10), false);
    test("Begin uguale ad end", tree.begin() != tree.end(), false);

    std::cout << "Albero dopo clear: " << tree << std::endl;
}

void testPrintIfInt() {
    std::cout << "=== Test printIF int ===" << std::endl;

    Tree<int> tree;

    tree.insert(11);
    tree.insert(1);
    tree.insert(20);
    tree.insert(5);
    tree.insert(2);

    struct GreaterThanTen {
        bool operator()(int value) const {
            return value > 10;
        }
    };

    std::cout << "Valore ottenuto:" << std::endl;
    printIF(tree, GreaterThanTen());

    std::cout << "Valore atteso:" << std::endl;
    std::cout << "11" << std::endl;
    std::cout << "20" << std::endl;
}

void testAssignmentOperatorInt() {
    std::cout << "=== Test operator= ===" << std::endl;

    Tree<int> source;
    source.insert(1);
    source.insert(2);

    Tree<int> destination;
    destination.insert(999);

    destination = source;

    test("Size assegnazione", destination.size(), 2);
    test("Contains 1", destination.contains(1), true);
    test("Contains 999", destination.contains(999), false);
}

// Test tipi custom
void testCostruttoreDaIntervalloPerson() {
    std::cout << "=== Test costruttore da intervallo Person ===" << std::endl;

    std::vector<Person> values;

    values.push_back(Person(10));
    values.push_back(Person(5));
    values.push_back(Person(20));
    values.push_back(Person(3));

    Tree<Person, PersonCompare> tree(values.begin(), values.end());

    test("Size", tree.size(), 4);

    std::cout << "Valore ottenuto: " << tree << std::endl;
    std::cout << "Valore atteso: [Person(3), Person(5), Person(10), Person(20)]" << std::endl;
}

void testInserimentoDuplicatoPerson() {
    std::cout << "=== Test duplicato Person ===" << std::endl;

    Tree<Person, PersonCompare> tree;

    tree.insert(Person(10));

    try {
        tree.insert(Person(10));

        std::cout << "[FAIL] Nessuna eccezione lanciata" << std::endl;
    }
    catch (const DuplicateElementException&) {
        std::cout << "[PASS] Eccezione correttamente lanciata" << std::endl;
    }

    test("Size invariata", tree.size(), 1);
}

void testSubtreePerson() {
    std::cout << "=== Test subtree Person ===" << std::endl;

    Tree<Person, PersonCompare> tree;

    tree.insert(Person(10));
    tree.insert(Person(5));
    tree.insert(Person(20));
    tree.insert(Person(3));
    tree.insert(Person(8));

    Tree<Person, PersonCompare> subtree =
        tree.subtree(Person(5));

    test("Size subtree",
         subtree.size(),
         3);

    std::cout << "Valore ottenuto: "
              << subtree
              << std::endl;

    std::cout << "Valore atteso: "
              << "[Person(3), Person(5), Person(8)]"
              << std::endl;
}

void testPrintIfPerson() {
    std::cout << "=== Test printIF Person ===" << std::endl;

    Tree<Person, PersonCompare> tree;

    tree.insert(Person(10));
    tree.insert(Person(5));
    tree.insert(Person(20));
    tree.insert(Person(3));
    tree.insert(Person(8));

    std::cout << "Valore ottenuto:" << std::endl;
    printEvenPeople(tree);

    std::cout << "Valore atteso:" << std::endl;
    std::cout << "Person(8)" << std::endl;
    std::cout << "Person(10)" << std::endl;
    std::cout << "Person(20)" << std::endl;
}

int main() {
    std::cout << "Inizio test" <<std::endl;

    testAlberoVuotoInt();
    testSingoloElementoInt();
    testInserimentiMultipliInt();
    testInserimentoDuplicatoInt();
    testCostruttoreDaIntervalloInt();
    testEccezioneCostruttoreDaIntervalloInt();
    testCopyConstructorInt();
    testSubtreeInt();
    testIteratorInt();
    testClearInt();
    testPrintIfInt();
    testAssignmentOperatorInt();

    testCostruttoreDaIntervalloPerson();
    testInserimentoDuplicatoPerson();
    testSubtreePerson();
    testPrintIfPerson();

    return 0;
}