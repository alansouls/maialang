# CMakeList.txt : CMake project for my_gc_language, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (my_gc_language "my_gc_language.cpp" "my_gc_language.h" "gc.h" "gc.cpp" "variable.h" "scope.h" "scope.cpp" "variable.cpp" "expression.h" "expression.cpp" "function.h" "function.cpp" "interpreter.h" "interpreter.cpp" "string_utils.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET my_gc_language PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
