#include "OrgChart.hpp"

using namespace ariel;

OrgChart::Tree* OrgChart::Tree::find(const string& name){
    if (this->name.empty()){
        throw invalid_argument("can't add sub before root");
    }
    Tree* current = this;
    while(current != nullptr){
        if (current->name == name){
            return current;
        }
        current = current->next_level_order;
    }
    throw invalid_argument("employer doesn't exist");
}

void OrgChart::Tree::insert(Tree* node, Order order){
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
    Tree* child_node = new Tree(child, this);
    child_node->level = level+1;

    //since I'm checking everything using level order, I will update it last.

    //reverse order
    Tree* uncle = next_level_order; //get closest right node with father's level and child
    while (uncle != nullptr && uncle->level == level){
        if (!uncle->children.empty()){
            child_node->insert(uncle->children.at(0), REVERSE_ORDER);
            break;
        }
        uncle = uncle->next_level_order;
    }
    if (uncle == nullptr || uncle->level != level){
        uncle = this; //get leftmost node with father's level (could be father)
        while (uncle->prev_level_order != nullptr && uncle->prev_level_order->level == level){
            uncle = uncle->prev_level_order;
        }
        child_node->insert(uncle, REVERSE_ORDER);
    }

    //preorder
    if (children.empty()){
        child_node->insert(this, PREORDER);
    }
    else{
        Tree* prev_node = children.back();
        while (!prev_node->children.empty()){
            prev_node = prev_node->children.back();
        }
        child_node->insert(prev_node, PREORDER);
    }

    //level order
    if (!children.empty()){
        child_node->insert(children.back(), LEVEL_ORDER);
    }
    else{
        Tree* uncle = prev_level_order; //get closest left node with father's level and child
        while (uncle != nullptr && uncle->level == level){
            if (!uncle->children.empty()){
                child_node->insert(uncle->children.back(), LEVEL_ORDER);
                break;
            }
            uncle = uncle->prev_level_order;
        }
        if (uncle == nullptr || uncle->level != level){
            uncle = this; //get rightmost node with father's level (could be father)
            while (uncle->next_level_order != nullptr && uncle->next_level_order->level == level){
                uncle = uncle->next_level_order;
            }
            child_node->insert(uncle, LEVEL_ORDER);
        }
    }

    children.push_back(child_node);
}

string& OrgChart::Tree::get_name(){
    return name;
}

void OrgChart::Tree::set_name(string new_name){
    name = move(new_name);
}

OrgChart::Tree* OrgChart::Tree::get_next_level_order(){
    return next_level_order;
}

OrgChart::Tree* OrgChart::Tree::get_next_reverse_order(){
    return next_reverse_order;
}

OrgChart::Tree* OrgChart::Tree::get_prev_reverse_order(){
    return prev_reverse_order;
}

OrgChart::Tree* OrgChart::Tree::get_next_preorder(){
    return next_preorder;
}

size_t OrgChart::Tree::size() const{
    return name.size();
}

int OrgChart::Tree::length() const{
    return (int)name.size();
}

char OrgChart::Tree::at(unsigned int index) const{
    return name.at(index);
}

OrgChart::Iterator& OrgChart::Iterator::operator++(){
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
    return current;
}

string& OrgChart::Iterator::operator*(){
    return current->get_name();
}

bool OrgChart::Iterator::operator==(const Iterator& it) const{
    return current == it.current;
}

bool OrgChart::Iterator::operator!=(const Iterator& it) const{
    return !(current == it.current);
}

OrgChart& OrgChart::add_root(const string& name){
    root.set_name(name);
    return *this;
}

OrgChart& OrgChart::add_sub(const string& father, const string& child){
    Tree* father_node = root.find(father);
    father_node->add_sub(child);

    return *this;
}

OrgChart::Iterator OrgChart::begin_level_order(){
    if (root.size() == 0){
        throw invalid_argument("chart is empty!");
    }
    return Iterator(&root, LEVEL_ORDER);
}

OrgChart::Iterator OrgChart::end_level_order() const{
    if (root.size() == 0){
        throw invalid_argument("chart is empty!");
    }
    return Iterator(nullptr, LEVEL_ORDER);
}

OrgChart::Iterator OrgChart::begin_reverse_order(){
    if (root.size() == 0){
        throw invalid_argument("chart is empty!");
    }
    Tree* current = &root;
    while (current != nullptr){
        if (current->get_prev_reverse_order() == nullptr){
            return Iterator(current, REVERSE_ORDER);
        }
        current = current->get_next_level_order();
    }
    return Iterator(nullptr, REVERSE_ORDER);
}

OrgChart::Iterator OrgChart::reverse_order() const{ //end_reverse_order
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

OrgChart::Iterator OrgChart::begin(){
    return begin_level_order();
}

OrgChart::Iterator OrgChart::end() const{
    return end_level_order();
}

string& OrgChart::Iterator::get_name(){
    return current->get_name();
}

string& OrgChart::get_name(){
    return root.get_name();
}

ostream& ariel::operator<<(ostream& os, OrgChart& orgchart){
    os << orgchart.get_name();
    for (auto it = ++orgchart.begin(); it != orgchart.end(); ++it){
        os << ", " << it->get_name();
    }
    return os;
}