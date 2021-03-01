
/**
 * Copyright 2021 Jesús Abelardo Saldívar Aguilar
 *
 *
 */

#include "sprites.h"


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

