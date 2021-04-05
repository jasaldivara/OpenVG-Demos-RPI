#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <VG/openvg.h>
#include <VG/vgu.h>

#include "rpi.h"

 
EGL_STATE_T state, *p_state = &state;

VGPath get_window_path(float x, float y, float w, float h, float radius){
	static VGfloat coords[16 * 2];
	coords[0] = x + radius;
	coords[1] = y;
	coords[2] = w - (radius * 2);
	coords[3] = 0;
	coords[4] = radius;
	coords[5] = 0;
	coords[6] = radius;
	coords[7] = radius;
	coords[8] = 0;
	coords[9] = h - (radius * 2);
	coords[10] = 0;
	coords[11] = radius;
	coords[12] = 0 - radius;
	coords[13] = radius;
	coords[14] = ( radius * 2 ) - w;
	coords[15] = 0;
	coords[16] = 0 - radius;
	coords[17] = 0;
	coords[18] = 0 - radius;
	coords[19] = 0 - radius;
	coords[20] = 0;
	coords[21] = ( radius * 2 ) - h;
	coords[22] = 0;
	coords[23] = 0 - radius;
	coords[24] = radius;
	coords[25] = 0 - radius;
	
	static const VGubyte cmds[] = {
	VG_MOVE_TO_ABS,
	VG_LINE_TO_REL,
	VG_QUAD_TO_REL,
	VG_LINE_TO_REL,
	VG_QUAD_TO_REL,
	VG_LINE_TO_REL,
	VG_QUAD_TO_REL,
	VG_LINE_TO_REL,
	VG_QUAD_TO_REL,
	VG_CLOSE_PATH
	};

	VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, 
			       VG_PATH_DATATYPE_F, 1, 0, 0, 0,
			       VG_PATH_CAPABILITY_APPEND_TO);
	
	vgAppendPathData(path, 10, cmds, coords);
	
	return path;
}

void draw(EGL_DISPMANX_WINDOW_T *nativewindow){

	VGPaint fill_paint, stroke_paint;
	VGfloat fill_color[] = { 
0.91,
0.9,
0.91, 1 };
	VGfloat stroke_color[] = { 
0.2,
0.241176471,
0.278431373, 1, };

    fill_paint = vgCreatePaint();
    vgSetParameterfv(fill_paint, VG_PAINT_COLOR, 4, fill_color);
    
    VGPath wpath = get_window_path(200, 300, 600, 400, 5);
    
    stroke_paint = vgCreatePaint();
    vgSetParameterfv(stroke_paint, VG_PAINT_COLOR, 4, stroke_color);
    
    float clearColor[4] = {1, 1, 1, 0};
    vgSetfv(VG_CLEAR_COLOR, 4, clearColor);

    vgClear(0, 0, nativewindow->width, nativewindow->height);

    vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_OVER);

    vgSetPaint(fill_paint, VG_FILL_PATH);
    vgSetPaint(stroke_paint, VG_STROKE_PATH);
    
    //vgSetfv(VG_CLEAR_COLOR, 4, sprite_path->bg_color);
    vgSetf(VG_STROKE_LINE_WIDTH, 2);
    //vgSeti(VG_STROKE_CAP_STYLE, sprite_path->cap_style);
    //vgSeti(VG_STROKE_JOIN_STYLE, sprite_path->join_style);
    
    vgDrawPath(wpath, VG_STROKE_PATH | VG_FILL_PATH);   
    
    eglSwapBuffers(p_state->display, p_state->surface);

    vgFlush();
}



void sig_handler(int sig) {
    printf("Abortando proceso...\n");
    eglTerminate(p_state->display);
    exit(1);
}

int
main(int argc, char *argv[])
{ 
    EGL_DISPMANX_WINDOW_T nativewindow;

    signal(SIGINT, sig_handler);

    init_egl(p_state);
    init_dispmanx(&nativewindow);
    egl_from_dispmanx(p_state, &nativewindow);

    draw(&nativewindow);
    
    getchar();
    //eglSwapBuffers(p_state->display, p_state->surface);

    eglTerminate(p_state->display);

    exit(0);
}

