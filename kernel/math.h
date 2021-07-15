/**
 * Math functions
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

unsigned int int_log10(unsigned int);

/** 
 * absolute value function
 *
 * https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl 
 */
template<typename T>
T
abs(T number)
{
    if (number < 0)
        return (number * -1);
    return number;
}

unsigned int pow(unsigned int, unsigned int);
