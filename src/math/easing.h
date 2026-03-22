#ifndef TIME_EASING_H
#define TIME_EASING_H

typedef double (*ease_fn_t)(double);

typedef struct {
    double start;
    double end;
    double duration;
} anim_t;

/* Linear */
double ease_linear(double t);

/* Quadratic */
double ease_in_quad(double t);
double ease_out_quad(double t);
double ease_in_out_quad(double t);

/* Cubic */
double ease_in_cubic(double t);
double ease_out_cubic(double t);
double ease_in_out_cubic(double t);

/* Quartic */
double ease_in_quart(double t);
double ease_out_quart(double t);
double ease_in_out_quart(double t);

/* Sine */
double ease_in_sine(double t);
double ease_out_sine(double t);
double ease_in_out_sine(double t);

/* Exponential */
double ease_in_expo(double t);
double ease_out_expo(double t);

/* Elastic */
double ease_in_elastic(double t);
double ease_out_elastic(double t);

/* Back */
double ease_in_back(double t);
double ease_out_back(double t);

/* Animation helpers */
double anim_value(anim_t anim, double t_seconds, ease_fn_t ease_fn);
int    anim_done(anim_t anim, double t_seconds);

/* Angle interpolation (degrees) */
double angle_lerp(double from_deg, double to_deg, double t);
double angle_normalize(double deg);
double angle_distance(double from_deg, double to_deg);

#endif
