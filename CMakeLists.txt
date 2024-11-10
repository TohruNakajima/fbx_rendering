# CMakeList.txt : 最上位の CMake プロジェクト ファイル。グローバル構成を行います
# また、サブプロジェクトをここに含めます。
#
cmake_minimum_required (VERSION 3.8)

# サポートされている場合は、MSVC コンパイラのホット リロードを有効にします。
cmake_policy(SET CMP0079 NEW)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


project ("fbx_models")

# サブプロジェクトを含めます。
add_subdirectory ("fbx_models")
set(GLEW_INCLUDE_DIR "C:/Program Files/glew/include")
set(GLEW_LIBRARY "C:/Program Files/glew/lib/Release/x64/glew32s.lib")

target_include_directories(fbx_models PRIVATE "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.7/include")
target_include_directories(fbx_models PRIVATE "C:/Program Files/glew/include")
target_include_directories(fbx_models PRIVATE "C:/Program Files/glfw/include")

set(GLM_INCLUDE_DIR "C:/Program Files/glm/include")

target_include_directories(fbx_models PRIVATE ${GLM_INCLUDE_DIR})

target_include_directories(fbx_models PRIVATE ${GLEW_INCLUDE_DIR})
target_link_libraries(fbx_models PRIVATE "C:/Program Files/glfw/lib-vc2022/glfw3.lib")



# Set the runtime library for Debug/Release configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # For Debug builds, use the Debug FBX SDK library
    set(FBX_SDK_LIB "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.7/lib/x64/debug/libfbxsdk-md.lib")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MDd")  # Use /MDd for Debug mode (Multi-threaded Debug DLL)
    set(LIBXML2_LIB "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.7/lib/x64/debug/libxml2-md.lib")
    set(LIBZ_LIB "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.7/lib/x64/debug/zlib-md.lib")
    set(GLEW_LIB "C:/Program Files/glew/lib/Release/x64/glew32s.lib")
    set(GLFW_INCLUDE_DIR "C:/Program Files/glfw/include")
    set(GLFW_LIBRARY "C:/Program Files/glfw/lib-vc2022/glfw3.lib")  # Path to the glfw3.lib file
    set(GLM_LIBRARY "C:/Program Files/glm/lib/Debug/glm.lib")


else()
    # For Release builds, use the Release FBX SDK library
    set(FBX_SDK_LIB "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.7/lib/x64/release/libfbxsdk-md.lib")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD")  # Use /MD for Release mode (Multi-threaded DLL)
    set(LIBXML2_LIB "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.7/lib/x64/release/libxml2-md.lib")
    set(LIBZ_LIB "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.7/lib/x64/release/zlib-md.lib")
    set(GLEW_LIB "C:/Program Files/glew/lib/Release/x64/glew32s.lib")
    set(GLFW_INCLUDE_DIR "C:/Program Files/glfw/include")
    set(GLFW_LIBRARY "C:/Program Files/glfw/lib-vc2022/glfw3.lib")  # Path to the glfw3.lib file
    set(GLM_LIBRARY "C:/Program Files/glm/lib/Release/glm.lib")
endif()

include_directories(${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR})

target_link_libraries(fbx_models PRIVATE ${GLEW_LIBRARY} OpenGL::GL ${GLFW_LIBRARY})

set(LIBXML2_INCLUDE_DIR "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.7/include/libxml2")


# Link the FBX SDK library
target_link_libraries(fbx_models PRIVATE ${FBX_SDK_LIB})
target_link_libraries(fbx_models PRIVATE ${LIBXML2_LIB})
target_link_libraries(fbx_models PRIVATE ${LIBZ_LIB})
target_link_libraries(fbx_models PRIVATE ${GLEW_LIB})
target_link_libraries(fbx_models PRIVATE ${GLM_LIBRARY})

find_package(Threads REQUIRED)
target_link_libraries(fbx_models PRIVATE Threads::Threads)

# Find OpenGL
find_package(OpenGL REQUIRED)