#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QMainWindow>
#include <QComboBox>
#include <QGridLayout>
#include <QMetaType>
#include <QMap>
#include <memory>

#include "1.getting_started/1.1hello_window/HelloWindow.h"
#include "1.getting_started/2.1hello_triangle/HelloTriangle.h"
#include "1.getting_started/2.2hello_triangle_indexed/HelloTriangleIndexed.h"
#include "1.getting_started/2.3hello_triangle_exercise1/HelloTriangleExercise1.h"
#include "1.getting_started/2.4hello_triangle_exercise2/HelloTriangleExercise2.h"
#include "1.getting_started/2.5hello_triangle_exercise3/HelloTriangleExercise3.h"
#include "1.getting_started/3.2shaders_interpolation/ShadersInterpolation.h"
#include "1.getting_started/4.1textures/Textures.h"
#include "1.getting_started/4.4textures_exercise2/TexturesExercise2.h"
#include "1.getting_started/4.5textures_exercise3/TexturesExercise3.h"
#include "1.getting_started/4.6textures_exercise4/TexturesExercise4.h"
#include "1.getting_started/5.1transformations/Transformations.h"
#include "1.getting_started/6.1coordinate_systems/CoordinateSystems.h"
#include "1.getting_started/6.2coordinate_systems_depth/CoordinateSystemsDepth.h"
#include "1.getting_started/6.3coordinate_systems_exercise/CoordinateSystemsExercise.h"
#include "1.getting_started/7.1camera_circle/CameraCircle.h"
#include "1.getting_started/7.2camera_interact/CameraInteract.h"
#include "1.getting_started/7.4camera_class/CameraClass.h"
#include "2.lighting/1.colors/Colors.h"
#include "2.lighting/2.1basic_lighting_diffuse/BasicLightingDiffuse.h"
#include "2.lighting/2.2basic_lighting_specular/BasicLightingSpecular.h"
#include "2.lighting/2.3basic_lighting_exercise1/BasicLightingExercise1.h"
#include "2.lighting/2.5basic_lighting_exercise3/BasicLightingExercise3.h"
#include "2.lighting/3.1materials/Materials.h"
#include "2.lighting/4.1lighting_maps_diffuse_map/LightingMapsDiffuseMap.h"
#include "2.lighting/4.2lighting_maps_specular_map/LightingMapsSpecularMap.h"
#include "2.lighting/4.3lighting_maps_exercise2/LightingMapsExercise2.h"
#include "2.lighting/4.4lighting_maps_exercise4/LightingMapsExercise4.h"
#include "2.lighting/5.1light_casters_directional/LightCastersDirectional.h"
#include "2.lighting/5.2light_casters_point/LightCastersPoint.h"
#include "2.lighting/5.3light_casters_spot/LightCastersSpot.h"
#include "2.lighting/6.1multiple_lights/MultipleLights.h"
#include "3.model_loading/1.model_loading/ModelLoading.h"
#include "4.advanced_opengl/1.1depth_testing/DepthTesting.h"
#include "4.advanced_opengl/2.stencil_testing/StencilTesting.h"
#include "4.advanced_opengl/3.1blending_discard/BlendingDiscard.h"
#include "4.advanced_opengl/3.2blending_sort/BlendingSort.h"
#include "4.advanced_opengl/4.face_culling_exercise1/FaceCullingExercise1.h"
#include "4.advanced_opengl/5.1framebuffers/Framebuffers.h"
#include "4.advanced_opengl/5.2framebuffers_exercise1/FramebuffersExercise1.h"
#include "4.advanced_opengl/6.1cubemaps_skybox/CubemapsSkybox.h"
#include "4.advanced_opengl/6.2cubemaps_environment_mapping/CubemapsEnvironmentMapping.h"
#include "4.advanced_opengl/8.advanced_glsl_ubo/AdvancedGlslUbo.h"
#include "4.advanced_opengl/9.1geometry_shader_houses/GeometryShaderHouses.h"
#include "4.advanced_opengl/9.2geometry_shader_exploding/GeometryShaderExploding.h"
#include "4.advanced_opengl/9.3geometry_shader_normals/GeometryShaderNormals.h"
#include "4.advanced_opengl/10.1instancing_quads/InstancingQuads.h"
#include "4.advanced_opengl/10.2asteroids/Asteroids.h"
#include "4.advanced_opengl/10.3asteroids_instanced/AsteroidsInstanced.h"
#include "5.advanced_lighting/1.advanced_lighting/AdvancedLighting.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();

private:
    void showGLWindows();
    void registerMetaObject();

private:
    QWidget* m_container;
    QGridLayout* m_layout;
    QComboBox* m_combox;

    QOpenGLWidget* m_openglWidget;

    QList<const QMetaObject*> m_metaObjectList;
    QMap<QString, const QMetaObject*> m_metaObjectMap;
};
#endif // MAINWINDOW_H
