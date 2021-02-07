add_executable(encoders_pio)

pico_generate_pio_header(encoders_pio ${CMAKE_CURRENT_LIST_DIR}/encoders.pio)

target_sources(encoders_pio PRIVATE encoders.c)

target_link_libraries(encoders_pio PRIVATE
        pico_stdlib
        hardware_pio
        )

pico_add_extra_outputs(encoders_pio)

# add url via pico_set_program_url
example_auto_set_url(encoders_pio)
