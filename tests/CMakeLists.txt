# ============================================================================
#  Tests
# ============================================================================

include(FetchContent)
#fetch Catch2

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1        
    GIT_SHALLOW    TRUE          # get one commit not the full git history
    SYSTEM)                      # treat its headers as system headers
FetchContent_MakeAvailable(Catch2)

# ---------------------------------------------------------------------------
#  The test executable
# ---------------------------------------------------------------------------
#
#add new test files
add_executable(atium_tests
    # smoke_test.cpp
    year_fraction_tests.cpp
    gen_schedules_test.cpp
)

target_link_libraries(atium_tests PRIVATE
    atium                       # atium lib
    atium_warnings              # atium warning flags
    Catch2::Catch2WithMain)     # Catch2 supplies main()

# ---------------------------------------------------------------------------
#  Register each TEST_CASE with CTest individually, so `ctest` reports them
#  by name instead of as one opaque binary.
# ---------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(atium_tests)
