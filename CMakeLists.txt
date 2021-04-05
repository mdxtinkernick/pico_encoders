cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#set(PICO_SDK_PATH "/home/pi/pico/pico-sdk")

include(pico_sdk_import.cmake)

project(pico_encoders C CXX ASM)

pico_sdk_init()

add_executable(encoders_pio)

pico_generate_pio_header(encoders_pio ${CMAKE_CURRENT_LIST_DIR}/encoders.pio)

target_sources(encoders_pio PRIVATE encoders.c)

target_link_libraries(encoders_pio PRIVATE
        pico_stdlib
        hardware_pio
        hardware_dma
        hardware_irq
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(encoders_pio 1)
pico_enable_stdio_uart(encoders_pio 0)

pico_add_extra_outputs(encoders_pio)

# add url via pico_set_program_url
#example_auto_set_url(encoders_pio)
