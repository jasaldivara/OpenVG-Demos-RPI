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
#include "sprites.h"
#include "spritedata1.h"

#define PI 3.1416
 
EGL_STATE_T state, *p_state = &state;

VGfloat stroke_desp = 0.0f;
VGfloat rota = 0.0f;


typedef struct {
    JS_VG_SPRITE *sprite;
    float dr;
    float dx;
    float dy;
    void *sig
} SPRITE_REBOTA;

SPRITE_REBOTA *list_bouncers = NULL;

void addBouncer(int w, int h, float radius){
    float x, y, ang, vel, dr, dx, dy;
    float rangox, rangoy;
    JS_VG_SPRITE *sprite;
    
    sprite = malloc(sizeof(JS_VG_SPRITE));
    rangox = w - (2 * radius);
    rangoy = h - (2 * radius);
    
    x = ( ( (float)rand() / (float)RAND_MAX ) * rangox ) + radius;
    y = ( ( (float)rand() / (float)RAND_MAX ) * rangoy ) + radius;
    
    ang = ( ( (float)rand() / (float)RAND_MAX ) * PI * 2);
    vel = ( ( (float)rand() / (float)RAND_MAX ) * 8);
    dr = ( ( (float)rand() / (float)RAND_MAX ) * 4) - 2;

    //dx = ( ( (float)rand() / (float)RAND_MAX ) * 4 ) - 2;
    //dy = ( ( (float)rand() / (float)RAND_MAX ) * 4 ) - 2;

    dx = sin(ang) * vel;
    dy = cos(ang) * vel;
    
    //x = 
    sprite->translate[0] = x;
    sprite->translate[1] = y;
    sprite->scale[0] = 1;
    sprite->scale[1] = 1;
    sprite->rotate = 0;
    
    sprite->data = &sprite_group_1;
    sprite->init_sprite = init_sprite_group_array;
    sprite->draw_func = draw_sprite_group_array;
    
    init_sprite(sprite);
    
    SPRITE_REBOTA *rebota = malloc(sizeof(SPRITE_REBOTA));
    rebota->sprite = sprite;
    rebota->dr = dr;
    rebota->dx = dx;
    rebota->dy = dy;
    rebota->sig = list_bouncers;
    list_bouncers = rebota;
}

void procesaRebota(SPRITE_REBOTA *rebota, int scenewidth, int sceneheight){
    rebota->sprite->rotate += rebota->dr;
    rebota->sprite->translate[0] += rebota->dx;
    rebota->sprite->translate[1] += rebota->dy;
    
    if (rebota->sprite->translate[0] + 100 >= scenewidth){
	rebota->sprite->translate[0] = scenewidth - 100;
	if (rebota->dx > 0){
	    rebota->dx *= -1;
	    rebota->dr += 0 - rebota->dy;
	    rebota->dr /= 2;
	}
    }
    if (rebota->sprite->translate[0] - 100 <= 0){
	rebota->sprite->translate[0] = 100;
	if (rebota->dx < 0){
	    rebota->dx *= -1;
	    rebota->dr +=  rebota->dy;
	    rebota->dr /= 2;
	}
    }
    if (rebota->sprite->translate[1] + 100 >= sceneheight){
	rebota->sprite->translate[1] = sceneheight - 100;
	if (rebota->dy > 0){
	    rebota->dy *= -1;
	    rebota->dr +=  rebota->dx;
	    rebota->dr /= 2;
	}
    }
    if (rebota->sprite->translate[1] - 100 <= 0){
	rebota->sprite->translate[1] = 100;
	if (rebota->dy < 0){
	    rebota->dy *= -1;
	    rebota->dr += 0 - rebota->dx;
	    rebota->dr /= 2;
	}
    }
};

void draw(EGL_DISPMANX_WINDOW_T *nativewindow){

    float clearColor[4] = {1, 1, 1, 0};
    float dx = 1;
    float dy = 1;
    vgSetfv(VG_CLEAR_COLOR, 4, clearColor);

    vgClear(0, 0, nativewindow->width, nativewindow->height);

    vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_OVER);
    
    while (1){
	// ¿Por qué ellipse.c no necesita llamar a esta función durante ciclo?
	vgSetfv(VG_CLEAR_COLOR, 4, clearColor);
	
    	vgClear(0, 0, nativewindow->width, nativewindow->height);

    	//simple_shape();
	//draw_sprite(sprite_escena);
	
	for (SPRITE_REBOTA *rebota = list_bouncers; rebota != NULL; rebota = rebota->sig){
	    draw_sprite(rebota->sprite);
	}
	
	eglSwapBuffers(p_state->display, p_state->surface);

	for (SPRITE_REBOTA *rebota = list_bouncers; rebota != NULL; rebota = rebota->sig){
	    procesaRebota(rebota, nativewindow->width, nativewindow->height);
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

int
main(int argc, char *argv[])
{ 
    EGL_DISPMANX_WINDOW_T nativewindow;
    
    JS_VG_SPRITE_LIST lista_sprites = {
	.sprite = &sprite_grupo,
	.sig = NULL
    };
    
    
    srand((unsigned int) time(NULL));
    signal(SIGINT, sig_handler);

    init_egl(p_state);
    init_dispmanx(&nativewindow);
    egl_from_dispmanx(p_state, &nativewindow);
    
    //init_sprite(&sprite_escena);

    addBouncer(nativewindow.width, nativewindow.height, 100);
    addBouncer(nativewindow.width, nativewindow.height, 100);
    addBouncer(nativewindow.width, nativewindow.height, 100);
    addBouncer(nativewindow.width, nativewindow.height, 100);

    draw(&nativewindow);
    eglSwapBuffers(p_state->display, p_state->surface);

    sleep(20);
    eglTerminate(p_state->display);

    exit(0);
}

