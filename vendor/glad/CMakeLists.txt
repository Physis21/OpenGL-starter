add_library(glad)

target_sources(glad
    PRIVATE glad.c
    PUBLIC
        FILE_SET HEADERS
        FILES glad.h
)