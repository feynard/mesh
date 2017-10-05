#ifndef LIST_HPP
#define LIST_HPP

// Generic, oredered, doubly linked list

using namespace std;

template <class D>
class List {

private:

    // Inner data container
    struct Node {
        D var;
        Node* next;
        Node* prev;
        Node(const D & x): var(x), next(0), prev(0) {}
    };

    Node* root_;        // Head of a list
    Node* tail_;        // Pointer to the tail of a list
    Node* current_;     // Inner iterator
    unsigned int n_;             // Number of elements

public:

    //
    // Constructor and destructor
    //

    List();

    // O(n)
    ~List();

    //
    // Methods
    //

    // Iterator
    void set_iterator();
    bool iterator();
    void iterate();
    D & get_iterator();

    // Add element to the tail, O(1)
    void push(const D & x);

    // Add element to the beginning, O(1)
    void push_head(const D & x);

    // Delete the last (tail) element and return its copy, O(1)
    D pop();

    // Delete the first element and return its copy, O(1)
    D pop_head();

    // ???
    // Delete a specific element, O(n)
//    void remove(const D & x);

    // Removes i-th element
    void remove_by_index(unsigned int i);

    // ???
    // Check if the list contains an element, O(N)
    bool contains(const D& x);

    // Return the i-j-slice of the list (including both ends), O(n)
    List<D> slice(int i, int j);

    // Length, O(1)
    int length();

    // Tail, O(1)
    D & tail();

    // Head, O(1)
    D & root();

    //
    // Operator overloading
    //

    // Indexing operator, O(N)
    D & operator [] (unsigned int i);

    friend istream & operator >> (istream& in, List<D> & list)
    {
        D element;
        in >> element;
        list.push(element);
        return in;
    }

    friend ostream & operator << (ostream& out, List<D> & list)
    {
        out << list.pop();
        return out;
    }

};

//
// Implementation
//

template <class D>
List<D>::List()
{
    root_ = 0, tail_ = 0, n_ = 0;
}

template <class D>
List<D>::~List()
{
   Node *p = root_, *prev;
   while (p) {
       prev = p, p = p -> next;
       delete prev;
   }
}

template <class D>
void List<D>::set_iterator()
{
    current_ = root_;
}

template <class D>
bool List<D>::iterator()
{
    return current_ != 0 ? true : false;
}

template <class D>
void List<D>::iterate()
{
    current_ = current_ -> next;
}

template <class D>
D & List<D>::get_iterator()
{
    return current_ -> var;
}

template <class D>
void List<D>::push(const D & x)
{
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

template <class D>
void List<D>::push_head(const D & x)
{
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

template <class D>
D List<D>::pop()
{
    if (root_ == 0)
        return 0;

    D tmp = tail_ -> var;
    tail_ = tail_ -> prev;
    delete tail_ -> next;
    tail_ -> next = 0;
    n_--;
    return tmp;
}

template <class D>
D List<D>::pop_head()
{
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

/*
template <class D>
void List<D>::remove(const D & x)
{
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
*/

template <class D>
void List<D>::remove_by_index(unsigned int i)
{
    if (i >= n_)
        return;

    n_--;
    Node *p = root_;

    for (unsigned int j = 0; j < i; j++)
        p = p -> next;

    if (p == root_ && p != tail_) {
        root_ = root_ -> next;
        delete root_ -> prev;
        root_ -> prev = 0;
        return;
    } else if (p == tail_ && p != root_) {
        tail_ = p -> prev;
        p -> prev -> next = 0;
        delete p;
        return;
    } else if (p == tail_ && p == root_) {
        delete root_;
        root_ = 0;
        tail_ = 0;
        return;
   }

   p -> prev -> next = p -> next;
   p -> next -> prev = p -> prev;
   delete p;
}

template <class D>
bool List<D>::contains(const D& x)
{
    Node *p = root_;
    while (p) {
        if (p -> var == x)
            return true;

        p = p -> next;
    }

    return false;
}

template <class D>
List<D> List<D>::slice(int i, int j)
{
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

template <class D>
int List<D>::length()
{
    return n_;
}

template <class D>
D & List<D>::tail()
{
    return tail_ -> var;
}

template <class D>
D & List<D>::root()
{
    return root_ -> var;
}

template <class D>
D & List<D>::operator [] (unsigned int i)
{
    Node *p = root_;
    for (unsigned int j = 0; j < i; j++)
        p = p -> next;

    return p -> var;
}

#endif
