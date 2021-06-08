cmake_minimum_required(VERSION 3.19)
project(TEXAS_TM4C_Experiment)

set(CMAKE_CXX_STANDARD 14)

add_compile_definitions(RTOS_LOS)

include_directories(LiteOS/arch/arm/arm-m/include)
include_directories(LiteOS/kernel/include)
include_directories(LiteOS/kernel/base/include)
include_directories(LiteOS/kernel/extended/include)
include_directories(LiteOS/OS_CONFIG)

include_directories(C:/ti/TivaWare_C_Series-2.2.0.295/driverlib)
include_directories(C:/ti/TivaWare_C_Series-2.2.0.295/inc)

include_directories(Drivers)
include_directories(Config)
include_directories(Application)
include_directories(Multimedia)

file(GLOB_RECURSE SOURCES
        "LiteOS/*.*"
        "Config/*.*"
        "Drivers/*.*"
        "Application/*.*"
        "Multimedia/*.*"
        "RTE/Device/TM4C1294NCPDT/*.*"
        "RTE/RTOS/*.*")

add_executable(TEXAS_TM4C_Experiment
        ${SOURCES}
        main.c)
