// Generic, oredered, doubly linked list

//
//          root    1       2    ... tail
//          next -> next -> next ... next -> null
//  null <- prev <- prev <- prev ... prev
//

//
// Loops:
//
// list.set_iterator();
// while(list.i()) {
//     ...
//     list.iterate();
// }
//
// for(list.set_iterator(); list.iterator(); list.iterate()) {
//     ...
// }
//


template <class D> class List {

private:

    // Inner data container
    struct Node {
        D var;
        Node* next;
        Node* prev;
        Node(const D& x): var(x), next(0), prev(0) {}
    };

    Node* root_;        // Head of a list
    Node* tail_;        // Pointer to the tail of a list
    Node* current_;     // Inner iterator
    int n_;             // Number of elements

public:

    //
    // Constructor and destructor
    //

    List(): root_(0), tail_(0), n_(0) {}

    // O(n)
    ~List() {
        Node *p = root_, *prev;
        while (p) {
            prev = p, p = p -> next;
            delete prev;
        }
    }

    //
    // Methods
    //

    // Iterator
    void set_iterator()
    { current_ = root_; }

    bool iterator()
    { return current_ != 0 ? true : false; }

    void iterate()
    { current_ = current_ -> next; }

    D& get_iterator()
    { return current_ -> var; }


    // Add element to the tail, O(1)
    void push(const D& x) {
        if (root_ == 0) {
            root_ = new Node(x);
            tail_ = root_;
        } else {
            tail_ -> next = new Node(x);
            tail_ -> next -> prev = tail_;
            tail_ = tail_ -> next;
        }

        n_++;
    }

    // Add element to the beginning, O(1)
    void push_head(const D& x) {
        if (root_ == 0) {
            root_ = new Node(x);
            tail_ = root_;
        } else {
            Node *p = new Node(x);
            p -> next = root_;
            root_ -> prev = p;
            root_ = p;
        }

        n_++;
    }

    // Delete the last (tail) element, O(1)
    D pop() {
        if (root_ == 0)
            return 0;

        D tmp = tail_ -> var;
        tail_ = tail_ -> prev;
        delete tail_ -> next;
        tail_ -> next = 0;
        n_--;
        return tmp;
    }

    // Delete the first element, O(1)
    D pop_head() {
        if (root_ == 0)
            return 0;

        D tmp = root_ -> var;
        if (root_ -> next == 0) {
            delete root_;
            root_ = 0, tail_ = 0;
        } else {
            Node *p = root_;
            root_ = root_ -> next;
            root_ -> prev = 0;
            delete p;
        }

        n_--;
        return tmp;
    }

    // Delete a specific element, O(n)
    void remove(const D& x) {
        Node *p = root_;

        while (p) {
            if (p -> var == x) {
                if (p != root_ && p != tail_) {
                    p -> prev -> next = p -> next;
                    p -> next -> prev = p -> prev;
                    delete p;

                } else if (p == root_) {
                    if (p -> next == 0) {
                        delete root_;
                        root_ = 0, tail_ = 0;
                    }

                    root_ = root_ -> next;
                    delete root_ -> prev;

                } else if (p == tail_) {
                    tail_ = tail_ -> prev;
                    delete tail_ -> next;
                    tail_ -> next = 0;
                }

                n_--;
            }

            p = p -> next;
        }
    }

    // Check if the list contains an element, O(N)
    bool contains(const D& x) {
        Node *p = root_;
        while (p) {
            if (p -> var == x)
                return true;

            p = p -> next;
        }

        return false;
    }

    // Return the i-j-slice of the list (including both ends), O(n)
    List<D> slice(int i, int j) {
        List<D> new_list;
        Node *p = root_;

        // Variable p points to the i-th element
        for (int k = 0; k < i; k++)
            p = p -> next;

        // Push all the necessary elements to the new list
        for (int k = 0; k <= j - i; k++, p = p -> next)
            new_list.push(p -> var);

        return new_list;
    }

    // Length, O(1)
    int length() { return n_; }

    // Tail, O(1)
    D tail() {
        if (root_ == 0)
            return 0;
        else
            return tail_ -> var;
    }

    // Head, O(1)
    D root() {
        if (root_ == 0)
            return 0;
        else
            return root_ -> var;
    }


    //
    // Operator overloading
    //

    // Indexing operator, O(N)
    D& operator [] (int i) {
        Node *p = root_;
        for (int j = 0; j < i; j++)
            p = p -> next;

        return p -> var;
    }

    // Input, push, O(1)
    friend std::istream& operator >> (std::istream& in, List<D>& list) {
        D element;
        in >> element;
        list.push(element);
        return in;
    }

    // Output, equivalent to pop, that is complexity is O(1)
    friend std::ostream& operator << (std::ostream& out, List<D>& list) {
        out << list.pop();
        return out;
    }
};
