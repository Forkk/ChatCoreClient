set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)

### translation stuff

file(GLOB TRANSLATION_FILES ${CMAKE_CURRENT_LIST_DIR}/*.ts)

qt5_create_translation(TRANSLATION_MESSAGES ${FILES_TO_TRANSLATE} ${TRANSLATION_FILES})
qt5_add_translation(TRANSLATION_QM ${TRANSLATION_FILES})
add_custom_target(translations_update DEPENDS ${TRANSLATION_MESSAGES})
add_custom_target(translations DEPENDS ${TRANSLATION_QM})

if(APPLE AND UNIX) ## OSX
        install(FILES ${TRANSLATION_QM} DESTINATION QChatCoreClient.app/Contents/Resources/translations)
else()
	install(FILES ${TRANSLATION_QM} DESTINATION translations)
endif()
