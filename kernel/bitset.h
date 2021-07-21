#pragma once

#include <stddef.h>
#include <stdint.h>

namespace std {

class bitset
{
  private:
    size_t size;
    uint8_t *buffer;

  public:
    bool operator[](size_t);
    void set(size_t);
    void unset(size_t);

    inline size_t get_size()
    {
        return this->size;
    }

    inline void set_buffer(uint8_t *buffer)
    {
        this->buffer = buffer;
    }

    inline void set_size(size_t size)
    {
        this->size = size;
    }
};

} // namespace std
