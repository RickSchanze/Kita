# 定义函数，接受子目录名作为参数
function(kita_add_subdirectory DIR_NAME)
    # 计算二进制目录下的子目录绝对路径
    set(GENERATED_DIR_PATH "${KITA_GLOBAL_GENERATED_DIR}/${DIR_NAME}")

    # 设置全局变量：格式为 <Dir>_GENERATED
    set(${DIR_NAME}_GENERATED_DIR
            ${GENERATED_DIR_PATH}
            CACHE INTERNAL "Path to generated files for ${DIR_NAME}"
    )
    # 收集目录下所有的.generated.hpp和.generated.cpp文件
    file(GLOB GENERATED_FILES "${GENERATED_DIR_PATH}/*.cpp" "${GENERATED_DIR_PATH}/*.hpp")
    set(${DIR_NAME}_GENERATED_FILES ${GENERATED_FILES} CACHE INTERNAL "All generated files for ${DIR_NAME}")
    # 添加子目录到构建系统
    add_subdirectory(${DIR_NAME})
endfunction()

kita_add_subdirectory(Core)
kita_add_subdirectory(Object)

include_directories(Generated)
