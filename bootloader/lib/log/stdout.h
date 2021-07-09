/**
 * Logging macros
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

/* (I) [bootloader]  message */
#define info(message, ...)                                                     \
  printf("(I) [bootloader]  " message "\n", ##__VA_ARGS__)

/* (D) [bootloader]  {file:function:line} message */
#define debug(message, ...)                                                    \
  printf("(D) [bootloader]  {%s:%s:%d} " message "\n", __FILE__,               \
         __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)

/* (W) [bootloader]  {file:function:line} message */
#define warning(message, ...)                                                  \
  printf("(W) [bootloader]  {%s:%s:%d} " message "\n", __FILE__,               \
         __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)

/* (E) [bootloader]  {file:function:line} message */
#define error(message, ...)                                                    \
  printf("(E) [bootloader]  {%s:%s:%d} " message "\n", __FILE__,               \
         __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
