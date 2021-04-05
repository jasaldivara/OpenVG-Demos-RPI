

#include <stdio.h>
#include <assert.h>

#include <vc_dispmanx.h>
#include <bcm_host.h>
#include <EGL/egl.h>

EGLDisplay			egldisplay;
EGLConfig			eglconfig;
EGLSurface			eglsurface;
EGLContext			eglcontext;

//void inicia_egl(EGL_DISPMANX_WINDOW_T *nativewindow);
void inicia_vc(EGL_DISPMANX_WINDOW_T *nativewindow);
void deinit(void);

