add_executable(wasm-nm driver.cpp)
add_dependencies(wasm-nm WasmBin)
target_link_libraries(wasm-nm WasmBin)