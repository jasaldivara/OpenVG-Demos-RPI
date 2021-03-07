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

void draw(EGL_DISPMANX_WINDOW_T *nativewindow, JS_VG_SPRITE *sprite_escena){

    float clearColor[4] = {1, 1, 1, 1};
    float dx = 1;
    float dy = 1;
    float ds = 1.01;
    vgSetfv(VG_CLEAR_COLOR, 4, clearColor);

    vgClear(0, 0, nativewindow->width, nativewindow->height);

    vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_OVER);
    
    while (1){
	// ¿Por qué ellipse.c no necesita llamar a esta función durante ciclo?
	//vgSetfv(VG_CLEAR_COLOR, 4, clearColor);
    	vgClear(0, 0, nativewindow->width, nativewindow->height);

    	//simple_shape();
	draw_sprite(sprite_escena);
	eglSwapBuffers(p_state->display, p_state->surface);

	//stroke_desp += 1.0f;
	sprite_grupo.rotate += 1.0f;
	sprite_grupo.scale[0] *= ds;
	sprite_grupo.scale[1] *= ds;
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
	if (sprite_grupo.scale[0] > 10){
	    sprite_grupo.scale[0] = 10;
	    ds = 0.99;
	}
	if (sprite_grupo.scale[0] < 0.02){
	    sprite_grupo.scale[0] = 0.02;
	    ds = 1.01;
	}
	
    //vgFlush();
    }

    vgFlush();
}


void sig_handler(int sig) {
    printf("Abortando proceso...\n");
    eglTerminate(p_state->display);
    exit(1);
}
/*
typedef struct {
    int
} SPRITE_BOUNCE;
*/

void sprite_bounce_routine(JS_VG_SPRITE *sprite){
    
}


int
main(int argc, char *argv[])
{ 
    EGL_DISPMANX_WINDOW_T nativewindow;
    
    JS_VG_SPRITE_LIST lista_sprites = {
	.sprite = &sprite_grupo,
	.sig = NULL
    };
    
    JS_VG_SPRITE sprite_escena = {
	.translate = { 0, 0 },
	.scale = { 1, 1 },
	.rotate = 0,
	.data = &lista_sprites,
	.init_sprite = init_sprite_group_list,
	.draw_func = draw_sprite_group_list
    };

    signal(SIGINT, sig_handler);

    init_egl(p_state);
    init_dispmanx(&nativewindow);
    egl_from_dispmanx(p_state, &nativewindow);
    
    init_sprite(&sprite_escena);

    draw(&nativewindow, &sprite_escena);
    eglSwapBuffers(p_state->display, p_state->surface);

    sleep(20);
    eglTerminate(p_state->display);

    exit(0);
}

