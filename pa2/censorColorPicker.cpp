/*
*  File: censorColorPicker.cpp
*  Implements the censor color picker for CPSC 221 PA2.
*
*/

#include "censorColorPicker.h"

#include <math.h> // gives access to sqrt function

/*
*  Useful function for computing the Euclidean distance between two PixelPoints
*/
double PointDistance(PixelPoint a, PixelPoint b) {
  unsigned int diff_x, diff_y;
  if (a.x > b.x)
    diff_x = a.x - b.x;
  else
    diff_x = b.x - a.x;
  if (a.y > b.y)
    diff_y = a.y - b.y;
  else
    diff_y = b.y - a.y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}

CensorColorPicker::CensorColorPicker(unsigned int b_width, PixelPoint ctr, unsigned int rad, PNG& inputimage)
{
  // Assign members
  blockwidth = b_width;
  center = ctr;
  radius = rad;
  img = inputimage;

  // helper variables
  unsigned int width = img.width();
  unsigned int height = img.height();
  HSLAPixel average;

  // resize blockyimg to have the same size as original img
  blockyimg.resize(width, height);

  // iterate thorugh original image to set blockyimg's Pixel colour
  for (unsigned int x = 0; x < width; x+=blockwidth) {
    for (unsigned int y = 0; y < height; y+=blockwidth) {
      // calculate average HSLAPixel colour
      average = calcAverage(x, y);
    
      // set block starts at (x,y) with average pixel colour calculated
      for (unsigned int row = 0; row < blockwidth && row < width; row++) {
        for (unsigned int col = 0; col < blockwidth && col < height; col++) {
          *blockyimg.getPixel(x+row, y+col) = average;
        }
      }
    }
  }
}

HSLAPixel CensorColorPicker::operator()(PixelPoint p)
{
  // if p is outside circular region to censor, return original img's pixel
  if (PointDistance(center, p) > radius) { return *img.getPixel(p.x, p.y); }

  // else return blockyimg's pixel
  return *blockyimg.getPixel(p.x, p.y);
}

/*
 *  Helper function to calculate average HSLAPixel value in a block starting at (xCoor, yCoor)
 */
HSLAPixel CensorColorPicker::calcAverage(unsigned int xCoor, unsigned int yCoor) {
  unsigned int width = img.width();
  unsigned int height = img.height();

  double sumH=0.0, sumS=0.0, sumL=0.0, sumA=0.0;
  HSLAPixel temp;
  int count = 0;

  for (unsigned int x = xCoor; x < width && x < (xCoor+blockwidth); x++) {
    for (unsigned int y = yCoor; y < height && y < (yCoor+blockwidth); y++) {
      temp = *img.getPixel(x, y);
      sumH += temp.h;
      sumS += temp.s;
      sumL += temp.l;
      sumA += temp.a;
      count++;
    }
  }

  return HSLAPixel(sumH/count, sumS/count, sumL/count, sumA/count);
}
