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
        /*
        Modified default constructor, initialize the serial number to 0, which
        is the number of days elapsed from 1-Jan-1900 to 1-Jan-1900.
        */
        Date() : m_serial(0) {}

        // A different constructor where you can input the year, month, and day
        Date(unsigned year, unsigned month, unsigned day)
        {
            init(year, month, day);
        }

        // Overloading of the init function

        // Initialize with the input year, month, and day
        // We can no longer call the internal representation of the m_y, m_m, and m_d, m_is_leap
        void init(unsigned year, unsigned month, unsigned day)
        {
            check_valid(year, month, day);
            m_serial = days_epoch[year - 1900] + day_of_year(year, month, day);
        }

        // Initialize with the input serial number
        void init(unsigned s)
        {
            // A different check_valid function is called
            check_valid(s);
            m_serial = s;
        }

        // Overloading of the check_valid function to handle different sets of inputs
        static void check_valid(unsigned y, unsigned m, unsigned d);
        static void check_valid(unsigned serial);

        /*
        In C++, the comparison operators (<, ==, >, etc.) are not automatically defined
         for user-defined types (such as classes and structs). While these operators are
         built-in for fundamental types (like int, float, char, etc.), you need to explicitly
        define them for your custom types to specify how objects of those types should be compared.
        */
        bool operator<(const Date &d) const
        {
            return m_serial < d.m_serial;
        }

        bool operator==(const Date &d) const
        {
            return m_serial == d.m_serial;
        }

        bool operator>(const Date &d) const
        { /*
          This operator leverages on the operator< function
          */
            return d < (*this);
        }

        /*
        modified serial function to take in the year, month, and day
        since the internal representation of the year, month, and day is removed
        */
        unsigned serial(unsigned y, unsigned m, unsigned d) const
        {
            return days_epoch[y - 1900] + day_of_year(y, m, d);
        }

        static bool is_leap_year(unsigned yr);

        // Function to convert serial to year, month, and day
        void from_serial(unsigned serial, unsigned &year, unsigned &month, unsigned &day) const;

        // In YYYYMMDD format
        std::string to_string(bool pretty) const
        {
            unsigned year, month, day;
            // from serial modifies the year, month, and day by reference (in place)
            from_serial(m_serial, year, month, day);
            return pretty
                       ? std::to_string(day) + "-" + std::to_string(month) + "-" + std::to_string(year)
                       : std::to_string(year) + padding_dates(month) + padding_dates(day);
        }

    private:
        // Removed the internal representation of the year, month, day, and is_leap to simplify the code
        unsigned m_serial;
    };

    long operator-(const Date &d1, const Date &d2);

    inline double time_frac(const Date &d1, const Date &d2)
    {
        return static_cast<double>(d2 - d1) / 365.0;
    }

} // namespace minirisk