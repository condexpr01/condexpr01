# debug
if(FALSE)
	set(CMAKE_CXX_COMPILER clang++)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(CMAKE_BUILD_TYPE Debug)
endif()


#proj, ver
set(PROJECT_NAME "prog")
cmake_minimum_required(VERSION 3.5...4.4)
project(${PROJECT_NAME} C CXX)

#msvc utf8
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_definitions(-D_GNU_SOURCE)

#executable target
add_executable(${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories(
	${PROJECT_NAME}

	PRIVATE include
	PRIVATE include/imgui
	PRIVATE include/core
	PRIVATE include/core/gl
	PRIVATE include/core/sdl
	PRIVATE include/core/std
	PRIVATE include/glad
	PRIVATE include/stb
	PRIVATE include/exprtk
)

target_sources(
	${PROJECT_NAME}

	PRIVATE src/core/main.cpp
	PRIVATE src/core/loop.cpp

	PRIVATE src/glad/gl.cpp

	#PRIVATE src/stb/stb_vorbis.c
	PRIVATE src/stb/impl/stb_image_write.cpp

	PRIVATE src/imgui/imgui.cpp
	#PRIVATE src/imgui/imgui_demo.cpp
	PRIVATE src/imgui/imgui_draw.cpp
	PRIVATE src/imgui/imgui_tables.cpp
	PRIVATE src/imgui/imgui_widgets.cpp
	PRIVATE src/imgui/imgui_impl_opengl3.cpp
	PRIVATE src/imgui/imgui_impl_sdl3.cpp

	PRIVATE src/imgui/imgui_stdlib.cpp

	PRIVATE src/imgui/imgui_toggle.cpp
	PRIVATE src/imgui/imgui_toggle_palette.cpp
	PRIVATE src/imgui/imgui_toggle_presets.cpp
	PRIVATE src/imgui/imgui_toggle_renderer.cpp

	#PRIVATE src/imgui/implot.cpp
	#PRIVATE src/imgui/implot_demo.cpp
	#PRIVATE src/imgui/implot_items.cpp

	#PRIVATE src/imgui/imgui_md.cpp

	#PRIVATE src/imgui/implot3d.cpp
	#PRIVATE src/imgui/implot3d_demo.cpp
	#PRIVATE src/imgui/implot3d_items.cpp
	#PRIVATE src/imgui/implot3d_meshes.cpp

	#PRIVATE src/imgui/imnodes.cpp
)


#libromfs
set(LIBROMFS_PROJECT_NAME ${PROJECT_NAME})
set(LIBROMFS_RESOURCE_LOCATION "${CMAKE_SOURCE_DIR}/romfs")

add_subdirectory(libromfs)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBROMFS_LIBRARY})


#gl
find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
	target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
endif()


##########clone################
find_program(GIT_EXECUTABLE git git.cmd)

set(SDL_REPO "https://github.com/libsdl-org/SDL.git")
#set(SDL_REPO "git@github.com:libsdl-org/SDL.git")
#set(SDL_REPO "file:////home/condexpr01/Desktop/clonerepos/SDL/.git")

set(GLM_REPO "https://github.com/g-truc/glm.git")
#set(GLM_REPO "git@github.com:g-truc/glm.git")
#set(GLM_REPO "file:////home/condexpr01/Desktop/clonerepos/glm/.git")


set(SDL_CLONE_DIR "${CMAKE_SOURCE_DIR}/clone/SDL")
set(GLM_CLONE_DIR "${CMAKE_SOURCE_DIR}/clone/glm")

################
#   SDL3       #
################
find_package(SDL3)
if(NOT SDL3_FOUND AND GIT_EXECUTABLE)

	if(NOT EXISTS "${SDL_CLONE_DIR}/.git")
		execute_process(
			COMMAND "${GIT_EXECUTABLE}" clone
				--depth=1
				--branch release-3.4.8
				"${SDL_REPO}"
				--
				"${SDL_CLONE_DIR}"
		)
	endif()

	set(SDL_STATIC ON)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE)
	add_subdirectory("${SDL_CLONE_DIR}")

elseif(NOT SDL3_FOUND AND NOT GIT_EXECUTABLE)
	message(FATAL_ERROR "NO SDL3 FOUND, AND NO GIT FOR CLONING")

endif()

if(WIN32)
	if(CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
		target_link_options(${PROJECT_NAME}
			PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup
		)
	else()# GNU
		target_link_options(${PROJECT_NAME} PRIVATE -static -mwindows)
	endif()

endif()

if(TARGET SDL3::SDL3-static)
	target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)
else()
	target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
endif()


################
#   glm        #
################
find_package(glm)
if(NOT glm_FOUND AND GIT_EXECUTABLE)

	if(NOT EXISTS "${GLM_CLONE_DIR}/.git")
		execute_process(
			COMMAND "${GIT_EXECUTABLE}" clone
				--depth=1
				--branch 1.0.3
				"${GLM_REPO}"
				--
				"${GLM_CLONE_DIR}"
		)
	endif()

	add_subdirectory("${GLM_CLONE_DIR}")
	target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

elseif(NOT GLM_FOUND AND NOT GIT_EXECUTABLE)
	message(FATAL_ERROR "NO glm FOUND, AND NO GIT FOR CLONING")

endif()

