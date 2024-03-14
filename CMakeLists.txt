juce_add_plugin(Powder
  COMPANY_NAME "Tauri Invictus"
  BUNDLE_ID "com.tauriinvictus.powder"
  IS_SYNTH FALSE
  IS_MIDI_EFFECT FALSE
  COPY_PLUGIN_AFTER_BUILD TRUE
  VST3_COPY_DIR "D:/null_/DSP/JUCE/VSTVault"
  PLUGIN_MANUFACTURER_CODE "De4d"
  PLUGIN_CODE "P0wd"
  FORMATS VST3 Standalone
  PRODUCT_NAME "Powder Spectrum Analyzer"
  VST3_CATEGORIES Analyzer
)

juce_generate_juce_header(Powder)

target_sources(Powder
  PRIVATE
    PluginEditor.cpp
    PluginProcessor.cpp
    FFTComponent.cpp
    ControlPanel.cpp
)

target_compile_definitions(Powder
  PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    # JUCE_DSP_USE_STATIC_FFTW=1
)

target_link_libraries(Powder
  PRIVATE
    TauriLib
    juce::juce_audio_utils
    juce::juce_dsp
    Melatonin::Perfetto
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
