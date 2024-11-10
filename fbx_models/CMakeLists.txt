# CMakeList.txt : fbx_models の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (fbx_models "fbx_models.cpp" "fbx_models.h" "fbx_render.cpp" "stb_image.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET fbx_models PROPERTY CXX_STANDARD 20)
endif()

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
