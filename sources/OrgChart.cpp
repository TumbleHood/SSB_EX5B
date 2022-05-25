#include "OrgChart.hpp"

using namespace ariel;

OrgChart::Tree* OrgChart::Tree::find(const string& father){
    Tree* current = this;
    while(current->next_level_order != nullptr){
        if (current->next_level_order->name == father){
            return current->next_level_order;
        }
        current = current->next_level_order;
    }
    throw invalid_argument("Father does not exist!");
}

OrgChart::Tree& OrgChart::Tree::add_node(const string& father, const string child){
    Tree* father_node = find(father);
    Tree child_node = Tree(child, father_node);
    vector<Tree*> uncles = father_node->father->children;
    //no brothers
    if (father_node->children.empty()){
        //level order
        Tree* right_cousin = nullptr;
        for (unsigned int i = 0; i < uncles.size(); i++){
            if (uncles.at(i) == father_node){
                for (unsigned int j = i+1; j < uncles.size(); j++){
                    if (!uncles.at(j)->children.empty()){
                        right_cousin = uncles.at(j)->children.at(0);
                        break;
                    }
                }
                break;
            }
        }
        if (right_cousin == nullptr){
            child_node.next_level_order = nullptr;
            Tree* current = this;
            while (current->next_level_order != nullptr){
                current = current->next_level_order;
            }
            child_node.prev_level_order = current;
            current->next_level_order = &child_node;
        }
        else{
            child_node.next_level_order = right_cousin;
            child_node.prev_level_order = right_cousin->prev_level_order;
            right_cousin->next_level_order = &child_node;
        }

        //reverse level order
        if (right_cousin == nullptr){
            for (unsigned int i = 0; i < uncles.size(); i++){
                if (uncles.at(i) == father_node){
                    Tree* current = this;
                    while (current->next_level_order != nullptr){
                        if (current->prev_reverse_order == nullptr){
                            child_node.next_reverse_order = current;
                            current->prev_reverse_order = &child_node;
                            break;
                        }
                        current = current->next_level_order;
                    }
                    break;
                }
                if (!uncles.at(i)->children.empty()){
                    Tree* current_cousin = uncles.at(i)->children.back();
                    if (current_cousin->next_reverse_order == uncles.at(0)){
                        child_node.next_reverse_order = current_cousin->next_reverse_order;
                        child_node.prev_reverse_order = current_cousin;
                        current_cousin->next_reverse_order = &child_node;
                    }
                }
            }
        }
        else{
            child_node.next_reverse_order = right_cousin;
            child_node.prev_reverse_order = right_cousin->prev_reverse_order;
            right_cousin->prev_reverse_order = &child_node;
        }

        //preorder
        child_node.prev_preorder = father_node;
        child_node.next_preorder = father_node->next_preorder;
        father_node->next_preorder = &child_node;
    }
    //there are brothers
    else{
        //level order
        child_node.prev_level_order = father_node->children.back();
        child_node.next_level_order = child_node.prev_level_order->next_level_order;
        child_node.prev_level_order->next_level_order = &child_node;

        //reverse level order
        child_node.prev_reverse_order = father_node->children.back();
        child_node.next_reverse_order = child_node.prev_reverse_order->next_reverse_order;
        child_node.prev_reverse_order->next_reverse_order = &child_node;

        //preorder
        Tree* right_uncle = nullptr;
        for (unsigned int i = 0; i < uncles.size(); i++){
            if (uncles.at(i) == father_node){
                right_uncle = uncles.at(i+1);
                break;
            }
        }

        if (right_uncle == nullptr){
            child_node.next_preorder = nullptr;
            Tree* current = this;
            while (current->next_level_order != nullptr){
                if (current->next_preorder == nullptr){
                    child_node.prev_preorder = current;
                    current->next_preorder = &child_node;
                    break;
                }
                current = current->next_level_order;
            }
        }

        child_node.next_preorder = right_uncle;
        child_node.prev_preorder = right_uncle->prev_preorder;
        right_uncle->prev_preorder = &child_node;
    }
    father_node->children.push_back(&child_node);
}

string OrgChart::Tree::get_name(){
    return name;
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

bool OrgChart::Iterator::operator==(const Iterator& it){
    return current == it.current;
}

bool OrgChart::Iterator::operator!=(const Iterator& it){
    return !(*this==it);
}

OrgChart& OrgChart::add_root(const string name){
    root.set_name(name);
    return *this;
}

OrgChart& OrgChart::add_sub(const string& father, const string child){
    root.add_node(father, child);
    return *this;
}

OrgChart::Iterator OrgChart::begin_level_order(){
    return Iterator(&root, LEVEL_ORDER);
}

OrgChart::Iterator OrgChart::end_level_order(){
    return Iterator(nullptr, LEVEL_ORDER);
}

OrgChart::Iterator OrgChart::begin_reverse_order(){
    Tree* current = &root;
    while (current->get_next_level_order() != nullptr){
        if (current->get_prev_reverse_order() == nullptr){
            return Iterator(current, REVERSE_ORDER);
        }
    }
    throw invalid_argument("This shouldn't appear");
}

OrgChart::Iterator OrgChart::end_reverse_order(){
    return Iterator(nullptr, REVERSE_ORDER);
}

OrgChart::Iterator OrgChart::begin_preorder(){
    return Iterator(&root, PREORDER);
}

OrgChart::Iterator OrgChart::end_preorder(){
    return Iterator(nullptr, PREORDER);
}

OrgChart::Iterator OrgChart::begin(){
    return begin_level_order();
}

OrgChart::Iterator OrgChart::end(){
    return end_level_order();
}