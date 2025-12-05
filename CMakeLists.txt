cmake_minimum_required(VERSION 3.5.0)
project(raytracer VERSION 0.1.0 LANGUAGES C CXX)

# ============================================================
# ==================== Multi-threading =======================
# ============================================================
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Activer/désactiver le multithreading facilement
option(USE_THREADING "Enable multithreading" ON)

if(USE_THREADING)
    add_compile_definitions(USE_THREADING)
endif()

# ============================================================
# ====================== AABB Optimization ===================
# ============================================================
option(USE_AABB "Enable AABB bounding box optimization" ON)

if(USE_AABB)
    add_compile_definitions(USE_AABB)
endif()

# ============================================================
# ==================== Afficher la configuration =============
# ============================================================
message(STATUS "==========================================")
message(STATUS "       RAYTRACER CONFIGURATION")
message(STATUS "==========================================")
message(STATUS "  USE_THREADING : ${USE_THREADING}")
message(STATUS "  USE_AABB      : ${USE_AABB}")
message(STATUS "==========================================")



#exo1
#test
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(raytracer main.cpp)

target_include_directories(raytracer PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/src/raymath"
                           "${PROJECT_SOURCE_DIR}/src/rayimage"
                           "${PROJECT_SOURCE_DIR}/src/rayscene"
                           )

add_subdirectory(./src/raymath)
add_subdirectory(./src/rayimage)
add_subdirectory(./src/rayscene)
add_subdirectory(./src/lodepng)

target_link_libraries(raytracer 
                      PUBLIC 
                      rayscene
                      raymath
                      rayimage
                      lodepng
                      Threads::Threads
                      )






#exo1                     
# ===================================================================
# =======================  TESTING SECTION  ==========================
# ===================================================================

# --- Importer Catch2 (header-only) ---
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(catch2)

# --- Executable des tests ---
add_executable(raytracer_tests
    tests/test_raytracer.cpp
)

# Include pour les tests + lodepng
target_include_directories(raytracer_tests
    PRIVATE
        tests
        ${PROJECT_SOURCE_DIR}/src/lodepng
)

# Lier Catch2
target_link_libraries(raytracer_tests PRIVATE Catch2::Catch2WithMain lodepng)

# Déclarer le test pour CTest
add_test(NAME ray_tests COMMAND raytracer_tests)
