#ifndef TIME_SIDEREAL_H
#define TIME_SIDEREAL_H

/* Greenwich Mean Sidereal Time in degrees, given JD at 0h UT.
 * Returns GMST in degrees (0-360). */
double gmst_degrees(double jd_0h_ut);

/* Greenwich Apparent Sidereal Time (includes nutation).
 * Returns GAST in degrees. */
double gast_degrees(double jd);

/* Local Sidereal Time given GST and observer longitude.
 * longitude_deg: positive east, negative west.
 * Returns LST in degrees (0-360). */
double lst_degrees(double gst_deg, double longitude_deg);

#endif
