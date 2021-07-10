#@ main.cpp
aux_source_directory(. SRC_LIST)

#@Camara
aux_source_directory(Camara CAMARA)
include_directories(Camara)

#@ GreenSpot
aux_source_directory(GreenSpot GREEN_SPOT)
include_directories(GreenSpot)
#@ Serial
aux_source_directory(Serial SERIAL)
include_directories(Serial)
#@ Socket
aux_source_directory(DartSocket DART_SOCKET)
include_directories(DartSocket)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# 设置可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 生成可执行文件以及相关依赖
add_executable(${PROJECT_NAME} ${SRC_LIST} ${CAMARA}  ${GREEN_SPOT} ${SERIAL} ${DART_SOCKET})
link_directories(${OpenCV_LIBRARY_DIRS})

#pthread
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-pthread" LINK_FLAGS "-pthread")

# 连接动态库
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS}  pthread)
