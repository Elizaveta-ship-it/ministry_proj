#include "Tree.h"
#include "Exceptions.h"
#include <iostream>
#include <limits>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

void printMenu() {
    std::cout << "\nМЕНЮ\n";
    std::cout << "1. Добавить чиновника (вручную)\n";
    std::cout << "2. Загрузить министерство из файла\n";
    std::cout << "3. Сгенерировать случайное министерство\n";
    std::cout << "4. Вывести структуру министерства\n";
    std::cout << "5. Решить задачу (минимальная сумма взятки)\n";
    std::cout << "6. Очистить всё дерево\n";
    std::cout << "0. Выйти\n";
    std::cout << "Ваш выбор: ";
}

void addOfficialInteractive(Tree& tree) {
    int id, bribe, parentId;
    std::string name;

    std::cout << "\nДобавление чиновника\n";
    std::cout << "Введите ID чиновника: ";
    std::cin >> id;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InvalidInputException("ID должен быть целым числом!");
    }

    std::cout << "Введите имя или должность: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    if (name.empty()) {
        throw InvalidInputException("Имя не может быть пустым!");
    }

    std::cout << "Введите размер взятки: ";
    std::cin >> bribe;
    if (std::cin.fail() || bribe < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InvalidInputException("Взятка должна быть неотрицательным целым числом!");
    }

    if (tree.isEmpty()) {
        std::cout << "Это будет главный чиновник (без начальника).\n";
        parentId = -1;
    } else {
        std::cout << "Введите ID начальника: ";
        std::cin >> parentId;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InvalidInputException("ID начальника должен быть целым числом!");
        }
    }

    tree.addOfficial(id, name, bribe, parentId);
    std::cout << "Чиновник успешно добавлен.\n";
}

void loadFromFile(Tree& tree) {
    std::string filename;
    std::cout << "\nЗагрузка из файла\n";
    std::cout << "Введите имя файла: ";
    std::cin >> filename;

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw InvalidInputException("Не удалось открыть файл: " + filename);
    }

    tree.clear();

    int n;
    file >> n;
    if (file.fail() || n <= 0) {
        throw InvalidInputException("Неверный формат файла: первая строка должна содержать количество чиновников");
    }

    for (int i = 0; i < n; ++i) {
        int id, bribe, parentId;
        std::string name;

        file >> id >> name >> bribe >> parentId;
        if (file.fail()) {
            throw InvalidInputException("Ошибка чтения данных чиновника " + std::to_string(i + 1));
        }

        try {
            tree.addOfficial(id, name, bribe, parentId);
        } catch (const TreeException& e) {
            throw InvalidInputException("Ошибка при добавлении чиновника ID " + std::to_string(id) + ": " + e.what());
        }
    }

    file.close();
    std::cout << "Дерево успешно загружено из файла. Добавлено чиновников: " << n << "\n";
}

void generateRandomTree(Tree& tree) {
    int count;
    std::cout << "\nГенерация случайного дерева\n";
    std::cout << "Введите количество чиновников: ";
    std::cin >> count;

    if (std::cin.fail() || count <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InvalidInputException("Количество должно быть положительным целым числом!");
    }

    if (count > 1000) {
        std::cout << "Предупреждение: большое количество чиновников может занять много времени. Продолжить? (y/n): ";
        char answer;
        std::cin >> answer;
        if (answer != 'y' && answer != 'Y') {
            std::cout << "Генерация отменена.\n";
            return;
        }
    }

    tree.clear();

    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Создаём главного чиновника (ID = 1)
    std::string rootName = "Главный чиновник_" + std::to_string(1);
    int rootBribe = std::rand() % 100;  // взятка от 0 до 99
    tree.addOfficial(1, rootName, rootBribe, -1);

    // Для остальных чиновников
    for (int id = 2; id <= count; ++id) {
        // Случайное имя
        std::string name = "Чиновник_" + std::to_string(id);

        // Случайная взятка от 0 до 99
        int bribe = std::rand() % 100;

        // Случайный родитель среди уже существующих чиновников (от 1 до id-1)
        int parentId = 1 + (std::rand() % (id - 1));

        try {
            tree.addOfficial(id, name, bribe, parentId);
        } catch (const TreeException& e) {
            throw InvalidInputException("Ошибка при генерации чиновника ID " + std::to_string(id) + ": " + e.what());
        }
    }

    std::cout << "Случайное дерево успешно сгенерировано! Добавлено чиновников: " << count << "\n";
}

int main() {
    Tree tree;
    int choice;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "Министерство чиновников\n";
    std::cout << "Программа для расчёта минимальной суммы взяток\n";

    while (true) {
        printMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число от 0 до 6!\n";
            continue;
        }

        try {
            switch (choice) {
                case 0:
                    std::cout << "\nДо свидания!\n";
                    return 0;

                case 1:
                    addOfficialInteractive(tree);
                    break;

                case 2:
                    loadFromFile(tree);
                    break;

                case 3:
                    generateRandomTree(tree);
                    break;

                case 4:
                    tree.printTree();
                    break;

                case 5:
                    tree.solve();
                    break;

                case 6:
                    tree.clear();
                    std::cout << "Дерево очищено.\n";
                    break;

                default:
                    std::cout << "Неверный пункт меню. Попробуйте снова (0-6).\n";
                    break;
            }
        } catch (const TreeException& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Непредвиденная ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}
