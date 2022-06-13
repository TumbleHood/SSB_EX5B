//--------------------------------------------------------
//  Guy Gur-Arieh  -  System Software B  -  Exercise 5B
//--------------------------------------------------------

#include "OrgChart.hpp"

using namespace ariel;

OrgChart::Tree* OrgChart::Tree::find(const string& name){
    //this function finds a node in the tree and returns a pointer to it.
    //if it doesn't find one, it throws an error.

    if (this->name.empty()){//the tree is empty
        throw invalid_argument("can't add sub before root");
    }
    Tree* current = this;
    while(current != nullptr){//go over the whole tree via level order
        if (current->name == name){
            return current;
        }
        current = current->next_level_order;
    }
    throw invalid_argument("employer doesn't exist");
}

void OrgChart::Tree::insert(Tree* node, Order order){
    //this function simply "insert" a new node between an existing node and its predecessor
    //(or successor, depending on the order)

    switch(order){
        case(LEVEL_ORDER):
            next_level_order = node->next_level_order;
            prev_level_order = node;
            node->next_level_order = this;
            if (next_level_order != nullptr){
                next_level_order->prev_level_order = this;
            }
            break;
        case(REVERSE_ORDER):
            prev_reverse_order = node->prev_reverse_order;
            next_reverse_order = node;
            node->prev_reverse_order = this;
            if (prev_reverse_order != nullptr){
                prev_reverse_order->next_reverse_order = this;
            }
            break;
        case(PREORDER):
            next_preorder = node->next_preorder;
            prev_preorder = node;
            node->next_preorder = this;
            if (next_preorder != nullptr){
                next_preorder->prev_preorder = this;
            }
            break;
        default:
            break;
    }
}

void OrgChart::Tree::add_sub(const string& child){
    //this function is a helper function and is called with the "OrgChart::add_sub" function.

    Tree* child_node = new Tree(child, this);
    child_node->level = level+1;//the child's level is 1 more than its father's

    //since I'm checking everything using level order, I will update it last.

    //reverse order
    Tree* uncle = next_level_order; //get closest right node with father's level and child
    while (uncle != nullptr && uncle->level == level){
        if (!uncle->children.empty()){
            child_node->insert(uncle->children.at(0), REVERSE_ORDER);//add the leftmost child
            break;
        }
        uncle = uncle->next_level_order;
    }
    if (uncle == nullptr || uncle->level != level){
        uncle = this; //get leftmost node with father's level (could be father)
        while (uncle->prev_level_order != nullptr && uncle->prev_level_order->level == level){
            uncle = uncle->prev_level_order;
        }
        child_node->insert(uncle, REVERSE_ORDER);//add the leftmost node
    }

    //preorder
    if (children.empty()){//if there are no brothers, the previous node preorder-wise is the father.
        child_node->insert(this, PREORDER);
    }
    else{
        Tree* prev_node = children.back();
        while (!prev_node->children.empty()){//search for the last rightmost child.
            prev_node = prev_node->children.back();
        }
        child_node->insert(prev_node, PREORDER);
    }

    //level order
    if (!children.empty()){//if there are brothers, the previous node level-order-wise is the last brother.
        child_node->insert(children.back(), LEVEL_ORDER);
    }
    else{
        Tree* uncle = prev_level_order; //get closest left node with father's level and child
        while (uncle != nullptr && uncle->level == level){
            if (!uncle->children.empty()){
                child_node->insert(uncle->children.back(), LEVEL_ORDER);//add the rightmost child
                break;
            }
            uncle = uncle->prev_level_order;
        }
        if (uncle == nullptr || uncle->level != level){
            uncle = this; //get rightmost node with father's level (could be father)
            while (uncle->next_level_order != nullptr && uncle->next_level_order->level == level){
                uncle = uncle->next_level_order;
            }
            child_node->insert(uncle, LEVEL_ORDER);//add the rightmost node
        }
    }

    children.push_back(child_node);//finally, add the child.
}

string& OrgChart::Tree::get_name(){
    //returns the name
    return name;
}

void OrgChart::Tree::set_name(string new_name){
    //changing the name. clang-tidy recommended using "move".
    name = move(new_name);
}

OrgChart::Tree* OrgChart::Tree::get_next_level_order(){
    //used for iteration
    return next_level_order;
}

OrgChart::Tree* OrgChart::Tree::get_next_reverse_order(){
    //used for iteration
    return next_reverse_order;
}

OrgChart::Tree* OrgChart::Tree::get_prev_reverse_order(){
    //used to get to the first node while iterating using reverse level order.
    return prev_reverse_order;
}

OrgChart::Tree* OrgChart::Tree::get_next_preorder(){
    //used for iteration
    return next_preorder;
}

size_t OrgChart::Tree::size() const{
    //returns the length of the name
    return name.size();
}

int OrgChart::Tree::length() const{
    //same as size
    return (int)name.size();
}

char OrgChart::Tree::at(unsigned int index) const{
    //returns a specific character in the name
    return name.at(index);
}

OrgChart::Iterator& OrgChart::Iterator::operator++(){
    //proceed with the iteration.
    switch(order){
        case(LEVEL_ORDER):
            current = current->get_next_level_order();
            break;
        case(REVERSE_ORDER):
            current = current->get_next_reverse_order();
            break;
        case(PREORDER):
            current = current->get_next_preorder();
            break;
        default:
            break;
    }
    return *this;
}

OrgChart::Tree* OrgChart::Iterator::operator->(){
    //returns the "Tree*" object
    return current;
}

string& OrgChart::Iterator::operator*(){
    //returns the name of the current "Tree*" object
    return current->get_name();
}

bool OrgChart::Iterator::operator==(const Iterator& it) const{
    //pointers comparison
    return current == it.current;
}

bool OrgChart::Iterator::operator!=(const Iterator& it) const{
    //self explanatory
    return !(current == it.current);
}

OrgChart& OrgChart::add_root(const string& name){
    //simply changes the name of the root from "" (empty)
    root.set_name(name);
    return *this;
}

OrgChart& OrgChart::add_sub(const string& father, const string& child){
    //calls the helper function "Tree::add_sub"

    Tree* father_node = root.find(father);
    father_node->add_sub(child);

    return *this;
}

OrgChart::Iterator OrgChart::begin_level_order(){
    if (root.size() == 0){//check if the name is not "" (empty), thus checking if there ever was a root
        throw invalid_argument("chart is empty!");
    }
    return Iterator(&root, LEVEL_ORDER);//return the matching iterator, starting from the root
}

OrgChart::Iterator OrgChart::end_level_order() const{
    if (root.size() == 0){
        throw invalid_argument("chart is empty!");
    }
    return Iterator(nullptr, LEVEL_ORDER);//returns a null pointer, the end of the iteration
}

OrgChart::Iterator OrgChart::begin_reverse_order(){
    if (root.size() == 0){
        throw invalid_argument("chart is empty!");
    }
    Tree* current = &root;
    while (current != nullptr){//find the node with no previous reverse level order node
        if (current->get_prev_reverse_order() == nullptr){
            return Iterator(current, REVERSE_ORDER);
        }
        current = current->get_next_level_order();
    }
    return Iterator(nullptr, REVERSE_ORDER);
}

OrgChart::Iterator OrgChart::reverse_order() const{ //end_reverse_order originaly
    if (root.size() == 0){
        throw invalid_argument("chart is empty!");
    }
    return Iterator(nullptr, REVERSE_ORDER);
}

OrgChart::Iterator OrgChart::begin_preorder(){
    if (root.size() == 0){
        throw invalid_argument("chart is empty!");
    }
    return Iterator(&root, PREORDER);
}

OrgChart::Iterator OrgChart::end_preorder() const{
    if (root.size() == 0){
        throw invalid_argument("chart is empty!");
    }
    return Iterator(nullptr, PREORDER);
}

OrgChart::Iterator OrgChart::begin(){//uses level order
    return begin_level_order();
}

OrgChart::Iterator OrgChart::end() const{//uses level order
    return end_level_order();
}

string& OrgChart::get_name(){
    //returns the name
    return root.get_name();
}

ostream& ariel::operator<<(ostream& os, OrgChart& orgchart){
    //prints in level order
    os << orgchart.get_name();
    for (auto it = ++orgchart.begin(); it != orgchart.end(); ++it){
        os << ", " << *it;
    }
    return os;
}

OrgChart::Tree::~Tree(){ //destructor
    //every member of the tree (except "this") was made using new.
    //we go to the next node via level order and preform the destructor on him, until we reach a null pointer.

    Tree* current = next_level_order;
    delete current;
}

OrgChart::Tree::Tree(const Tree& tree){ //copy constructor
    name = tree.name;
    level = tree.level;
    children = tree.children;
    father = tree.father;
    next_level_order = tree.next_level_order;
    next_reverse_order = tree.next_reverse_order;
    next_preorder = tree.next_preorder;
    prev_level_order = tree.prev_level_order;
    prev_reverse_order = tree.prev_reverse_order;
    prev_preorder = tree.prev_preorder;
    tree.~Tree();
}

OrgChart::Tree& OrgChart::Tree::operator=(const Tree& tree){ //copy assignment operator
    if (this == &tree){
        return *this;
    }
    *this = Tree(tree);
    return *this;
}

OrgChart::Tree& OrgChart::Tree::operator=(Tree&& tree) noexcept{ //move assignment operator
    name = move(tree.name);
    level = tree.level;
    children = move(tree.children);
    father = tree.father;
    next_level_order = tree.next_level_order;
    next_reverse_order = tree.next_reverse_order;
    next_preorder = tree.next_preorder;
    prev_level_order = tree.prev_level_order;
    prev_reverse_order = tree.prev_reverse_order;
    prev_preorder = tree.prev_preorder;
    tree.~Tree();
    return *this;
}