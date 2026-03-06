#include "easing.h"
#include <math.h>

#define PI 3.14159265358979323846

/* --- Linear --- */

double ease_linear(double t) {
    return t;
}

/* --- Quadratic --- */

double ease_in_quad(double t) {
    return t * t;
}

double ease_out_quad(double t) {
    double u = 1.0 - t;
    return 1.0 - u * u;
}

double ease_in_out_quad(double t) {
    if (t < 0.5) {
        return 2.0 * t * t;
    }
    double u = -2.0 * t + 2.0;
    return 1.0 - u * u / 2.0;
}

/* --- Cubic --- */

double ease_in_cubic(double t) {
    return t * t * t;
}

double ease_out_cubic(double t) {
    double u = 1.0 - t;
    return 1.0 - u * u * u;
}

double ease_in_out_cubic(double t) {
    if (t < 0.5) {
        return 4.0 * t * t * t;
    }
    double u = -2.0 * t + 2.0;
    return 1.0 - u * u * u / 2.0;
}

/* --- Sine --- */

double ease_in_sine(double t) {
    return 1.0 - cos(t * PI / 2.0);
}

double ease_out_sine(double t) {
    return sin(t * PI / 2.0);
}

double ease_in_out_sine(double t) {
    return -(cos(PI * t) - 1.0) / 2.0;
}

/* --- Exponential --- */

double ease_in_expo(double t) {
    if (t == 0.0) return 0.0;
    return pow(2.0, 10.0 * (t - 1.0));
}

double ease_out_expo(double t) {
    if (t == 1.0) return 1.0;
    return 1.0 - pow(2.0, -10.0 * t);
}

/* --- Elastic --- */

double ease_in_elastic(double t) {
    if (t == 0.0) return 0.0;
    if (t == 1.0) return 1.0;
    double p = 0.3;
    double s = p / 4.0;
    double u = t - 1.0;
    return -(pow(2.0, 10.0 * u) * sin((u - s) * 2.0 * PI / p));
}

double ease_out_elastic(double t) {
    if (t == 0.0) return 0.0;
    if (t == 1.0) return 1.0;
    double p = 0.3;
    double s = p / 4.0;
    return pow(2.0, -10.0 * t) * sin((t - s) * 2.0 * PI / p) + 1.0;
}

/* --- Back --- */

double ease_in_back(double t) {
    double s = 1.70158;
    return t * t * ((s + 1.0) * t - s);
}

double ease_out_back(double t) {
    double s = 1.70158;
    double u = t - 1.0;
    return u * u * ((s + 1.0) * u + s) + 1.0;
}

/* --- Animation helpers --- */

double anim_value(anim_t anim, double t_seconds, ease_fn_t ease_fn) {
    double t;
    if (anim.duration <= 0.0) {
        t = 1.0;
    } else if (t_seconds <= 0.0) {
        t = 0.0;
    } else if (t_seconds >= anim.duration) {
        t = 1.0;
    } else {
        t = t_seconds / anim.duration;
    }
    double eased = ease_fn(t);
    return anim.start + (anim.end - anim.start) * eased;
}

int anim_done(anim_t anim, double t_seconds) {
    return t_seconds >= anim.duration ? 1 : 0;
}

/* --- Angle helpers (degrees) --- */

double angle_normalize(double deg) {
    double result = fmod(deg, 360.0);
    if (result < 0.0) {
        result += 360.0;
    }
    return result;
}

double angle_distance(double from_deg, double to_deg) {
    double diff = angle_normalize(to_deg - from_deg);
    if (diff > 180.0) {
        diff -= 360.0;
    }
    return diff;
}

double angle_lerp(double from_deg, double to_deg, double t) {
    return angle_normalize(from_deg + angle_distance(from_deg, to_deg) * t);
}
