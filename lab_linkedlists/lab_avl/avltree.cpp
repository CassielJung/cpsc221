/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file. IMPORTANT: read avltree.h to understand
 * what to do in this lab.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    /**
     * TODO: your code here. Make sure the following cases are included:
     * Case 1: subtree is NULL
     * Case 2: (key, value) pair should be inserted into left subtree
     * Case 3: (key, value) pair should be inserted into right subtree
     */

    // case1
    if (subtree == NULL) { 
        Node* newNode = new Node(key, value);
        subtree = newNode;
        return;
    } 
    // item already exist
    else if (subtree->key == key && subtree->value == value) {
        return;
    } 
    // case2
    else if (key < subtree->key) {
        if (subtree->left == NULL) subtree->height++;
        insert(subtree->left, key, value);
    } 
    // case3
    else if (key >= subtree->key) {
        if (subtree->right == NULL) subtree->height++;
        insert(subtree->right, key, value);
    }

    // updateHeight of subtree
    updateHeight(subtree);

    // Rebalance tree after insertion (don't remove this)
    rebalance(subtree);
}

template <class K, class V>
void AVLTree<K, V>::updateHeight(Node* node)
{
    // node has no child
    if (node->left == NULL && node->right == NULL) {
        node->height = 0;
    }

    // node has right child only
    else if (node->left == NULL) {
        node->height = node->right->height + 1;
    }

    // node has left child only
    else if (node->right == NULL) {
        node->height = node->left->height + 1;
    }

    // node has both child
    else {
        node->height = max(node->left->height, node->right->height) + 1;
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    *_out << __func__ << endl; // Outputs the rotation name (don't remove this)

    Node* newSubRoot = t->right;
    t->right = newSubRoot->left;
    newSubRoot->left = t;
    t = newSubRoot;

    // TODO: update the heights for t->left and t (in that order)
    updateHeight(t->left);
    updateHeight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    *_out << __func__ << endl; // Outputs the rotation name (don't remove this)

    Node * newSubRoot = t->left;
    t->left = newSubRoot->right;
    newSubRoot->right = t;
    t = newSubRoot;

    updateHeight(t->right);
    updateHeight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    *_out << __func__ << endl; // Outputs the rotation name (don't remove this)

    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    *_out << __func__ << endl; // Outputs the rotation name (don't remove this)

   rotateRight(t->right);
   rotateLeft(t);
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    /**
      * TODO: your code here. Make sure the following cases are included:
      * Cases 1-4: the four cases of tree imbalance as discussed in lecture
      * Case 5: the tree is already balanced. You MUST still correctly update
      * subtree's height
      */
    
    if (subtree == NULL) {
        return;
    }

    int rootBalance;
    if (subtree->left == NULL) {
        rootBalance = 0 - subtree->right->height - 1;
    } else if (subtree->right == NULL) {
        rootBalance = subtree->left->height + 1;
    } else {
        rootBalance = subtree->left->height - subtree->right->height;
    }

    // Case 5: no need to balance
    if (abs(rootBalance) <= 1) {
        return;
    }

    // Case 1: LL imbalance, Case 2: LR imbalance
    if (subtree->left != NULL && rootBalance == 2) {
        int leftBalance;
        if (subtree->left->left == NULL) {
            leftBalance = 0 - subtree->left->right->height - 1;
        } else if (subtree->left->right == NULL) {
            leftBalance = subtree->left->left->height + 1;
        } else {
            leftBalance = subtree->left->left->height - subtree->left->right->height;
        }

        if (leftBalance > 0) {
            rotateRight(subtree);
        } else if (leftBalance < 0) {
            rotateLeftRight(subtree);
        }
    }

    // Case 3: RR imbalance, Case 4: RL imbalance
    if (subtree->right != NULL && rootBalance == -2) {
        int rightBalance;
        if (subtree->right->left == NULL) {
            rightBalance = 0 - subtree->right->right->height - 1;
        } else if (subtree->right->right == NULL) {
            rightBalance = subtree->right->left->height + 1;
        } else {
            rightBalance = subtree->right->left->height - subtree->right->right->height;
        }
        
        if (rightBalance < 0) {
            rotateLeft(subtree);
        } else if (rightBalance > 0) {
            rotateRightLeft(subtree);
        }
    }

    updateHeight(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        remove(subtree->right, key);
    } else {
        /* Reached the node that we need to delete */
        if (subtree->left == NULL && subtree->right == NULL) {
            /* Case 1: Node to remove has no children */
            delete subtree;
            subtree = NULL;
            return;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /**
             * Case 2: Node to remove has two children
             * TODO: your code here. For testing purposes, you
             * should use the PREDECESSOR.
             */

            Node* temp = subtree->left;
            while (temp->right->right != NULL) {
                temp = temp->right;
            }

            Node* predecessor = temp->right;

            // copy key and value of predecessor
            subtree->key = predecessor->key;
            subtree->value = predecessor->value;

            // connect possible child of predecessor to parent of predecessor
            temp->right = predecessor->left;
            updateHeight(temp);

            delete predecessor;
            predecessor = NULL;

        } else {
            /* Case 3: Node to remove has one child */
            Node* curr = subtree;
            subtree = max(subtree->left, subtree->right);
            delete curr;
        }
    }
    updateHeight(subtree);
    rebalance(subtree);
}
