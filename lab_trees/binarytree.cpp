/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file, as well as binarytree.h.
 */

#include <iostream>
#include <queue>

using namespace std;

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
  // Call recursive helper function on root
  return height(root);
}

/**
 * Private helper function for the public height function, with an additional
 * Node* parameter to allow for recursive calls. NOTE: use this as an example
 * for writing your own private helper functions.
 * @param subRoot
 * @return The height of the subtree.
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
  // Base case
  if (subRoot == NULL)
    return -1;

  // Recursive definition
  return 1 + max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
  // Your code here
  printLeftToRight(root);

  // Do not remove this line - used for correct print output
  cout << endl;
}

/*
 * Private helper function for public printLeftToRight functionality.
 * Used inOrder traverse method to print left node first then root then right node
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node* subRoot) const
{
  if (subRoot != NULL) {
    printLeftToRight(subRoot->left);
    cout << to_string(subRoot->elem) << " ";
    printLeftToRight(subRoot->right);
  }
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 * (i.e. not creating a flipped copy).
 */
template <typename T>
void BinaryTree<T>::mirror()
{
  // Your code here
  mirror(root);
}

/*
 * Private helper function for public mirror function
 * swap left and right of current node for every node recursively
 */

template <typename T>
void BinaryTree<T>::mirror(Node* subRoot) const
{
  if (subRoot != NULL) {
    mirror(subRoot->left);
    mirror(subRoot->right);

    // swap left and right node
    Node * temp = subRoot->left;
    subRoot->left = subRoot->right;
    subRoot->right = temp;
  }
}

/**
 * Prints out all the possible paths from the root of the tree to any leaf node.
 * That is, all sequences starting at the root node and continuing downwards,
 * ending at a leaf node. Paths ending in a left node should be printed before
 * paths ending in a node further to the right.
 */
template <typename T>
void BinaryTree<T>::printPaths() const
{
  // Your code here
  queue<Node*> path;
  printPaths(root, path);
}

/*
 * Private function to print all Paths in the tree
 * Use queue to save previously passed nodes
 */
template <typename T>
void BinaryTree<T>::printPaths(Node* subRoot, queue<Node*> path) const
{
  // Base case: do nothing and return
  if (subRoot == NULL) {
    return;
  }

  // if subRoot is the leaf, print path
  if (subRoot->right == NULL && subRoot->left == NULL) {
    Node *temp;
    cout << "Path: ";
    path.push(subRoot);
    while (!path.empty()) {
      temp = path.front();
      path.pop();

      cout << to_string(temp->elem) << " ";
    }
    cout << endl;
  } else { // push to queue until the leaf
    path.push(subRoot);
    printPaths(subRoot->left, path);
    printPaths(subRoot->right, path);
  }
}

/**
 * Each node in a tree has a distance from the root node - the depth of that
 * node, or the number of edges along the path from that node to the root. This
 * function returns the sum of the distances of all nodes to the root node (the
 * sum of the depths of all the nodes). Your solution should take O(n) time,
 * where n is the number of nodes in the tree.
 * @return The sum of the distances of all nodes to the root.
 */
template <typename T>
int BinaryTree<T>::sumDistances() const
{
  // Your code here
  return sumDistances(root, 0);
}

template <typename T>
int BinaryTree<T>::sumDistances(const Node* subRoot, int dist) const
{
  if (subRoot == NULL) {
    return 0;
  }

  return dist + sumDistances(subRoot->left, dist+1) + sumDistances(subRoot->right, dist+1);
}

/**
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrdered() const
{
  // Your code here
  return false;
}
