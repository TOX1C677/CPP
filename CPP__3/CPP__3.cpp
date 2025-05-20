#include <iostream>
#include <string>
#include <locale>
#include <codecvt> // Для Windows (устаревшая, но работает)

using namespace std;

// Узел двусвязного списка
template <typename T>
class Node {
public:
    T data;
    Node* next;
    Node* prev;

    Node(T val) : data(val), next(nullptr), prev(nullptr) {}
};

// Шаблонный класс двусвязного списка
template <typename T>
class DoublyLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void append(T value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void remove(T value) {
        Node<T>* current = head;
        while (current) {
            if (current->data == value) {
                if (current->prev)
                    current->prev->next = current->next;
                else
                    head = current->next;

                if (current->next)
                    current->next->prev = current->prev;
                else
                    tail = current->prev;

                delete current;
                return;
            }
            current = current->next;
        }
    }

    bool find(T value) {
        Node<T>* current = head;
        while (current) {
            if (current->data == value) return true;
            current = current->next;
        }
        return false;
    }

    void display() {
        Node<T>* current = head;
        while (current) {
            cout << current->data << " <-> ";
            current = current->next;
        }
        cout << "NULL" << endl;
    }

    // Перегрузка для wstring
    void displayW() {
        Node<T>* current = head;
        while (current) {
            wcout << current->data << L" <-> ";
            current = current->next;
        }
        wcout << L"NULL" << endl;
    }

    ~DoublyLinkedList() {
        Node<T>* current = head;
        while (current) {
            Node<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
};

int main() {
    // Установка локали для корректного отображения русских символов
    setlocale(LC_ALL, "ru_RU.UTF-8");
    wcout.imbue(locale("ru_RU.UTF-8"));

    // Список целых чисел
    DoublyLinkedList<int> intList;
    intList.append(10);
    intList.append(20);
    intList.append(30);
    cout << "Список int: ";
    intList.display();

    intList.remove(20);
    cout << "После удаления 20: ";
    intList.display();

    cout << "Поиск 30: " << (intList.find(30) ? "Найдено" : "Не найдено") << endl;

    // Список строк (русские слова)
    DoublyLinkedList<wstring> stringList;
    stringList.append(L"Привет");
    stringList.append(L"Мир");
    stringList.append(L"Шаблон");

    wcout << L"Список строк: ";
    stringList.displayW();

    stringList.remove(L"Мир");
    wcout << L"После удаления 'Мир': ";
    stringList.displayW();

    wcout << L"Поиск 'Привет': " << (stringList.find(L"Привет") ? L"Найдено" : L"Не найдено") << endl;

    return 0;
}
