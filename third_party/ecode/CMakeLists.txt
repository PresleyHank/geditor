# Created by Alex
set(CMAKE_CXX_STANDARD 14)
set(ecode_include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/ CACHE INTERNAL "")
aux_source_directory(${ecode_include_dirs} sources)
add_library(ecode STATIC ${sources})
target_include_directories(ecode PUBLIC ${ecode_include_dirs})
