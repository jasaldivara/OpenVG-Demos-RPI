
/**
 * Copyright 2021 Jesús Abelardo Saldívar Aguilar
 *
 *
 */


#include <VG/openvg.h>
#include <VG/vgu.h>


typedef struct {
	VGfloat *coords;
	VGubyte *cmds;
	// VGPath path;
	int count_cmds;
	int count_coords;
} JS_VG_PATH;

typedef struct {
	VGfloat cx;
	VGfloat cy;
	VGfloat width;
	VGfloat height;
} JS_VG_ELLIPSE;

typedef struct {
	VGfloat x;
	VGfloat y;
	VGfloat width;
	VGfloat height;
	VGfloat startAngle;
	VGfloat angleExtent;
	int arcType;
} JS_VG_ARC;

//typedef struct JS_VG_SPRITE JS_VG_SPRITE;

typedef struct {
	VGfloat translate[2];
	VGfloat scale[2];
	VGfloat rotate;
	
	// TODO: Dash pattern
	
	void *data;
	void (* init_sprite)(void *);
	void (* draw_func)(void *);
		
} JS_VG_SPRITE;

typedef struct {
	JS_VG_SPRITE *sprite;
	void *sig;
} JS_VG_SPRITE_LIST;

typedef struct {
	int count;
	JS_VG_SPRITE *sprites;
} JS_VG_SPRITE_GROUP;

typedef struct {
	VGint cap_style;
	VGint join_style;
	VGfloat stroke_line_width;
	VGfloat fill_color[4];
	VGfloat stroke_color[4];
	VGfloat bg_color[4];
	
	// TODO: Dash pattern
	
	JS_VG_PATH *data;
	VGPath (* init_sp_path)(void *);
	//void (*draw_func)(void *);
	
	VGPaint fill_paint, stroke_paint;
	VGPath path;
	
} JS_VG_SPRITE_PATH;

void init_sprite(JS_VG_SPRITE *sprite);

void draw_sprite(JS_VG_SPRITE *sprite);

JS_VG_SPRITE_LIST *spriteListFromArray(int count, JS_VG_SPRITE *sprites);

void init_sprite_path(JS_VG_SPRITE_PATH *sprite_path);

void init_sprite_group(JS_VG_SPRITE_GROUP *sprite_group);

void draw_sprite_path(JS_VG_SPRITE_PATH *sprite_path);

void draw_sprite_group(JS_VG_SPRITE_GROUP *sprite_group);

VGPath inti_path_raw_path(void *data);

VGPath inti_path_ellipse(void *data);

VGPath inti_path_arc(void *data);
