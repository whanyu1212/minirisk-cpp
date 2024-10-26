#include <iostream>
#include <cassert>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include "Date.h"
using namespace minirisk;

void test_invalid_dates()
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

    std::cout << "Invalid date test count: " << invalid_count << std::endl;
    assert(invalid_count == num_tests); // Ensure all invalid dates were caught
    std::cout << "Date validation test passed." << std::endl;
    std::cout << "SUCCESS" << std::endl;
}
void test1()
{
    // We want to test that it is able to flag out invalid Date object created in different ways:
    // 1. Invalid year
    // 2. Invalid month
    // 3. Invalid day for a given month, especially for February in a leap year
    test_invalid_dates();
}

void test2()
{
}

void test3()
{
}

int main()
{
    unsigned int seed = static_cast<unsigned int>(time(0));
    std::cout << "The seed used ensure reproducibiltiy is : " << seed << std::endl;

    test1();
    test2();
    test3();
    return 0;
}