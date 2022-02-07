// File:        imglist.cpp
// Date:        2022-01-20 03:22
// Description: Contains partial implementation of ImgList class
//              for CPSC 221 2021W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "imglist.h"

#include <math.h> // provides fmin and fabs functions
#include <vector>

// #include <iostream>
// using namespace std;

/**************************
* MISCELLANEOUS FUNCTIONS *
**************************/

/*
* This function is NOT part of the ImgList class,
* but will be useful for one of the ImgList functions.
* Returns the "difference" between two hue values.
* PRE: hue1 is a double between [0,360).
* PRE: hue2 is a double between [0,360).
* 
* The hue difference is the absolute difference between two hues,
* but takes into account differences spanning the 360 value.
* e.g. Two pixels with hues 90 and 110 differ by 20, but
*      two pixels with hues 5 and 355 differ by 10.
*/
double HueDiff(double hue1, double hue2) {
  double dist1 = abs(hue1 - hue2);
  double dist2 = 360 - dist1;
  return min(dist1, dist2);
}

/*
 * Helper function to calculate average value of Hue
 * This function will be used to make sure h is in the range [0,360)
 */
double colourWheel (double h) {
  double retH = h;
  if (retH >=360) {
    retH -= 360;
  } else if (retH < 0) {
    retH += 360;
  }
  return retH;
}

/* 
 * Function to calculate average value of Hue
 * Return value is the average between two given Hue
 * Return value must be in the range [0,360) and also must be in the
 * same distance from two given Hue
 */
double avgHue (double h1, double h2) {
  double DELTA = 0.00001; // tiny positive number

  double halfDist = HueDiff(h1, h2) / 2;                 // distance from h1 and h2 to average
  double possibleAvg[4] = {colourWheel(h1-halfDist),     // hue in the halfDist from h1 and h2
                           colourWheel(h1+halfDist),
                           colourWheel(h2-halfDist),
                           colourWheel(h2+halfDist)};

  vector <double> avgH;
  for (int i = 0; i < 4; i++) {
    for (int j = i+1; j < 4; j++) {
      if (fabs(possibleAvg[i] - possibleAvg[j]) <= (DELTA/2)) {           // common value in possibleAvg[]
        avgH.push_back(possibleAvg[i]);                 // is the average hue value
      }
    }
  }

  sort(avgH.begin(), avgH.end());                       // when h1 and h2 are dimetric, it is possible
  return avgH.front();                                  // there exist two common value found, then get the min value
}

/*
 * Function to calculate average colour
 * Need to consider all aspects in colour (s, l, a, h)
 */
HSLAPixel *average(HSLAPixel *colour1, HSLAPixel *colour2) {
  HSLAPixel *avgColour = new HSLAPixel();

  avgColour->s = (colour1->s + colour2->s) / 2;
  avgColour->l = (colour1->l + colour2->l) / 2;
  avgColour->a = (colour1->a + colour2->a) / 2;
  avgColour->h = avgHue(colour1->h, colour2->h);

  return avgColour;
}

/*********************
* CONSTRUCTORS, ETC. *
*********************/

/*
* Default constructor. Makes an empty list
*/
ImgList::ImgList() {
  // set appropriate values for all member attributes here
  northwest = NULL;
  southeast = NULL;
}

/*
* Creates a list from image data
* PRE: img has dimensions of at least 1x1
*/
ImgList::ImgList(PNG& img) {
  // build the linked node structure and set the member attributes appropriately
  unsigned int width = img.width();
  unsigned int height = img.height();

  ImgNode *prev, *north = NULL;
  ImgNode *head_row;
  for (unsigned int y = 0; y < height; y++) {
    prev = NULL;
    for (unsigned int x = 0; x < width; x++) {
      ImgNode *curr = new ImgNode(); // create new node
      curr->colour = *img.getPixel(x,y); // set colour of the node

      // assign head_row node which is used to set north
      if (x == 0) head_row = curr;

      // set entry point to ImgList
      if (x == 0 && y == 0) northwest = curr;
      if (x == width-1 && y == height-1) southeast = curr;

      // set neighbours
      if (x > 0) {
        curr->west = prev;
        prev->east = curr;
      }

      if (y > 0) {
        curr->north = north;
        north->south = curr;
      }

      // update prev and north
      prev = curr;
      north = (y == 0) ? NULL : north->east; // north == NULL for top row
    }
    north = head_row;
  }
}

/*
* Copy constructor.
* Creates this this to become a separate copy of the data in otherlist
*/
ImgList::ImgList(const ImgList& otherlist) {
  // build the linked node structure using otherlist as a template
  Copy(otherlist);
}

/*
* Assignment operator. Enables statements such as list1 = list2;
*   where list1 and list2 are both variables of ImgList type.
* POST: the contents of this list will be a physically separate copy of rhs
*/
ImgList& ImgList::operator=(const ImgList& rhs) {
  // Re-build any existing structure using rhs as a template
  
  if (this != &rhs) { // if this list and rhs are different lists in memory
    // release all existing heap memory of this list
    Clear();    
    
    // and then rebuild this list using rhs as a template
    Copy(rhs);
  }
  
  return *this;
}

/*
* Destructor.
* Releases any heap memory associated with this list.
*/
ImgList::~ImgList() {
  // Ensure that any existing heap memory is deallocated
  Clear();
}

/************
* ACCESSORS *
************/

/*
* Returns the horizontal dimension of this list (counted in nodes)
* Note that every row will contain the same number of nodes, whether or not
*   the list has been carved.
* We expect your solution to take linear time in the number of nodes in the
*   x dimension.
*/
unsigned int ImgList::GetDimensionX() const {
  // replace the following line with your implementation
  ImgNode * curr = northwest;
  unsigned int count = 0;

  while (curr != NULL) {
    count++;
    curr = curr->east;
  }

  return count;
}

/*
* Returns the vertical dimension of the list (counted in nodes)
* It is useful to know/assume that the grid will never have nodes removed
*   from the first or last columns. The returned value will thus correspond
*   to the height of the PNG image from which this list was constructed.
* We expect your solution to take linear time in the number of nodes in the
*   y dimension.
*/
unsigned int ImgList::GetDimensionY() const {
  // replace the following line with your implementation
  ImgNode * curr = northwest;
  unsigned int count = 0;

  while (curr != NULL) {
    count++;
    curr = curr->south;
  }

  return count;
}

/*
* Returns the horizontal dimension of the list (counted in original pixels, pre-carving)
* The returned value will thus correspond to the width of the PNG image from
*   which this list was constructed.
* We expect your solution to take linear time in the number of nodes in the
*   x dimension.
*/
unsigned int ImgList::GetDimensionFullX() const {
  // replace the following line with your implementation
  ImgNode * curr = northwest;
  unsigned int count = 0;

  while (curr != NULL) {
    count += (curr->skipright + 1);
    curr = curr->east;
  }

  return count;
}

/*
* Returns a pointer to the node which best satisfies the specified selection criteria.
* The first and last nodes in the row cannot be returned.
* PRE: rowstart points to a row with at least 3 physical nodes
* PRE: selectionmode is an integer in the range [0,1]
* PARAM: rowstart - pointer to the first node in a row
* PARAM: selectionmode - criterion used for choosing the node to return
*          0: minimum luminance across row, not including extreme left or right nodes
*          1: node with minimum total of "hue difference" with its left neighbour and with its right neighbour.
*        In the (likely) case of multiple candidates that best match the criterion,
*        the left-most node satisfying the criterion (excluding the row's starting node)
*        will be returned.
* A note about "hue difference": For PA1, consider the hue value to be a double in the range [0, 360).
* That is, a hue value of exactly 360 should be converted to 0.
* The hue difference is the absolute difference between two hues,
* but be careful about differences spanning the 360 value.
* e.g. Two pixels with hues 90 and 110 differ by 20, but
*      two pixels with hues 5 and 355 differ by 10.
*/
ImgNode* ImgList::SelectNode(ImgNode* rowstart, int selectionmode) {
  // add your implementation below
  ImgNode* selected;
  ImgNode* curr = rowstart->east;

  if (!selectionmode) {
    double minLum = 2.0; // arbitary number that is over the maximum possible luminance

    while (curr->east != NULL) {
      if (curr->colour.l < minLum) {
        minLum = curr->colour.l;
        selected = curr;
      }
      curr = curr->east;
    }
  } else {
    double minHueDiff = 1000.0; // arbitary number that is over the maximum possible hue
    double currHueDiff;
    ImgNode *left, *right;

    while (curr->east != NULL) {
      left = curr->west;
      right = curr->east;

      // calculate total hue difference with current node's left and right neighbour
      currHueDiff = HueDiff(left->colour.h, curr->colour.h) + HueDiff(right->colour.h, curr->colour.h);

      if (currHueDiff < minHueDiff) {
        minHueDiff = currHueDiff;
        selected = curr;
      }

      curr = curr->east;
    }
  }

  return selected;
}


/*
* Renders this list's pixel data to a PNG, with or without filling gaps caused by carving.
* PRE: fillmode is an integer in the range of [0,2]
* PARAM: fillgaps - whether or not to fill gaps caused by carving
*          false: render one pixel per node, ignores fillmode
*          true: render the full width of the original image,
*                filling in missing nodes using fillmode
* PARAM: fillmode - specifies how to fill gaps
*          0: solid, uses the same colour as the node at the left of the gap
*          1: solid, using the averaged values (all channels) of the nodes at the left and right of the gap
*             Note that "average" for hue will use the closer of the angular distances,
*             e.g. average of 10 and 350 will be 0, instead of 180.
*             Average of diametric hue values will use the smaller of the two averages
*             e.g. average of 30 and 210 will be 120, and not 280
*                  average of 170 and 350 will be 80, and not 260
*          2: *** OPTIONAL - FOR BONUS ***
*             linear gradient between the colour (all channels) of the nodes at the left and right of the gap
*             e.g. a gap of width 1 will be coloured with 1/2 of the difference between the left and right nodes
*             a gap of width 2 will be coloured with 1/3 and 2/3 of the difference
*             a gap of width 3 will be coloured with 1/4, 2/4, 3/4 of the difference, etc.
*             Like fillmode 1, use the smaller difference interval for hue,
*             and the smaller-valued average for diametric hues
*/
PNG ImgList::Render(bool fillgaps, int fillmode) const {
  
  PNG outpng; //this will be returned later. Might be a good idea to resize it at some point.
  unsigned int width = fillgaps ? GetDimensionFullX() : GetDimensionX();
  unsigned int height = GetDimensionY();
  outpng.resize(width, height);

  // ImgNode to help traversing
  ImgNode *curr = northwest;
  ImgNode *head_row = northwest;

  HSLAPixel *pixel; // pixels on png to colour

  HSLAPixel left, right; // pixels to help fill gaps
  HSLAPixel *avgColour; // pixel to represent average colour

  for (unsigned int y = 0; y < height; y++) {
    head_row = head_row->south;
    for (unsigned int x = 0; x < width; x++) {
      // 1. get pixel
      pixel = outpng.getPixel(x, y);

      // 2. set colour
      *pixel = curr->colour;

      // 3. If there is a gap, based on fillgap take appropriate action
      if (curr->skipright > 0 && fillgaps) {
        left = curr->colour;
        right = (curr->east)->colour;

        // set colour to gap pixels based on fillmode
        for (int numGap = curr->skipright; numGap > 0; numGap--) {
          x++;
          pixel = outpng.getPixel(x,y);

          if (fillmode) {
            avgColour = average(&left, &right);
            *pixel = *avgColour;
            delete avgColour; // delete used average which won't be used again
            avgColour = NULL;
          } else {
            *pixel = left;
          }
        }
      }

      // 4. move curr to next node
      curr = curr->east;
    }
    // move curr to the head of next row
    curr = head_row;
  }

  return outpng;
}


/************
* MODIFIERS *
************/

/*
* Removes exactly one node from each row in this list, according to specified criteria.
* The first and last nodes in any row cannot be carved.
* PRE: this list has at least 3 nodes in each row
* PRE: selectionmode is an integer in the range [0,1]
* PARAM: selectionmode - see the documentation for the SelectNode function.
* POST: this list has had one node removed from each row. Neighbours of the created
*       gaps are linked appropriately, and their skip values are updated to reflect
*       the size of the gap.
*/
void ImgList::Carve(int selectionmode) {
  // add your implementation here
  ImgNode *head_row = northwest;
  ImgNode *selNode, *north, *west, *south, *east;

  for (unsigned y = 0; y < GetDimensionY(); y++) {
    // select node to carve
    selNode = SelectNode(head_row, selectionmode);

    // get neighbouring nodes
    north = selNode->north; // north == NULL iff y = 0
    south = selNode->south; // south == NULL iff y = GetDimensionY()
    west = selNode->west;   // west != NULL east != NULL
    east = selNode->east;   // since first and last coloumn is never selected

    // update neighbouring nodes
    if (north != NULL) {
      north->south = south;
      north->skipdown += (selNode->skipdown + 1);
    }
    if (south != NULL) {
      south->north = north;
      south->skipup += (selNode->skipup + 1);
    }
    west->east = east;
    west->skipright += (selNode->skipright + 1);
    east->west = west;
    east->skipleft += (selNode->skipleft + 1);

    // delete selNode
    delete selNode;
    selNode = NULL;

    // move to next row
    head_row = head_row->south;
  }

}

// note that a node on the boundary will never be selected for removal
/*
* Removes "rounds" number of nodes (up to a maximum of node width - 2) from each row,
* based on specific selection criteria.
* Note that this should remove one node from every row, repeated "rounds" times,
* and NOT remove "rounds" nodes from one row before processing the next row.
* PRE: selectionmode is an integer in the range [0,1]
* PARAM: rounds - number of nodes to remove from each row
*        If rounds exceeds node width - 2, then remove only node width - 2 nodes from each row.
*        i.e. Ensure that the final list has at least two nodes in each row.
* POST: this list has had "rounds" nodes removed from each row. Neighbours of the created
*       gaps are linked appropriately, and their skip values are updated to reflect
*       the size of the gap.
*/
void ImgList::Carve(unsigned int rounds, int selectionmode) {
  // add your implementation here
  if (rounds > (GetDimensionFullX() - 2)) rounds = GetDimensionFullX() - 2;
  for (unsigned int r = 0; r < rounds; r++) {
    Carve(selectionmode);
  }
}


/*
* Helper function deallocates all heap memory associated with this list,
* puts this list into an "empty" state. Don't forget to set your member attributes!
* POST: this list has no currently allocated nor leaking heap memory,
*       member attributes have values consistent with an empty list.
*/
void ImgList::Clear() {
  // add your implementation here
  ImgNode *curr = northwest;
  ImgNode *head_row = northwest;
  ImgNode *next;

  unsigned int width = GetDimensionX();
  unsigned int height = GetDimensionY();

  for (unsigned int y = 0; y < height; y++) {
    head_row = head_row->south;
    for (unsigned int x = 0; x < width; x++) {
      next = curr->east;
      delete curr;
      curr = next;
    }
    curr = head_row;
  }

  northwest = NULL;
  southeast = NULL;
}

/* ************************
*  * OPTIONAL - FOR BONUS *
** ************************
* Helper function copies the contents of otherlist and sets this list's attributes appropriately
* PRE: this list is empty
* PARAM: otherlist - list whose contents will be copied
* POST: this list has contents copied from by physically separate from otherlist
*/
void ImgList::Copy(const ImgList& otherlist) {
  
  
}

/*************************************************************************************************
* IF YOU DEFINED YOUR OWN PRIVATE FUNCTIONS IN imglist.h, YOU MAY ADD YOUR IMPLEMENTATIONS BELOW *
*************************************************************************************************/

