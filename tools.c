/**
 * circ - calculates the circular value according to a max and min values
 * @n: the value to be calculated
 * @min: minimum possible value
 * @max: maximum possible value
 *
 * Description: to better understand the function let's take an example:
 * a circle can have a maximum angel value of 359 and minimum of 0
 * so circ(350 + 40, 0, 359) = 30 .
 *
 * Return: Circular value of a number.
*/
float circ(float n, float min, float max)
{
        if (n > max)
                n = n - (max + 1);
        if (n < min)
                n = n + (max + 1);
        return n;
}

/**
 * absolute: calculates the absolute value of a number
 * n: the number
 * Return: the absolute value
*/
float absolute(float n)
{
        if (n < 0)
                return (n * -1);
        return n;
}
