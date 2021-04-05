
/* 
 * The code in this file is taken from https://jan.newmarch.name/RPi/OpenVG/Basic/
 * which is also adapted from openGL-RPi-tutorial-master/encode_OGL/
 */


#include <stdio.h>
#include <assert.h>

#include <vc_dispmanx.h>
#include <bcm_host.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
//#include <GLES2/gl2.h>
#include <VG/openvg.h>
#include <VG/vgu.h>



typedef struct
{
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    EGLConfig config;
} EGL_STATE_T;

void init_egl(EGL_STATE_T *state);

void init_dispmanx(EGL_DISPMANX_WINDOW_T *nativewindow); 

void egl_from_dispmanx(EGL_STATE_T *state,EGL_DISPMANX_WINDOW_T *nativewindow);

void simple_shape();

