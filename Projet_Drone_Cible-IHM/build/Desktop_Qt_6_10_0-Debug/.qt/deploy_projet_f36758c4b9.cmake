include(/home/guillian/projet2/build/Desktop_Qt_6_10_0-Debug/.qt/QtDeploySupport.cmake)
include("${CMAKE_CURRENT_LIST_DIR}/projet-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtserialport;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtwebengine;qtdeclarative;qtwebengine")

qt6_deploy_runtime_dependencies(
    EXECUTABLE /home/guillian/projet2/build/Desktop_Qt_6_10_0-Debug/projet
    GENERATE_QT_CONF
)
