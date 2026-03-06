#ifndef TIME_NUMEROLOGY_H
#define TIME_NUMEROLOGY_H

typedef struct {
    int root;       /* final reduced digit: 1-9, or 11, 22, 33 (master) */
    int is_master;  /* 1 if root is a master number, 0 otherwise */
    int sum;        /* intermediate sum before final reduction */
} num_root_t;

typedef struct {
    num_root_t life_path;
    num_root_t birth_day;     /* day of month reduced */
    num_root_t birth_month;   /* month reduced */
    num_root_t birth_year;    /* year reduced */
} num_profile_t;

/* Digital root with master number preservation (11, 22, 33). */
num_root_t num_reduce(int number);

/* Sum digits of a number once (no recursive reduction). */
int num_digit_sum(int number);

/* Life path: reduce month, day, year separately, sum and reduce. */
num_root_t num_life_path(int year, int month, int day);

/* Full numerology profile from birth date. */
num_profile_t num_profile(int year, int month, int day);

/* Personal year: reduce (current_year + birth_month + birth_day). */
num_root_t num_personal_year(int current_year, int birth_month, int birth_day);

/* Universal year: reduce all digits of the year. */
num_root_t num_universal_year(int year);

/* Personal day: reduce (personal_year_root + calendar_month + calendar_day). */
num_root_t num_personal_day(int current_year, int birth_month, int birth_day,
                            int calendar_month, int calendar_day);

/* Meaning keyword for a root number. Returns "?" for invalid. */
const char *num_meaning(int root);

/* Description for a root number. Returns "?" for invalid. */
const char *num_element(int root);

#endif
