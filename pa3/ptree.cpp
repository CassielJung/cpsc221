/*
*  File:        ptree.cpp
*  Description: Implementation of a partitioning tree class for CPSC 221 PA3
*  Date:        2022-03-03 01:53
*
*               ADD YOUR PRIVATE FUNCTION IMPLEMENTATIONS TO THE BOTTOM OF THIS FILE
*/

#include "ptree.h"
#include "hue_utils.h" // useful functions for calculating hue averages
#include "ptree-private.h" // helper functions

using namespace cs221util;
using namespace std;

// The following definition may be convenient, but is not necessary to use
typedef pair<unsigned int, unsigned int> pairUI;

/////////////////////////////////
// PTree private member functions
/////////////////////////////////

/*
*  Destroys all dynamically allocated memory associated with the current PTree object.
*  You may want to add a recursive helper function for this!
*  POST: all nodes allocated into the heap have been released.
*/
void PTree::Clear() {
  Clear(root);
}

/*
*  Copies the parameter other PTree into the current PTree.
*  Does not free any memory. Should be called by copy constructor and operator=.
*  You may want a recursive helper function for this!
*  PARAM: other - the PTree which will be copied
*  PRE:   There is no dynamic memory associated with this PTree.
*  POST:  This PTree is a physically separate copy of the other PTree.
*/
void PTree::Copy(const PTree& other) {
  Copy(root, other.root);
}

/*
*  Private helper function for the constructor. Recursively builds the tree
*  according to the specification of the constructor.
*  You *may* change this if you like, but we have provided here what we
*  believe will be sufficient to use as-is.
*  PARAM:  im - full reference image used for construction
*  PARAM:  ul - upper-left image coordinate of the currently building Node's image region
*  PARAM:  w - width of the currently building Node's image region
*  PARAM:  h - height of the currently building Node's image region
*  RETURN: pointer to the fully constructed Node
*/
Node* PTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
  // get average colour
  HSLAPixel avgc = getAvg(im, ul, w, h);

  // create current node (A,B are nullptr)
  Node* curr = new Node(ul, w, h, avgc);

  // If current node is the leaf(single pixel) skip building child
  if (w != 1 || h != 1) {
    // get width and height of child
    unsigned int wA, hA, wB, hB;
    pair<unsigned int, unsigned int> ulB;
    if (h > w) { // if height > width, tile vertically
      wA = w; wB = w;
      hA = h / 2;
      hB = h - hA;
      ulB = make_pair(ul.first, ul.second + hA);
    } else { // if width >= height, tile horizontally
      hA = h; hB = h;
      wA = w / 2;
      wB = w - wA;
      ulB = make_pair(ul.first + wA, ul.second);
    }

    // call BuildNode recursively to create child
    curr->A = BuildNode(im, ul, wA, hA);
    curr->B = BuildNode(im, ulB, wB, hB);
  }

  return curr;
}

////////////////////////////////
// PTree public member functions
////////////////////////////////

/*
*  Constructor that builds the PTree using the provided PNG.
*
*  The PTree represents the sub-image (actually the entire image) from (0,0) to (w-1, h-1) where
*  w-1 and h-1 are the largest valid image coordinates of the original PNG.
*  Each node corresponds to a rectangle of pixels in the original PNG, represented by
*  an (x,y) pair for the upper-left corner of the rectangle, and two unsigned integers for the
*  number of pixels on the width and height dimensions of the rectangular sub-image region the
*  node defines.
*
*  A node's two children correspond to a partition of the node's rectangular region into two
*  equal (or approximately equal) size regions which are either tiled horizontally or vertically.
*
*  If the rectangular region of a node is taller than it is wide, then its two children will divide
*  the region into vertically-tiled sub-regions of equal height:
*  +-------+
*  |   A   |
*  |       |
*  +-------+
*  |   B   |
*  |       |
*  +-------+
*
*  If the rectangular region of a node is wider than it is tall, OR if the region is exactly square,
*  then its two children will divide the region into horizontally-tiled sub-regions of equal width:
*  +-------+-------+
*  |   A   |   B   |
*  |       |       |
*  +-------+-------+
*
*  If any region cannot be divided exactly evenly (e.g. a horizontal division of odd width), then
*  child B will receive the larger half of the two subregions.
*
*  When the tree is fully constructed, each leaf corresponds to a single pixel in the PNG image.
* 
*  For the average colour, this MUST be computed separately over the node's rectangular region.
*  Do NOT simply compute this as a weighted average of the children's averages.
*  The functions defined in hue_utils.h and implemented in hue_utils.cpp will be very useful.
*  Computing the average over many overlapping rectangular regions sounds like it will be
*  inefficient, but as an exercise in theory, think about the asymptotic upper bound on the
*  number of times any given pixel is included in an average calculation.
* 
*  PARAM: im - reference image which will provide pixel data for the constructed tree's leaves
*  POST:  The newly constructed tree contains the PNG's pixel data in each leaf node.
*/
PTree::PTree(PNG& im) {
  root = BuildNode(im, make_pair(0, 0), im.width(), im.height());
}

/*
*  Copy constructor
*  Builds a new tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  This tree is constructed as a physically separate copy of other tree.
*/
PTree::PTree(const PTree& other) {
  Copy(other);
}

/*
*  Assignment operator
*  Rebuilds this tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  If other is a physically different tree in memory, all pre-existing dynamic
*           memory in this tree is deallocated and this tree is reconstructed as a
*           physically separate copy of other tree.
*         Otherwise, there is no change to this tree.
*/
PTree& PTree::operator=(const PTree& other) {
  // if this and &other is different in memory
  if (this != &other) {
    // deallocate all dynamic memory in this
    Clear();
    // then copy other
    Copy(other);
  }

  return *this;
}

/*
*  Destructor
*  Deallocates all dynamic memory associated with the tree and destroys this PTree object.
*/
PTree::~PTree() {
  Clear();
}

/*
*  Traverses the tree and puts the leaf nodes' color data into the nodes'
*  defined image regions on the output PNG.
*  For non-pruned trees, each leaf node corresponds to a single pixel that will be coloured.
*  For pruned trees, each leaf node may cover a larger rectangular region that will be
*  entirely coloured using the node's average colour attribute.
*
*  You may want to add a recursive helper function for this!
*
*  RETURN: A PNG image of appropriate dimensions and coloured using the tree's leaf node colour data
*/
PNG PTree::Render() const {
  // get PNG with full dimension
  PNG output;
  output.resize(root->width, root->height);

  Render(output, root);

  return output;
}


/*
*  Trims subtrees as high as possible in the tree. A subtree is pruned
*  (its children are cleared/deallocated) if ALL of its leaves have colour
*  within tolerance of the subtree root's average colour.
*  Pruning criteria should be evaluated on the original tree, and never on a pruned
*  tree (i.e. we expect that Prune would be called on any tree at most once).
*  When processing a subtree, you should determine if the subtree should be pruned,
*  and prune it if possible before determining if it has subtrees that can be pruned.
* 
*  You may want to add (a) recursive helper function(s) for this!
*
*  PRE:  This tree has not been previously pruned (and is not copied/assigned from a tree that has been pruned)
*  POST: Any subtrees (as close to the root as possible) whose leaves all have colour
*        within tolerance from the subtree's root colour will have their children deallocated;
*        Each pruned subtree's root becomes a leaf node.
*/
void PTree::Prune(double tolerance) {
  Prune(root, tolerance, root->avg);
}

/*
*  Returns the total number of nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::Size() const {
  return Size(root);
}

/*
*  Returns the total number of leaf nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::NumLeaves() const {
  return NumLeaves(root);
}

/*
*  Rearranges the nodes in the tree, such that a rendered PNG will be flipped horizontally
*  (i.e. mirrored over a vertical axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped horizontally.
*/
void PTree::FlipHorizontal() {
  FlipHorizontal(root, root->width);  
}

/*
*  Like the function above, rearranges the nodes in the tree, such that a rendered PNG
*  will be flipped vertically (i.e. mirrored over a horizontal axis).
*  This can be achieved by manipulation of the nodes' member attribute(s).
*  Note that this may possibly be executed on a pruned tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*
*  POST: Tree has been modified so that a rendered PNG will be flipped vertically.
*/
void PTree::FlipVertical() {
  FlipVertical(root, root->height);  
}

/*
    *  Provides access to the root of the tree.
    *  Dangerous in practice! This is only used for testing.
    */
Node* PTree::GetRoot() {
  return root;
}

//////////////////////////////////////////////
// PERSONALLY DEFINED PRIVATE MEMBER FUNCTIONS
//////////////////////////////////////////////

/* Find average colour of image block */
HSLAPixel PTree::getAvg(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
  double avghX, avghY, avgH, avgS, avgL;
  double sumhX = 0, sumhY = 0, sumS = 0, sumL = 0;
  unsigned int numPixel = 0;

  HSLAPixel* curr;
  // iterate through every pixel in the valid region
  for (unsigned int x = ul.first; x < w+ul.first; x++) {
    for (unsigned int y = ul.second; y < h+ul.second; y++) {
      curr = im.getPixel(x, y);

      // get sum of each component
      sumhX += Deg2X(curr->h);
      sumhY += Deg2Y(curr->h);
      sumS += curr->s;
      sumL += curr->l;

      numPixel++;
    }
  }

  // get average
  avghX = sumhX / numPixel;
  avghY = sumhY / numPixel;
  avgH = XY2Deg(avghX, avghY);
  avgS = sumS / numPixel;
  avgL = sumL / numPixel;

  return HSLAPixel(avgH, avgS, avgL);
}

/* Recursively find size(number of nodes) of tree */
int PTree::Size(Node* curr) const {
  if (curr == NULL) {
    return 0;
  }

  return Size(curr->A) + Size(curr->B) + 1;
}

/* Recursively find number of leaves of tree */
int PTree::NumLeaves(Node* curr) const {
  if (curr->A == NULL && curr->B == NULL) {
    return 1;
  }
  return NumLeaves(curr->A) + NumLeaves(curr->B);
}

/* Recursively delete all dynamically allocated memory of subtree */
void PTree::Clear(Node*& curr) {
  if (curr == NULL) {
    return;
  }

  Clear(curr->A);
  Clear(curr->B);

  delete[] curr;
  curr = NULL;
}

/* Recursively copy parameters from other to orig */
void PTree::Copy(Node*& orig, const Node* other) {
  if (other == NULL) {
    return;
  }

  // if orig is NULL, create a new node with other's parameter
  orig = new Node(other->upperleft, other->width, other->height, other->avg);
  
  // copy child of other recursively
  Copy(orig->A, other->A);
  Copy(orig->B, other->B);
}

/* Recursively fill output pixel */
void PTree::Render(PNG& output, Node* curr) const {
  if (curr->A == NULL && curr->B == NULL) {
    unsigned int ulX = (curr->upperleft).first;
    unsigned int ulY = (curr->upperleft).second;
    
    for (unsigned int x = ulX; x < (curr->width)+ulX; x++) {
      for (unsigned int y = ulY; y < (curr->height)+ulY; y++) {
        HSLAPixel *p = output.getPixel(x, y);
        p->h = curr->avg.h;
        p->s = curr->avg.s;
        p->l = curr->avg.l;
        p->a = curr->avg.a;
      }
    }

    return;
  }

  Render(output, curr->A);
  Render(output, curr->B);
}

/* Prune subtree recursively */
void PTree::Prune(Node* curr, double tolerance, const HSLAPixel& avgRoot) {
  if (curr == NULL) {
    return;
  }

  // if we need to prune curr, Clear its subtree
  if (shouldPrune(curr, tolerance, avgRoot)) {
    Clear(curr->A);
    Clear(curr->B);
  } else { // else prune their child
    Prune(curr->A, tolerance, curr->A->avg);
    Prune(curr->B, tolerance, curr->B->avg);
  }
}

/* Check if subtree should be pruned */
bool PTree::shouldPrune(Node* curr, double tolerance, const HSLAPixel& avgRoot) {
  // if curr is leaf check colour difference
  if (curr->A == NULL && curr->B == NULL) {
    return (avgRoot.dist(curr->avg) <= tolerance);
  }

  return (shouldPrune(curr->A, tolerance, avgRoot) & shouldPrune(curr->B, tolerance, avgRoot));
}

/* Recursively modify leaves upperleft coordinate (modify x coordinate only) */
void PTree::FlipHorizontal(Node* curr, unsigned int wOrig) {
  if (curr == NULL) {
    return;
  }

  unsigned int upperRightX = curr->upperleft.first + curr->width - 1;
  curr->upperleft.first = wOrig-1-upperRightX;

  FlipHorizontal(curr->A, wOrig);
  FlipHorizontal(curr->B, wOrig);
}

/* Recursively modify leaves upperleft coordinate (modify y coordinate only) */
void PTree::FlipVertical(Node* curr, unsigned int hOrig) {
  if (curr == NULL) {
    return;
  }

  unsigned int lowerleftY = curr->upperleft.second + curr->height - 1;
  curr->upperleft.second = hOrig-1-lowerleftY;

  FlipVertical(curr->A, hOrig);
  FlipVertical(curr->B, hOrig);
}