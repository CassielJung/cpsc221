#ifndef FLATTREE_H
#define FLATTREE_H

#include <vector>
#include <string>

#include "binarytree.h"

template <class T>
struct FlatTree {
  public:
  vector<T> data;
  FlatTree(): data(1) {}
  FlatTree(const vector<T>& es): data(es.size()+1) {
    std::copy(es.begin(), es.end(), data.begin()+1);
  }
  /**
   * Same as function in BinaryTree, see binarytree.h
   **/
  void printLeftToRight() const;
  /**
   * Get the element from following the given path string.
   * You may assume the path string is valid
   * - contains only 'L' and 'R'
   * - the represented path leads to an existing element in the tree
   * Example:
   *            __ A __
   *         __/       \__
   *       B               E
   *     /   \
   *   C       D
   *
   * getElement("")   => A
   * getElement("R")  => E
   * getElement("LL") => C
   **/
  T& getElement(const string&);
  /**
   * Construct a FlatTree with the same structure as the given BinaryTree.
   * the existing tree is destroyed.
   * you may assume that the input tree is complete.
   **/
  void fromBinaryTree(const BinaryTree<T>&);
  /**
   * Construct a BinaryTree with the same structure as this tree.
   **/
  BinaryTree<T> toBinaryTree() const;

  private:
  void printLeftToRight(const vector<T> data, unsigned long index) const;
  void fromBinaryTree(typename BinaryTree<T>::Node *subRoot, vector<T> &data, long unsigned index) const;
  void toBinaryTree(const vector<T>& data, long unsigned index, typename BinaryTree<T>::Node* subRoot) const;

};
#include "flattree.cpp"
#endif
