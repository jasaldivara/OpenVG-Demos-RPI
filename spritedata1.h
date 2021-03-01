
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

JS_VG_SPRITE_PATH sprite_path_1 = {
	.cap_style = VG_CAP_ROUND,
	.join_style = VG_JOIN_ROUND,
	.stroke_line_width = 7.5,
	.fill_color = { 
0.81,
0.72,
1, 0.75 },
	.stroke_color = { 
0.0,
0.541176471,
0.878431373, 0.75, },
	.bg_color = { 1.0, 1.0, 1.0, 0.75 },
	.data = &path_1,
	.init_sp_path = inti_path_raw_path
};

JS_VG_SPRITE sprite_1 = {
	.translate = { 600, 250 },
	.scale = { 1, 1 },
	.rotate = 45,
	.data = &sprite_path_1,
	.init_sprite = init_sprite_path,
	.draw_func = draw_sprite_path
};

