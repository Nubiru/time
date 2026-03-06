#include "numerology.h"

static int abs_int(int n)
{
    return n < 0 ? -n : n;
}

int num_digit_sum(int number)
{
    int n = abs_int(number);
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

num_root_t num_reduce(int number)
{
    num_root_t r;
    int n = abs_int(number);

    for (;;) {
        if (n == 11 || n == 22 || n == 33) {
            r.root = n;
            r.is_master = 1;
            r.sum = n;
            return r;
        }
        if (n < 10) {
            r.root = n;
            r.is_master = 0;
            r.sum = n;
            return r;
        }
        r.sum = n;
        n = num_digit_sum(n);
    }
}

num_root_t num_life_path(int year, int month, int day)
{
    num_root_t yr = num_reduce(num_digit_sum(abs_int(year)));
    num_root_t mo = num_reduce(month);
    num_root_t dy = num_reduce(day);

    int total = yr.root + mo.root + dy.root;
    return num_reduce(total);
}

num_profile_t num_profile(int year, int month, int day)
{
    num_profile_t p;
    p.birth_year = num_reduce(num_digit_sum(abs_int(year)));
    p.birth_month = num_reduce(month);
    p.birth_day = num_reduce(day);
    p.life_path = num_life_path(year, month, day);
    return p;
}

num_root_t num_personal_year(int current_year, int birth_month, int birth_day)
{
    num_root_t yr = num_reduce(num_digit_sum(abs_int(current_year)));
    num_root_t mo = num_reduce(birth_month);
    num_root_t dy = num_reduce(birth_day);
    int total = yr.root + mo.root + dy.root;
    return num_reduce(total);
}

num_root_t num_universal_year(int year)
{
    return num_reduce(num_digit_sum(abs_int(year)));
}

num_root_t num_personal_day(int current_year, int birth_month, int birth_day,
                            int calendar_month, int calendar_day)
{
    num_root_t py = num_personal_year(current_year, birth_month, birth_day);
    num_root_t cm = num_reduce(calendar_month);
    num_root_t cd = num_reduce(calendar_day);
    int total = py.root + cm.root + cd.root;
    return num_reduce(total);
}

const char *num_meaning(int root)
{
    switch (root) {
        case 1:  return "Leadership";
        case 2:  return "Partnership";
        case 3:  return "Expression";
        case 4:  return "Foundation";
        case 5:  return "Freedom";
        case 6:  return "Responsibility";
        case 7:  return "Analysis";
        case 8:  return "Power";
        case 9:  return "Completion";
        case 11: return "Illumination";
        case 22: return "Master Builder";
        case 33: return "Master Teacher";
        default: return "?";
    }
}

const char *num_element(int root)
{
    switch (root) {
        case 1:  return "The Initiator";
        case 2:  return "The Mediator";
        case 3:  return "The Communicator";
        case 4:  return "The Builder";
        case 5:  return "The Adventurer";
        case 6:  return "The Nurturer";
        case 7:  return "The Seeker";
        case 8:  return "The Achiever";
        case 9:  return "The Humanitarian";
        case 11: return "The Visionary";
        case 22: return "The Architect";
        case 33: return "The Healer";
        default: return "?";
    }
}
