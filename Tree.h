#ifndef UNTITLED4_TREE_H
#define UNTITLED4_TREE_H

#include "Exceptions.h"
#include <string>
#include <memory>

class Node {
public:
    int id;
    std::string name;
    int bribe;

    // Представление дерева: первый ребёнок и следующий ребёнок
    std::shared_ptr<Node> firstChild;
    std::shared_ptr<Node> nextChild;
    std::shared_ptr<Node> parent;

    Node(int _id, const std::string& _name, int _bribe);
};

class Tree {
private:
    std::shared_ptr<Node> root;

    // Поиск узла по ID (рекурсивный обход всего дерева)
    std::shared_ptr<Node> findNode(std::shared_ptr<Node> node, int id);

    // Добавление ребёнка к родителю
    void addChild(std::shared_ptr<Node> parent, std::shared_ptr<Node> child);

    // Рекурсивные вспомогательные функции
    int computeMinCost(std::shared_ptr<Node> node);
    void printTreeRec(std::shared_ptr<Node> node, int level);

    // Вывод пути минимальной стоимости
    void printOptimalPath(std::shared_ptr<Node> node, bool isFirst);

    // Вывод детализации пути
    void printDetailsPath(std::shared_ptr<Node> node, int level);

    // Проверка существования ID
    bool idExists(std::shared_ptr<Node> node, int id);

    // Получение лучшего ребёнка
    std::shared_ptr<Node> getBestChild(std::shared_ptr<Node> node);

public:
    Tree();
    ~Tree() = default;

    void addOfficial(int id, const std::string& name, int bribe, int parentId);
    void solve();
    void printTree();
    void clear();
    bool isEmpty() const { return root == nullptr; }
};

#endif //UNTITLED4_TREE_H
