#include "HSLAPixel.h"

namespace cs221util {
    HSLAPixel::HSLAPixel() {
        s = 0.0;
        l = 1.0;
        a = 1.0;
    }

    HSLAPixel::HSLAPixel(double hue, double sat, double lum) {
        h = hue;
        s = sat;
        l = lum;
    }

    HSLAPixel::HSLAPixel(double hue, double sta, double lum, double alp) {
        h = hue;
        s = sta;
        l = lum;
        a = alp;
    }
}