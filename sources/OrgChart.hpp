#include <string>
#include <iostream>
#include <vector>

using namespace std;

namespace ariel{
    enum Order{LEVEL_ORDER, REVERSE_ORDER, PREORDER};

    class OrgChart{
        private:
            class Tree{
                private:
                    string name;
                    Tree* father;
                    vector<Tree*> children;
                    Tree* next_level_order;
                    Tree* next_reverse_order;
                    Tree* next_preorder;
                    Tree* prev_level_order;
                    Tree* prev_reverse_order;
                    Tree* prev_preorder;
                public:
                    Tree(const string name, Tree* father): name(name), father(father){};
                    Tree& add_node(const string& father, const string child);
                    string get_name();
                    void set_name(const string name);
                    Tree* find(const string& name);
                    Tree* get_next_level_order();
                    Tree* get_next_reverse_order();
                    Tree* get_prev_reverse_order();
                    Tree* get_next_preorder();
            };
            Tree root;
            int size;
            class Iterator{
                private:
                    Tree* current;
                    Order order;
                public:
                    Iterator(Tree* current, const Order order): current(current), order(order){};
                    bool operator==(const Iterator& it);
                    bool operator!=(const Iterator& it);
                    Iterator& operator++();
                    Tree* operator->();
            };
        public:
            OrgChart& add_root(const string name);
            OrgChart& add_sub(const string& father, const string child);
            Iterator begin_level_order();
            Iterator end_level_order();
            Iterator begin_reverse_order();
            Iterator end_reverse_order();
            Iterator begin_preorder();
            Iterator end_preorder();
            Iterator begin();
            Iterator end();
    };
};