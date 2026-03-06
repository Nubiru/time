#ifndef TIME_MOON_NODES_H
#define TIME_MOON_NODES_H

typedef struct {
    double north_node;     /* ecliptic longitude of ascending node (degrees, 0-360) */
    double south_node;     /* ecliptic longitude of descending node (= north + 180) */
    int north_sign;        /* zodiac sign index of north node (0-11) */
    int south_sign;        /* zodiac sign index of south node (0-11) */
    double north_degree;   /* degree within sign (0-30) */
    double south_degree;   /* degree within sign (0-30) */
} lunar_nodes_t;

/* Compute mean lunar node positions at a given Julian Day.
 * Uses mean node (averaged, no nutation). Smooth for display. */
lunar_nodes_t lunar_nodes_mean(double jd);

/* Compute true lunar node positions (with largest periodic correction).
 * More accurate but oscillates. */
lunar_nodes_t lunar_nodes_true(double jd);

/* Name for the north node. */
const char *lunar_node_north_name(void);

/* Name for the south node. */
const char *lunar_node_south_name(void);

/* Symbols for the nodes. */
const char *lunar_node_north_symbol(void);
const char *lunar_node_south_symbol(void);

/* Check if a given ecliptic longitude is within `orb` degrees of either node.
 * Returns: 0=not near, 1=near north node, 2=near south node. */
int lunar_node_proximity(double longitude, double orb, double jd);

/* Nodal period in days (~18.6 years = ~6798.4 days). */
double lunar_nodal_period(void);

#endif
