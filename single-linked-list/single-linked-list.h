#include <iostream>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template<typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;

        Node(const Type &val, Node *next)
                : value(val), next_node(next) {
        }

        Type value;
        Node *next_node = nullptr;
    };

    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template<typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node *node) : node_(node) {
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType *;
        using reference = ValueType &;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type> &other) noexcept {
            node_ = other.node_;
        }

        BasicIterator &operator=(const BasicIterator &rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type> &rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type> &rhs) const noexcept {
            return node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type> &rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type> &rhs) const noexcept {
            return node_ != rhs.node_;
        }

        BasicIterator &operator++() noexcept {
            assert(&node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto temp(*this);
            ++(*this);
            return temp;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(&node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(&node_ != nullptr);
            return &node_->value;
        }

    private:
        Node *node_ = nullptr;
    };

    size_t size_ = 0;
    Node head_;

public:
    SingleLinkedList() : size_(0) {
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    void PushFront(const Type &value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    using value_type = Type;
    using reference = value_type &;
    using const_reference = const value_type &;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        if (size_) {
            return Iterator{head_.next_node};
        }
        return end();
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        return Iterator{nullptr};
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        if (size_) {
            return Iterator{head_.next_node};
        }
        return cend();
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        return Iterator{nullptr};
    }

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        // Реализуйте самостоятельно
        return ConstIterator{ &head_ };
    }

    void Clear() noexcept {
        Node *current_node = head_.next_node;
        while (current_node != nullptr) {
            Node *nextNode = current_node->next_node;
            delete current_node;
            current_node = nextNode;
        }
        head_.next_node = nullptr;
        size_ = 0;
    }

    ~SingleLinkedList() {
        Clear();
    }

    SingleLinkedList(std::initializer_list<Type> values) : size_(values.size()) {
        Node** cur_node = &head_.next_node;
        for(const auto& value : values){
            *cur_node = new Node(value, nullptr);
            cur_node = &((*cur_node)->next_node);
        }
    }

    SingleLinkedList(const SingleLinkedList &other) : size_(other.size_){
        Node** cur_node = &head_.next_node;
        Node* other_node = other.head_.next_node;
        while (other_node != nullptr){
            *cur_node = new Node(other_node->value, nullptr);
            cur_node = &((*cur_node)->next_node);
            other_node = other_node->next_node;
        }
    }

    SingleLinkedList &operator=(const SingleLinkedList &rhs) {
        if (this != &rhs) {
            SingleLinkedList temp(rhs);
            swap(temp);
        }
        return *this;
    }


    void swap(SingleLinkedList &other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(&pos.node_ != nullptr);
        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator{ pos.node_->next_node };
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(&pos.node_ != nullptr);
        auto temp = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = temp;
        --size_;
        return Iterator{ pos.node_->next_node };
    }

    void PopFront() noexcept {
        if(size_ == 0){
            return;
        }
        EraseAfter(Iterator{ &head_ });
    }
};

template<typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept {
lhs.swap(rhs);
}

template<typename Type>
bool operator==(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }


    for (auto lhs_it = lhs.begin(), rhs_it = rhs.begin(); lhs_it != lhs.end(); ++lhs_it, ++rhs_it) {
        if (*lhs_it != *rhs_it) {
            return false;
        }
    }
    return true;
}

template<typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return !(lhs == rhs);
}

template<typename Type>
bool operator<(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    for (auto lhs_it = lhs.begin(), rhs_it = rhs.begin();
         lhs_it != lhs.end() && rhs_it != rhs.end(); ++lhs_it, ++rhs_it) {
        if (*lhs_it < *rhs_it) {
            return true;
        }
        if (*lhs_it > *rhs_it) {
            return false;
        }
    }
    return lhs.GetSize() < rhs.GetSize();
}

template<typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return !(rhs < lhs);
}

template<typename Type>
bool operator>(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return rhs < lhs;
}

template<typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return !(lhs < rhs);
}