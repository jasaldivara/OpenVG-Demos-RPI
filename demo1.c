#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <VG/openvg.h>
#include <VG/vgu.h>

#include "rpi.h"
#include "sprites.h"
#include "spritedata1.h"

 
EGL_STATE_T state, *p_state = &state;

VGfloat stroke_desp = 0.0f;
VGfloat rota = 0.0f;

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
    vgAppendPathData(path, 16, sp1cmds, sp1coords);

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
    vgRotate(rota);

    vgDrawPath(path, VG_STROKE_PATH|VG_FILL_PATH);

    vgLoadIdentity();
    
    // Destruir trazo para liberar memoria GPU
    // TODO: Quizá sea mejor crear trazos una sola vez para no tener que
    // destruirlos, y así optimizar tiempo/velocidad.
    vgDestroyPath(path);
}

void draw(EGL_DISPMANX_WINDOW_T *nativewindow){
    float c = 1.0;
    float clearColor[4] = {c, c, c, c};  // white, no transparency
    float dx = 1;
    float dy = 1;
    vgSetfv(VG_CLEAR_COLOR, 4, clearColor);

    vgClear(0, 0, nativewindow->width, nativewindow->height);

    vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_OVER);
    
    while (1){
	// ¿Por qué ellipse.c no necesita llamar a esta función durante ciclo?
	//vgSetfv(VG_CLEAR_COLOR, 4, clearColor);
    	vgClear(0, 0, nativewindow->width, nativewindow->height);

    	//simple_shape();
	draw_sprite(&sprite_grupo);
	eglSwapBuffers(p_state->display, p_state->surface);

	//stroke_desp += 1.0f;
	sprite_grupo.rotate += 1.0f;
	sprite_grupo.translate[0] += dx;
	sprite_grupo.translate[1] += dy;
	
	if (sprite_grupo.translate[0] + 100 >= nativewindow->width){
	    sprite_grupo.translate[0] = nativewindow->width - 100;
	    dx = -1;
	}
	if (sprite_grupo.translate[0] - 100 <= 0){
	    sprite_grupo.translate[0] = 100;
	    dx = 1;
	}
	if (sprite_grupo.translate[1] + 100 >= nativewindow->height){
	    sprite_grupo.translate[1] = nativewindow->height - 100;
	    dy = -1;
	}
	if (sprite_grupo.translate[1] - 100 <= 0){
	    sprite_grupo.translate[1] = 100;
	    dy = 1;
	}
	
    //vgFlush();
    }

    vgFlush();
}


void sig_handler(int sig) {
    printf("Abortando preceso...\n");
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
    
    init_sprite(&sprite_grupo);

    draw(&nativewindow);
    eglSwapBuffers(p_state->display, p_state->surface);

    sleep(20);
    eglTerminate(p_state->display);

    exit(0);
}

