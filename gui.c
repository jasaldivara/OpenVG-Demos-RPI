
/**
 * Copyright 2021 Jesús Abelardo Saldívar Aguilar
 *
 *
 * This will be a demonstration of drawing a Graphical User Interface
 * using OpenVG.
 *
 * At this moment, it only draws a rectangle (a window) with rounded corners
 * on the screen.
 *
 */
 
 
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <VG/openvg.h>
#include <VG/vgu.h>

#include "vc.h"


VGfloat color_yellow[] = { 
0.9,
0.9,
0.6, 0.9 };
VGfloat color_blue[] = { 
0.2,
0.441176471,
0.878431373, 0.9, };

VGfloat color_green[] = { 
0.0,
0.7,
0.3, 0.9 };

VGfloat color_orange[] = { 
0.99,
0.8,
0.6, 0.9 };
VGfloat color_purple[] = { 
0.7,
0.241176471,
0.78431373, 0.9, };

VGfloat color_black[] = { 
0.2,
0.241176471,
0.28431373, 1, };

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

void drawRect(float x, float y, float w, float h, float radius, float border_width, VGfloat *bg[4] , VGfloat *fg[4] ){

    VGPaint fill_paint, stroke_paint;

    fill_paint = vgCreatePaint();
    vgSetParameterfv(fill_paint, VG_PAINT_COLOR, 4, bg);
    
    VGPath wpath = get_window_path(x, y, w, h, radius);
    
    stroke_paint = vgCreatePaint();
    vgSetParameterfv(stroke_paint, VG_PAINT_COLOR, 4, fg);

    vgSetPaint(fill_paint, VG_FILL_PATH);
    vgSetPaint(stroke_paint, VG_STROKE_PATH);
    
    //vgSetfv(VG_CLEAR_COLOR, 4, sprite_path->bg_color);
    vgSetf(VG_STROKE_LINE_WIDTH, border_width);
    //vgSeti(VG_STROKE_CAP_STYLE, sprite_path->cap_style);
    //vgSeti(VG_STROKE_JOIN_STYLE, sprite_path->join_style);
    
    vgDrawPath(wpath, VG_STROKE_PATH | VG_FILL_PATH);   
}

void draw(EGL_DISPMANX_WINDOW_T *nativewindow){

    
    float clearColor[4] = {1, 1, 1, 0};
    vgSetfv(VG_CLEAR_COLOR, 4, clearColor);

    vgClear(0, 0, nativewindow->width, nativewindow->height);

    vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_OVER);

    drawRect(200, 200, 500, 500, 10, 5, &color_green, &color_yellow);
    drawRect(220, 648, 400, 32, 5, 1, &color_blue, &color_black);

    eglSwapBuffers(egldisplay, eglsurface);

    vgFlush();
}



void sig_handler(int sig) {
    printf("Abortando proceso...\n");
    deinit();
    exit(1);
}

int
main(int argc, char *argv[])
{ 
    EGL_DISPMANX_WINDOW_T nativewindow;

    signal(SIGINT, sig_handler);


    inicia_vc(&nativewindow);

    draw(&nativewindow);
    
    getchar();

    deinit();

    exit(0);
}

