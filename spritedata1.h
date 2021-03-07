
/**
 * Copyright 2021 Jesús Abelardo Saldívar Aguilar
 *
 * This file contains the source for graphical data, and is distributed
 * under the Creative Commons Attribution-ShareAlike 4.0 International
 * Public License.
 *
 */

static const VGfloat sp1coords[]   = {

0,        100,
50,       100,
100,      50,
100,      0,

100,      -66.666,
116.666,  -66.666,
133.333,  -83.333,
133.333,  -100,
133.333,  -116.666,
116.666,  -133.333,
100,      -133.333,
83.333,   -133.333,
66.666,   -116.666,
66.666,   -100,
66.666,   -116.666,
50,       -133.333,
33.333,   -133.333,
16.666,   -133.333,
0,        -116.666,
0,        -100,
0,        -116.666,
-16.666,  -133.333,
-33.333,  -133.333,
-50,      -133.333,
-66.666,  -116.666,
-66.666,  -100,
-66.666,  -116.666,
-83.333,  -133.333,
-100,     -133.333,
-116.666, -133.333,
-133.333, -116.666,
-133.333, -100,
-133.333, -83.333,
-116.666, -66.666,
-100,     -66.666,

-100,     0,
-100,     50,
-50,      100,
0,        100

};


static const VGubyte sp1cmds[] = {
	VG_MOVE_TO_ABS,

	VG_CUBIC_TO_ABS,
	VG_LINE_TO_ABS,

	VG_CUBIC_TO_ABS,
	VG_CUBIC_TO_ABS,
	VG_CUBIC_TO_ABS,
	VG_CUBIC_TO_ABS,
	VG_CUBIC_TO_ABS,
	VG_CUBIC_TO_ABS,
	VG_CUBIC_TO_ABS,
	VG_CUBIC_TO_ABS,
	VG_CUBIC_TO_ABS,
	VG_CUBIC_TO_ABS,

	VG_LINE_TO_ABS,
	VG_CUBIC_TO_ABS,

	VG_CLOSE_PATH
};


JS_VG_PATH path_1 = {
	.coords = sp1coords,
	.cmds = sp1cmds,
	.count_cmds = 16
};

JS_VG_ARC a_1 = {
	.x = 0,
	.y = 0,
	.width = 20,
	.height = 30,
	.startAngle = 45,
	.angleExtent = 315,
	.arcType = VGU_ARC_PIE
};

JS_VG_ELLIPSE e_1 = {
	.cx = 0,
	.cy = 0,
	.width = 50,
	.height = 75
};

JS_VG_SPRITE_PATH sprite_path_1 = {
	.cap_style = VG_CAP_ROUND,
	.join_style = VG_JOIN_ROUND,
	.stroke_line_width = 5,
	.fill_color = { 
0.81,
0.72,
1, 1 },
	.stroke_color = { 
0.0,
0.541176471,
0.878431373, 1, },
	.bg_color = { 1.0, 1.0, 1.0, 0.75 },
	.data = &path_1,
	.init_sp_path = inti_path_raw_path
};

JS_VG_SPRITE_PATH sprite_path_ellipse = {
	.cap_style = VG_CAP_BUTT,
	.join_style = VG_JOIN_MITER,
	.stroke_line_width = 5,
	.fill_color = { 
0.69,
0.87,
0.90, 1, },
	.stroke_color = { 
0.0,
0.54,
0.87, 1,  },
	.bg_color = { 1.0, 1.0, 1.0, 0.75 },
	.data = &e_1,
	.init_sp_path = inti_path_ellipse
};

JS_VG_SPRITE_PATH sprite_path_arc = {
	.cap_style = VG_CAP_BUTT,
	.join_style = VG_JOIN_MITER,
	.stroke_line_width = 0,
	.fill_color = { 
0.0,
0.54,
0.87, 1,  },
	.stroke_color = { 
0.0,
0.54,
0.87, 1, },
	.bg_color = { 1.0, 1.0, 1.0, 0.75 },
	.data = &a_1,
	.init_sp_path = inti_path_arc
};

JS_VG_SPRITE group_array[5] = {{
	.translate = { 0, 0 },
	.scale = { 1, 1 },
	.rotate = 0,
	.data = &sprite_path_1,
	.init_sprite = init_sprite_path,
	.draw_func = draw_sprite_path
},
{
	.translate = { -50, 0 },
	.scale = { 1, 1 },
	.rotate = 0,
	.data = &sprite_path_ellipse,
	.init_sprite = init_sprite_path,
	.draw_func = draw_sprite_path
},
{
	.translate = { -50, 0 },
	.scale = { 1, 1 },
	.rotate = 0,
	.data = &sprite_path_arc,
	.init_sprite = init_sprite_path,
	.draw_func = draw_sprite_path
},
{
	.translate = { 20, 0 },
	.scale = { 1, 1 },
	.rotate = 0,
	.data = &sprite_path_ellipse,
	.init_sprite = init_sprite_path,
	.draw_func = draw_sprite_path
},
{
	.translate = { 20, 0 },
	.scale = { 1, 1 },
	.rotate = 0,
	.data = &sprite_path_arc,
	.init_sprite = init_sprite_path,
	.draw_func = draw_sprite_path
}};

JS_VG_SPRITE_GROUP sprite_group_1 = {
	.count = 5,
	.sprites = group_array
};

JS_VG_SPRITE sprite_grupo = {
	.translate = { 600, 250 },
	.scale = { 1, 1 },
	.rotate = 45,
	.data = &sprite_group_1,
	.init_sprite = init_sprite_group_array,
	.draw_func = draw_sprite_group_array
};



