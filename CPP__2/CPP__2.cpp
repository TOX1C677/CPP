#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <locale>
using namespace std;

// Базовый класс строительного объекта
class ConstructionObject {
protected:
    string name;
    int floors;
    double area;
    int currentPhase;
    bool isPaused;

public:
    ConstructionObject(string n, int f, double a) : name(n), floors(f), area(a), currentPhase(0), isPaused(false) {}
    virtual void nextStep() = 0;
    virtual void showStatus() const = 0;
    virtual void pause() { isPaused = true; }
    virtual void resume() { isPaused = false; }
    virtual bool getPauseStatus() const { return isPaused; }
    virtual ~ConstructionObject() {}
};

// Фаза строительства
class ConstructionPhase {
public:
    string name;
    double laborCoefficient;
    map<string, double> requiredMaterials;

    ConstructionPhase(string n, double lc, map<string, double> rm)
        : name(n), laborCoefficient(lc), requiredMaterials(rm) {
    }
};

// Стратегии закупки материалов
class MaterialPurchaseStrategy {
public:
    virtual void purchaseMaterials(map<string, double>& inventory, string material, double amount) = 0;
    virtual ~MaterialPurchaseStrategy() {}
};

class JustInTimeStrategy : public MaterialPurchaseStrategy {
public:
    void purchaseMaterials(map<string, double>& inventory, string material, double amount) override {
        inventory[material] += amount;
    }
};

class StockpileStrategy : public MaterialPurchaseStrategy {
public:
    void purchaseMaterials(map<string, double>& inventory, string material, double amount) override {
        inventory[material] += amount * 1.5;
    }
};

class LargeStockpileStrategy : public MaterialPurchaseStrategy {
public:
    void purchaseMaterials(map<string, double>& inventory, string material, double amount) override {
        inventory[material] += amount * 2.0;
    }
};

// Глобальная переменная для отслеживания увеличения стоимости материалов
double priceIncreaseCoefficient = 1.0;  // Начальный коэффициент стоимости

// Реализация строительного объекта
class Building : public ConstructionObject {
private:
    vector<ConstructionPhase> phases;
    map<string, double> usedMaterials;
    double totalCost;
    shared_ptr<MaterialPurchaseStrategy> strategy;

public:
    Building(string n, int f, double a, vector<ConstructionPhase> p, shared_ptr<MaterialPurchaseStrategy> s)
        : ConstructionObject(n, f, a), phases(p), totalCost(0), strategy(s) {
    }

    void nextStep() override {
        if (isPaused) {
            cout << "Строительство приостановлено." << endl;
            return;
        }
        if (currentPhase >= phases.size()) {
            cout << "Объект сдан в эксплуатацию." << endl;
            return;
        }

        ConstructionPhase& phase = phases[currentPhase];
        cout << "Фаза " << phase.name << " начата." << endl;

        for (const auto& mat : phase.requiredMaterials) {
            double amount = mat.second * area;
            double adjustedPrice = mat.second * priceIncreaseCoefficient; // Применяем увеличение стоимости, но не меняем количество
            strategy->purchaseMaterials(usedMaterials, mat.first, amount);
            totalCost += adjustedPrice * amount * 100 / 80; // Стоимость в долларах
        }

        currentPhase++;
    }

    void showStatus() const override {
        cout << "Объект: " << name << ", Этажей: " << floors << ", Площадь: " << area << " м2" << endl;
        cout << "Текущая фаза: " << (currentPhase < phases.size() ? phases[currentPhase].name : "Объект сдан в эксплуатацию") << endl;
        cout << "Статус: " << (isPaused ? "Приостановлено" : "Активно") << endl;
        cout << "Затрачено: " << totalCost << " долларов." << endl;
        cout << "Использованные материалы:" << endl;
        for (const auto& mat : usedMaterials) {
            cout << "  " << mat.first << ": " << mat.second << " ед." << endl;
        }
    }
};

// Меню для управления строительными объектами
void menu(vector<shared_ptr<Building>>& buildings) {
    int choice;
    while (true) {
        cout << "\nМеню:\n";
        cout << "1. Добавить объект строительства\n";
        cout << "2. Перейти к следующему шагу строительства\n";
        cout << "3. Поставить строительство на паузу\n";
        cout << "4. Возобновить строительство\n";
        cout << "5. Показать статус всех объектов\n";
        cout << "6. Выход\n";
        cout << "Выберите действие: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            int floors;
            double area;
            int strategyChoice;
            cout << "Введите название объекта: ";
            cin >> name;
            cout << "Введите количество этажей: ";
            cin >> floors;
            cout << "Введите площадь (м2): ";
            cin >> area;

            cout << "Выберите стратегию закупки материалов:\n";
            cout << "1 - По необходимости\n";
            cout << "2 - С запасом\n";
            cout << "3 - С большим запасом\n";
            cout << "Ваш выбор: ";
            cin >> strategyChoice;

            shared_ptr<MaterialPurchaseStrategy> strategy;
            if (strategyChoice == 2)
                strategy = make_shared<StockpileStrategy>();
            else if (strategyChoice == 3)
                strategy = make_shared<LargeStockpileStrategy>();
            else
                strategy = make_shared<JustInTimeStrategy>();

            vector<ConstructionPhase> phases = {
                {"Фундамент", 2.0, { {"Бетон", 0.5}, {"Арматура", 0.2} }},
                {"Стены", 1.5, { {"Кирпич", 1.0}, {"Раствор", 0.3} }},
                {"Чистовая отделка", 1.3, { {"Штукатурка", 0.5}, {"Краска", 0.3} }},
                {"Крыша", 1.2, { {"Черепица", 0.7}, {"Дерево", 0.4} }}
            };

            buildings.push_back(make_shared<Building>(name, floors, area, phases, strategy));
            cout << "Объект добавлен!" << endl;

            // Увеличиваем коэффициент стоимости на 10%
            priceIncreaseCoefficient *= 1.1;
        }
        else if (choice == 2) {
            for (auto& building : buildings) {
                building->nextStep();
            }
        }
        else if (choice == 3) {
            for (auto& building : buildings) {
                building->pause();
            }
            cout << "Все объекты строительства приостановлены." << endl;
        }
        else if (choice == 4) {
            for (auto& building : buildings) {
                building->resume();
            }
            cout << "Все объекты строительства возобновлены." << endl;
        }
        else if (choice == 5) {
            for (const auto& building : buildings) {
                building->showStatus();
            }
        }
        else if (choice == 6) {
            break;
        }
        else {
            cout << "Неверный ввод, попробуйте снова." << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<shared_ptr<Building>> buildings;
    menu(buildings);
    return 0;
}
