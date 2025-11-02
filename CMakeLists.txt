add_subdirectory(3rd)

add_library(openai
    src/openai.cpp
)
target_include_directories(openai PUBLIC include)
target_link_libraries(openai PRIVATE curl nlohmann_json::nlohmann_json)
