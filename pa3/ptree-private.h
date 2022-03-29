/*
*  File:        ptree-private.h
*  Description: Private functions for the PTree class
*  Date:        2022-03-06 03:30
*
*               DECLARE YOUR PTREE PRIVATE MEMBER FUNCTIONS HERE
*/

#ifndef _PTREE_PRIVATE_H_
#define _PTREE_PRIVATE_H_

#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"

/////////////////////////////////////////////////
// DEFINE YOUR PRIVATE MEMBER FUNCTIONS HERE
//
// Just write the function signatures.
//
// Example:
//
// Node* MyHelperFunction(int arg_a, bool arg_b);
//
/////////////////////////////////////////////////

// For BuildNode()
HSLAPixel getAvg(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h);

// For Size()
int Size(Node* curr) const;

// For NumLeaves()
int NumLeaves(Node* curr) const;

// For Clear()
void Clear(Node*& curr);

// For Copy()
void Copy(Node*& orig, const Node* other);

// For Render()
void Render(PNG& output, Node* curr) const;

// For Prune()
void Prune(Node* curr, double tolerance, const HSLAPixel& avgRoot);
bool shouldPrune(Node* curr, double tolerance, const HSLAPixel& avgRoot);

// For FlipHorizontal()
void FlipHorizontal(Node* curr, unsigned int wOrig);

// For FlipVertical()
void FlipVertical(Node* curr, unsigned int hOrig);

#endif