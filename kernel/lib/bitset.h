/**
 * Bitset class
 *
 * An "array of bits"
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace std {

/**
 * Class that manages a imaginary array of bits
 *
 * It's an array of uint8_t where you can access each bit with operator[]. Also you can set/unset
 * bits
 *
 * @info You need to provide your own buffer
 */
class bitset
{
  private:
    size_t size;
    uint8_t *buffer;

  public:
    bitset();
    void operator=(bitset &&);
    bool operator[](size_t);
    void set(size_t);
    void unset(size_t);

    /**
     * Return buffer pointer
     */
    uint8_t *get_buffer()
    {
        return this->buffer;
    }

    /**
     * Set buffer pointer
     */
    void set_buffer(uint8_t *buffer)
    {
        this->buffer = buffer;
    }

    /**
     * Return number of bits
     */
    size_t get_size()
    {
        return this->size;
    }

    /**
     * Set number of bits
     *
     * @warning Not used to check correct access
     */
    void set_size(size_t size)
    {
        this->size = size;
    }
};

} // namespace std
