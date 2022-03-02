/*
*  File: imageTileColorPicker.cpp
*  Implements the image tile color picker for CPSC 221 PA2.
*
*/

#include "imageTileColorPicker.h"

ImageTileColorPicker::ImageTileColorPicker(PNG& otherimage) {
  // copy otherimage to pick the pixels
  img_other = otherimage;
}

HSLAPixel ImageTileColorPicker::operator()(PixelPoint p) {
  // get appropriate x and y coordinate of img_other
  // modulo was used for the edge cases where img_other has different size
  unsigned int x = p.x % img_other.width();
  unsigned int y = p.y % img_other.height();

  return *img_other.getPixel(x, y);
}