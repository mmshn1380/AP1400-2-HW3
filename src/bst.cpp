#include "bst.h"

BST::Node::Node(int value, Node* left, Node* right)
    : value { value }
    , left { left }
    , right { right }
{
}

BST::Node::Node()
    : value { 0 }
    , left { nullptr }
    , right { nullptr }
{
}
BST::Node::Node(const Node& node)
    : value { node.value }
    , left { node.left }
    , right { node.right }
{
}

BST::Node*& BST::get_root()
{
    return root;
}

void BST::bfs(std::function<void(Node*& node)> func) 
{
    std::queue<BST::Node*> q;
    if (root == nullptr)
        return;
    BST::Node* current;
    q.push(root);
    while (!q.empty()) {
        current=q.front();
        func(current);
        q.pop();
        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }
}

std::ostream& operator<<(std::ostream& stream, BST& bst)
{
    stream << std::string(30, '*') << std::endl;
    size_t sz{0};
    bst.bfs([&](BST::Node*& node) { 
        sz++;
        stream<<*node;
    });
    stream << "Binary Search tree size: " << sz << std::endl;
    stream << std::string(30, '*') << std::endl;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, BST::Node& node){
    stream <<&node ;
    stream.width(10);
    stream<<"  => value:"<<node.value;
    stream.width(10);
    stream<<"left:"<<node.left;
    stream.width(10);
    stream<<"right:"<<node.right<<std::endl;
    return stream;
}

std::partial_ordering BST::Node::operator<=>(int Value)
{
    return value <=> Value;
}

bool BST::Node::operator==(int Value)
{
    return value == Value;
}

BST& BST::operator=(BST& bst)
{
    if (this != &bst) {
        BST::~BST();
        bst.bfs([this](BST::Node*& node) {
            add_node(node->value);
        });
    }
    return *this;
}

BST& BST::operator=(BST&& bst)
{
    if (this == &bst) 
        return *this;
    BST::~BST();
    root = bst.root;
    bst.root = nullptr;
    return *this;
}

BST& BST::operator++()
{
    bfs([&](BST::Node*& node)
    { node->value++; });
    return *this;
}

BST BST::operator++(int)
{
    BST new_b { *this };
    ++(*this);
    return new_b;
}

size_t BST::length()
{
    size_t cnt {0};
    std::function func=[&](BST::Node*& node)
    { 
        cnt++;
    };
    this->bfs(func);
    return cnt;
}

bool BST::add_node(int value)
{
    BST::Node* newNode { new BST::Node(value, nullptr, nullptr) };
    BST::Node* child { new BST::Node()};
    child=root;
    BST::Node* parent { new BST::Node() };
    if (root == nullptr) {
        root = newNode;
        return true;
    } 
    while(child!=nullptr){
        parent=child;
        if (value > child->value){
            child=child->right;
        }
        else if(value < child->value){
            child=child->left;
        }else{
            return false;
        }
    }
    if(parent->left==nullptr  && parent->value>value){
        parent->left=newNode;
        return true;
    }else if(parent->right==nullptr  && parent->value<value){
        parent->right=newNode;
        return true;
    }
    return false;
}
BST::Node** BST::find_node(int value){
    BST::Node** current { new BST::Node* };
    *current=root;
    while(*current!=nullptr){
        if((*current)->value==value){
            return current;
        }else if((*current)->value>value){
            *current=(*current)->left;
        }else{
            *current=(*current)->right;
        }
    }
    return nullptr;
}

BST::Node** BST::find_parrent(int value){
    BST::Node** current {new BST::Node*};
    *current=root;
    if(root->value==value){
        return nullptr;
    }
    while(*current!=nullptr){
        if(((*current)->left!=nullptr && (*current)->left->value==value) || ((*current)->right!=nullptr && (*current)->right->value==value)){
            return current;
        }
        if((*current)->value>value){
            *current=(*current)->left;
        }else if((*current)->value<value){
            *current=(*current)->right;
        }
    }
    return nullptr;
}
BST::Node** BST::find_successor(int value)
{
    BST::Node** current { new BST::Node* };
    *current=*BST::find_node(value);
    if (*current == nullptr)
        return nullptr;
    
    if ((*current)->left == nullptr)
        return current;

    *current = (*current)->left;
    while ((*current)->right != nullptr) {
        (*current) = (*current)->right;
    }
    return current;
}


bool BST::delete_node(int value)
{   
    if (BST::find_node(value) == nullptr)
        return false;

    BST::Node** par {BST::find_parrent(value)};
    BST::Node** changeing_node { BST::find_successor(value) };
    BST::Node** removing_node { BST::find_node(value) };
    if((*removing_node)->value==(*changeing_node)->value){
        if(par==nullptr){
            root=(*removing_node)->right;
            return true;
        }
        if((*par)->left!=nullptr && (*par)->left->value==(*removing_node)->value){
            if ((*removing_node)->right==nullptr){
                (*par)->left=nullptr;
            }else{
                (*par)->left=(*removing_node)->right;
            }
            return true;
        }else{
            if ((*removing_node)->right==nullptr){
                (*par)->right=nullptr;
            }else{
                (*par)->right=(*removing_node)->right;
            }
            return true;
        }
    }else{
        if((*removing_node)->right==nullptr){
            if(*par==nullptr){
                root=(*removing_node)->left;
                return true;
            }
            if((*par)->left==*removing_node){
                (*par)->left=(*removing_node)->left;
                return true;
            }else{
                (*par)->right=(*removing_node)->left;
                return true;
            }
        }else{
            BST::delete_node((*changeing_node)->value);
            (*changeing_node)->left=(*removing_node)->left;
            (*changeing_node)->right=(*removing_node)->right;
            if(par==nullptr){
                root=*changeing_node;
                return true;
            }
            if((*par)->left==*removing_node){
                (*par)->left=*changeing_node;
                return true;
            }else{
                (*par)->right=*changeing_node;
                return true;
            }
        }
    }
}


BST::BST(std::initializer_list<int> nodes)
    : root { nullptr }
{
    for (int i : nodes) {
        this->add_node(i);
    }
}

BST::BST(BST& bst)
    : root { nullptr }
{
    bst.bfs([&](BST::Node*& node) 
    { this->add_node(node->value); });
}

BST::BST(BST&& bst)
{
    root = bst.get_root();
    bst.root = nullptr;
}

BST::~BST()
{
    std::vector<Node*> nodes;
    bfs([&nodes](BST::Node*& node) { nodes.push_back(node); });
    for (auto& node : nodes)
        delete node;
}
