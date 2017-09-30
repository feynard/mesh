#ifndef COLORSCHEME_HPP
#define COLORSCHEME_HPP

#include "vec.hpp"

typedef vec4 ColorScheme[9];

extern ColorScheme* active_colorscheme;

const ColorScheme solarized = {
    vec4(vec3(  0,  43,  54) / 256.0, 1),   // Base
    vec4(vec3(181, 137,   0) / 256.0, 1),   // Yellow
    vec4(vec3(203,  75,  22) / 256.0, 1),   // Orange
    vec4(vec3(220,  50,  47) / 256.0, 1),   // Red
    vec4(vec3(211,  54, 130) / 256.0, 1),   // Magenta
    vec4(vec3( 38, 139, 210) / 256.0, 1),   // Blue
    vec4(vec3( 42, 161, 152) / 256.0, 1),   // Cyan
    vec4(vec3(133, 153,   0) / 256.0, 1),   // Green
    vec4(vec3(  0,   0,   0) / 256.0, 1)    // Base #2

};

const ColorScheme gruvbox = {
    vec4(vec3( 40,  40,  40) / 256.0, 1),   // Base
    vec4(vec3(202,  39,  38) / 256.0, 1),   // Red
    vec4(vec3(152, 150,  41) / 256.0, 1),   // Green
    vec4(vec3(213, 152,  49) / 256.0, 1),   // Yellow
    vec4(vec3( 72, 133, 135) / 256.0, 1),   // Blue
    vec4(vec3(175,  99, 134) / 256.0, 1),   // Purple
    vec4(vec3(106, 156, 108) / 256.0, 1),   // Aqua
    vec4(vec3(251, 128,  45) / 256.0, 1),   // Orange
    vec4(vec3(  0,   0,   0) / 256.0, 1)    // Base #2
};


#endif
