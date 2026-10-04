#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include "custom_driver.h"

static void dumpArgs( const struct shell* sh, size_t argc, char** argv )
{
    shell_print( sh, "argc = %d", argc );
    for ( size_t cnt = 0; cnt < argc; cnt++ )
    {
        shell_print( sh, "argv[%d] = %s", cnt, argv[cnt] );
    }
}

static int cmd_sensor_info( const struct shell* sh, size_t argc, char** argv )
{
    const struct device* pDev;

    dumpArgs( sh, argc, argv );

    pDev = device_get_by_dt_nodelabel( argv[1] );
    if ( pDev )
    {
        shell_print( sh, "%s", pDev->name );
        shell_print( sh, "Device State: %s", device_is_ready( pDev ) ? "READY" : "NOT READY" );
        shell_print( sh, "Pin State: %s", custom_driver_extra( pDev) ? "ON" : "OFF" );
    }
    else 
    {
        shell_error( sh, "Invalid node label" );
        return -EINVAL;
    }
    
    return 0;
}

static int cmd_sensor_fetch( const struct shell* sh, size_t argc, char** argv )
{
    const struct device* pDev;

    dumpArgs( sh, argc, argv );
    pDev = device_get_by_dt_nodelabel( argv[1] );
    if ( pDev )
    {
        sensor_sample_fetch( pDev );
    }
    else 
    {
        shell_error( sh, "Invalid node label" );
        return -EINVAL;
    }


    return 0;
}

static int cmd_sensor_get( const struct shell* sh, size_t argc, char** argv )
{
    const struct device* pDev;

    dumpArgs( sh, argc, argv );
    pDev = device_get_by_dt_nodelabel( argv[1] );
    if ( pDev )
    {
        sensor_channel_get( pDev, SENSOR_CHAN_ALL, NULL );
    }
    else 
    {
        shell_error( sh, "Invalid node label" );
        return -EINVAL;
    }


    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sub_sensor,
    SHELL_CMD_ARG( fetch, NULL, SHELL_HELP("led on", "<node label name>"), cmd_sensor_fetch, 2, 0 ),
    SHELL_CMD_ARG( read, NULL, SHELL_HELP("led off", "<node label name>"), cmd_sensor_get, 2, 0 ),
    SHELL_CMD_ARG( info, NULL, SHELL_HELP("custom driver info", "<node label name>"), cmd_sensor_info, 2, 0 ),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER( sensor,
                    &sub_sensor,
                    "custom_driver_shell commands",
                    NULL
                  );
