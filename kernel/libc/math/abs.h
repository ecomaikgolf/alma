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