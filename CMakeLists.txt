cmake_minimum_required(VERSION 3.22)

project(DCO6 VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Plugin version ───────────────────────────────────────────────────────────
# Cambiar DCO6_VERSION y DCO6_CODE en cada release.
# Codigo: 4 chars exactos, unico por version para que los DAWs los distingan.
set(DCO6_VERSION "1.1")
set(DCO6_CODE    "D611")


include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.12
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(DCO6
    COMPANY_NAME                "YourStudio"
    PLUGIN_MANUFACTURER_CODE    "Ystd"
    PLUGIN_CODE                 "${DCO6_CODE}"
    FORMATS                     VST3 Standalone
    PRODUCT_NAME                "DCO6 v${DCO6_VERSION}"
    IS_SYNTH                    TRUE
    NEEDS_MIDI_INPUT            TRUE
    NEEDS_MIDI_OUTPUT           FALSE
    IS_MIDI_EFFECT              FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD     FALSE
    VST3_CATEGORIES             "Instrument|Synth"
)

target_sources(DCO6
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/SynthEngine/ADSR.cpp
        Source/SynthEngine/DCO.cpp
        Source/SynthEngine/Filter.cpp
        Source/SynthEngine/LFO.cpp
        Source/SynthEngine/Chorus.cpp
        Source/SynthEngine/Delay.cpp
        Source/SynthEngine/Voice.cpp
        Source/SynthEngine/PolyEngine.cpp
)

target_include_directories(DCO6
    PRIVATE
        Source
)

target_compile_definitions(DCO6
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

juce_generate_juce_header(DCO6)

target_link_libraries(DCO6
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_graphics
        juce::juce_gui_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
