cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(practica4 C CXX ASM)

pico_sdk_init()

add_executable(practica4
    main.c
    kernel_drivers.c
    kernel_service.c
    svc_handler.s
    pendsv.s
    scheduler.c       
    user_app.c
    syscalls.s
)

pico_enable_stdio_usb(practica4 1)
pico_enable_stdio_uart(practica4 0)

target_link_libraries(practica4
    pico_stdlib
    hardware_exception
)

pico_add_extra_outputs(practica4)