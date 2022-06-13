//--------------------------------------------------------
//  Guy Gur-Arieh  -  System Software B  -  Exercise 5B
//--------------------------------------------------------

#include <string>
#include <iostream>
#include <vector>

using namespace std;

namespace ariel{
    //"Order" enum. used for switch cases
    enum Order{LEVEL_ORDER, REVERSE_ORDER, PREORDER};

    class OrgChart{
        private:
            //"Tree" class. a private class used to store the actual orgchart.
            class Tree{
                private:
                    string name;
                    int level;
                    vector<Tree*> children;
                    Tree* father;
                    Tree* next_level_order;
                    Tree* next_reverse_order;
                    Tree* next_preorder;
                    Tree* prev_level_order;
                    Tree* prev_reverse_order;
                    Tree* prev_preorder;
                public:
                    Tree(const string& name, Tree* father): name(name), level(0), father(father),
                        next_level_order(nullptr), next_reverse_order(nullptr), next_preorder(nullptr),
                        prev_level_order(nullptr), prev_reverse_order(nullptr), prev_preorder(nullptr){};
                    ~Tree(); //destructor
                    Tree(const Tree& tree); //copy constructor
                    Tree(Tree&& tree) = default; //move constructor
                    Tree& operator=(const Tree& tree); //copy assignment operator
                    Tree& operator=(Tree&& tree) noexcept; //move assignment operator
                    void add_sub(const string& child);
                    string& get_name();
                    void set_name(string new_name);
                    Tree* find(const string& name);
                    void insert(Tree* node, Order order);
                    Tree* get_next_level_order();
                    Tree* get_next_reverse_order();
                    Tree* get_prev_reverse_order();
                    Tree* get_next_preorder();
                    size_t size() const;
                    int length() const;
                    char at(unsigned int index) const;
            };
            Tree root;
            //"Iterator" class. used to iterate the orgchart.
            class Iterator{
                private:
                    Tree* current;
                    Order order;
                public:
                    Iterator(Tree* current, const Order order): current(current), order(order){};
                    bool operator==(const Iterator& it) const;
                    bool operator!=(const Iterator& it) const;
                    Iterator& operator++();
                    Tree* operator->();
                    string& operator*();
            };
        public:
            //the OrgChart objects starts as a single "Tree" object with an empty name and no father.
            OrgChart(): root(Tree("", nullptr)){};
            OrgChart& add_root(const string& name);
            OrgChart& add_sub(const string& father, const string& child);
            Iterator begin_level_order();
            Iterator end_level_order() const;
            Iterator begin_reverse_order();
            Iterator reverse_order() const; //end_reverse_order originaly
            Iterator begin_preorder();
            Iterator end_preorder() const;
            Iterator begin();
            Iterator end() const;
            string& get_name();
            size_t size() const;

            friend ostream& operator<<(ostream& os, OrgChart& orgchart);
    };
};