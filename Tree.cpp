#include "Tree.h"
#include <iostream>
#include <limits>

Node::Node(int _id, const std::string& _name, int _bribe)
    : id(_id), name(_name), bribe(_bribe),
      firstChild(nullptr), nextChild(nullptr), parent(nullptr) {}

Tree::Tree() : root(nullptr) {}

std::shared_ptr<Node> Tree::findNode(std::shared_ptr<Node> node, int id) {
    if (!node) return nullptr;
    if (node->id == id) return node;

    // Ищем среди детей
    auto child = node->firstChild;
    while (child) {
        auto found = findNode(child, id);
        if (found) return found;
        child = child->nextChild;
    }

    return nullptr;
}

bool Tree::idExists(std::shared_ptr<Node> node, int id) {
    return findNode(node, id) != nullptr;
}

void Tree::addChild(std::shared_ptr<Node> parent, std::shared_ptr<Node> child) {
    if (!parent->firstChild) {
        // Первый ребёнок
        parent->firstChild = child;
    } else {
        // Ищем последнего брата и добавляем
        auto sibling = parent->firstChild;
        while (sibling->nextChild) {
            sibling = sibling->nextChild;
        }
        sibling->nextChild = child;
    }
    child->parent = parent;
}

void Tree::addOfficial(int id, const std::string& name, int bribe, int parentId) {
    // Проверяем, существует ли уже такой ID
    if (root && idExists(root, id)) {
        throw InvalidInputException("Чиновник с таким ID уже существует!");
    }

    auto newNode = std::make_shared<Node>(id, name, bribe);

    if (parentId == -1) {
        if (root) {
            throw InvalidInputException("Главный чиновник уже существует!");
        }
        root = newNode;
    } else {
        if (!root) {
            throw InvalidInputException("Сначала добавьте главного чиновника (parentId = -1)!");
        }

        auto parent = findNode(root, parentId);
        if (!parent) {
            throw NotFoundException("Чиновник с ID " + std::to_string(parentId) + " не найден!");
        }
        addChild(parent, newNode);
    }
}

int Tree::computeMinCost(std::shared_ptr<Node> node) {
    if (!node) return 0;

    // Если нет детей -> только взятка
    if (!node->firstChild) {
        return node->bribe;
    }

    // Ищем минимальную стоимость среди всех детей
    int minChildCost = std::numeric_limits<int>::max();
    auto child = node->firstChild;

    while (child) {
        int childCost = computeMinCost(child);
        if (childCost < minChildCost) {
            minChildCost = childCost;
        }
        child = child->nextChild;
    }

    return node->bribe + minChildCost;
}

std::shared_ptr<Node> Tree::getBestChild(std::shared_ptr<Node> node) {
    if (!node || !node->firstChild) return nullptr;

    int minCost = std::numeric_limits<int>::max();
    std::shared_ptr<Node> bestChild = nullptr;

    auto child = node->firstChild;
    while (child) {
        int cost = computeMinCost(child);
        if (cost < minCost) {
            minCost = cost;
            bestChild = child;
        }
        child = child->nextChild;
    }

    return bestChild;
}

void Tree::printOptimalPath(std::shared_ptr<Node> node, bool isFirst) {
    if (!node) return;

    // Выводим текущий узел
    if (!isFirst) {
        std::cout << " -> ";
    }
    std::cout << node->id;

    // Если есть дети, идём по лучшему ребёнку
    if (node->firstChild) {
        auto bestChild = getBestChild(node);
        if (bestChild) {
            printOptimalPath(bestChild, false);
        }
    }
}

void Tree::printDetailsPath(std::shared_ptr<Node> node, int level) {
    if (!node) return;

    std::string indent(level * 2, ' ');
    std::cout << indent << "ID " << node->id << " (\"" << node->name
              << "\") - взятка: " << node->bribe << " у.е.\n";

    if (node->firstChild) {
        auto bestChild = getBestChild(node);
        if (bestChild) {
            printDetailsPath(bestChild, level + 1);
        }
    }
}

void Tree::solve() {
    if (!root) {
        throw TreeException("Дерево пусто! Добавьте хотя бы главного чиновника.");
    }

    int totalCost = computeMinCost(root);

    std::cout << "\nРезультат\n";
    std::cout << "Минимальная сумма взяток: " << totalCost << " у.е.\n";
    std::cout << "Порядок получения подписей (по ID чиновников):\n  ";

    // Выводим путь без использования контейнеров
    printOptimalPath(root, true);
    std::cout << std::endl;

    // Выводим детализацию
    std::cout << "\nДетализация:\n";
    printDetailsPath(root, 1);
}

void Tree::printTreeRec(std::shared_ptr<Node> node, int level) {
    if (!node) return;

    std::string indent(level * 2, ' ');
    std::cout << indent << "ID: " << node->id
              << ", \"" << node->name << "\""
              << ", взятка: " << node->bribe << " у.е.\n";

    // Рекурсивно выводим всех детей
    auto child = node->firstChild;
    while (child) {
        printTreeRec(child, level + 1);
        child = child->nextChild;
    }
}

void Tree::printTree() {
    if (!root) {
        std::cout << "Дерево пусто.\n";
        return;
    }

    std::cout << "\nСтруктура министерства:\n";
    printTreeRec(root, 1);
}

void Tree::clear() {
    root = nullptr;
}
