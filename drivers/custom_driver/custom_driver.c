#include "zephyr/device.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT custom_driver

LOG_MODULE_REGISTER(custom_driver, LOG_LEVEL_INF);

struct custom_driver_config {
    int test;
    struct gpio_dt_spec gpio;
};

struct custom_driver_data {
    int state;
};

static int custom_get( const struct device* dev,
                        enum sensor_channel chan,
                        struct sensor_value *val )
{
    const struct custom_driver_config* pConfig = dev->config; 
    gpio_pin_set_dt( &pConfig->gpio, 0 );
    return 0;
}

static int custom_fetch( const struct device* dev,
                            enum sensor_channel chan )
{
    const struct custom_driver_config* pConfig = dev->config; 
    gpio_pin_set_dt( &pConfig->gpio, 1 );
    return 0;
}

static DEVICE_API( sensor, custom_api ) = {
        .channel_get = custom_get,
        .sample_fetch = custom_fetch,
};

static int custom_driver_init ( const struct device *dev )
{
    const struct custom_driver_config* pConfig = dev->config; 
    if (!gpio_is_ready_dt(&pConfig->gpio)) return -ENODEV;

    if (gpio_pin_configure_dt(&pConfig->gpio, GPIO_OUTPUT_ACTIVE) < 0) return -ENODEV;
    LOG_INF( "Custom Driver Init!" );
    return 0;
}

#define DRIVER_INIT(inst) \
    static struct custom_driver_data data_##inst; \
    static const struct custom_driver_config cfg_##inst = { \
        .test = 3, \
        .gpio = GPIO_DT_SPEC_GET(DT_ALIAS(app_led), gpios ) \
    }; \
    DEVICE_DT_INST_DEFINE( inst , \
                            custom_driver_init, \
                            NULL, \
                            &data_##inst, \
                            &cfg_##inst, \
                            POST_KERNEL, \
                            80, \
                            &custom_api \
                        );

DT_INST_FOREACH_STATUS_OKAY(DRIVER_INIT);

int custom_driver_extra( const struct device* dev )
{
    const struct custom_driver_config* pConfig = dev->config;  
    struct custom_driver_data* pData = dev->data;
    pData->state = gpio_pin_get_dt( &pConfig->gpio );
    return pData->state;
}






