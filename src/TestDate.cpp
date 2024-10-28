#include <iostream>
#include <cassert>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include "Date.h"
using namespace minirisk;

void test_invalid_dates_by_y_m_d()
{
    unsigned invalid_count = 0;
    const unsigned num_tests = 1000;

    for (unsigned i = 0; i < num_tests; ++i)
    {
        unsigned year, month, day;
        if (rand() % 3 == 0)
        {
            // Generate an invalid year in the range [first_year - 1, last_year + 1]
            if (rand() % 2 == 0)
            {
                year = Date::first_year - 1 - (rand() % 100);
            }
            else
            {
                year = Date::last_year + (rand() % 100);
            }
            month = 1 + (rand() % 12); // Valid month
            day = 1 + (rand() % 28);   // Valid day
        }
        else if (rand() % 2 == 0)
        {
            // Generate an invalid month
            year = Date::first_year + (rand() % (Date::last_year - Date::first_year));
            if (rand() % 2 == 0)
            {
                month = 0;
            }
            else
            {
                month = 13 + (rand() % 10); // Random number between 13 and 22
            }
            day = 1 + (rand() % 28); // Valid day
        }
        else
        {
            // Generate an invalid day for a given month

            // days_in_month is a private member of Date class, so we just copy it here for simplicity
            std::array<unsigned, 12> days_in_month = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
            year = Date::first_year + (rand() % (Date::last_year - Date::first_year));
            month = 1 + (rand() % 12); // Valid month
            unsigned dmax = days_in_month[month - 1] + ((month == 2 && Date::is_leap_year(year)) ? 1 : 0);
            day = dmax + 1 + (rand() % 10); // Random number greater than dmax
        }

        try
        {
            Date invalid_date(year, month, day);
            assert(false); // Should not reach here; fail the test if no exception is thrown
        }
        catch (const std::invalid_argument &e)
        {
            ++invalid_count;
        }
    }

    assert(invalid_count == num_tests); // Ensure all invalid dates were caught
    std::cout << "SUCCESS" << std::endl;
}

void test_invalid_dates_by_serial()
{
    unsigned invalid_count = 0;
    const unsigned num_tests = 1000;
    for (unsigned i = 0; i < num_tests; ++i)
    {
        unsigned serial;
        if (rand() % 2 == 0)
        {
            // Generate a negative serial number
            serial = -1 - (rand() % 1000);
        }
        else
        {
            // Generate a serial number beyond the valid range
            serial = 109572 + 1 + (rand() % 1000);
        }

        try
        {
            Date::check_valid(serial);
            assert(false);
        }
        catch (const std::invalid_argument &e)
        {
            ++invalid_count;
        }
    }

    assert(invalid_count == num_tests); // Ensure all invalid serials were caught
    std::cout << "SUCCESS" << std::endl;
}

void test1()
{
    // Chains the testing functions together
    std::cout << "Testing requirement 1:" << std::endl;
    std::cout << "Testing invalid dates by year, month, and day" << std::endl;
    test_invalid_dates_by_y_m_d();
    std::cout << "" << std::endl;
    std::cout << "Testing invalid dates by serial" << std::endl;
    test_invalid_dates_by_serial();
    std::cout << "" << std::endl;
}

void test_date_to_serial_conversion()
{
    int invalid_count = 0;
    const std::array<unsigned, 12> days_in_month = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

    for (unsigned year = Date::first_year; year < Date::last_year; ++year)
    {
        for (unsigned month = 1; month <= 12; ++month)
        {
            unsigned dmax = days_in_month[month - 1] + ((month == 2 && Date::is_leap_year(year)) ? 1 : 0);
            for (unsigned day = 1; day <= dmax; ++day)
            {
                Date d(year, month, day);
                // Convert the date to serial
                unsigned serial = d.serial(year, month, day);
                Date::check_valid(serial);
                // Temp variables to store the year, month, and day
                unsigned year2, month2, day2;
                // Convert the serial back to date
                d.Date::from_serial(serial, year2, month2, day2);
                if (year != year2 || month != month2 || day != day2)
                {
                    ++invalid_count;
                }
            }
        }
    }
    assert(invalid_count == 0);
    std::cout << "SUCCESS" << std::endl;
}

void test2()
{
    std::cout << "Testing requirement 2:" << std::endl;
    std::cout << "Testing date to serial conversion" << std::endl;
    test_date_to_serial_conversion();
    std::cout << "" << std::endl;
}
void test_contiguous_serial()
{
    unsigned invalid_count = 0;
    const std::array<unsigned, 12> days_in_month = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

    for (unsigned year = Date::first_year; year < Date::last_year; ++year)
    {
        for (unsigned month = 1; month <= 12; ++month)
        {
            unsigned dmax = days_in_month[month - 1] + ((month == 2 && Date::is_leap_year(year)) ? 1 : 0);
            for (unsigned day = 1; day < dmax; ++day)
            {
                Date d1(year, month, day);
                Date d2(year, month, day + 1);
                unsigned serial1 = d1.serial(year, month, day);
                unsigned serial2 = d2.serial(year, month, day + 1);
                if (serial2 - serial1 != 1)
                {
                    ++invalid_count;
                }
            }
            // Handle the transition to the next month
            if (month < 12)
            {
                Date d1(year, month, dmax);
                Date d2(year, month + 1, 1);
                unsigned serial1 = d1.serial(year, month, dmax);
                unsigned serial2 = d2.serial(year, month + 1, 1);
                if (serial2 - serial1 != 1)
                {
                    ++invalid_count;
                }
            }
            // Handle the transition to the next year
            else if (year < Date::last_year - 1)
            {
                Date d1(year, 12, 31);
                Date d2(year + 1, 1, 1);
                unsigned serial1 = d1.serial(year, 12, 31);
                unsigned serial2 = d2.serial(year + 1, 1, 1);
                if (serial2 - serial1 != 1)
                {
                    ++invalid_count;
                }
            }
        }
    }
    assert(invalid_count == 0);
    std::cout << "SUCCESS" << std::endl;
}

void test3()
{
    std::cout << "Testing requirement 3:" << std::endl;
    std::cout << "Testing contiguous serial numbers" << std::endl;
    test_contiguous_serial();
    std::cout << "" << std::endl;
}

int main()
{
    unsigned int seed = static_cast<unsigned int>(time(0));
    std::cout << "The seed used ensure reproducibiltiy is : " << seed << std::endl;
    std::cout << "" << std::endl;

    test1();
    test2();
    test3();
    return 0;
}