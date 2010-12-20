include(${QT_USE_FILE})

set(SOURCES
    circle.cpp
    circle.h
    object.cpp
    object.h
    rect.cpp
    rect.h
    vector.h
    world.cpp
    world.h
)

set(MOC_SOURCES
    object.h
)

qt4_wrap_cpp(MOC_OUTPUT ${MOC_SOURCES})

add_library(physics ${SOURCES} ${MOC_OUTPUT})
target_link_libraries(physics ${QT_LIBRARIES})
