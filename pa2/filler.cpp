/*
*  File:        filler.cpp
*  Description: Implementation of functions in the filler namespace.
*
*/

#include "filler.h"

/*
*  Performs a flood fill using breadth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
animation filler::FillBFS(FillerConfig& config) {
  // call fill with Queue as OrderingStructure
  return Fill<Queue>(config);
}

/*
*  Performs a flood fill using depth first search.
*
*  PARAM:  config - FillerConfig struct to setup the fill
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
animation filler::FillDFS(FillerConfig& config) {
  // call fill with Stack as OrderingStructure
  return Fill<Stack>(config);
}

/*
*  Run a flood fill on an image starting at the seed point
*
*  PARAM:  config - FillerConfig struct with data for flood fill of image
*  RETURN: animation object illustrating progression of flood fill algorithm
*/
template <template <class T> class OrderingStructure> animation filler::Fill(FillerConfig& config)
{
  /*
  * You need to implement this function!
  *
  * This is the basic description of a flood-fill algorithm: Every fill
  * algorithm requires an ordering structure, which is passed to this
  * function via its template parameter. For a breadth-first-search
  * fill, that structure is a Queue, for a depth-first-search, that
  * structure is a Stack. To begin the algorithm, you simply place the
  * given point in the ordering structure, marking it processed
  * (the way you mark it is a design decision you'll make yourself).
  * We have a choice to either change the color, if appropriate, when we
  * add the point to the OS, or when we take it off. In our test cases,
  * we have assumed that you will change the color when a point is added
  * to the structure.
  * Until the structure is empty, you do the following:
  *
  * 1.     Remove a point from the ordering structure, and then...
  *
  *        1.    add its unprocessed neighbors (up/down/left/right) whose color values are
  *              within (or equal to) tolerance distance from the center,
  *              to the ordering structure, and
  *              mark them as processed.
  *        2.    if it is an appropriate frame, send the current PNG to the
  *              animation (as described below).
  *
  * 2.     When implementing your breadth-first-search and
  *        depth-first-search fills, you will need to explore neighboring
  *        pixels (up/down/left/right) in some order.
  *
  *        While the order in which you examine neighbors does not matter
  *        for a proper fill, you must use the same order as we do for
  *        your animations to come out like ours! The order you should put
  *        neighboring pixels **ONTO** the queue or stack is based on the
  *        following priority condition:
  *        ** MINIMUM COLOR DISTANCE FROM THE CURRENT PIXEL **
  *        Ties are broken first by minimum y-coordinate, then by minimum x-coordinate.
  *        The HSLAPixel.dist() function will be useful, and you should
  *        take advantage of the functionality in your PriorityNeighbours class.
  * 
  *        If you process the neighbours in a different order, your fill may
  *        still work correctly, but your animations will be different
  *        from the grading scripts!
  *
  * 3.     For every k pixels filled, **starting at the kth pixel**, you
  *        must add a frame to the animation, where k = frameFreq.
  *
  *        For example, if frameFreq is 4, then after the 4th pixel has
  *        been filled you should add a frame to the animation, then again
  *        after the 8th pixel, etc.  You must only add frames for the
  *        number of pixels that have been filled, not the number that
  *        have been checked. So if frameFreq is set to 1, a pixel should
  *        be filled every frame.
  *
  * 4.     Finally, as you leave the function, send one last frame to the
  *        animation. This frame will be the final result of the fill, and
  *        it will be the one we test against.
  *
  */

  int framecount = 0; // increment after processing one pixel; used for producing animation frames (step 3 above)
  animation anim;
  OrderingStructure<PixelPoint> os;
  PixelPoint curr;
  PriorityNeighbours neighbour;
  PixelPoint up, down, left, right;

  unsigned int height = config.img.height();
  unsigned int width = config.img.width();

  // initialize visited 2D vector  
  vector<vector<bool>> isVisited;
  isVisited.resize(height);
  for (unsigned int row = 0; row < height; row++) {
    isVisited[row].resize(width);
    for (unsigned int col = 0; col < width; col++) {
      isVisited[row][col] = false;
    }
  }

  // add the seedpoint to the OrderingStructure
  os.Add(config.seedpoint);
  fillPixel(config.seedpoint, config);
  framecount++;
  if (framecount % config.frameFreq == 0) {
    anim.addFrame(config.img);
  }

  while (!os.IsEmpty()) {
    // get the first element in os
    curr = os.Remove();

    // PriorityNeighbours used to determine priority of neighbours
    neighbour = PriorityNeighbours(curr.color);    

    // add up
    if (curr.y > 0 && !isVisited[curr.y-1][curr.x]) {
      up.x = curr.x; 
      up.y = curr.y-1;
      up.color = *config.img.getPixel(up.x, up.y);
      isVisited[up.y][up.x] = true;

      if (up.color.dist(config.seedpoint.color) <= config.tolerance) { 
        neighbour.Insert(up);
      }
    }

    // add down
    if (curr.y < height-1 && !isVisited[curr.y+1][curr.x]) {
      down.x = curr.x;
      down.y = curr.y+1;
      down.color = *config.img.getPixel(down.x, down.y);
      isVisited[down.y][down.x] = true;

      if (down.color.dist(config.seedpoint.color) <= config.tolerance) { 
        neighbour.Insert(down); 
      }
    }

    // add left
    if (curr.x > 0 && !isVisited[curr.y][curr.x-1]) {
      left.x = curr.x-1;
      left.y = curr.y;
      left.color = *config.img.getPixel(left.x, left.y);
      isVisited[left.y][left.x] = true;

      if (left.color.dist(config.seedpoint.color) <= config.tolerance) { 
        neighbour.Insert(left); 
      }
    }

    // add right
    if (curr.x < width-1 && !isVisited[curr.y][curr.x+1]) {
      right.x = curr.x+1;
      right.y = curr.y;
      right.color = *config.img.getPixel(right.x, right.y);
      isVisited[right.y][right.x] = true;
    
      if (right.color.dist(config.seedpoint.color) <= config.tolerance) {
        neighbour.Insert(right);
      }
    }

    // add neighbours in priority order to os
    while (!neighbour.IsEmpty()) {
      PixelPoint temp = neighbour.Remove();
      os.Add(temp);
      fillPixel(temp, config);

      // if k pixels have been filled, add frame to animation
      framecount++;
      if (framecount % config.frameFreq == 0) {
        anim.addFrame(config.img);
      }
    }
  }
  
  // add final fram to anim
  anim.addFrame(config.img);
  return anim;
}

void filler::fillPixel(PixelPoint pp, FillerConfig& config) {
  HSLAPixel *p = config.img.getPixel(pp.x, pp.y);
  *p = (*config.picker)(pp);
}

