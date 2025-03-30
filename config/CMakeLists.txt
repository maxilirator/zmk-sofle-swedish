zephyr_library_sources(config/behaviors/beh_modquad.c)

# Let the build system know about your custom devicetree binding
dtc_overlay_src += config/behaviors/behavior_modquad.yaml
