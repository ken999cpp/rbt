#include "RBT.h"
BinarySearchTree::Node::Node(Key key, Value value, 
    Node *parent, Node *left, Node *right): keyValuePair({key, value}), parent(parent), left(left), right(right), isRed(true) {}
BinarySearchTree::Node::~Node()
{
    delete left;
    delete right;
}
BinarySearchTree::Node::Node(const Node &other): keyValuePair(other.keyValuePair), parent(nullptr), right(nullptr), left(nullptr),
    isRed(false)
    {
        if (other.left)
        {
            left = new Node(*other.left);
            left -> parent = this;
        }
        if (other.right)
        {
            right = new Node(*other.right);
            right -> parent = this;
        }
    }
bool BinarySearchTree::Node::operator==(const Node &other) const
{
    return (other.keyValuePair.first == keyValuePair.first && other.keyValuePair.second == keyValuePair.second);
}
void BinarySearchTree::Node::output_node_tree() const
{
    if (left)
    {
        left -> output_node_tree();
    }
    std::cout << "Key: " << keyValuePair.first << " Value: " << keyValuePair.second << std::endl;
    if (right && right -> keyValuePair.first != std::numeric_limits<Key>::max())
    {
        right -> output_node_tree();
    }
}
void BinarySearchTree::balance(Node *x)
{
    while (x != _root && x -> parent -> isRed)
    {
        bool isleft = (x -> parent == x -> parent -> parent -> left);
        Node *uncle = isleft ?  x -> parent -> parent -> right : x -> parent -> parent -> left;
        if (uncle && uncle -> isRed)
        {
            x -> parent -> isRed = false;
            uncle -> isRed = false;
            x -> parent -> parent -> isRed = true;
            x = x -> parent -> parent;
        }
        else
        {
            if (isleft)
            {
                if (x == x -> parent -> right)
                {
                    x = x -> parent;
                    rotateLeft(x);
                }
            }
            else
            {
                if (x == x -> parent -> left)
                {
                    x = x -> parent;
                    rotateRight(x);
                }
            }
            x -> parent -> isRed = false;
            x -> parent -> parent -> isRed = true;
            if (isleft)
            {
                rotateRight(x -> parent -> parent);
            }
            else
            {
                rotateLeft(x -> parent -> parent);
            }
        }
    }
    _root -> isRed = false;
}
BinarySearchTree::Node* BinarySearchTree::rotateLeft(Node *x)
{
    Node *y = x -> right;
    x -> right = y -> left;
    if (y -> left)
    {
        y -> left -> parent = x;
    }
    y -> parent = x -> parent;
    if (!(x -> parent))
    {
        _root = y;
    }
    else if (x == x -> parent -> left)
    {
        x -> parent -> left = y;
    }
    else
    {
        x -> parent -> right = y;
    }
    y -> left = x;
    x -> parent = y;
    return y;
}
BinarySearchTree::Node* BinarySearchTree::rotateRight(Node *x)
{
    Node *y = x -> left;
    x -> left = y -> right;
    if (y -> right)
    {
        y -> right -> parent = x;
    }
    y -> parent = x -> parent;
    if ((!x -> parent))
    {
        _root = y;
    }
    else if (x == x -> parent -> right)
    {
        x -> parent -> right = y;
    }
    else
    {
        x -> parent -> left = y;
    }
    y -> right = x;
    x -> parent = y;
    return y;
}
void BinarySearchTree::Node::insert(const Key &key, const Value &value)
{
    if (key < keyValuePair.first)
    {
        if (!left)
        {
            left = new Node(key, value, this);
        }
        else
        {
            left -> insert(key, value);
        }
    }
    else if (key > keyValuePair.first)
    {
        if (!right)
        {
            right = new Node(key, value, this);
        }
        else
        {
            right -> insert(key, value);
        }
    }
    else
    {
        if (!right)
        {
            right = new Node(key, value, this);
        }
        else
        {
            right -> insert(key, value);
        }
    }
}
void BinarySearchTree::insert(const Key &key, const Value &value)
{
    if (!_root)
    {
        _root = new Node(key, value);
        _root -> isRed = false;
        _root -> right = new Node(std::numeric_limits<Key>::max(), 0);
        _root -> right -> parent = _root;
        _size = 1;
    }
    else
    {
        if (key >= max() -> first)
        {
            _root -> erase(std::numeric_limits<Key>::max());
            _root -> insert(key, value);
            Node *inserted = find(key).getNode();
            if (inserted)
            {
                balance(inserted);
            }

            _root -> insert(std::numeric_limits<Key>::max(), 0);
        }
        else
        {
            _root -> insert(key, value);
            Node *inserted = find(key).getNode();
            if (inserted)
            {
                balance(inserted);
            }
        }
        _size++;
    }
}
BinarySearchTree::Node *BinarySearchTree::Node::erase_child(Node *target)
{
    if (!target -> left && !target -> right)
    {
        delete target;
        return nullptr;
    }
    else if (!target -> left)
    {
        Node *temp = target -> right;
        temp -> parent = target -> parent;
        target -> left = nullptr;
        target -> right = nullptr;
        delete target;
        return temp;
    }
    else if (!target -> right)
    {
        Node *temp = target -> left;
        temp -> parent = target -> parent;
        target -> left = nullptr;
        target -> right = nullptr;
        delete target;
        return temp;
    }
    else
    {
        Node *rt_subtree = target -> right;
        while (rt_subtree -> left)
        {
            rt_subtree = rt_subtree -> left;
        }
        target -> keyValuePair = rt_subtree -> keyValuePair;
        rt_subtree -> parent -> right = erase_child(rt_subtree);
        return target;
    }
}
void BinarySearchTree::Node::erase(const Key &key)
{
    if (key < keyValuePair.first)
    {
        if (left)
        {
            if (left -> keyValuePair.first == key)
            {
                left = erase_child(left);
            }
            else
            {
                left -> erase(key);
            }
        }
        else
        {
            return;
        }
    }
    else if (key > keyValuePair.first)
    {
        if (right)
        {
            if (right -> keyValuePair.first == key)
            {
                right = erase_child(right);
            }
            else
            {
                right -> erase(key);
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
}
void BinarySearchTree::erase(const Key &key)
{
    if (!_root) 
    {
        return;
    }
    while (find(key) != end())
    {
        if (_root)
        {
            Node *nodeToDelete = find(key).getNode();
            Node *parentOfDelete = nodeToDelete ? nodeToDelete -> parent : nullptr;
            if (_root -> keyValuePair.first == key)
            {
                _root = Node::erase_child(_root);
                if (_root)
                {
                    _root -> parent = nullptr;
                    _root -> isRed = false;
                }
            }
            else
            {
                _root -> erase(key);
            }
            _size--;
            if (parentOfDelete)
            {
                balance(parentOfDelete);
            }
        }
    }
}
BinarySearchTree::BinarySearchTree(const BinarySearchTree &other): _size(other._size), _root(nullptr)
{
    if (other._root)
    {
        _root = new Node(*other._root);
    }
}
BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree &other)
{
    if (this != &other)
    {
        delete _root;
        _root = nullptr;
        if (other._root)
        {
            _root = new Node(*other._root);
        }
        else
        {
            _root = nullptr;
        }
        _size = other._size;
    }
    return *this;
}
BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept : _root(other._root), _size(other._size)
{
    other._root = nullptr;
    other._size = 0;
}
BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept
{
    BinarySearchTree temp(std::move(other));
    std::swap(this -> _root, temp._root);
    std::swap(this -> _size, temp._size);
    return *this;
}
BinarySearchTree::~BinarySearchTree()
{
    delete _root;
    _size = 0;
}
BinarySearchTree::Iterator::Iterator(Node *node): _node(node) {}
BinarySearchTree::Node* BinarySearchTree::Iterator::getNode() const
{
    return _node;
}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*()
{
    return _node -> keyValuePair;
}
const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const
{
    return _node -> keyValuePair;
}
std::pair<Key, Value>* BinarySearchTree::Iterator::operator->()
{
    return &(_node -> keyValuePair);
}
const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const
{
    return &(_node -> keyValuePair);
}
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++()
{
    if (_node == nullptr || _node->keyValuePair.first == std::numeric_limits<Key>::max())
    {
        return *this;
    }
    if (_node -> right && _node -> right -> keyValuePair.first == _node -> keyValuePair.first)
    {
        _node = _node -> right;
        return *this;
    }
    if (_node -> right)
    {
        _node = _node -> right;
        if (_node -> keyValuePair.first == std::numeric_limits<Key>::max())
        {
            return *this;
        }
        else
        {
            while (_node -> left)
            {
                _node = _node -> left;
            }
        }
    }
    else
    {
        while (_node -> parent && _node == _node -> parent -> right)
        {
            _node = _node -> parent;
        }
        _node = _node -> parent;
    }
    return *this;
}
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--()
{

    if (!_node)
    {
        return Iterator(nullptr);
    }
    if (_node -> keyValuePair.first == std::numeric_limits<Key>::max())
    {
        _node = _node -> parent;
        return Iterator(_node);
    }
    if (_node -> left)
    {
        _node = _node -> left;
        while (_node -> right)
        {
            _node = _node -> right;
        }
    } 
    else
    {
        Node* parent = _node -> parent;
        while (parent && _node == parent -> left)
        {
            _node = parent;
            parent = parent -> parent;
        }
        _node = parent;
    }
    return *this;
}
BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int)
{
    Iterator tmp = *this;
    --(*this);
    return tmp;
}
bool BinarySearchTree::Iterator::operator==(const Iterator &other) const
{
    return _node == other._node;
}
bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const
{
    return _node != other._node;
}
BinarySearchTree::ConstIterator::ConstIterator(const Node *node): _node(node) {}
const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const
{
    return _node -> keyValuePair;
}
const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const
{
    return &(_node -> keyValuePair);
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++()
{
    if (_node == nullptr || _node->keyValuePair.first == std::numeric_limits<Key>::max())
    {
        return *this;
    }
    if (_node -> right && _node -> right -> keyValuePair.first == _node -> keyValuePair.first)
    {
        _node = _node -> right;
        return *this;
    }
    if (_node -> right)
    {
        _node = _node -> right;
        if (_node -> keyValuePair.first == std::numeric_limits<Key>::max())
        {
            return *this;
        }
        else
        {
            while (_node -> left)
            {
                _node = _node -> left;
            }
        }
    }
    else
    {
        while (_node -> parent && _node == _node -> parent -> right)
        {
            _node = _node -> parent;
        }
        _node = _node -> parent;
    }
    return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int)
{
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--()
{
    if (!_node)
    {
        return ConstIterator(nullptr);
    }
    if (_node -> keyValuePair.first == std::numeric_limits<Key>::max())
    {
        _node = _node -> parent;
        return ConstIterator(_node);
    }
    if (_node -> left)
    {
        _node = _node -> left;
        while (_node -> right)
        {
            _node = _node -> right;
        }
    }
    else
    {
        Node* parent = _node -> parent;
        while (parent && _node == parent -> left)
        {
            _node = parent;
            parent = parent -> parent;
        }
        _node = parent;
    }
    return *this;
}
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int)
{
    ConstIterator tmp = *this;
    --(*this);
    return tmp;
}
bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const
{
    return _node == other._node;
}
bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const
{
    return _node != other._node;
}
BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const
{
    Node *current = _root;
    Node *firstmatch = nullptr;
    while (current)
    {
        if (key < current -> keyValuePair.first)
        {
            current = current -> left;
        }
        else if (key > current  ->keyValuePair.first)
        {
            current = current -> right;
        }
        else
        {
            firstmatch = current;
            while (current -> left && current -> left -> right && current -> left -> right -> keyValuePair.first == key)
            {
                current = current -> left -> right;
                firstmatch = current;
            }
            return ConstIterator(firstmatch);
        }
    }
    return cend();
}
BinarySearchTree::Iterator BinarySearchTree::find(const Key &key)
{
    Node *current = _root;
    Node *firstmatch = nullptr;
    while (current)
    {
        if (key < current -> keyValuePair.first)
        {
            current = current -> left;
        }
        else if (current->keyValuePair.first < key)
        {
            current = current -> right;
        }
        else
        {
            firstmatch = current;
            while (current -> left && current -> left -> right && current -> left -> right -> keyValuePair.first == key)
            {
                current = current -> left -> right;
                firstmatch = current;
            }
            return Iterator(firstmatch);
        }
    }
    return end();
}
std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key)
{
    Iterator itfirst = find(key);
    if (itfirst == end())
    {
        return std::make_pair(itfirst, end());
    }
    Iterator itsecond = itfirst;
    while (itsecond != end() && (*itsecond).first == key)
    {
        itsecond++;
    }
    return std::make_pair(itfirst, itsecond);
}
std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key &key) const
{
    ConstIterator itfirst = find(key);
    if (itfirst == cend())
    {
        return std::make_pair(itfirst, cend());
    }
    ConstIterator itsecond = itfirst;
    while (itsecond != cend() && (*itsecond).first == key)
    {
        itsecond++;
    }
    return std::make_pair(itfirst, itsecond);
}
BinarySearchTree::ConstIterator BinarySearchTree::min() const
{
    if (_root)
    {
        Node *current = _root;
        while (current -> left)
        {
            current = current -> left;
        }
        return ConstIterator(current);
    }
    return cend();
}
BinarySearchTree::ConstIterator BinarySearchTree::max() const
{
    if (_root)
    {
        Node *current = _root;
        while (current -> right)
        {
            current = current -> right;
        }
        return ConstIterator(current -> parent);
    }
    return cend();
}
BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const
{   
    auto er = equalRange(key);
    auto min = er.first;
    for (auto it = er.first; it != er.second; ++it)
    {
        if (min -> second > it -> second)
        {
            min = it;
        }
    }
    return ConstIterator(min);
}
BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const
{
    auto er = equalRange(key);
    auto max = er.first;
    for (auto it = er.first; it != er.second; ++it)
    {
        if (max -> second < it -> second)
        {
            max = it;
        }
    }
    return ConstIterator(max);
}
BinarySearchTree::Iterator BinarySearchTree::begin()
{
    Node *current = _root;
    if (current)
    {
        while (current -> left)
        {
            current = current -> left;
        }
        return Iterator(current);
    }
    return end();
}
BinarySearchTree::Iterator BinarySearchTree::end()
{
    if (!_root)
    {
        throw std::logic_error("Tree is empty");
    }
    Node *current = _root;
    while (current -> right && current -> right -> keyValuePair.first != std::numeric_limits<Key>::max())
    {
        current = current -> right;
    }
    return Iterator(current -> right);
}
BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
    Node *current = _root;
    if (current)
    {
        while (current -> left)
        {
            current = current -> left;
        }
        return ConstIterator(current);
    }
    return cend();
}
BinarySearchTree::ConstIterator BinarySearchTree::cend() const
{
    if (!_root)
    {
        throw std::logic_error("Tree is empty");
    }
    Node *current = _root;
    while (current -> right && current -> right -> keyValuePair.first != std::numeric_limits<Key>::max())
    {
        current = current -> right;
    }
    return ConstIterator(current -> right);
}
size_t BinarySearchTree::size() const
{
    return _size;
}
void BinarySearchTree::output_tree()
{
    if (_root)
    {
        _root -> output_node_tree();
    }
}
size_t BinarySearchTree::traversing(Node *node) const
{
    if (!node)
    {
        return 0;
    }
    size_t left_height = traversing(node -> left);
    size_t right_height = traversing(node -> right);
    return std::max(left_height, right_height) + 1;
}
size_t BinarySearchTree::max_height() const
{
    if (!_root)
    {
        return 0;
    }
    return traversing(_root);
}
std::ostream& operator<<(std::ostream& os, const BinarySearchTree::Iterator& it)
{
    std::pair<Key, Value> p = *it;
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}
std::ostream& operator<<(std::ostream& os, const BinarySearchTree::ConstIterator& it)
{
    std::pair<Key, Value> p = *it;
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}