idf_component_register(SRCS "A2dpSource.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES esp_peripherals bt bluetooth_service audio_stream
                    )
