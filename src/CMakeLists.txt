file(GLOB_RECURSE source CONFIGURE_DEPENDS *.h *.cpp *.qrc *.ui)
set(source_shaders ${CMAKE_CURRENT_SOURCE_DIR}/shaders.qrc)
set(source_resources ${PROJECT_SOURCE_DIR}/resources/resources.qrc)

find_package(Qt5 COMPONENTS Gui OpenGL REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_executable(learnopengl ${source} ${source_shaders} ${source_resources})

set_target_properties(learnopengl PROPERTIES 
    AUTOUIC ON
    AUTOMOC ON
    AUTORCC ON
    WIN32_EXECUTABLE OFF
)

target_link_libraries(learnopengl PRIVATE assimp::assimp)
target_link_libraries(learnopengl PRIVATE Qt5::Gui Qt5::OpenGL)

target_include_directories(learnopengl PRIVATE .)