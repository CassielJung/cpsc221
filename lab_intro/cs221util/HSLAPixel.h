#ifndef CS221UTIL_HSLAPIXEL_H
#define CS221UTIL_HSLAPIXEL_H

namespace cs221util {
    class HSLAPixel {
        public:
            /* variables */
            double h;   // hue
            double s;   // saturation
            double l;   // luminance
            double a;   // alpha

            /* contructors declaration */
            HSLAPixel();
            HSLAPixel(double hue, double sat, double lum);
            HSLAPixel(double hue, double sta, double lum, double alp);
    };
}

#endif