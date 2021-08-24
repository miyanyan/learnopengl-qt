QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SRC_PATH=$$PWD

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    1.getting_started/1.1hello_window/HelloWindow.cpp \
    1.getting_started/2.1hello_triangle/HelloTriangle.cpp \
    1.getting_started/2.2hello_triangle_indexed/HelloTriangleIndexed.cpp \
    1.getting_started/2.3hello_triangle_exercise1/HelloTriangleExercise1.cpp \
    1.getting_started/2.4hello_triangle_exercise2/HelloTriangleExercise2.cpp \
    1.getting_started/2.5hello_triangle_exercise3/HelloTriangleExercise3.cpp \
    1.getting_started/3.2shaders_interpolation/ShadersInterpolation.cpp \
    1.getting_started/4.1textures/Textures.cpp \
    1.getting_started/4.4textures_exercise2/TexturesExercise2.cpp \
    1.getting_started/4.5textures_exercise3/TexturesExercise3.cpp \
    1.getting_started/4.6textures_exercise4/TexturesExercise4.cpp \
    1.getting_started/5.1transformations/Transformations.cpp \
    1.getting_started/6.1coordinate_systems/CoordinateSystems.cpp \
    1.getting_started/6.2coordinate_systems_depth/CoordinateSystemsDepth.cpp \
    1.getting_started/6.3coordinate_systems_exercise/CoordinateSystemsExercise.cpp \
    1.getting_started/7.1camera_circle/CameraCircle.cpp \
    1.getting_started/7.2camera_interact/CameraInteract.cpp \
    1.getting_started/7.4camera_class/CameraClass.cpp \
    2.lighting/1.colors/Colors.cpp \
    2.lighting/2.1basic_lighting_diffuse/BasicLightingDiffuse.cpp \
    2.lighting/2.2basic_lighting_specular/BasicLightingSpecular.cpp \
    2.lighting/2.3basic_lighting_exercise1/BasicLightingExercise1.cpp \
    2.lighting/2.5basic_lighting_exercise3/BasicLightingExercise3.cpp \
    2.lighting/3.1materials/Materials.cpp \
    2.lighting/4.1lighting_maps_diffuse_map/LightingMapsDiffuseMap.cpp \
    2.lighting/4.2lighting_maps_specular_map/LightingMapsSpecularMap.cpp \
    2.lighting/4.3lighting_maps_exercise2/LightingMapsExercise2.cpp \
    2.lighting/4.4lighting_maps_exercise4/LightingMapsExercise4.cpp \
    2.lighting/5.1light_casters_directional/LightCastersDirectional.cpp \
    2.lighting/5.2light_casters_point/LightCastersPoint.cpp \
    2.lighting/5.3light_casters_spot/LightCastersSpot.cpp \
    2.lighting/6.1multiple_lights/MultipleLights.cpp \
    3.model_loading/1.model_loading/ModelLoading.cpp \
    4.advanced_opengl/1.1depth_testing/DepthTesting.cpp \
    4.advanced_opengl/10.1instancing_quads/InstancingQuads.cpp \
    4.advanced_opengl/2.stencil_testing/StencilTesting.cpp \
    4.advanced_opengl/3.1blending_discard/BlendingDiscard.cpp \
    4.advanced_opengl/3.2blending_sort/BlendingSort.cpp \
    4.advanced_opengl/4.face_culling_exercise1/FaceCullingExercise1.cpp \
    4.advanced_opengl/5.1framebuffers/Framebuffers.cpp \
    4.advanced_opengl/5.2framebuffers_exercise1/FramebuffersExercise1.cpp \
    4.advanced_opengl/6.1cubemaps_skybox/CubemapsSkybox.cpp \
    4.advanced_opengl/6.2cubemaps_environment_mapping/CubemapsEnvironmentMapping.cpp \
    4.advanced_opengl/8.advanced_glsl_ubo/AdvancedGlslUbo.cpp \
    4.advanced_opengl/9.1geometry_shader_houses/GeometryShaderHouses.cpp \
    4.advanced_opengl/9.2geometry_shader_exploding/GeometryShaderExploding.cpp \
    4.advanced_opengl/9.3geometry_shader_normals/GeometryShaderNormals.cpp \
    main.cpp \
    MainWindow.cpp \
    utils/CameraUtil.cpp \
    utils/Mesh.cpp \
    utils/ModelUtil.cpp \
    utils/OpenGLUniformbufferObject.cpp

HEADERS += \
    1.getting_started/1.1hello_window/HelloWindow.h \
    1.getting_started/2.1hello_triangle/HelloTriangle.h \
    1.getting_started/2.2hello_triangle_indexed/HelloTriangleIndexed.h \
    1.getting_started/2.3hello_triangle_exercise1/HelloTriangleExercise1.h \
    1.getting_started/2.4hello_triangle_exercise2/HelloTriangleExercise2.h \
    1.getting_started/2.5hello_triangle_exercise3/HelloTriangleExercise3.h \
    1.getting_started/3.2shaders_interpolation/ShadersInterpolation.h \
    1.getting_started/4.1textures/Textures.h \
    1.getting_started/4.4textures_exercise2/TexturesExercise2.h \
    1.getting_started/4.5textures_exercise3/TexturesExercise3.h \
    1.getting_started/4.6textures_exercise4/TexturesExercise4.h \
    1.getting_started/5.1transformations/Transformations.h \
    1.getting_started/6.1coordinate_systems/CoordinateSystems.h \
    1.getting_started/6.2coordinate_systems_depth/CoordinateSystemsDepth.h \
    1.getting_started/6.3coordinate_systems_exercise/CoordinateSystemsExercise.h \
    1.getting_started/7.1camera_circle/CameraCircle.h \
    1.getting_started/7.2camera_interact/CameraInteract.h \
    1.getting_started/7.4camera_class/CameraClass.h \
    2.lighting/1.colors/Colors.h \
    2.lighting/2.1basic_lighting_diffuse/BasicLightingDiffuse.h \
    2.lighting/2.2basic_lighting_specular/BasicLightingSpecular.h \
    2.lighting/2.3basic_lighting_exercise1/BasicLightingExercise1.h \
    2.lighting/2.5basic_lighting_exercise3/BasicLightingExercise3.h \
    2.lighting/3.1materials/Materials.h \
    2.lighting/4.1lighting_maps_diffuse_map/LightingMapsDiffuseMap.h \
    2.lighting/4.2lighting_maps_specular_map/LightingMapsSpecularMap.h \
    2.lighting/4.3lighting_maps_exercise2/LightingMapsExercise2.h \
    2.lighting/4.4lighting_maps_exercise4/LightingMapsExercise4.h \
    2.lighting/5.1light_casters_directional/LightCastersDirectional.h \
    2.lighting/5.2light_casters_point/LightCastersPoint.h \
    2.lighting/5.3light_casters_spot/LightCastersSpot.h \
    2.lighting/6.1multiple_lights/MultipleLights.h \
    3.model_loading/1.model_loading/ModelLoading.h \
    4.advanced_opengl/1.1depth_testing/DepthTesting.h \
    4.advanced_opengl/10.1instancing_quads/InstancingQuads.h \
    4.advanced_opengl/2.stencil_testing/StencilTesting.h \
    4.advanced_opengl/3.1blending_discard/BlendingDiscard.h \
    4.advanced_opengl/3.2blending_sort/BlendingSort.h \
    4.advanced_opengl/4.face_culling_exercise1/FaceCullingExercise1.h \
    4.advanced_opengl/5.1framebuffers/Framebuffers.h \
    4.advanced_opengl/5.2framebuffers_exercise1/FramebuffersExercise1.h \
    4.advanced_opengl/6.1cubemaps_skybox/CubemapsSkybox.h \
    4.advanced_opengl/6.2cubemaps_environment_mapping/CubemapsEnvironmentMapping.h \
    4.advanced_opengl/8.advanced_glsl_ubo/AdvancedGlslUbo.h \
    4.advanced_opengl/9.1geometry_shader_houses/GeometryShaderHouses.h \
    4.advanced_opengl/9.2geometry_shader_exploding/GeometryShaderExploding.h \
    4.advanced_opengl/9.3geometry_shader_normals/GeometryShaderNormals.h \
    Config.h \
    MainWindow.h \
    utils/CameraUtil.h \
    utils/CubemapTextureUtil.h \
    utils/Mesh.h \
    utils/ModelUtil.h \
    utils/OpenGLUniformbufferObject.h \
    utils/ShaderUtil.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc

INCLUDEPATH += $$PWD/thirdparty/assimp_x64-windows/include
DEPENDPATH += $$PWD/thirdparty/assimp_x64-windows

LIBS += -L$$PWD/thirdparty/assimp_x64-windows/lib/ -lassimp-vc141-mt



