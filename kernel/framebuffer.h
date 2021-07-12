#pragma once

struct Framebuffer
{
    unsigned int *base;
    unsigned long long buffer_size;
    unsigned int width;
    unsigned int height;
    unsigned int ppscl;
};
