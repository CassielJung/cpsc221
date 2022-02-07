// linked_list.cc -- functions for linked_list lab (cs221)

#include "linked_list.h"

/**
 * Inserts a new Node (with key=newKey) at the head of the linked_list.
 * PRE: head is the first node in a linked_list (if NULL, linked_list is empty)
 * PRE: newKey is the value for the key in the new Node
 * POST: the new Node is now the head of the linked_list
 */
void insert(Node*& head, int newKey) {
  Node * curr = new Node;
  curr->key  = newKey;
  curr->next = head;

  head = curr;
}

/**
 * Print the keys of a linked_list in order, from head to tail
 * PRE: head is the first node in a linked_list (if NULL, linked_list is empty)
 */
void print(Node* head) {
  std::cout << "[";
  for (Node* curr = head; curr != NULL; curr = curr->next){
    std::cout << curr->key;
    if (curr->next != NULL) std::cout << ", ";
  }
  std::cout << "]" << std::endl;
}

/**
 * Returns the size (number of Nodes) of the linked_list
 * PRE: head is the first node in a linked_list (if NULL, linked_list is empty)
 */
int size(Node* head){
  if (! head) return 0;
  return 1 + size(head->next);
}

/**
 * Copies the keys in a linked list to a vector.
 * PRE: head is the first node in a linked_list (if NULL, linked_list is empty)
 * POST: a new vector<int> containing the keys in the correct order has been returned.
 */
std::vector<int> to_vector(Node* head) {
  std::vector<int> result;
  for (Node* curr = head; curr != NULL; curr = curr->next){
    result.push_back(curr->key);
  }
  return result;
}

/**
 * Delete the last Node in the linked_list
 * PRE: head is the first Node in a linked_list (if NULL, linked_list is empty)
 * POST: the last Node of the linked_list has been removed
 * POST: if the linked_list is now empty, head has been changed
 * POST: else head remains the first Node in the linked_list
 */
void delete_last_element(Node*& head){
  // if head is NULL, nothing to delete
  if (head == NULL ) {
    return;
  }

  // if size == 1, delete head
  if (size(head) == 1) {
    delete head;
    head = NULL;
    return;
  }

  // get the second last node
  Node *curr = head;
  while ((curr->next)->next != NULL) {
    curr = curr->next;
  }

  // delete last node
  delete curr->next;
  curr->next = NULL;
}

/**
 * Removes an existing Node (with key=oldKey) from the linked_list.
 * PRE: head is the first node in a linked_list (if NULL, linked_list is empty)
 * PRE: oldKey is the value of the key in the Node to be removed
 * PRE: if no Node with key=oldKey exists, the linked_list has not changed
 * POST: if a Node with key=oldKey was found, then it was deleted
 * POST: other Nodes with key=oldKey might still be in the linked_list
 * POST: head is the new first Node of the linked_list, if updated
 */
void remove(Node*& head, int oldKey) {
  // if head is NULL, list is empty, nothing to delete
  if (head == NULL ) {
    return;
  }

  Node *curr = head;
  
  // head has oldKey
  if (head->key == oldKey) {
    head = head->next;
    delete curr;
    return;
  }

  // find node that its next has oldKey
  while ((curr->next)->key != oldKey) {
    curr = curr->next;

    // Node with oldKey not found
    if (curr == NULL) {
      return;
    }
  }

  // Node was in the middle
  Node *node2remove = curr->next;
  curr->next = node2remove->next;
  delete node2remove;
}

/**
 * Insert a new Node (with key=newKey) after an existing Node (with key=oldKey)
 * If there is no existing Node with key=oldKey, then no action.
 * PRE: head is the first Node in a linked_list (if NULL, linked_list is empty)
 * PRE: oldKey is the value to look for (in the key of an existing Node)
 * PRE: newKey is the value of the key in the new Node (that might be inserted)
 * POST: if no Node with key=oldKey was found, then the linked_list has not changed
 * POST: else a new Node (with key=newKey) is right after the Node with key = oldKey.
 */
void insert_after(Node* head, int oldKey, int newKey){
  Node *newNode = new Node();
  newNode->key = newKey;

  Node *curr = head;
  int found = 0;
  while (curr != NULL) {
    if (curr->key == oldKey) {
      newNode->next = curr->next;
      curr->next = newNode;
      found = 1;
      break;
    }

    curr = curr->next;
  }

  if (!found) {
    delete newNode;
    newNode = NULL;
  }
}

/**
 * Create a new linked_list by merging two existing linked_lists.
 * PRE: list1 is the first Node in a linked_list (if NULL, then it is empty)
 * PRE: list2 is the first Node in another linked_list (if NULL, then it is empty)
 * POST: A new linked_list is returned that contains new Nodes with the keys from
 * the Nodes in list1 and list2, starting with the key of the first Node of list1,
 * then the key of the first Node of list2, etc.
 * When one list is exhausted, the remaining keys come from the other list.
 * For example: [1, 2] and [3, 4, 5] would return [1, 3, 2, 4, 5]
 */
Node* interleave(Node* list1, Node* list2){
  if (list1 == NULL && list2 == NULL) return NULL;

  Node *list3 = new Node();

  Node *curr1 = list1;
  Node *curr2 = list2;
  Node *curr3 = list3;
  int listNum = 1;

  for (int i = 0; i < (size(list1) + size(list2)); i++) {
    if (curr1 == NULL) {
      listNum = 2;
    } else if (curr2 == NULL) {
      listNum = 1;
    }
    
    if (listNum == 1) {
      curr3->key = curr1->key;

      listNum = 2;
      curr1 = curr1->next;
    } else if (listNum == 2) {
      curr3->key = curr2->key;

      listNum = 1;
      curr2 = curr2->next;
    }

    if (i != size(list1)+size(list2)-1) {
      curr3->next = new Node();
      curr3 = curr3->next;
    }
  }

  return list3;

}
