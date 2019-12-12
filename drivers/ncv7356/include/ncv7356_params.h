/*
 * Copyright (C) 2019 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_ncv7356
 *
 * @{
 * @file
 * @brief       Default configuration for NCV7356 devices
 *
 * @author      Jannes Volkens <jannes.volkens@haw-hamburg.de>
 */

#ifndef NCV7356_PARAMS_H
#define NCV7356_PARAMS_H

#include "board.h"
#include "ncv7356.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name   Set default configuration parameters for the ncv7356
 * @ingroup config
 * @{
 */
 #ifndef NCV7356_DRIVER
 #define NCV7356_DRIVER     &ncv7356_driver
 #endif

 #ifndef NCV7356_TRX
 #define NCV7356_TRX        { .driver = NCV7356_DRIVER}
 #endif

 #ifndef NCV7356_PARAMS
 #define NCV7356_PARAMS     { .trx = NCV7356_TRX, \
                              .mode0_pin = NCV7356_MODE0_PIN, \
                              .mode1_pin = NCV7356_MODE1_PIN}
 #endif
 /**@}*/

 /**
  * @brief   NCV7356 configuration
  */
 static const ncv7356_trx_t ncv7356_params[] =
 {
     NCV7356_PARAMS
 };

#ifdef __cplusplus
}
#endif

#endif /* NCV7356_PARAMS_H */
/** @} */
