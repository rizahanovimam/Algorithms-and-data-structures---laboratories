#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;


template <typename T>
class Vector {
private:
    T* data;    // Указатель на массив в куче
    int sz;     // Текущий размер (количество живых элементов)
    int cap;    // Выделенная память (ёмкость)

public:
    // 1. Конструктор по умолчанию
    Vector() : data(nullptr), sz(0), cap(0) {}

    // 2. Конструктор размера n
    explicit Vector(int n) : sz(n), cap(n) {
        if (n < 0) throw invalid_argument("Размер не может быть отрицательным");
        data = (cap > 0) ? new T[cap] : nullptr;
        for (int i = 0; i < sz; i++) data[i] = T();
    }

    // 3. Конструктор копирования (Глубокое копирование)
    Vector(const Vector& other) : sz(other.sz), cap(other.cap) {
        data = (cap > 0) ? new T[cap] : nullptr;
        for (int i = 0; i < sz; i++) data[i] = other.data[i];
    }

    // 4. Конструктор перемещения
    Vector(Vector&& other) noexcept : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr; // Опустошаем донора
        other.sz = 0;
        other.cap = 0;
    }

    // 5. Деструктор
    ~Vector() {
        delete[] data; // Освобождаем выделенную память массива
    }

    // 6. Оператор присваивания копированием 
    Vector& operator=(const Vector& other) {
        if (this != &other) { // Защита от самоприсваивания
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    // Вспомогательный метод обмена содержимым
    void swap(Vector& other) noexcept {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
    }

    // 7. Оператор присваивания перемещением
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) { // Защита от самоприсваивания
            delete[] data; // Чистим свою старую память

            // Забираем ресурсы донора
            data = other.data;
            sz = other.sz;
            cap = other.cap;

            // Зануляем донора
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    // Доступ по индексу (Неконстантный — для чтения и записи)
    T& operator[](int index) {
        if (index < 0 || index >= sz) throw out_of_range("Индекс вне диапазона");
        return data[index];
    }

    // Доступ по индексу (Константный — только для чтения из const-объектов)
    const T& operator[](int index) const {
        if (index < 0 || index >= sz) throw out_of_range("Индекс вне диапазона");
        return data[index];
    }

    // Геттеры доступа к приватным полям (Инкапсуляция)
    int size() const { return sz; }
    int capacity() const { return cap; }
    bool empty() const { return sz == 0; }

    // Резервирование физической памяти под будущие элементы
    void reserve(int newCap) {
        if (newCap <= cap) return;
        T* newData = new T[newCap];
        for (int i = 0; i < sz; i++) newData[i] = data[i]; // Переносим данные
        delete[] data; // Удаляем старый массив
        data = newData;
        cap = newCap;
    }

    // Добавление элемента в конец (с автоматическим удвоением ёмкости)
    void push_back(const T& value) {
        if (sz >= cap) {
            int newCap = (cap == 0) ? 1 : cap * 2;
            reserve(newCap);
        }
        data[sz++] = value;
    }

    // Логическое удаление элемента с конца
    void pop_back() {
        if (empty()) throw underflow_error("Вектор пуст");
        sz--; // Сдвигаем видимую границу вектора назад за O(1)
    }

    // Реализация итераторов (сырые указатели) для работы range-based for
    T* begin() { return data; }
    T* end() { return data + sz; } // Указывает на ячейку ЗА последним элементом
    const T* begin() const { return data; }
    const T* end() const { return data + sz; }

    // Фильтрация элементов "на месте" 
    void filter(bool (*predicate)(T)) {
        int writeIndex = 0; // Индекс записи отфильтрованных элементов
        for (int readIndex = 0; readIndex < sz; readIndex++) {
            if (predicate(data[readIndex])) {
                data[writeIndex] = data[readIndex]; // Сдвигаем подходящие элементы вперед
                writeIndex++;
            }
        }
        sz = writeIndex; // Обновляем размер, отсекая мусорный хвост
    }
};


// Функция-предикат: проверяет число на чётность
bool isEven(int x) {
    return x % 2 == 0;
}

int main() {
    setlocale(LC_ALL, "ru");

    cout << "=== Тестирование Vector ===" << endl;

    // Наполнение вектора числами от 1 до 10
    Vector<int> v;
    for (int i = 1; i <= 10; i++) {
        v.push_back(i);
    }

    cout << "Исходный вектор: ";
    for (int x : v) cout << x << " ";
    cout << "\nРазмер: " << v.size() << " | Вместимость: " << v.capacity() << endl;

    cout << "\nПрименяем фильтр (оставляем только чётные числа)..." << endl;
    v.filter(isEven);

    cout << "После filter (только четные): ";
    for (int x : v) cout << x << " ";
    cout << "\nНовый размер: " << v.size() << " | Вместимость: " << v.capacity() << endl;

    cout << "\nПроверка добавления нового элемента после фильтра:" << endl;
    v.push_back(99); // Должен затереть первый скрытый элемент в памяти
    cout << "После push_back(99): ";
    for (int x : v) cout << x << " ";
    cout << "\nФинальный размер: " << v.size() << " | Вместимость: " << v.capacity() << endl;

    return 0;
}
