# Set GLFW, OpenGL, and ImGui source directories
# set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)

# add ImGui source files
# file(GLOB IMGUI_SOURCES
# ${IMGUI_DIR}/*.cpp
# ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
# ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
# )
set(GLM_DIR ${CMAKE_SOURCE_DIR}/../third_party/glm)

file(GLOB SOURCE_FILES
  component/*.cpp
  engine/*.cpp
  entity/*.cpp
  system/*.cpp
  util/*.cpp
  #${IMGUI_SOURCES}
)

file(GLOB TEST_FILES
  test_harness/*.cpp
  test/*.cpp
)

# Create library
add_library(MazeLib 
  ${SOURCE_FILES}
)

add_library(MazeTestLib 
  ${SOURCE_FILES}
  ${TEST_FILES}
)


# Include directories
target_include_directories(MazeLib PUBLIC
  ${GLM_DIR}
  component/
  engine/
  entity/
  system/
  util/
)

target_include_directories(MazeTestLib PUBLIC
  test_harness/
  test/
)

# Find dependencies
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
# find_package(LibXml2 REQUIRED)
# find_package(assimp REQUIRED)
# find_package(PkgConfig REQUIRED)

# pkg_check_modules(LIBXML++ REQUIRED libxml++-2.6)

# target_include_directories(SpectrumRendererLibrary PUBLIC
# ${LIBXML++_INCLUDE_DIRS}
# )

# Link external libraries
target_link_libraries(MazeLib PRIVATE 
  OpenGL::GL 
  glfw 
  GLEW::GLEW
# ${LIBXML++_LIBRARIES}
  assimp
)

target_link_libraries(MazeTestLib PRIVATE 
  MazeLib
)
