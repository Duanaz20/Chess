# CMakeList.txt : CMake project for Chess, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Chess "Chess.cpp" "board.h" "pieces.h" "board.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chess PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
# Set path to SFML
set(SFML_DIR "D:/sfml/SFML-2.5.1/lib/cmake/SFML")

# Find SFML
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# Link SFML to your app
target_link_libraries(Chess sfml-graphics sfml-window sfml-system)