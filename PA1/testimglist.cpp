// File:        testimglist.cpp
// Date:        2022-01-24 11:35
// Description: Contains basic tests for ImgList class
//              for CPSC 221 2021W2 PA1
//              You may add your own additional tests

#include "imglist.h"

#include <iostream>

using namespace std;
using namespace cs221util;

// test function definitions
void TestConstructorRender();
void TestCarveMode0();
void TestRenderMode2();

void testSelectNode0();
void testSelectNode1();

int main(void) {

  // call the various test functions
  // you may comment and uncomment these as needed

  TestConstructorRender();
  TestCarveMode0();
  TestRenderMode2();

  testSelectNode0();
  testSelectNode1();

  return 0;
}

void testSelectNode0() {
  cout << "Test SelectNode with selectionmode 0" << endl;
  PNG testPNG(3, 1);
  testPNG.getPixel(0,0)->l = 0;
  testPNG.getPixel(1,0)->l = 0.25;
  testPNG.getPixel(2,0)->l = 0;

  ImgList testList(testPNG);
  ImgNode *testSel = testList.SelectNode(testList.northwest, 0);
  if (testSel->colour.l == 0.25) cout << " test1 passed" << endl;

  PNG testPNG2(4, 1);
  testPNG2.getPixel(0,0)->l = 0;
  testPNG2.getPixel(1,0)->l = 0.2;
  testPNG2.getPixel(2,0)->l = 0.75;
  testPNG2.getPixel(3,0)->l = 0;

  ImgList testList2(testPNG2);
  ImgNode *testSel2 = testList2.SelectNode(testList2.northwest, 0);
  if (testSel2->colour.l == 0.2) cout << " test2 passed" << endl;

  PNG testPNG3(5, 2);
  testPNG3.getPixel(0,0)->l = 0;
  testPNG3.getPixel(1,0)->l = 0.2;
  testPNG3.getPixel(2,0)->l = 0.75;
  testPNG3.getPixel(3,0)->l = 0.1;
  testPNG3.getPixel(4,0)->l = 0.3;
  testPNG3.getPixel(0,1)->l = 0.025;
  testPNG3.getPixel(1,1)->l = 0.2;
  testPNG3.getPixel(2,1)->l = 0.75;
  testPNG3.getPixel(3,1)->l = 0.05;
  testPNG3.getPixel(4,1)->l = 0;

  ImgList testList3(testPNG3);
  ImgNode *testSel3 = testList3.SelectNode(testList3.northwest, 0);
  if (testSel3->colour.l == 0.1) cout << " test3 row 0 passed" << endl;
  testSel3 = testList3.SelectNode(testList3.northwest->south, 0);
  if (testSel3->colour.l == 0.05) cout << " test3 row 1 passed" << endl;
}

void testSelectNode1() {
  cout << "Test SelectNode with selectionmode 1" << endl;

  PNG testPNG(3, 1);
  testPNG.getPixel(0,0)->h = 10;
  testPNG.getPixel(1,0)->h = 25;
  testPNG.getPixel(2,0)->h = 10;

  ImgList testList(testPNG);
  ImgNode *testSel = testList.SelectNode(testList.northwest, 1);
  if (testSel->colour.h == 25) cout << " test1 passed" << endl;

  PNG testPNG2(5, 1);
  testPNG2.getPixel(0,0)->h = 300; 
  testPNG2.getPixel(1,0)->h = 210; // SumHueDiff = 90 + 90 = 180
  testPNG2.getPixel(2,0)->h = 120; // SumHueDiff = 170 + 115 = 285
  testPNG2.getPixel(3,0)->h = 5; // SumHueDiff = 115 + 5 = 120
  testPNG2.getPixel(4,0)->h = 10; 

  ImgList testList2(testPNG2);
  ImgNode *testSel2 = testList2.SelectNode(testList2.northwest, 1);
  if (testSel2->colour.h == 5) cout << " test2 passed" << endl;
  else cout << " test2; selNode hue = " << testSel2->colour.h << endl;

  PNG testPNG3(5, 2);
  testPNG3.getPixel(0,0)->h = 1;
  testPNG3.getPixel(1,0)->h = 2; // SumHueDiff = 1 + 62 = 63
  testPNG3.getPixel(2,0)->h = 300; // SumHueDiff = 62 + 67 = 129
  testPNG3.getPixel(3,0)->h = 0; // SumHueDiff = 60 + 1 = 61
  testPNG3.getPixel(4,0)->h = 1;

  testPNG3.getPixel(0,1)->h = 10;
  testPNG3.getPixel(1,1)->h = 9; // SumHueDiff = 1 + 16 = 17
  testPNG3.getPixel(2,1)->h = 25; // SumHueDiff = 16 + 18 = 34
  testPNG3.getPixel(3,1)->h = 26; // SumHueDiff = 1 + 2 = 3
  testPNG3.getPixel(4,1)->h = 24;

  ImgList testList3(testPNG3);
  ImgNode *testSel3 = testList3.SelectNode(testList3.northwest, 1);
  if (testSel3->colour.h == 0) cout << " test3 row 0 passed" << endl;
  else cout << " test3 row 0; selNode hue = " << testSel3->colour.h << endl;
  testSel3 = testList3.SelectNode(testList3.northwest->south, 1);
  if (testSel3->colour.h == 26) cout << " test3 row 1 passed" << endl;
  else cout << " test3 row 1; selNode hue = " << testSel3->colour.h << endl;
}

void TestConstructorRender() {
  cout << "Entered TestConstructorRender function" << endl;

  PNG inimg;
  inimg.readFromFile("input-images/3x4.png");

  if (inimg.height() == 4 && inimg.width() == 3) {
    cout << "Input PNG 3x4.png successfully read." << endl;
    cout << "Creating ImgList object using input PNG... ";
    ImgList list3x4(inimg);
    cout << "done." << endl;

    cout << "Attempting to render unmodified list data to output PNG... ";
    PNG outputimg = list3x4.Render(false, 0);
    cout << "done." << endl;

    cout << "Writing output PNG to 3x4-0-0.png... ";
    outputimg.writeToFile("output-images/3x4-0-0.png");
    cout << "done." << endl;

    cout << "Exiting TestConstructorRender function" << endl <<endl;
  }
  else {
    cout << "Input PNG image not read. Exiting..." << endl;
  }
}

void TestCarveMode0() {
  cout << "Entered TestCarveMode0 function" << endl;

  PNG inimg;
  inimg.readFromFile("input-images/5x6.png");

  if (inimg.height() == 6 && inimg.width() == 5) {
    cout << "Input PNG 3x4.png successfully read." << endl;
    cout << "Creating ImgList object using input PNG... ";
    ImgList list5x6(inimg);
    cout << "done." << endl;

    cout << "Attempting to carve list using selection mode 0... ";
    list5x6.Carve(1, 0);
    cout << "done." << endl;

    cout << "Determining physical node dimensions of carved image... ";
    unsigned int postcarvex = list5x6.GetDimensionX();
    unsigned int postcarvey = list5x6.GetDimensionY();
    cout << "done." << endl;

    cout << "Expected horizontal dimension: 4" << endl;
    cout << "Your horizontal dimension: " << postcarvex << endl;

    cout << "Rendering carved image using rendermode 0... ";
    PNG outputimg1 = list5x6.Render(false, 0);
    cout << "done." << endl;

    cout << "Writing output PNG to 5x6-0-0.png... ";
    outputimg1.writeToFile("output-images/5x6-0-0.png");
    cout << "done." << endl;

    cout << "Rendering carved image using rendermode 1 and fillmode 0... ";
    PNG outputimg2 = list5x6.Render(true, 0);
    cout << "done." << endl;

    cout << "Writing output PNG to 5x6-1-0.png... ";
    outputimg2.writeToFile("output-images/5x6-1-0.png");
    cout << "done." << endl;

    cout << "Rendering carved image using rendermode 1 and fillmode 1... ";
    PNG outputimg3 = list5x6.Render(true, 1);
    cout << "done." << endl;

    cout << "Writing output PNG to 5x6-1-1.png... ";
    outputimg3.writeToFile("output-images/5x6-1-1.png");
    cout << "done." << endl;

    cout << "Carving a second pixel from image... ";
    list5x6.Carve(1, 0);
    cout << "done." << endl;

    cout << "Determining physical node dimensions of carved image... ";
    postcarvex = list5x6.GetDimensionX();
    postcarvey = list5x6.GetDimensionY();
    cout << "done." << endl;

    cout << "Expected horizontal dimension: 3" << endl;
    cout << "Your horizontal dimension: " << postcarvex << endl;

    cout << "Rendering carved image using rendermode 0... ";
    PNG outputimg4 = list5x6.Render(false, 0);
    cout << "done." << endl;

    cout << "Writing output PNG to 5x6-0-0.png... ";
    outputimg4.writeToFile("output-images/5x6-0-0-c2.png");
    cout << "done." << endl;

    // cout << "Rendering carved image using rendermode 1 and fillmode 2... ";
    // PNG outputimg5 = list5x6.Render(true, 2);
    // cout << "done." << endl;

    // cout << "Writing output PNG to 5x6-1-2.png... ";
    // outputimg5.writeToFile("output-images/5x6-1-2.png");
    // cout << "done." << endl;

    cout << "Rendering carved image using rendermode 1 and fillmode 1... ";
    PNG outputimg5 = list5x6.Render(true, 1);
    cout << "done." << endl;

    cout << "Writing output PNG to 5x6-1-1.png... ";
    outputimg5.writeToFile("output-images/5x6-1-1.png");
    cout << "done." << endl;

    cout << "Exiting TestCarveMode0 function" << endl << endl;
  }
  else {
    cout << "Input PNG image not read. Exiting..." << endl;
  }
}

void TestRenderMode2() {
  cout << "Entered TestRenderMode2 function" << endl;

  PNG inimg;
  inimg.readFromFile("input-images/6x6grad.png");

  if (inimg.height() == 6 && inimg.width() == 6) {
    cout << "Input PNG 6x6.png successfully read." << endl;
    cout << "Creating ImgList object using input PNG... ";
    ImgList list6x6(inimg);
    cout << "done." << endl;

    cout << "Attempting to carve more pixels than allowable in mode 0... ";
    list6x6.Carve(12, 0);
    cout << "done." << endl;

    cout << "Determining physical node dimensions of carved image... ";
    unsigned int postcarvex = list6x6.GetDimensionX();
    unsigned int postcarvey = list6x6.GetDimensionY();
    cout << "done." << endl;

    cout << "Expected horizontal dimension: 2" << endl;
    cout << "Your horizontal dimension: " << postcarvex << endl;

    cout << "Rendering carved image using rendermode 0... ";
    PNG outputimg1 = list6x6.Render(false, 0);
    cout << "done." << endl;

    cout << "Writing output PNG to 6x6-0-0.png... ";
    outputimg1.writeToFile("output-images/6x6-0-0.png");
    cout << "done." << endl;

    // cout << "Rendering carved image using rendermode 1 and fillmode 2... ";
    // PNG outputimg2 = list6x6.Render(true, 2);
    // cout << "done." << endl;

    // cout << "Writing output PNG to 6x6-1-2.png... ";
    // outputimg2.writeToFile("output-images/6x6-1-2.png");
    // cout << "done." << endl;

    cout << "Rendering carved image using rendermode 1 and fillmode 1... ";
    PNG outputimg2 = list6x6.Render(true, 1);
    cout << "done." << endl;

    cout << "Writing output PNG to 6x6-1-1.png... ";
    outputimg2.writeToFile("output-images/6x6-1-1.png");
    cout << "done." << endl;

    cout << "Exiting TestRenderMode2 function" << endl;
  }
  else {
    cout << "Input PNG image not read. Exiting..." << endl;
  }
}