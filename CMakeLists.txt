cmake_minimum_required(VERSION 3.21)  # 适配 Qt6 最佳实践，支持更完善的 Qt 模块处理

project(VideoPlayer VERSION 0.1 LANGUAGES CXX)

# 启用 Qt 的自动处理功能
set(CMAKE_AUTOUIC ON)   # 自动处理.ui文件（Qt界面设计文件）
set(CMAKE_AUTOMOC ON)   # 自动处理Qt的元对象系统（Q_OBJECT宏等）
set(CMAKE_AUTORCC ON)   # 自动处理Qt资源文件（.qrc）

# 设置 C++ 标准为 C++17 并要求编译器必须支持该标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 库：指定需要的 Qt 组件（Widgets 用于界面，Multimedia 和 MultimediaWidgets 用于音视频处理）
# 优先查找 Qt6，如果找不到则查找 Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

# 定义项目源文件列表，包含所有需要编译的文件
set(PROJECT_SOURCES
        main.cpp             # 程序入口文件
        VideoPlayer.cpp      # 主窗口实现文件
        VideoPlayer.h        # 主窗口头文件
        VideoPlayer.ui       # 主窗口界面设计文件
        Resource.qrc         # 资源文件（包含图片、图标等）
)

# 根据 Qt 版本选择不同的可执行文件生成方式
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # Qt6 及以上版本使用 qt_add_executable 命令生成可执行文件
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION  # 手动处理最终化步骤（如 macOS 应用束设置）
        ${PROJECT_SOURCES}   # 引入上面定义的源文件列表
    )
else()
    # Qt5 版本的处理方式
    if(ANDROID)
        # 安卓平台下生成共享库（.so）
        add_library(VideoPlayer SHARED
            ${PROJECT_SOURCES}
        )
    else()
        # 其他平台（Windows、Linux、macOS）生成可执行文件
        add_executable(VideoPlayer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接 Qt 库到项目：将 Qt 的 Widgets、Multimedia、MultimediaWidgets 模块链接到可执行文件
target_link_libraries(VideoPlayer PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
)

# Qt6.1 之前的版本需要手动设置 MACOSX_BUNDLE_GUI_IDENTIFIER
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VideoPlayer)
endif()

# 设置目标可执行文件的属性
set_target_properties(VideoPlayer PROPERTIES
    ${BUNDLE_ID_OPTION}                      # 应用唯一标识（仅 macOS）
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}           # 应用束版本号
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}  # 短版本号（如 0.1）
    MACOSX_BUNDLE TRUE                       # 生成macOS应用束（.app）
    WIN32_EXECUTABLE TRUE                    # 生成Windows GUI应用（而非控制台应用）
)

# 引入 GNU 安装目录规范（定义标准安装路径变量）
include(GNUInstallDirs)

# 定义安装规则：指定目标文件的安装位置
install(TARGETS VideoPlayer
    BUNDLE DESTINATION .                         # macOS 应用束安装到当前目录
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}  # 库文件安装到标准库目录
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}  # 可执行文件安装到标准二进制目录
)

# Qt6 特有的最终化步骤：完成应用程序的构建配置（如生成 plist 文件等）
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VideoPlayer)
endif()
