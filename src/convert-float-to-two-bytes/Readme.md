# Encoding of a float value on a two byte array

## Most naive approach
In this example, we want to encode a (unsigned) float value to a given number of bits (e.g 2 times 8 bits here).
We have as a requirements the accuracy of the Less Significant Bit [(LSB)](./main.cpp#L8). 
That is what is the value of the float when the bit array is `000...001`.

We compute the scaled value.
$$
V_{scaled}~=~\frac{V}{LSB}
$$

We then just convert the value to an integer.
Using a [static cast](./main.cpp#L13)

### Limitations
- What if the float value you have can be negative ? 
- What if by design, the float value cannot be 0 but instead, is contained withing a given range ? (Think about temperature reading).
- The scaling and `static_cast` will not do the best rounding computation...
