
/**
 * Copyright 2021 Jesús Abelardo Saldívar Aguilar
 *
 *
 * This file contains various functions for manipulating and displaying sprites.
 */

#include "sprites.h"
#include <stddef.h>
#include <stdlib.h>


void init_sprite(JS_VG_SPRITE *sprite){

    sprite->init_sprite(sprite->data);
}

void init_sprite_path(JS_VG_SPRITE_PATH *sprite_path){

    sprite_path->fill_paint = vgCreatePaint();
    vgSetParameterfv(sprite_path->fill_paint, VG_PAINT_COLOR, 4, sprite_path->fill_color);
    
    sprite_path->stroke_paint = vgCreatePaint();
    vgSetParameterfv(sprite_path->stroke_paint, VG_PAINT_COLOR, 4, sprite_path->stroke_color);
    
    sprite_path->path = sprite_path->init_sp_path(sprite_path->data);
}

void init_sprite_group_array(JS_VG_SPRITE_GROUP *sprite_group){
    JS_VG_SPRITE *array = sprite_group->sprites;
    
    for (int i = 0; i < sprite_group->count; i++){
	
	array[i].init_sprite(array[i].data);
    }
}

void init_sprite_group_list(JS_VG_SPRITE_LIST *sprite_list){
    JS_VG_SPRITE *sprite;
    
    while (sprite_list != NULL){
	sprite = sprite_list->sprite;
	sprite->init_sprite(sprite->data);
	sprite_list = sprite_list->sig;
    }

}

void draw_sprite(JS_VG_SPRITE *sprite){

    VGfloat matrix[9];
	    
    vgGetMatrix(matrix);
    
    vgTranslate(sprite->translate[0], sprite->translate[1]);
    vgRotate(sprite->rotate);
    vgScale(sprite->scale[0], sprite->scale[1]);
    
    // Llamar a la funcion interna de dibujo
    //vgDrawPath(sprite->path, VG_STROKE_PATH | VG_FILL_PATH);
    sprite->draw_func(sprite->data);
    
    vgLoadMatrix(matrix);
}
void draw_sprite_path(JS_VG_SPRITE_PATH *sprite_path){

    vgSetPaint(sprite_path->fill_paint, VG_FILL_PATH);
    vgSetPaint(sprite_path->stroke_paint, VG_STROKE_PATH);
    
    vgSetfv(VG_CLEAR_COLOR, 4, sprite_path->bg_color);
    vgSetf(VG_STROKE_LINE_WIDTH, sprite_path->stroke_line_width);
    vgSeti(VG_STROKE_CAP_STYLE, sprite_path->cap_style);
    vgSeti(VG_STROKE_JOIN_STYLE, sprite_path->join_style);
    
    vgDrawPath(sprite_path->path, VG_STROKE_PATH | VG_FILL_PATH);
}


void draw_sprite_group_array(JS_VG_SPRITE_GROUP *sprite_group){
    JS_VG_SPRITE *array = sprite_group->sprites;
    
    for (int i = 0; i < sprite_group->count; i++){
	
	draw_sprite(&array[i]);
	//array[i].draw_func(array[i].data);
    }
}


void draw_sprite_group_list(JS_VG_SPRITE_LIST *sprite_list){
    JS_VG_SPRITE_LIST *list = sprite_list;
    JS_VG_SPRITE *sprite;
    
    while (list != NULL){
	sprite = list->sprite;
	draw_sprite(sprite);
	list = list->sig;
    }

}



JS_VG_SPRITE_LIST *spriteListFromArray(int count, JS_VG_SPRITE *sprites){
    
    JS_VG_SPRITE_LIST *inicio = NULL;
    JS_VG_SPRITE_LIST *sig = NULL;
    
    if (count == 0){
	return NULL;
    }
    
    for (int i = 0; i < count; i++){
	sig = malloc(sizeof(JS_VG_SPRITE_LIST));
	sig->sig = inicio;
	inicio = sig;
	sig->sprite = &sprites[i];
    }
    
    return inicio;
}

// Un iniciador de trazo para cada tipo de forma:
// Trazo "en crudo", cuadrados, polígonos, alipses, arcos,
// usando librería VGU

VGPath inti_path_raw_path(void *data){
	JS_VG_PATH *path_struct = data;
	
	// TODO: Mejorar llamada a vgCreatePath
	VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, 
			       VG_PATH_DATATYPE_F, 1, 0, 0, 0,
			       VG_PATH_CAPABILITY_APPEND_TO);
	
	vgAppendPathData(path, path_struct->count_cmds, path_struct->cmds, path_struct->coords);
	
	return path;
}

VGPath inti_path_ellipse(void *data){
	JS_VG_ELLIPSE *ellipse_data = data;
	
	// TODO: Mejorar llamada a vgCreatePath
	VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, 
			       VG_PATH_DATATYPE_F, 1, 0, 0, 0,
			       VG_PATH_CAPABILITY_APPEND_TO);
	vguEllipse(path, ellipse_data->cx, ellipse_data->cy,
		    ellipse_data->width, ellipse_data->height);
	
	return path;
}

VGPath inti_path_arc(void *data){
    JS_VG_ARC *arc_data = data;

    // TODO: Mejorar llamada a vgCreatePath
    VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, 
			    VG_PATH_DATATYPE_F, 1, 0, 0, 0,
			    VG_PATH_CAPABILITY_APPEND_TO);
    vguArc(path, arc_data->x, arc_data->y, arc_data->width,
		arc_data->height, arc_data->startAngle,
		arc_data->angleExtent, arc_data->arcType);
    
    return path;
    
}

