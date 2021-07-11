/**
 * Bootloader return codes
 *
 * @author Ernesto Martínez García <me@ecomaikgolf.com>
 */

#pragma once

enum {
  SUCCESS,
  INCORRECT_ARGC,
  KERNEL_LOAD_FAILURE,
  GOP_RETRIEVE_FAILURE,
  FRAMEBUFFER_FAILURE,
  PSF1_FAILURE,
};
