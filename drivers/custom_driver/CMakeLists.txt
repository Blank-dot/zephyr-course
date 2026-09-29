if ( CONFIG_CUSTOM_DRIVER )
    target_sources( app PRIVATE custom_driver.c )
    target_include_directories( app PRIVATE . )
endif()
