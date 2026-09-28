#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;
    const struct device* dev = DEVICE_DT_GET_ANY(custom_driver);
    struct sensor_value value;

    if ( !device_is_ready(dev) )
    {
        return -ENODEV;
    }

    while (1) {
        if ( led_state )
        {
            sensor_sample_fetch( dev );
        }
        else
        {
            sensor_channel_get( dev, SENSOR_CHAN_ALL, &value );
        }
        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
