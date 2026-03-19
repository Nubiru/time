#include "camera_motion.h"
#include <math.h>

static const float DEFAULT_ROTATION_FRICTION = 0.05f; /* 5% remains after 1 sec */
static const float DEFAULT_ZOOM_FRICTION     = 0.01f; /* 1% remains after 1 sec */
static const float VELOCITY_EPSILON          = 0.0001f;

camera_motion_t camera_motion_create(void)
{
    return camera_motion_create_custom(DEFAULT_ROTATION_FRICTION,
                                       DEFAULT_ZOOM_FRICTION);
}

camera_motion_t camera_motion_create_custom(float rotation_friction,
                                             float zoom_friction)
{
    camera_motion_t m;
    m.az_velocity       = 0.0f;
    m.el_velocity       = 0.0f;
    m.rotation_friction = rotation_friction;
    m.dragging          = 0;
    m.zoom_velocity     = 0.0f;
    m.zoom_friction     = zoom_friction;
    m.az_out            = 0.0f;
    m.el_out            = 0.0f;
    m.zoom_out          = 0.0f;
    return m;
}

camera_motion_t camera_motion_drag(camera_motion_t m,
                                    float delta_az, float delta_el, float dt)
{
    if (!m.dragging) {
        /* Starting new drag — cancel any coast */
        m.az_velocity = 0.0f;
        m.el_velocity = 0.0f;
        m.dragging    = 1;
    }

    /* Track velocity as instantaneous speed */
    if (dt > 0.0f) {
        m.az_velocity = delta_az / dt;
        m.el_velocity = delta_el / dt;
    }

    /* Output direct delta during drag */
    m.az_out   = delta_az;
    m.el_out   = delta_el;
    m.zoom_out = 0.0f;
    return m;
}

camera_motion_t camera_motion_release(camera_motion_t m)
{
    m.dragging = 0;
    /* Velocity preserved from last drag frame — coast begins */
    return m;
}

camera_motion_t camera_motion_scroll(camera_motion_t m, float scroll_delta)
{
    m.zoom_velocity += scroll_delta;
    return m;
}

camera_motion_t camera_motion_update(camera_motion_t m, float dt)
{
    if (dt <= 0.0f) {
        m.az_out   = 0.0f;
        m.el_out   = 0.0f;
        m.zoom_out = 0.0f;
        return m;
    }

    /* Rotation: coast when not dragging */
    if (!m.dragging) {
        m.az_out = m.az_velocity * dt;
        m.el_out = m.el_velocity * dt;

        /* Frame-rate independent friction: v *= friction^dt */
        float rot_decay = powf(m.rotation_friction, dt);
        m.az_velocity *= rot_decay;
        m.el_velocity *= rot_decay;

        /* Snap to zero below epsilon */
        if (fabsf(m.az_velocity) < VELOCITY_EPSILON) {
            m.az_velocity = 0.0f;
        }
        if (fabsf(m.el_velocity) < VELOCITY_EPSILON) {
            m.el_velocity = 0.0f;
        }
    }
    /* (if dragging, az_out/el_out already set by camera_motion_drag) */

    /* Zoom: always coast (zoom events are impulses, not sustained) */
    m.zoom_out = m.zoom_velocity * dt;
    float zoom_decay = powf(m.zoom_friction, dt);
    m.zoom_velocity *= zoom_decay;
    if (fabsf(m.zoom_velocity) < VELOCITY_EPSILON) {
        m.zoom_velocity = 0.0f;
    }

    return m;
}

int camera_motion_active(camera_motion_t m, float epsilon)
{
    return fabsf(m.az_velocity) > epsilon
        || fabsf(m.el_velocity) > epsilon
        || fabsf(m.zoom_velocity) > epsilon;
}

camera_motion_t camera_motion_stop(camera_motion_t m)
{
    m.az_velocity   = 0.0f;
    m.el_velocity   = 0.0f;
    m.zoom_velocity = 0.0f;
    m.az_out        = 0.0f;
    m.el_out        = 0.0f;
    m.zoom_out      = 0.0f;
    m.dragging      = 0;
    return m;
}
