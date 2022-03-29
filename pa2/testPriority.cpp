/*
* File:        testPriority.cpp
* Description: Basic test cases for CPSC 221 2021W2 PA2 - PriorityNeighbours
*              Add your own code to this!
*
* Date:        2022-02-10 22:32
*
*/

#define CATCH_CONFIG_MAIN
#include <iostream>
#include "cs221util/catch.hpp"

#include "cs221util/HSLAPixel.h"
#include "cs221util/PNG.h"

#include "pixelpoint.h"
#include "priority.h"

using namespace cs221util;
using namespace std;

TEST_CASE("PriorityNeighbours::insertion_removal_isempty", "[weight=1][part=priority]") {
  //cout << "Testing PriorityNeighbours..." << endl;
  PriorityNeighbours pn(HSLAPixel(180, 0.5, 0.5));
  vector<PixelPoint> result;
  vector<PixelPoint> expected;

  // declare a few PixelPoint objects
  // feel free to add to/change these to test your special conditions!
  PixelPoint pp1(3, 4, HSLAPixel(175, 0.5, 0.5)); // priority #1, short distance from reference point
  PixelPoint pp2(30, 45, HSLAPixel(120, 0.25, 0.75)); // priority #2, longer distance from reference point
  PixelPoint pp3(35, 45, HSLAPixel(120, 0.25, 0.75)); // priority #3, same distance as pp2 and same y-coordinate
  PixelPoint pp4(16, 12, HSLAPixel(30, 0.9, 0.1)); // priority #4, long distance from reference point

  // get the expected priority order
  expected.push_back(pp1);
  expected.push_back(pp2);
  expected.push_back(pp3);
  expected.push_back(pp4);

  // put the PixelPoint objects into your PriorityNeighbours collection in some order
  // (feel free to change the insertion order)
  pn.Insert(pp4);
  pn.Insert(pp1);
  pn.Insert(pp3);
  pn.Insert(pp2);

  while (!pn.IsEmpty()) {
    result.push_back(pn.Remove());
  }
  REQUIRE(result == expected);
}

TEST_CASE("PriorityNeighbours1::insertion_removal_isempty", "[weight=1][part=priority]") {
  //cout << "Testing PriorityNeighbours..." << endl;
  PriorityNeighbours pn(HSLAPixel(0, 0, 0));
  vector<PixelPoint> result;
  vector<PixelPoint> expected;

  // declare a few PixelPoint objects
  // feel free to add to/change these to test your special conditions!
  PixelPoint pp1(3, 4, HSLAPixel(1, 0, 0)); // priority #1, short distance from reference point
  PixelPoint pp2(30, 45, HSLAPixel(359, 0, 0)); // priority #2, longer distance from reference point
  PixelPoint pp3(35, 45, HSLAPixel(5, 0, 0)); // priority #3, same distance as pp2 and same y-coordinate
  PixelPoint pp4(16, 12, HSLAPixel(7, 0.9, 0.1)); // priority #4, long distance from reference point
  PixelPoint pp5(20, 12, HSLAPixel(7, 0.9, 0.1));
  PixelPoint pp6(30, 49, HSLAPixel(40, 1, 1));
  PixelPoint pp7(44, 49, HSLAPixel(70, 0.9, 0.1));
  PixelPoint pp8(16, 50, HSLAPixel(180, 0.9, 0.1));


  // get the expected priority order
  expected.push_back(pp1);
  expected.push_back(pp2);
  expected.push_back(pp3);
  expected.push_back(pp4);
  expected.push_back(pp5);
  expected.push_back(pp6);
  expected.push_back(pp7);
  expected.push_back(pp8);

  // put the PixelPoint objects into your PriorityNeighbours collection in some order
  // (feel free to change the insertion order)
  pn.Insert(pp4);
  pn.Insert(pp1);
  pn.Insert(pp8);
  pn.Insert(pp5);
  pn.Insert(pp3);
  pn.Insert(pp6);
  pn.Insert(pp2);
  pn.Insert(pp7);

  while (!pn.IsEmpty()) {
    result.push_back(pn.Remove());
  }
  REQUIRE(result == expected);
}

TEST_CASE("PriorityNeighbours1::insertion_removal_isempty", "[weight=1][part=priority]") {
  //cout << "Testing PriorityNeighbours..." << endl;
  PriorityNeighbours pn(HSLAPixel(0, 0, 0));
  vector<PixelPoint> result;
  vector<PixelPoint> expected;

  // declare a few PixelPoint objects
  // feel free to add to/change these to test your special conditions!
  PixelPoint pp1(3, 4, HSLAPixel(1, 0, 0)); // priority #1, short distance from reference point
  PixelPoint pp2(30, 45, HSLAPixel(359, 0, 0)); // priority #2, longer distance from reference point
  PixelPoint pp3(35, 45, HSLAPixel(5, 0, 0)); // priority #3, same distance as pp2 and same y-coordinate
  PixelPoint pp4(16, 12, HSLAPixel(7, 0.9, 0.1)); // priority #4, long distance from reference point
  PixelPoint pp5(20, 12, HSLAPixel(7, 0.9, 0.1));
  PixelPoint pp6(30, 49, HSLAPixel(40, 1, 1));
  PixelPoint pp7(44, 49, HSLAPixel(70, 0.9, 0.1));
  PixelPoint pp8(16, 50, HSLAPixel(180, 0.9, 0.1));


  // get the expected priority order
  expected.push_back(pp1);
  expected.push_back(pp2);
  expected.push_back(pp3);
  expected.push_back(pp4);
  expected.push_back(pp5);
  expected.push_back(pp6);
  expected.push_back(pp7);
  expected.push_back(pp8);

  // put the PixelPoint objects into your PriorityNeighbours collection in some order
  // (feel free to change the insertion order)
  pn.Insert(pp4);
  pn.Insert(pp8);
  pn.Insert(pp2);
  pn.Insert(pp5);
  pn.Insert(pp1);
  pn.Insert(pp3);
  pn.Insert(pp6);
  pn.Insert(pp7);

  while (!pn.IsEmpty()) {
    result.push_back(pn.Remove());
  }
  REQUIRE(result == expected);
}