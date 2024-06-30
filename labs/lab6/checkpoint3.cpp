#include <iostream>
#include <string>


// Note: It's ok that all the member variables are public for this
// tiny class.  We'll build up to a more robust and complete linked
// list implementation in lecture 11.
template <class T>
class Node {
public:
  T value;
  Node<T> *ptr;
};



template <class T>
void print(Node<T> *data, const std::string &label) {
  std::cout << label;
  Node<T> *tmp = data;
  while (tmp != NULL) {
    std::cout << " " << tmp->value;
    tmp = tmp->ptr;
  }
  std::cout << std::endl;
}



template <class T>
void reverse(Node<T>* &input) {
  //edge case with empty list
  if (input == nullptr) {
    return;
  }

  //create end pointer to final node
  Node<T>* end = input;
  while (end->ptr != nullptr) {
    end = end->ptr;
  }

  //starting at the end, each pointer should point backwards, and the first pointer should be null
  while (input->ptr != nullptr) {
    /*use 2 temp nodes: temp1 loops forwards and points to node on the left, 
    temp2 loops backwards and points to node on the right*/
    Node<T>* temp1 = input;
    while (temp1->ptr->ptr != nullptr) {
      temp1 = temp1->ptr;
    }
    Node<T>* temp2 = end;
    while (temp2->ptr != nullptr) {
      temp2 = temp2->ptr;
    }
    //make temp2's pointer point backwards, and temp1's pointer = null
    temp2->ptr = temp1;
    temp1->ptr = nullptr;
  }

  input = end;
}




int main() {

  // manually create a linked list of notes with 4 elements
  Node<int>* my_list = new Node<int>; 
  my_list->value = 1; 
  my_list->ptr = new Node<int>;
  my_list->ptr->value = 2;
  my_list->ptr->ptr = new Node<int>;
  my_list->ptr->ptr->value = 3;
  my_list->ptr->ptr->ptr = new Node<int>;
  my_list->ptr->ptr->ptr->value = 4;
  my_list->ptr->ptr->ptr->ptr = NULL;

  
  print(my_list,"my_list before");
  reverse(my_list);
  print(my_list,"my_list after ");
  std::cout << std::endl;

  //testing list with 2 nodes
  Node<int>* list2 = new Node<int>;
  list2->value = 1;
  list2->ptr = new Node<int>;
  list2->ptr->value = 2;
  list2->ptr->ptr = nullptr;

  print(list2, "list2 before");
  reverse(list2);
  print(list2, "list2 after");
  std::cout << std::endl;

  //testing list with 1 node
  Node<int>* list3 = new Node<int>;
  list3->value = 1;
  list3->ptr = nullptr;

  print(list3, "list3 before");
  reverse(list3);
  print(list3, "list3 after");
  std::cout << std::endl;

  //testing empty list
  Node<int>* list4 = nullptr;
  print(list4, "list4 before");
  reverse(list4);
  print(list4, "list4 after");
  std::cout << std::endl;

  //testing list with strings
  Node<std::string>* list5 = new Node<std::string>; 
  list5->value = "apple"; 
  list5->ptr = new Node<std::string>;
  list5->ptr->value = "banana";
  list5->ptr->ptr = new Node<std::string>;
  list5->ptr->ptr->value = "cat";
  list5->ptr->ptr->ptr = new Node<std::string>;
  list5->ptr->ptr->ptr->value = "dog";
  list5->ptr->ptr->ptr->ptr = NULL;

  
  print(list5,"list5 before");
  reverse(list5);
  print(list5,"list5 after");
  std::cout << std::endl;


  // Note: We are not deleting any of the Nodes we created...  so this
  // program has memory leaks!  More on this in lecture 11.

}

// ===========================================================================