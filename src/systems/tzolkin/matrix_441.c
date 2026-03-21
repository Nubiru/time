#include "matrix_441.h"
#include <string.h>

/* 441 Base Matrix — 21x21 grid, 11 concentric circuits.
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 */

/* --- Static circuit data (indexed 0-10, circuit 1-11) --- */

static const m441_circuit_t CIRCUITS[M441_CIRCUIT_COUNT] = {
    {  1, "Mercury",          80 },
    {  2, "Venus",            72 },
    {  3, "Earth",            64 },
    {  4, "Mars",             56 },
    {  5, "Maldek",           48 },
    {  6, "Jupiter",          40 },
    {  7, "Saturn",           32 },
    {  8, "Uranus",           24 },
    {  9, "Neptune",          16 },
    { 10, "Pluto",             8 },
    { 11, "Galactic Nucleus",  1 }
};

/* --- Helpers --- */

static int m441_in_bounds(int row, int col)
{
    return row >= 1 && row <= M441_SIZE && col >= 1 && col <= M441_SIZE;
}

static int m441_abs(int x)
{
    return x < 0 ? -x : x;
}

static int m441_max(int a, int b)
{
    return a > b ? a : b;
}

/* --- Function implementations --- */

int m441_circuit_count(void)
{
    return M441_CIRCUIT_COUNT;
}

m441_circuit_t m441_circuit_get(int num)
{
    if (num < 1 || num > M441_CIRCUIT_COUNT) {
        m441_circuit_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return CIRCUITS[num - 1];
}

int m441_circuit_for_position(int row, int col)
{
    if (!m441_in_bounds(row, col))
        return 0;
    int dr = m441_abs(row - M441_CENTER);
    int dc = m441_abs(col - M441_CENTER);
    return M441_CIRCUIT_COUNT - m441_max(dr, dc);
}

int m441_total_units(void)
{
    return M441_TOTAL;
}

int m441_is_tubo_mauri(int row, int col)
{
    if (!m441_in_bounds(row, col))
        return 0;
    return col == M441_CENTER ? 1 : 0;
}

int m441_is_equatorial(int row, int col)
{
    if (!m441_in_bounds(row, col))
        return 0;
    return row == M441_CENTER ? 1 : 0;
}

int m441_is_center(int row, int col)
{
    return (row == M441_CENTER && col == M441_CENTER) ? 1 : 0;
}

int m441_dimensions_of_time(void)
{
    return M441_TOTAL / 49;
}
