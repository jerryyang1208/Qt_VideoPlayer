cmake_minimum_required(VERSION 3.21)
project(VideoPlayer VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

# 自动开启 MOC / UIC / RCC，统一 Qt 构建流程
qt_standard_project_setup()

# 编译 Qt 资源文件（图标等）
qt_add_resources(RESOURCE_FILES Resource.qrc)

# 可执行目标
qt_add_executable(VideoPlayer
    main.cpp
    VideoPlayer.cpp
    VideoPlayer.h
    VideoPlayer.ui
    ${RESOURCE_FILES}
)

# 链接 Qt 库
target_link_libraries(VideoPlayer PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)

# Windows 下设为 FALSE 保留控制台（方便看日志），发布版改回 TRUE
set_target_properties(VideoPlayer PROPERTIES
    WIN32_EXECUTABLE FALSE
)

# macOS  bundle 配置
if(APPLE)
    set_target_properties(VideoPlayer PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VideoPlayer
    )
endif()
