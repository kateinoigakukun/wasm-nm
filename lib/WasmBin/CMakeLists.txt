add_library(WasmBin STATIC)
target_sources(WasmBin
  PRIVATE
    Deserialize.cpp)

if(XCODE)
  target_sources(WasmBin
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/../../include/lib/WasmBin/Module.h
      ${CMAKE_CURRENT_SOURCE_DIR}/../../include/lib/WasmBin/Deserialize.h)
endif()