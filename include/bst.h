#ifndef BST_H
#define BST_H

#include <functional>
#include <compare>
#include <initializer_list>
#include <string>
#include <iomanip>
#include <iostream>
#include <vector>
#include <queue>

class BST
{
public:
    class Node
    {
    public:
        Node(int value, Node* left, Node* right);
        Node();
        Node(const Node& node);
        friend std::ostream& operator<<(std::ostream& stream, Node& node);
        std::partial_ordering operator<=>(int Value);
        bool operator==(int Value);

        int value;
        Node* left;
        Node* right;
    };
    BST(std::initializer_list<int> nodes);
    BST(BST& bst);
    BST(BST&& bst);
    Node*& get_root();
    void bfs(std::function<void(Node*& node)> func);
    size_t length();
    bool add_node(int value);
    Node** find_node(int value);
    Node** find_parrent(int value);
    Node** find_successor(int value);
    bool delete_node(int value);
    ~BST();
    friend std::ostream& operator<<(std::ostream& stream, BST& bst);
    BST& operator++();
    BST operator++(int);
    BST& operator=(BST& bst);
    BST& operator=(BST&& bst);
private:
    Node* root;
};

#endif //BST_H