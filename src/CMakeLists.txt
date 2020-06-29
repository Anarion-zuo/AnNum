project (AnNum)

#aux_source_directory(. DIR_SRCS)
add_library(anarionnum STATIC
#        ../include/container/number_trait.h
        numeric_env/NumericConfig.cpp
        numeric_env/NumericComponent.cpp
        numeric_env/NumericContext.cpp

        data/Series.cpp
        data/SeriesOperator.cpp

        operators/RandomGenerator.cpp

        model/BatchLinearRegression.cpp
        )

add_library(anarionbase STATIC IMPORTED)
set_target_properties(anarionbase PROPERTIES IMPORTED_LOCATION ../lib/libanarionbase.a)

add_library(anariondata STATIC IMPORTED)
set_target_properties(anariondata PROPERTIES IMPORTED_LOCATION ../lib/libanariondata.a)

target_link_libraries(anarionnum pthread anarionbase anariondata)
include_directories(../include)
include_directories(../include/base)
include_directories(../include/base_data)
