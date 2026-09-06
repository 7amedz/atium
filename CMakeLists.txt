# ============================================================================
#  atium
# ============================================================================
# Cmake version required
cmake_minimum_required(VERSION 3.25)

project(atium
    VERSION 0.1.0
    DESCRIPTION "Multi-curve term structure bootstrapping engine"
    LANGUAGES CXX)

# ---------------------------------------------------------------------------
#  Language standard
# ---------------------------------------------------------------------------
#  C++23!
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# lsp needs this for parsing sources (catch2 stuff)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# off by default
option(ATIUM_LINK_COMPILE_COMMANDS "Symlink compile_commands.json into the project root" OFF)

# symlinked to project root so lsp can find it
if(ATIUM_LINK_COMPILE_COMMANDS)
    file(CREATE_LINK
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
        SYMBOLIC)
endif()

# ---------------------------------------------------------------------------
#  Warning flags
# ---------------------------------------------------------------------------
#
#
# Settings bundle, not a real library. Builds nothing; just hands the flags to
# whoever links it, so Catch2 never gets them.
add_library(atium_warnings INTERFACE)    #create 
target_compile_options(atium_warnings INTERFACE   #add the warning flags to atium_warnings
    -Wall
    -Wextra
    -Wpedantic              # reject non-standard extensions
    -Wconversion            # implicit narrowing that may change the value
    -Wsign-conversion       # signed <-> unsigned, the off-by-one source
    -Wnon-virtual-dtor      # deleting a LogLinear through an Interpolator* is
                            #   UB without a virtual destructor
    -Woverloaded-virtual    # a derived function that hides rather than overrides
    -Wshadow                # inner name shadowing an outer one
    -Wdouble-promotion      # accidental float -> double
    -Wold-style-cast        # (double)x instead of static_cast<double>(x)
    -Wnull-dereference
    -Wcast-qual             # casting away const
    -Wno-c2y-extensions    #catch2 test case macros fire this warning so we turn it off
    #must be after Wpedantic
)

# ---------------------------------------------------------------------------
#  The library
# ---------------------------------------------------------------------------
#
#
#
add_library(atium STATIC src/year_fraction.cpp src/schedule.cpp)
target_include_directories(atium  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(atium   PRIVATE atium_warnings)



# ---------------------------------------------------------------------------
#  Subdirectories
# ---------------------------------------------------------------------------
#  build tests. ctest --preset debug
enable_testing()
add_subdirectory(tests)

#  maybe add benchmarking later
