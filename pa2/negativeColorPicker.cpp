/*
*  File: negativeColorPicker.cpp
*  Implements the negative color picker for CPSC 221 PA2.
*
*/

#include "negativeColorPicker.h"

NegativeColorPicker::NegativeColorPicker(PNG& inputimg)
{
  // copy inputimg
  img = inputimg;
}

HSLAPixel NegativeColorPicker::operator()(PixelPoint p)
{
  // get pixel from original img
  HSLAPixel original = *img.getPixel(p.x, p.y);

  // calculate negative values of H/S/L/A values
  double negH = original.h < 180 ? original.h + 180 : original.h - 180;
  double negS = original.s;
  double negL = 1.0- original.l;
  double negA = original.a; 
  
  // return HSLPixel with negative H/S/L/A values
  return HSLAPixel(negH, negS, negL, negA);
}
