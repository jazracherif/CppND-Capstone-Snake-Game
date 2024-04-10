# Following tutorial https://www.jibbow.com/posts/rapidjson-cmake/
include(ExternalProject)

# Download RapidJson
ExternalProject_Add(
    rapidjson
    PREFIX "cmake/rapidjson"
    GIT_REPOSITORY "https://github.com/Tencent/rapidjson.git"
    GIT_TAG f54b0e47a08782a6131cc3d60f94d038fa6e0a51
    TIMEOUT 10
    CMAKE_CTEST_ARGUMENTS   
        -DRAPIDJSON_BUILD_TESTS=OFF
        -DRAPIDJSON_BUILD_DOC=OFF
        -DRAPIDJSON_BUILD_EXAMPLES=OFF
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

# Prepare RapidJson (RapidJson is a header-only library)
ExternalProject_Get_Property(rapidjson source_dir)
set(RAPIDJSON_INCLUDE_DIR {$source_dir}/include)
