#include "focus_flow.h"
#include <string.h>

static float clampf(float v, float lo, float hi)
{
    if (v < lo) { return lo; }
    if (v > hi) { return hi; }
    return v;
}

focus_flow_t focus_flow_create(void)
{
    focus_flow_t ff;
    memset(&ff, 0, sizeof(ff));
    ff.state        = FOCUS_IDLE;
    ff.dim_target   = FOCUS_DIM_TARGET;
    ff.dim_speed    = FOCUS_DIM_SPEED;
    ff.reveal_speed = FOCUS_REVEAL_SPEED;
    return ff;
}

focus_flow_t focus_flow_begin(focus_flow_t flow,
                              camera_pose_t current,
                              camera_pose_t destination,
                              ct_type_t type, int object_id)
{
    flow.state       = FOCUS_FLYING;
    flow.home        = current;
    flow.destination = destination;
    flow.target_type = type;
    flow.target_id   = object_id;
    flow.dim_mix     = 0.0f;
    flow.reveal_mix  = 0.0f;

    flow.path = camera_path_spring(current, destination,
                                   FOCUS_FLY_STIFF, FOCUS_FLY_DAMP);
    return flow;
}

focus_flow_t focus_flow_update(focus_flow_t flow, float dt)
{
    if (dt <= 0.0f) {
        return flow;
    }

    switch (flow.state) {
    case FOCUS_IDLE:
        break;

    case FOCUS_FLYING:
        flow.path = camera_path_update(flow.path, dt);
        flow.dim_mix = clampf(flow.dim_mix + flow.dim_speed * dt, 0.0f, 1.0f);

        if (camera_path_done(flow.path)) {
            flow.state = FOCUS_ARRIVED;
            flow.dim_mix = 1.0f;
        }
        break;

    case FOCUS_ARRIVED:
        flow.reveal_mix = clampf(flow.reveal_mix + flow.reveal_speed * dt,
                                 0.0f, 1.0f);
        if (flow.reveal_mix >= 1.0f) {
            flow.state = FOCUS_LOCKED;
        }
        break;

    case FOCUS_LOCKED:
        /* Stable — nothing to animate */
        break;

    case FOCUS_RELEASING:
        flow.path = camera_path_update(flow.path, dt);
        flow.dim_mix = clampf(flow.dim_mix - flow.dim_speed * dt, 0.0f, 1.0f);
        flow.reveal_mix = clampf(flow.reveal_mix - flow.reveal_speed * dt,
                                 0.0f, 1.0f);

        if (camera_path_done(flow.path) && flow.dim_mix <= 0.0f) {
            flow.state = FOCUS_IDLE;
            flow.dim_mix = 0.0f;
            flow.reveal_mix = 0.0f;
        }
        break;
    }

    return flow;
}

focus_flow_t focus_flow_release(focus_flow_t flow)
{
    if (flow.state == FOCUS_IDLE) {
        return flow;
    }

    camera_pose_t current = camera_path_pose(flow.path);
    flow.state = FOCUS_RELEASING;
    flow.path  = camera_path_spring(current, flow.home,
                                    FOCUS_RET_STIFF, FOCUS_RET_DAMP);
    return flow;
}

focus_flow_t focus_flow_cancel(focus_flow_t flow)
{
    flow.state      = FOCUS_IDLE;
    flow.dim_mix    = 0.0f;
    flow.reveal_mix = 0.0f;
    memset(&flow.path, 0, sizeof(flow.path));
    return flow;
}

focus_flow_t focus_flow_retarget(focus_flow_t flow,
                                 camera_pose_t new_dest,
                                 ct_type_t type, int object_id)
{
    if (flow.state == FOCUS_IDLE) {
        return flow;
    }

    flow.destination = new_dest;
    flow.target_type = type;
    flow.target_id   = object_id;
    flow.state       = FOCUS_FLYING;
    flow.reveal_mix  = 0.0f;

    flow.path = camera_path_retarget(flow.path, new_dest);
    return flow;
}

focus_state_e focus_flow_state(focus_flow_t flow)
{
    return flow.state;
}

camera_pose_t focus_flow_pose(focus_flow_t flow)
{
    if (flow.state == FOCUS_IDLE) {
        camera_pose_t zero;
        memset(&zero, 0, sizeof(zero));
        return zero;
    }
    return camera_path_pose(flow.path);
}

int focus_flow_active(focus_flow_t flow)
{
    return flow.state != FOCUS_IDLE;
}

float focus_flow_dim(focus_flow_t flow)
{
    return flow.dim_mix;
}

float focus_flow_reveal(focus_flow_t flow)
{
    return flow.reveal_mix;
}

layer_state_t focus_flow_apply_layers(focus_flow_t flow,
                                      layer_state_t base,
                                      layer_id_t focused_layer)
{
    if (flow.dim_mix <= 0.0f) {
        return base;
    }

    for (int i = 0; i < LAYER_COUNT; i++) {
        layer_id_t id = (layer_id_t)i;

        /* Skip focused layer and HUD — they stay bright */
        if (id == focused_layer || id == LAYER_HUD) {
            continue;
        }

        base = layer_blend_opacity(base, id, flow.dim_target, flow.dim_mix);
    }

    return base;
}
