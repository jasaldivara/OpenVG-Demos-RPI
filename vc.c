
/*
 *
 * Copyright (c) 2007 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Materials. 
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 */

#include "vc.h"

void inicia_egl(EGL_DISPMANX_WINDOW_T *nativewindow){

	static const EGLint s_configAttribs[] =
	{
		EGL_RED_SIZE,		8,
		EGL_GREEN_SIZE, 	8,
		EGL_BLUE_SIZE,		8,
		EGL_ALPHA_SIZE, 	8,
		EGL_LUMINANCE_SIZE, EGL_DONT_CARE,			//EGL_DONT_CARE
		EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENVG_BIT,
		EGL_SAMPLES,		1,
		EGL_NONE
	};
	EGLint numconfigs;

	egldisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(egldisplay, NULL, NULL);
	assert(eglGetError() == EGL_SUCCESS);
	eglBindAPI(EGL_OPENVG_API);

	eglChooseConfig(egldisplay, s_configAttribs, &eglconfig, 1, &numconfigs);
	assert(eglGetError() == EGL_SUCCESS);
	assert(numconfigs == 1);
	
	eglsurface = eglCreateWindowSurface(egldisplay, eglconfig, nativewindow, NULL);
	assert(eglGetError() == EGL_SUCCESS);
	eglcontext = eglCreateContext(egldisplay, eglconfig, NULL, NULL);
	assert(eglGetError() == EGL_SUCCESS);
	eglMakeCurrent(egldisplay, eglsurface, eglsurface, eglcontext);
	assert(eglGetError() == EGL_SUCCESS);
}

void inicia_vc(EGL_DISPMANX_WINDOW_T *nativewindow){

  uint32_t width, height;
  int status = 0;
  
  DISPMANX_ELEMENT_HANDLE_T dispman_element;
  DISPMANX_DISPLAY_HANDLE_T dispman_display;
  DISPMANX_UPDATE_HANDLE_T dispman_update;
  VC_RECT_T dst_rect;
  VC_RECT_T src_rect;

  bcm_host_init();

  status = graphics_get_display_size(0 /* screen number */, &width, &height);
  assert ( status >= 0 );

  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.width = width;
  dst_rect.height = height;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = width << 16;
  src_rect.height = height << 16;

  dispman_display = vc_dispmanx_display_open( 0 /* screen number */);
  dispman_update = vc_dispmanx_update_start( 0 );

  dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display,
				1 /*layer*/, &dst_rect, 0 /*src*/,
				&src_rect, DISPMANX_PROTECTION_NONE, 
				0 /*alpha*/, 0 /*clamp*/, 0 /*transform*/);
  nativewindow->element = dispman_element;
  nativewindow->width = width;
  nativewindow->height = height;
  vc_dispmanx_update_submit_sync(dispman_update);

  inicia_egl(nativewindow);
}

void deinit(void)
{
	eglMakeCurrent(egldisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	assert(eglGetError() == EGL_SUCCESS);
	eglTerminate(egldisplay);
	assert(eglGetError() == EGL_SUCCESS);
	eglReleaseThread();
}

