/*
* File:        priority.cpp
* Description: Implements a priority ordering structure specially made for determining the
*              order of neighbours visited in PA2's filler::fill function.
*              Note that this does NOT serve the same purpose as the stack/queue
*              ordering structure which you have also been asked to implement.
*
*              Ignoring resize costs, any single insertion or removal operation
*              should cost at most O(n), where n is the number of PixelPoints
*              stored in the collection.
*
* Date:        2022-02-10 02:04
*
*/

#include "priority.h"
#include <assert.h>

// used to compare priority
#define DELTA 0.0000005
#include <cmath>

using namespace cs221util;
using namespace std;

/*
*  Default constructor
*  Initializes refcolor to the default color according to the HSLAPixel implementation.
*/
PriorityNeighbours::PriorityNeighbours() {
  refcolor = HSLAPixel();  
}

/*
*  Parameterized constructor
*  Initializes refcolor to the supplied value.
*/
PriorityNeighbours::PriorityNeighbours(HSLAPixel ref) {
  refcolor.h = ref.h;
  refcolor.s = ref.s;
  refcolor.l = ref.l;
  refcolor.a = ref.a;
}

/*
*  Inserts an item into the collection
*  PARAM: p - item to be inserted
*  POST:  the collection contains p, along with all previously existing items.
*/
void PriorityNeighbours::Insert(PixelPoint p) {
  int size = points.size();

  // if collection is empty, just push p at the back
  if (size == 0) {
    points.push_back(p);
    return;
  }

  // check if p has highest/lowest priority to avoid binarySearch
  if (higherPriority(p, points[0])) {
    points.insert(points.begin(), p);
    return;
  } else if (higherPriority(points[size-1], p)) {
    points.push_back(p);
    return;
  }

  /* EDGE CASE: collection has only 2 elements*/
  if (size == 2) {
    if (higherPriority(p, points[0])) {
      points.insert(points.begin(), p); // p inserted at start
    } else if (higherPriority(points[1], p)) {
      points.push_back(p); // p inserted at the back
    } else {
      points.insert(points.begin()+1, p); // p inserted at the midpoint
    }
    return;
  }

  // if collection has 3 or more elements use binary search
  int index = binarySearch(p, 0, size-1);
  points.insert(points.begin()+index, p);
}

/*
*  Removes and returns an item from the collection satisfying the priority condition
*  PRE:    the collection is not empty (but it would be good to check anyway!).
*  POST:   the item satisfying the priority condition is removed from the collection.
*  RETURN: the item satisfying the priority condition
*
*  Priority condition:
*    The PixelPoint in the collection whose color is the minimum distance away
*    from the reference color is the priority item to be returned.
*
*    In case of multiple items with the same priority value (i.e. minimal distance
*    away from the reference color), the one with the minimal y-coordinate will be
*    selected.
*    In case of multiple items with the same priority value and the same y-coordinate,
*    the one with the minimal x-coordinate will be selected.
*
*    ***ATTENTION*** The dist() function in HSLAPixel.h will be very helpful!
*
*  Combined with Insert(), think about the time complexity of maintaining the
*  priority order and/or accessing the priority element in this specific application!
*/
PixelPoint PriorityNeighbours::Remove() {
  // get the first element in points which must have the highest priority
  PixelPoint ret(points[0].x, points[0].y, points[0].color);

  // erase the first element
  points.erase(points.begin());

  return ret;
}

/*
*  Checks if the collection is empty
*  RETURN: true, if the collection is empty
*          false, otherwise
*/
bool PriorityNeighbours::IsEmpty() const {
  return (points.size() == 0);  
}

/*
*  Returns the value of the reference color
*/
HSLAPixel PriorityNeighbours::GetReferenceColor() const {
  return refcolor;  
}

/*
*  Sets the reference color attribute
*  POST: refcolor is set to the supplied value
*/
void PriorityNeighbours::SetReferenceColor(HSLAPixel ref) {
  refcolor.h = ref.h;
  refcolor.s = ref.s;
  refcolor.l = ref.l;
  refcolor.a = ref.a;
}

/*
*  Find correct position of p in the collection
*  PRE: The collection is not empty and in sorted priority order (highest priority at start)
*  POST: collection must stay the same
*  RETURN: return iterator at one position after the position p should be inserted
*/
int PriorityNeighbours::binarySearch(PixelPoint p, int start, int end) const {
  if (start <= end) {
    // get the mid index between start and end
    int mid = start + (end - start) / 2;
    if (start == end) {
      mid = start;
    }

    // if points[mid-1] has higher priority than p and points[mid] has lower priority than p
    // p should be inserted at mid
    if (higherPriority(points[mid-1], p) && higherPriority(p, points[mid])) {
      return mid;
    }

    // if points[mid] has lower priority than p, p should be inserted before mid
    if (higherPriority(p, points[mid])) {
      return binarySearch(p, start, mid-1);
    }

    // the only possible case at this point is p's position is at right sub-vector
    return binarySearch(p, mid+1, end);
  }

  // could not find the correct position
  return -1;
}

/*
*  Determine if the first argument has the higher or equal priority compare to the second argument
*  RETURN: true if p1 has higher or equal priority and false if p2 has higher priority
*/
bool PriorityNeighbours::higherPriority(PixelPoint p1, PixelPoint p2) const {
  double p1Dist = p1.color.dist(refcolor);
  double p2Dist = p2.color.dist(refcolor);

  // if p1 is closer to refcolor, return true
  if (p2Dist - p1Dist > DELTA) {
    return true;
  }

  // if p1 and p2 are in similar distance (negligible difference) from refcolor
  if (fabs(p1Dist - p2Dist) < DELTA) {
    // compare y coordinate
    if (p1.y < p2.y) {
      return true;
    } else if (p1.y == p2.y) {
      // if p1 and p2 have same y coordinate, compare x coordinate
      if (p1.x <= p2.x) {
        return true;
      }
    }
  }

  // in every other cases, return false; p2 has higher priority than p1
  return false;
}