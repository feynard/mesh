#ifndef COLORSCHEME_HPP
#define COLORSCHEME_HPP

#include "vec.hpp"

typedef vec4 ColorScheme[8];

const ColorScheme solarized = {
    vec4(vec3(  0,  43,  54) / 256.0, 1),   // Base
    vec4(vec3(181, 137,   0) / 256.0, 1),   // Yellow
    vec4(vec3(203,  75,  22) / 256.0, 1),   // Orange
    vec4(vec3(220,  50,  47) / 256.0, 1),   // Red
    vec4(vec3(211,  54, 130) / 256.0, 1),   // Magenta
    vec4(vec3( 38, 139, 210) / 256.0, 1),   // Blue
    vec4(vec3( 42, 161, 152) / 256.0, 1),   // Cyan
    vec4(vec3(133, 153,   0) / 256.0, 1)    // Green
};

#endif
