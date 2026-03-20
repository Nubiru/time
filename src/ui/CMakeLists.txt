# src/ui/CMakeLists.txt — UI modules

set(UI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/time_hud.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fmt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/animation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/glyph_layout.c
    ${CMAKE_CURRENT_SOURCE_DIR}/card_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zodiac_glyphs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hexagram_visual.c
    ${CMAKE_CURRENT_SOURCE_DIR}/location_presets.c
    ${CMAKE_CURRENT_SOURCE_DIR}/scale_hud.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system_scale_map.c
    ${CMAKE_CURRENT_SOURCE_DIR}/time_format.c
    ${CMAKE_CURRENT_SOURCE_DIR}/card_layout.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tzolkin_card.c
    ${CMAKE_CURRENT_SOURCE_DIR}/astro_summary.c
    ${CMAKE_CURRENT_SOURCE_DIR}/astro_fmt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/golden_layout.c
    ${CMAKE_CURRENT_SOURCE_DIR}/theme.c
    ${CMAKE_CURRENT_SOURCE_DIR}/accessibility.c
    ${CMAKE_CURRENT_SOURCE_DIR}/chinese_fmt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hd_card.c
    ${CMAKE_CURRENT_SOURCE_DIR}/i18n.c
    ${CMAKE_CURRENT_SOURCE_DIR}/iching_card.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lunar_display.c
    ${CMAKE_CURRENT_SOURCE_DIR}/rtl_layout.c
    ${CMAKE_CURRENT_SOURCE_DIR}/scroll_layers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/help_overlay.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ui_state.c
    ${CMAKE_CURRENT_SOURCE_DIR}/layer_panel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/command_palette.c
    ${CMAKE_CURRENT_SOURCE_DIR}/time_control.c
    ${CMAKE_CURRENT_SOURCE_DIR}/toast_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ui_html.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ui_bridge.c
    ${CMAKE_CURRENT_SOURCE_DIR}/timeline_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/theme_css.c
    ${CMAKE_CURRENT_SOURCE_DIR}/touch_gestures.c
    ${CMAKE_CURRENT_SOURCE_DIR}/view_registry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_score.c
    ${CMAKE_CURRENT_SOURCE_DIR}/share_moment.c
    ${CMAKE_CURRENT_SOURCE_DIR}/view_adapt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/a11y_score.c
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_engine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system_theme.c
    ${CMAKE_CURRENT_SOURCE_DIR}/timeline.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sensitivity.c
    ${CMAKE_CURRENT_SOURCE_DIR}/input_motion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_event.c
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_culture.c
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_meditation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/welcome_text.c
    ${CMAKE_CURRENT_SOURCE_DIR}/wisdom_display.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pianist_score.c
    ${CMAKE_CURRENT_SOURCE_DIR}/midi_output.c
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_vis_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_a11y.c
    ${CMAKE_CURRENT_SOURCE_DIR}/audio_queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zoom_depth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/focus_flow.c
    ${CMAKE_CURRENT_SOURCE_DIR}/symbol_atlas.c
    ${CMAKE_CURRENT_SOURCE_DIR}/visual_transition.c
    ${CMAKE_CURRENT_SOURCE_DIR}/a11y_narrative.c
    ${CMAKE_CURRENT_SOURCE_DIR}/focus_mode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/greeting.c
    ${CMAKE_CURRENT_SOURCE_DIR}/text_reveal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/auto_theme.c
    ${CMAKE_CURRENT_SOURCE_DIR}/share_template.c
    ${CMAKE_CURRENT_SOURCE_DIR}/loading_sequence.c
    ${CMAKE_CURRENT_SOURCE_DIR}/text_choreo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/input_dispatch.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kf_track.c
    ${CMAKE_CURRENT_SOURCE_DIR}/drama_seq.c
    ${CMAKE_CURRENT_SOURCE_DIR}/seasonal_greeting.c
    ${CMAKE_CURRENT_SOURCE_DIR}/card_depth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/earth_transition.c
    ${CMAKE_CURRENT_SOURCE_DIR}/motion_prefs.c
    PARENT_SCOPE
)

if(NOT EMSCRIPTEN)
    add_library(time_time_hud        OBJECT time_hud.c)
    add_library(time_fmt             OBJECT fmt.c)
    add_library(time_animation       OBJECT animation.c)
    add_library(time_glyph_layout    OBJECT glyph_layout.c)
    add_library(time_card_data       OBJECT card_data.c)
    add_library(time_zodiac_glyphs   OBJECT zodiac_glyphs.c)
    add_library(time_hexagram_visual OBJECT hexagram_visual.c)
    add_library(time_location_presets OBJECT location_presets.c)
    add_library(time_scale_hud       OBJECT scale_hud.c)
    add_library(time_system_scale_map OBJECT system_scale_map.c)
    add_library(time_time_format     OBJECT time_format.c)
    add_library(time_card_layout     OBJECT card_layout.c)
    add_library(time_tzolkin_card    OBJECT tzolkin_card.c)
    add_library(time_astro_summary   OBJECT astro_summary.c)
    add_library(time_astro_fmt       OBJECT astro_fmt.c)
    add_library(time_golden_layout   OBJECT golden_layout.c)
    add_library(time_theme           OBJECT theme.c)
    add_library(time_accessibility   OBJECT accessibility.c)
    add_library(time_chinese_fmt     OBJECT chinese_fmt.c)
    add_library(time_hd_card         OBJECT hd_card.c)
    add_library(time_i18n            OBJECT i18n.c)
    add_library(time_iching_card     OBJECT iching_card.c)
    add_library(time_lunar_display   OBJECT lunar_display.c)
    add_library(time_rtl_layout      OBJECT rtl_layout.c)
    add_library(time_scroll_layers   OBJECT scroll_layers.c)
    add_library(time_help_overlay    OBJECT help_overlay.c)
    add_library(time_ui_state        OBJECT ui_state.c)
    add_library(time_layer_panel     OBJECT layer_panel.c)
    add_library(time_command_palette  OBJECT command_palette.c)
    add_library(time_time_control    OBJECT time_control.c)
    add_library(time_toast_message   OBJECT toast_message.c)
    add_library(time_ui_html         OBJECT ui_html.c)
    add_library(time_timeline_data   OBJECT timeline_data.c)
    add_library(time_theme_css      OBJECT theme_css.c)
    add_library(time_touch_gestures  OBJECT touch_gestures.c)
    add_library(time_view_registry   OBJECT view_registry.c)
    add_library(time_audio_score    OBJECT audio_score.c)
    add_library(time_share_moment   OBJECT share_moment.c)
    add_library(time_view_adapt    OBJECT view_adapt.c)
    add_library(time_a11y_score    OBJECT a11y_score.c)
    add_library(time_system_theme OBJECT system_theme.c)
    add_library(time_timeline    OBJECT timeline.c)
    add_library(time_sensitivity OBJECT sensitivity.c)
    add_library(time_input_motion OBJECT input_motion.c)
    add_library(time_audio_event OBJECT audio_event.c)
    add_library(time_audio_culture OBJECT audio_culture.c)
    add_library(time_audio_meditation OBJECT audio_meditation.c)
    add_library(time_welcome_text    OBJECT welcome_text.c)
    add_library(time_wisdom_display  OBJECT wisdom_display.c)
    add_library(time_pianist_score   OBJECT pianist_score.c)
    add_library(time_midi_output     OBJECT midi_output.c)
    add_library(time_audio_vis_data  OBJECT audio_vis_data.c)
    add_library(time_audio_a11y      OBJECT audio_a11y.c)
    add_library(time_audio_queue     OBJECT audio_queue.c)
    add_library(time_zoom_depth      OBJECT zoom_depth.c)
    add_library(time_focus_flow      OBJECT focus_flow.c)
    add_library(time_symbol_atlas    OBJECT symbol_atlas.c)
    add_library(time_visual_transition OBJECT visual_transition.c)
    add_library(time_a11y_narrative    OBJECT a11y_narrative.c)
    add_library(time_focus_mode      OBJECT focus_mode.c)
    add_library(time_greeting        OBJECT greeting.c)
    add_library(time_text_reveal     OBJECT text_reveal.c)
    add_library(time_auto_theme      OBJECT auto_theme.c)
    add_library(time_share_template  OBJECT share_template.c)
    add_library(time_loading_sequence OBJECT loading_sequence.c)
    add_library(time_text_choreo     OBJECT text_choreo.c)
    add_library(time_input_dispatch  OBJECT input_dispatch.c)
    add_library(time_kf_track        OBJECT kf_track.c)
    add_library(time_drama_seq       OBJECT drama_seq.c)
    add_library(time_seasonal_greeting OBJECT seasonal_greeting.c)
    add_library(time_card_depth      OBJECT card_depth.c)
    add_library(time_earth_transition OBJECT earth_transition.c)
    add_library(time_motion_prefs    OBJECT motion_prefs.c)
    # ui_bridge.c, audio_engine.c are Emscripten-only (S1) — no native OBJECT lib
endif()
