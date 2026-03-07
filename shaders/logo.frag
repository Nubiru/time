#version 300 es
precision highp float;

/* logo.frag — TIME Brand Mark: The Phi Spiral
 *
 * A golden spiral rendered procedurally. No textures, no images.
 * The code IS the logo. Consistent with "every pixel earned."
 *
 * Mathematical basis:
 *   r(theta) = a * phi^(2*theta/PI)
 *   Each quarter-turn grows by exactly phi.
 *
 * Brand colors:
 *   Solar Gold:     vec3(1.0, 0.85, 0.55)   — The Sun, source of time
 *   Celestial Teal: vec3(0.2, 0.75, 0.8)    — Cosmic bridge
 *   Space Black:    vec3(0.024, 0.027, 0.036) — The canvas
 *
 * Uniforms:
 *   u_time       — seconds since start (drives draw animation)
 *   u_resolution — viewport size in pixels
 */

uniform float u_time;
uniform vec2  u_resolution;

out vec4 frag_color;

#define PI      3.14159265358979323846
#define TAU     6.28318530717958647692
#define PHI     1.6180339887498948482
#define LOG_PHI 0.48121182505960344750

/* Golden spiral growth rate: 2 * ln(phi) / PI */
#define SPIRAL_B (LOG_PHI * 2.0 / PI)

/* Brand colors */
const vec3 SOLAR_GOLD     = vec3(1.0, 0.85, 0.55);
const vec3 CELESTIAL_TEAL = vec3(0.2, 0.75, 0.8);
const vec3 SPACE_BLACK    = vec3(0.024, 0.027, 0.036);

/* Maximum spiral extent (revolutions) */
const float MAX_REV = 2.5;
const float MAX_ANGLE = MAX_REV * TAU;

/* Animation: spiral draws over phi^2 seconds (2.618s) */
const float DRAW_DURATION = 2.618;

/* Find distance from point to nearest golden spiral arm.
 * Returns the minimum radial distance.
 * spiral_angle: output — the spiral parameter at the closest point. */
float spiral_sdf(vec2 p, float a, float max_theta, out float spiral_angle) {
    float r = length(p);
    spiral_angle = 0.0;

    if (r < 0.0001) return r;

    float theta = atan(p.y, p.x);

    /* rho: the spiral angle that would produce this radius */
    float rho = log(r / a) / SPIRAL_B;

    /* Find nearest wrap of the spiral */
    float base_diff = rho - theta;
    float base_wrap = floor(base_diff / TAU + 0.5);

    float best_dist = 1e6;

    for (int dk = -2; dk <= 2; dk++) {
        float try_angle = theta + (base_wrap + float(dk)) * TAU;

        /* Skip angles outside the drawn range */
        if (try_angle < -0.1 || try_angle > max_theta + 0.1) continue;

        float try_r = a * exp(SPIRAL_B * try_angle);
        float d = abs(r - try_r);

        if (d < best_dist) {
            best_dist = d;
            spiral_angle = try_angle;
        }
    }

    return best_dist;
}

void main() {
    /* Aspect-corrected coordinates, centered, range ~[-1.2, 1.2] */
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution) / min(u_resolution.x, u_resolution.y);
    uv *= 2.4;

    float r = length(uv);

    /* Spiral starting radius */
    float a = 0.04;

    /* Animation progress */
    float draw_t = clamp(u_time / DRAW_DURATION, 0.0, 1.0);

    /* Ease-out: decelerates as it completes (phi-smoothed) */
    float eased = 1.0 - pow(1.0 - draw_t, float(PHI));
    float current_max_angle = eased * MAX_ANGLE;

    /* Compute spiral distance */
    float angle_at;
    float dist = spiral_sdf(uv, a, current_max_angle, angle_at);

    float revolution = angle_at / TAU;

    /* Reject points beyond drawn extent or before spiral start */
    float visible = step(0.0, angle_at) * step(angle_at, current_max_angle);

    /* Line width: tapers outward — thicker near center, thinner at edge.
     * Each revolution multiplies width by phi^-0.35 */
    float base_width = 0.016;
    float width = base_width * pow(float(PHI), -max(revolution, 0.0) * 0.35);

    /* Soft anti-aliased line */
    float line = smoothstep(width, width * 0.15, dist) * visible;

    /* Glow aura around the line */
    float glow_sigma = width * 3.5;
    float glow = exp(-dist * dist / (glow_sigma * glow_sigma)) * visible;

    /* Drawing tip: bright point at the leading edge of the animation */
    float tip_proximity = abs(angle_at - current_max_angle);
    float tip_brightness = exp(-tip_proximity * tip_proximity / 0.4)
                         * (1.0 - draw_t * 0.7) * visible;

    /* Color: Solar Gold at center -> Celestial Teal at outer edge */
    float color_blend = clamp(revolution / MAX_REV, 0.0, 1.0);
    vec3 spiral_color = mix(SOLAR_GOLD, CELESTIAL_TEAL, color_blend);

    /* Breathing glow after spiral is fully drawn (phi^-1 Hz = 0.618 Hz) */
    float breath = 1.0;
    if (draw_t >= 1.0) {
        float phase = (u_time - DRAW_DURATION) * 0.618;
        breath = 0.88 + 0.12 * sin(phase * TAU);
    }

    /* Compose spiral */
    float alpha = (line * 0.92 + glow * 0.28 + tip_brightness * 0.5) * breath;
    vec3 color = mix(SPACE_BLACK, spiral_color, alpha);

    /* Center glow: the Sun — origin of time */
    float sun_glow = exp(-r * r / 0.006) * draw_t;
    color += SOLAR_GOLD * sun_glow * 0.65;

    /* Subtle star dust in background (pseudo-random) */
    float noise = fract(sin(dot(uv * 73.156, vec2(12.9898, 78.233))) * 43758.5453);
    float stars = step(0.998, noise) * 0.08;
    color += vec3(stars);

    frag_color = vec4(color, 1.0);
}
