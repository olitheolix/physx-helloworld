# Build a minimal PhysX simulation.

PROJECT(hello-world)
CMAKE_MINIMUM_REQUIRED(VERSION 3.19)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- PhysX ---

INCLUDE(FetchContent)
FETCHCONTENT_DECLARE(
  physx
  GIT_REPOSITORY https://github.com/olitheolix/PhysX.git
  GIT_TAG        clang16
)
FETCHCONTENT_MAKEAVAILABLE(physx)

# Create Linux build.
SET(TARGET_BUILD_PLATFORM "linux")

# Mandatory paths.
SET(PHYSX_ROOT_DIR ${physx_SOURCE_DIR}/physx)
SET(PHYSX_INCLUDE_DIRS {PHYSX_ROOT_DIR}/include/)
SET(PX_OUTPUT_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR})
SET(PX_OUTPUT_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Build static libraries and skip the snippets.
SET(PX_BUILDSNIPPETS OFF CACHE BOOL "Generate the snippets")
SET(PX_GENERATE_STATIC_LIBRARIES ON CACHE BOOL "Generate static libraries")

# For reference: additional options I found in PhysX 5.2.
# SET(PX_BUILDPVDRUNTIME "Generate the OmniPVD project" OFF)
# SET(PX_CMAKE_SUPPRESS_REGENERATION "Disable zero_check projects" OFF)
# SET(PX_COPY_EXTERNAL_DLL "Copy external dlls into SDK bin directory" OFF)
# SET(PX_FLOAT_POINT_PRECISE_MATH "Float point precise math" OFF)
# SET(PX_SCALAR_MATH "Disable SIMD math" OFF)
# SET(PX_USE_NVTX "Enabled NVTX profiling" OFF)


# Add PhysX to the build tree.
ADD_SUBDIRECTORY(${PHYSX_ROOT_DIR}/compiler/public)

# --- Hello World ---
ADD_EXECUTABLE(helloworld helloworld.cpp)
TARGET_LINK_LIBRARIES(helloworld
  PhysXExtensions
  PhysX
  PhysXCooking
  PhysXCommon
  PhysXFoundation
)
