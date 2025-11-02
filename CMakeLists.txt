add_library(openai
    openai.cpp
)
target_include_directories(openai PUBLIC include)
target_link_libraries(openai PRIVATE curl)
