#include <iomanip>

#include "Date.h"

namespace minirisk
{

    struct DateInitializer : std::array<unsigned, Date::n_years>
    {
        DateInitializer()
        {
            for (unsigned i = 0, s = 0, y = Date::first_year; i < size(); ++i, ++y)
            {
                (*this)[i] = s;
                s += 365 + (Date::is_leap_year(y) ? 1 : 0);
            }
        }
    };

    const std::array<unsigned, 12> Date::days_in_month = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
    const std::array<unsigned, 12> Date::days_ytd{{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}};
    const std::array<unsigned, Date::n_years> Date::days_epoch(static_cast<const std::array<unsigned, Date::n_years> &>(DateInitializer()));

    /* The function checks if a given year is a leap year.
        Leap year must be a multiple of 4, but it cannot be a multiple of 100 without also being a multiple of 400.
    */
    bool Date::is_leap_year(unsigned year)
    {
        return ((year % 4 != 0) ? false : (year % 100 != 0) ? true
                                      : (year % 400 != 0)   ? false
                                                            : true);
    }

    // The function pads a zero before the month or day if it has only one digit.
    std::string Date::padding_dates(unsigned month_or_day)
    {
        std::ostringstream os;
        os << std::setw(2) << std::setfill('0') << month_or_day;
        return os.str();
    }

    /*
    The implementation of check_valid for Date that takes in the year, month, and day.
    The function checks if the year is within the range of first_year and last_year.
    The function checks if the month is between 1 and 12.
    The function checks if the day is between 1 and the maximum number of days in the month.
    */
    void Date::check_valid(unsigned y, unsigned m, unsigned d)
    {
        MYASSERT(y >= first_year, "The year must be no earlier than year " << first_year << ", got " << y);
        MYASSERT(y < last_year, "The year must be smaller than year " << last_year << ", got " << y);
        MYASSERT(m >= 1 && m <= 12, "The month must be a integer between 1 and 12, got " << m);
        unsigned dmax = days_in_month[m - 1] + ((m == 2 && is_leap_year(y)) ? 1 : 0);
        MYASSERT(d >= 1 && d <= dmax, "The day must be a integer between 1 and " << dmax << ", got " << d);
    }

    /*
    The implementation of check_valid for Date that takes in the serial number.
    The function checks if the serial number is a non-negative integer.
    The function checks if the serial number is smaller than the maximum number of days in the last year
    which is 109572.
    */
    void Date::check_valid(unsigned serial)
    {
        MYASSERT(serial >= 0, "The serial number must be a non-negative integer, got " << serial);
        MYASSERT(serial < days_epoch[n_years - 1] + 365 + (is_leap_year(last_year) ? 1 : 0), "The serial number must be smaller than " << days_epoch[n_years - 1] + 365 + (is_leap_year(last_year) ? 1 : 0) << ", got " << serial);
    }

    /*
    Modified the day_of_year function to take in the year, month, and day since
    the internal representation of the year, month, and day is removed.
    */
    unsigned Date::day_of_year(unsigned y, unsigned m, unsigned d) const
    {
        return days_ytd[m - 1] + ((m > 2 && is_leap_year(y)) ? 1 : 0) + (d - 1);
    }

    /*
    A function to convert the serial number to year, month, and day
    respectively. The function takes in the serial number and returns
    the year, month, and day by reference.
    */
    void Date::from_serial(unsigned serial, unsigned &year, unsigned &month, unsigned &day) const
    {
        const unsigned days_per_year = 365;
        const unsigned days_per_leap_year = 366;
        const unsigned epoch_year = Date::first_year;

        year = epoch_year;
        while (serial >= (is_leap_year(year) ? days_per_leap_year : days_per_year))
        {
            serial -= is_leap_year(year) ? days_per_leap_year : days_per_year;
            ++year;
        }

        static const unsigned days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        month = 1;
        while (serial >= days_in_month[month - 1] + ((month == 2 && is_leap_year(year)) ? 1 : 0))
        {
            serial -= days_in_month[month - 1] + ((month == 2 && is_leap_year(year)) ? 1 : 0);
            ++month;
        }

        day = serial + 1;
    }

    /*  The function calculates the distance between two Dates.
        d1 > d2 is allowed, which returns the negative of d2-d1.
    */
    long operator-(const Date &d1, const Date &d2)
    {
        unsigned s1 = d1.m_serial;
        unsigned s2 = d2.m_serial;
        return static_cast<long>(s1) - static_cast<long>(s2);
    }

} // namespace minirisk