target_sources(
        OrionEngine
        PUBLIC
        PRIVATE
        Private/Main.cc
)
target_include_directories(
        OrionEngine
        PUBLIC
        Public
        PRIVATE
        Private
)
target_link_libraries(
        OrionEngine
        PUBLIC
        OrionEngine::Core
)
target_compile_features(
        OrionEngine
        PUBLIC
        cxx_std_17
)

