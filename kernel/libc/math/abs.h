#include "libstdc++/type_traits.h"

/**
 * absolute value function
 *
 * https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl
 */
template <typename T>
requires(is_integral<T>::value || is_floating_point<T>::value) 
int
abs(T number)
{
	if(number < 0)
		return number * -1;
	return number;
}
