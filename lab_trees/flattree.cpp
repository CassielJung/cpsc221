#include <vector>
#include <string>
#include <iostream>
#include "binarytree.h"

using namespace std;

template <typename T>
void FlatTree<T>::printLeftToRight() const {
    // Your code here
    printLeftToRight(data, 1);

    // Do not remove this line - used for correct print output
    cout << endl;
}

/*
 * Private helper function for public printLeftToRight functionality
 */
template <typename T>
void FlatTree<T>::printLeftToRight(const vector<T> data, unsigned long index) const
{
  if (index < data.size()) {
    printLeftToRight(data, index*2);
    cout << data[index] << " ";
    printLeftToRight(data, index*2+1);
  }
}

template <typename T>
T& FlatTree<T>::getElement(const string& path) {
  // Your code here
  // handle edge case
  if (path.empty()) {
    return data[1];
  }

  // change string path to char vector
  vector<char> pathChar(path.begin(), path.end());

  int index = 1;
  // Move index based on character on path
  for (unsigned long i = 0; i < pathChar.size(); i++) {
    if (pathChar[i]=='L') {
        index = index * 2;
    } else if (pathChar[i]=='R') {
        index = index * 2 + 1;
    }
  }

  return data[index];
}

template <typename T>
void FlatTree<T>::fromBinaryTree(const BinaryTree<T>& tree) {
  data[0] = 0; // put any number to the 0th element of data
  fromBinaryTree(tree.root, data, 1);
}

/*
 * Private helper function for public fromBinaryTree functionality
 */
template <typename T>
void FlatTree<T>::fromBinaryTree(typename BinaryTree<T>::Node *subRoot, vector<T>& data, long unsigned index) const {
  if (subRoot != NULL) {
    if (data.size()-1 < index) {
      data.resize(index+1);
    }
    data[index] = subRoot->elem;
    fromBinaryTree(subRoot->left, data, index*2);
    fromBinaryTree(subRoot->right, data, index*2+1);
  }
}

template <typename T>
BinaryTree<T> FlatTree<T>::toBinaryTree() const {
  // returning binary tree
  BinaryTree<T> ret = BinaryTree<T>();

  // set the root of new BinaryTree
  ret.root = new typename BinaryTree<T>::Node(data[1]);
  toBinaryTree(data, 1, ret.root);

  return ret;
}

/*
 * Private helper function for public toBinaryTree functionality
 */
template <typename T>
void FlatTree<T>::toBinaryTree(const vector<T>& data, long unsigned index, typename BinaryTree<T>::Node* subRoot) const {
  // Your code here
  if (index*2 < data.size()) {
    // create new left node
    typename BinaryTree<T>::Node* left = new typename BinaryTree<T>::Node(data[index*2]);
    subRoot->left = left;

    toBinaryTree(data, index*2, subRoot->left);
  }

  if (index*2+1 < data.size()) {
    // create new right node
    typename BinaryTree<T>::Node* right = new typename BinaryTree<T>::Node(data[index*2+1]);
    subRoot->right = right;

    toBinaryTree(data, index*2+1, subRoot->right);
  }
}
