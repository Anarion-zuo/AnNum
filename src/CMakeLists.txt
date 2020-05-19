project (AnNum)

#aux_source_directory(. DIR_SRCS)
add_library(anarionnum STATIC

        numeric_env/NumericConfig.cpp
        numeric_env/NumericComponent.cpp
        numeric_env/NumericContext.cpp
        )

add_library(anarionbase STATIC IMPORTED)
set_target_properties(anarionbase PROPERTIES IMPORTED_LOCATION ../lib/libanarionbase.a)

target_link_libraries(anarionnum pthread anarionbase)
include_directories(../include)
include_directories(../include/base)
