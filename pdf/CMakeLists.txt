kcoreaddons_add_plugin(pdfthumbnail INSTALL_NAMESPACE "kf${QT_MAJOR_VERSION}/thumbcreator")

set(CMAKE_REQUIRED_LIBRARIES Poppler::Qt5 Qt5::Core Qt5::Gui)

target_sources(pdfthumbnail PRIVATE
    pdfcreator.cpp
)

target_link_libraries(pdfthumbnail
    KF${QT_MAJOR_VERSION}::KIOGui
    KF${QT_MAJOR_VERSION}::KDcraw
    Poppler::Qt5
    Qt::Gui
)
