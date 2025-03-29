#include <device.h>
#include <zmk/behavior.h>
#include <zmk/keymap.h>
#include <zmk/hid.h>
#include <zmk/behavior_queue.h>

struct behavior_modquad_config
{
    uint32_t no_mod;
    uint32_t shift;
    uint32_t altgr;
    uint32_t shift_altgr;
};

static int modquad_press(struct zmk_behavior_binding *binding,
                         struct zmk_behavior_binding_event event)
{
    const struct device *dev = binding->behavior_dev;
    const struct behavior_modquad_config *cfg = dev->config;
    uint8_t mods = zmk_hid_get_explicit_mods();

    uint32_t keycode;

    if ((mods & MOD_LSFT) && (mods & MOD_RALT))
    {
        keycode = cfg->shift_altgr;
    }
    else if (mods & MOD_RALT)
    {
        keycode = cfg->altgr;
    }
    else if (mods & MOD_LSFT)
    {
        keycode = cfg->shift;
    }
    else
    {
        keycode = cfg->no_mod;
    }

    return behavior_key_press(&((struct zmk_behavior_binding){
                                  .behavior_dev = NULL,
                                  .param1 = keycode}),
                              event);
}

static int modquad_release(struct zmk_behavior_binding *binding,
                           struct zmk_behavior_binding_event event)
{
    return behavior_key_release(binding, event);
}

static const struct behavior_driver_api modquad_api = {
    .binding_pressed = modquad_press,
    .binding_released = modquad_release,
};

#define KP_INST(n)                                                          \
    static struct behavior_modquad_config modquad_config_##n = {            \
        .no_mod = DT_INST_PROP_BY_IDX(n, bindings, 0),                      \
        .shift = DT_INST_PROP_BY_IDX(n, bindings, 1),                       \
        .altgr = DT_INST_PROP_BY_IDX(n, bindings, 2),                       \
        .shift_altgr = DT_INST_PROP_BY_IDX(n, bindings, 3),                 \
    };                                                                      \
                                                                            \
    DEVICE_DT_INST_DEFINE(n, NULL, NULL, NULL, &modquad_config_##n,         \
                          POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, \
                          &modquad_api);

DT_INST_FOREACH_STATUS_OKAY(KP_INST)
