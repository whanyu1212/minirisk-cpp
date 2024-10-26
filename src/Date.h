#pragma once

#include "Macros.h"
#include <string>
#include <array>

namespace minirisk
{

    struct Date
    {
    public:
        static const unsigned first_year = 1900;
        static const unsigned last_year = 2200;
        static const unsigned n_years = last_year - first_year;

    private:
        static std::string padding_dates(unsigned);

        // number of days elapsed from beginning of the year
        // unsigned day_of_year() const;
        unsigned day_of_year(unsigned y, unsigned m, unsigned d) const;

        friend long operator-(const Date &d1, const Date &d2);

        static const std::array<unsigned, 12> days_in_month;   // num of days in month M in a normal year
        static const std::array<unsigned, 12> days_ytd;        // num of days since 1-jan to 1-M in a normal year
        static const std::array<unsigned, n_years> days_epoch; // num of days since 1-jan-1900 to 1-jan-yyyy (until 2200)

    public:
        // Default constructor
        // Initializes the date to 1-Jan-1970 by default.
        // Date() : m_y(1970), m_m(1), m_d(1), m_is_leap(false) {}

        /*
        Default constructor, initialize the number of days since 1-Jan-1900 to 0.
        */
        Date() : m_serial(0) {}

        // Constructor where the input value is checked.
        Date(unsigned year, unsigned month, unsigned day)
        {
            init(year, month, day);
        }

        void init(unsigned year, unsigned month, unsigned day)
        {
            check_valid(year, month, day);
            // m_y = (unsigned short)year;
            // m_m = (unsigned char)month;
            // m_d = (unsigned char)day;
            // m_is_leap = is_leap_year(year);

            m_serial = days_epoch[year - 1900] + day_of_year(year, month, day);
        }

        static void check_valid(unsigned y, unsigned m, unsigned d);

        bool operator<(const Date &d) const
        {
            return m_serial < d.m_serial;
        }

        bool operator==(const Date &d) const
        {
            return m_serial == d.m_serial;
        }

        bool operator>(const Date &d) const
        {
            return d < (*this);
        }

        // number of days since 1-Jan-1900
        // unsigned serial() const
        // {
        //     return days_epoch[m_y - 1900] + day_of_year();
        // }
        unsigned serial(unsigned y, unsigned m, unsigned d) const
        {
            return days_epoch[y - 1900] + day_of_year(y, m, d);
        }

        static bool is_leap_year(unsigned yr);

        // In YYYYMMDD format
        std::string to_string(bool pretty) const
        {
            unsigned year, month, day;
            from_serial(m_serial, year, month, day);
            return pretty
                       ? std::to_string(day) + "-" + std::to_string(month) + "-" + std::to_string(year)
                       : std::to_string(year) + padding_dates(month) + padding_dates(day);
        }

    private:
        unsigned m_serial;

        // Convert serial to year, month, and day
        void from_serial(unsigned serial, unsigned &year, unsigned &month, unsigned &day) const;
    };

    long operator-(const Date &d1, const Date &d2);

    inline double time_frac(const Date &d1, const Date &d2)
    {
        return static_cast<double>(d2 - d1) / 365.0;
    }

} // namespace minirisk