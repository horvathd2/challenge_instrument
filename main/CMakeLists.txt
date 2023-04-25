add_executable(picow_tcpip_client_background
        picow_tcp_client.c
        )

pico_generate_pio_header(picow_tcpip_client_background ${CMAKE_CURRENT_LIST_DIR}/encoder.pio)

target_include_directories(picow_tcpip_client_background PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
        )

target_link_libraries(picow_tcpip_client_background
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        pico_multicore
        hardware_pio
        hardware_pwm
        )

pico_add_extra_outputs(picow_tcpip_client_background)
pico_enable_stdio_usb(picow_tcpip_client_background 1)