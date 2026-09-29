#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <custom_driver.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device* dev = DEVICE_DT_GET_ANY(custom_driver);
    struct sensor_value value;
    int led_state;
    if ( !device_is_ready(dev) )
    {
        return -ENODEV;
    }

    while (1) {
        led_state = custom_driver_extra( dev );
        if ( led_state )
        {
            sensor_channel_get( dev, SENSOR_CHAN_ALL, &value );
        }
        else
        {
            sensor_sample_fetch( dev );
        }
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
