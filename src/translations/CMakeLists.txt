set(SOURCES
    openorbiter_de.ts
)

qt4_add_translation(QM_FILES ${SOURCES})

add_custom_target(translations ALL DEPENDS ${QM_FILES})

install(FILES ${QM_FILES} DESTINATION "${DATA_PATH}/translations")
