#include <iostream>
#include <string>
#include <vector>

// ============================================================
// КЛАСС SUPPLIER (Поставщик)
// ============================================================
class Supplier {
private:
    std::string name;
    std::string country;

public:
    Supplier(const std::string& n, const std::string& c)
        : name(n), country(c) {
        std::cout << "  [Поставщик] Создан: " << name
            << " (" << country << ")" << std::endl;
    }

    ~Supplier() {
        std::cout << "  [Поставщик] Удалён: " << name << std::endl;
    }

    std::string getName() const { return name; }        // ← const + возврат
    std::string getCountry() const { return country; }  // ← const + возврат

    void print() const {
        std::cout << "   - " << name << " (" << country << ")" << std::endl;
    }
};

// ============================================================
// КЛАСС PRODUCT (Товар) + СТАТИЧЕСКИЙ СЧЁТЧИК
// ============================================================
class Product {
private:
    std::string name;
    double price;
    std::string article;

    static int totalCount;

public:
    Product(const std::string& n, double p, const std::string& a)  // ← n, p, a
        : name(n), price(p), article(a) {                           // ← n, p, a
        totalCount++;
        std::cout << "  [Товар] Создан: " << name
            << " (арт. " << article << ", " << price << " руб.)"
            << " | Всего: " << totalCount << std::endl;
    }

    ~Product() {
        totalCount--;
        std::cout << "  [Товар] Удалён: " << name
            << " | Всего: " << totalCount << std::endl;
    }

    std::string getName() const { return name; }
    double getPrice() const { return price; }
    std::string getArticle() const { return article; }

    static int getTotalCount() {
        return totalCount;
    }

    void print() const {                                    // ← const
        std::cout << "   - " << name
            << " (арт. " << article
            << ", " << price << " руб.)" << std::endl;
    }
};

int Product::totalCount = 0;

// ============================================================
// КЛАСС WAREHOUSE (Склад) — SINGLETON + АГРЕГАЦИЯ
// ============================================================
class Warehouse {
private:
    static Warehouse* instance;
    std::string name;
    std::vector<Product*> products;
    std::vector<Supplier*> suppliers;   // ← Supplier, не Suplier

    Warehouse(const std::string& n) : name(n) {
        std::cout << "=== СКЛАД \"" << name << "\" ОТКРЫТ ===" << std::endl;
    }

public:
    Warehouse(const Warehouse&) = delete;
    Warehouse& operator=(const Warehouse&) = delete;

    ~Warehouse() {
        std::cout << "=== СКЛАД \"" << name << "\" ЗАКРЫТ ===" << std::endl;
    }

    static Warehouse* getInstance() {
        if (instance == nullptr) {
            instance = new Warehouse("Центральный");
        }
        return instance;
    }

    void addProduct(Product* p) {
        products.push_back(p);
        std::cout << "  [Склад] Принят товар: " << p->getName() << std::endl;
    }

    void addSupplier(Supplier* s) {
        suppliers.push_back(s);
        std::cout << "  [Склад] Добавлен поставщик: " << s->getName() << std::endl;
    }

    void print() const {
        std::cout << "\n--- Склад \"" << name << "\" ---" << std::endl;

        std::cout << "Поставщики (" << suppliers.size() << "):" << std::endl;
        for (const auto* s : suppliers) {
            s->print();
        }

        std::cout << "Товары (" << products.size() << "):" << std::endl;
        for (const auto* p : products) {
            p->print();
        }

        std::cout << "Всего товаров в системе: "
            << Product::getTotalCount() << std::endl;
    }
};

Warehouse* Warehouse::instance = nullptr;

// ============================================================
// MAIN
// ============================================================
int main() {
    setlocale(LC_ALL, "ru");

    std::cout << "========== 1. СОЗДАНИЕ ТОВАРОВ И ПОСТАВЩИКОВ ==========\n";

    Product p1("Молоко", 89.90, "A001");
    Product p2("Хлеб", 45.50, "A002");
    Product p3("Сыр", 320.00, "A003");

    Supplier s1("Молочный комбинат", "Россия");
    Supplier s2("Хлебозавод №1", "Россия");

    std::cout << "\n========== 2. ПОЛУЧЕНИЕ СКЛАДА (Singleton) ==========\n";

    Warehouse* w1 = Warehouse::getInstance();
    Warehouse* w2 = Warehouse::getInstance();

    std::cout << "w1 == w2 (один и тот же склад): "
        << (w1 == w2 ? "ДА" : "НЕТ") << std::endl;

    std::cout << "\n========== 3. ДОБАВЛЕНИЕ НА СКЛАД (Агрегация) ==========\n";

    w1->addSupplier(&s1);
    w1->addSupplier(&s2);
    w1->addProduct(&p1);
    w1->addProduct(&p2);
    w1->addProduct(&p3);

    std::cout << "\n========== 4. СОДЕРЖИМОЕ СКЛАДА ==========\n";
    w1->print();

    std::cout << "\n========== 5. УНИЧТОЖЕНИЕ СКЛАДА ==========\n";
    delete Warehouse::getInstance();

    std::cout << "\n========== 6. ПОСЛЕ УНИЧТОЖЕНИЯ СКЛАДА ==========\n";
    std::cout << "Товары ВСЁ ЕЩЁ существуют (агрегация!):\n";
    p1.print();
    p2.print();
    p3.print();

    std::cout << "\nПоставщики ВСЁ ЕЩЁ существуют (агрегация!):\n";
    s1.print();
    s2.print();

    std::cout << "\nВсего товаров в системе: "
        << Product::getTotalCount() << std::endl;

    std::cout << "\n========== 7. КОНЕЦ ПРОГРАММЫ ==========\n";
    std::cout << "(Здесь удаляются товары и поставщики — локальные переменные)\n";

    std::cout << "\nНажмите Enter для выхода...";
    std::cin.get();

    return 0;
}