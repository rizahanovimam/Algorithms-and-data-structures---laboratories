// lb 2
#include <iostream>
#include <stdexcept>

using namespace std;
// ============================================================
// ШАБЛОННЫЙ КЛАСС Set<T> (Множество на основе BST)
// ============================================================
template <typename T>
class Set { 
private:
    // Структура узла дерева
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root; // Корень дерева
    size_t count; // Количество уникальных элементов

    // Вспомогательная функция для рекурсивного удаления дерева
    void clear(Node* node) {
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // Вспомогательная функция для вставки
    Node* insert(Node* node, const T& value, bool& inserted) {
        if (node == nullptr) {
            inserted = true; // Элемент добавлен
            count++;
            return new Node(value);
        }
        if (value < node->data) {
            node->left = insert(node->left, value, inserted);
        }
        else if (value > node->data) {
            node->right = insert(node->right, value, inserted);
        }
        // Если value == node->data, ничего не делаем (дубликаты запрещены)
        return node;
    }

    // Вспомогательная функция для поиска
    bool contains(Node* node, const T& value) const {
        if (node == nullptr) return false;
        if (value == node->data) return true;
        if (value < node->data) return contains(node->left, value);
        return contains(node->right, value);
    }

    // Вспомогательная функция для удаления узла
    Node* remove(Node* node, const T& value, bool& removed) {
        if (node == nullptr) return nullptr;

        if (value < node->data) {
            node->left = remove(node->left, value, removed);
        }
        else if (value > node->data) {
            node->right = remove(node->right, value, removed);
        }
        else {
            // Нашли узел для удаления
            removed = true;
            count--;

            // Случай 1: Нет детей или один ребенок
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Случай 2: Два ребенка. Ищем минимальный в правом поддереве
            Node* temp = node->right;
            while (temp->left != nullptr) temp = temp->left;

            node->data = temp->data; // Копируем значение
            node->right = remove(node->right, temp->data, removed); // Удаляем дубликат
            count++; // Возвращаем счетчик, так как remove внутри уменьшит его еще раз
        }
        return node;
    }

    // Вспомогательная функция для вывода
    void print(Node* node) const {
        if (node != nullptr) {
            print(node->left);
            cout << node->data << " ";
            print(node->right);
        }
    }

public:
    // Конструктор — O(1)
    Set() : root(nullptr), count(0) {}

    // Деструктор — O(n)
    ~Set() {
        clear(root);
    }

    // insert (вставка) — O(log n) в среднем, O(n) в худшем
    void insert(const T& value) {
        bool inserted = false;
        root = insert(root, value, inserted);
    }

    // contains (проверка наличия) — O(log n) в среднем
    bool contains(const T& value) const {
        return contains(root, value);
    }

    // remove (удаление) — O(log n) в среднем
    void remove(const T& value) {
        bool removed = false;
        root = remove(root, value, removed);
    }

    // size (размер) — O(1)
    size_t size() const {
        return count;
    }

    // Вывод (для демонстрации) — O(n)
    void print() const {
        cout << "[ ";
        print(root);
        cout << "]" << endl;
    }
};

// ============================================================
// MAIN
// ============================================================
int main() {
    setlocale(LC_ALL, "ru");

    Set<int> mySet;

    cout << "Добавляем элементы: 50, 30, 70, 20, 40, 60, 80" << endl;
    mySet.insert(50);
    mySet.insert(30);
    mySet.insert(70);
    mySet.insert(20);
    mySet.insert(40);
    mySet.insert(60);
    mySet.insert(80);

    cout << "Содержимое множества (отсортировано благодаря BST): ";
    mySet.print();
    cout << "Размер: " << mySet.size() << "\n\n";

    cout << "Проверка contains(40): " << (mySet.contains(40) ? "true" : "false") << endl;
    cout << "Проверка contains(90): " << (mySet.contains(90) ? "true" : "false") << "\n\n";

    cout << "Удаляем элемент 30..." << endl;
    mySet.remove(30);
    cout << "Содержимое после удаления: ";
    mySet.print();
    cout << "Размер: " << mySet.size() << "\n\n";

    cout << "Пытаемся добавить дубликат (50)..." << endl;
    mySet.insert(50);
    cout << "Размер остался прежним: " << mySet.size() << endl;

    cout << "\nНажмите Enter для выхода...";
    cin.get();
    return 0;
}