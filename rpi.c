
/* 
 * The code in this file is taken from https://jan.newmarch.name/RPi/OpenVG/Basic/
 * which is also adapted from openGL-RPi-tutorial-master/encode_OGL/
 */


#include "rpi.h"




void init_egl(EGL_STATE_T *state)
{
    EGLint num_configs;
    EGLBoolean result;

    //bcm_host_init();

    static const EGLint attribute_list[] =
	{
	    EGL_RED_SIZE, 8,
	    EGL_GREEN_SIZE, 8,
	    EGL_BLUE_SIZE, 8,
	    EGL_ALPHA_SIZE, 8,
	    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	    EGL_RENDERABLE_TYPE, EGL_OPENVG_BIT,
	    EGL_NONE
	};

    static const EGLint context_attributes[] =
	{
	    EGL_CONTEXT_CLIENT_VERSION, 2,
	    EGL_NONE
	};

    // get an EGL display connection
    state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    // initialize the EGL display connection
    result = eglInitialize(state->display, NULL, NULL);

    // get an appropriate EGL frame buffer configuration
    result = eglChooseConfig(state->display, attribute_list, &state->config, 1, &num_configs);
    assert(EGL_FALSE != result);

    //result = eglBindAPI(EGL_OPENGL_ES_API);
    result = eglBindAPI(EGL_OPENVG_API);
    assert(EGL_FALSE != result);

    // create an EGL rendering context
    state->context = eglCreateContext(state->display, 
				      state->config, EGL_NO_CONTEXT, 
				      NULL);
				      // breaks if we use this: context_attributes);
    assert(state->context!=EGL_NO_CONTEXT);
}

void init_dispmanx(EGL_DISPMANX_WINDOW_T *nativewindow) {   
    int32_t success = 0;   
    uint32_t screen_width;
    uint32_t screen_height;

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    bcm_host_init();

    // create an EGL window surface
    success = graphics_get_display_size(0 /* LCD */, 
					&screen_width, 
					&screen_height);
    assert( success >= 0 );

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = screen_width;
    dst_rect.height = screen_height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = screen_width << 16;
    src_rect.height = screen_height << 16;        

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

    dispman_element = 
	vc_dispmanx_element_add(dispman_update, dispman_display,
				0/*layer*/, &dst_rect, 0/*src*/,
				&src_rect, DISPMANX_PROTECTION_NONE, 
				0 /*alpha*/, 0/*clamp*/, 0/*transform*/);

    // Build an EGL_DISPMANX_WINDOW_T from the Dispmanx window
    nativewindow->element = dispman_element;
    nativewindow->width = screen_width;
    nativewindow->height = screen_height;
    vc_dispmanx_update_submit_sync(dispman_update);

    printf("Got a Dispmanx window\n");
}

void egl_from_dispmanx(EGL_STATE_T *state, 
		       EGL_DISPMANX_WINDOW_T *nativewindow) {
    EGLBoolean result;

    state->surface = eglCreateWindowSurface(state->display, 
					    state->config, 
					    nativewindow, NULL );
    assert(state->surface != EGL_NO_SURFACE);

    // connect the context to the surface
    result = eglMakeCurrent(state->display, state->surface, state->surface, state->context);
    assert(EGL_FALSE != result);
}

void simple_shape() {
    VGint cap_style = VG_CAP_ROUND;
    VGint join_style = VG_JOIN_ROUND;
    VGfloat color[4] = {0.1, 1.0, 1.0, 0.5};
    VGfloat color_2[4] = {0.3, 0.5, 0.9, 0.6};
    VGfloat white_color[4] = {1.0, 1.0, 1.0, 0.7}; //1.0};
    VGPaint fill, fill2;

    static const VGubyte cmds[] = {VG_MOVE_TO_ABS,
				   VG_LINE_TO_ABS,
				   VG_LINE_TO_ABS,
				   VG_CLOSE_PATH
    };

    // TODO: Modificar coordenadas para adaptar a tamaño de pantalla
    static const VGfloat coords[]   = {-150, -150, 
				       150, -150, 
				       0, 150
    };



    VGfloat dash_pattern[2] = { 20.f, 20.f };
    VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, 
			       VG_PATH_DATATYPE_F, 1, 0, 0, 0,
			       VG_PATH_CAPABILITY_APPEND_TO);
    vgAppendPathData(path, 16, cmds, coords);

    fill = vgCreatePaint();
    fill2 = vgCreatePaint();
    vgSetParameterfv(fill, VG_PAINT_COLOR, 4, color);
    vgSetPaint(fill, VG_FILL_PATH);
    vgSetParameterfv(fill2, VG_PAINT_COLOR, 4, color_2);
    vgSetPaint(fill2, VG_STROKE_PATH);

    vgSetfv(VG_CLEAR_COLOR, 4, white_color);
    vgSetf(VG_STROKE_LINE_WIDTH, 10);
    vgSeti(VG_STROKE_CAP_STYLE, cap_style);
    vgSeti(VG_STROKE_JOIN_STYLE, join_style);
    //vgSetfv(VG_STROKE_DASH_PATTERN, 2, dash_pattern);
    //vgSetf(VG_STROKE_DASH_PHASE, stroke_desp);

    //vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
    vgTranslate(400.0f, 300.0f);
    vgScale(1.5, 1.5);
    //vgRotate(rota);

    vgDrawPath(path, VG_STROKE_PATH|VG_FILL_PATH);

    vgLoadIdentity();
    
    // Destruir trazo para liberar memoria GPU
    // TODO: Quizá sea mejor crear trazos una sola vez para no tener que
    // destruirlos, y así optimizar tiempo/velocidad.
    vgDestroyPath(path);
}

