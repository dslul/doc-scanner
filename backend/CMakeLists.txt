include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/include"
)

set(
    Doc_Scannerbackend_SRCS
    modules/Doc_Scanner/backend.cpp
    modules/Doc_Scanner/scanimage.cpp
    modules/Doc_Scanner/imagewarp.cpp
    modules/Doc_Scanner/pointvectorutils.cpp
)

# Make the unit test files visible on qtcreator
add_custom_target(Doc_Scannerbackend_UNITTEST_QML_FILES ALL SOURCES "tests/unit/tst_mytype.qml")

add_library(Doc_Scannerbackend MODULE
    ${Doc_Scannerbackend_SRCS}
)

#link opencv library
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_calib3d.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_core.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_features2d.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_flann.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_highgui.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_imgcodecs.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_imgproc.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_ml.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_objdetect.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_photo.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_shape.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_stitching.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_superres.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_videoio.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_video.so)
target_link_libraries(Doc_Scannerbackend ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/lib/libopencv_videostab.so)
#link_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

set_target_properties(Doc_Scannerbackend PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY Doc_Scanner)

qt5_use_modules(Doc_Scannerbackend Gui Qml Quick)

# Copy qmldir file to build dir for running in QtCreator
add_custom_target(Doc_Scannerbackend-qmldir ALL
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/modules/Doc_Scanner/qmldir ${CMAKE_CURRENT_BINARY_DIR}/Doc_Scanner
    DEPENDS ${QMLFILES}
)

# Install plugin file
install(TARGETS Doc_Scannerbackend DESTINATION ${QT_IMPORTS_DIR}/Doc_Scanner/)
install(FILES   modules/Doc_Scanner/qmldir DESTINATION ${QT_IMPORTS_DIR}/Doc_Scanner/)
# Install opencv
install(FILES   modules/Doc_Scanner/lib/libopencv_calib3d.so            DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_calib3d.so.3.1        DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_calib3d.so.3.1.0      DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_core.so               DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_core.so.3.1           DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_core.so.3.1.0         DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_features2d.so         DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_features2d.so.3.1     DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_features2d.so.3.1.0   DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_flann.so              DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_flann.so.3.1          DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_flann.so.3.1.0        DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_highgui.so            DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_highgui.so.3.1        DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_highgui.so.3.1.0      DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_imgcodecs.so          DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_imgcodecs.so.3.1      DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_imgcodecs.so.3.1.0    DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_imgproc.so            DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_imgproc.so.3.1        DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_imgproc.so.3.1.0      DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_ml.so                 DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_ml.so.3.1             DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_ml.so.3.1.0           DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_objdetect.so          DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_objdetect.so.3.1      DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_objdetect.so.3.1.0    DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_photo.so              DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_photo.so.3.1          DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_photo.so.3.1.0        DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_shape.so              DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_shape.so.3.1          DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_shape.so.3.1.0        DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_stitching.so          DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_stitching.so.3.1      DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_stitching.so.3.1.0    DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_superres.so           DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_superres.so.3.1       DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_superres.so.3.1.0     DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_ts.a                  DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_videoio.so            DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_videoio.so.3.1        DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_videoio.so.3.1.0      DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_video.so              DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_video.so.3.1          DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_video.so.3.1.0        DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_videostab.so          DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_videostab.so.3.1      DESTINATION ${QT_IMPORTS_DIR}/)
install(FILES   modules/Doc_Scanner/lib/libopencv_videostab.so.3.1.0    DESTINATION ${QT_IMPORTS_DIR}/)
