add_subdirectory(3rd)

add_library(openai
    src/openai.cpp
    src/response.cpp
)
target_include_directories(openai PUBLIC include)
target_link_libraries(openai PRIVATE curl nlohmann_json::nlohmann_json)
target_compile_features(openai PUBLIC cxx_std_23)
