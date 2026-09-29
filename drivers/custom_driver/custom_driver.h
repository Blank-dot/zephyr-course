#ifndef CUSTOM_DRIVER_H
#define CUSTOM_DRIVER_H

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif
int custom_driver_extra( const struct device* dev );
#ifdef __cplusplus
}
#endif

#endif
