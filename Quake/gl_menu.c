/*
Copyright (C) 1996-2001 Id Software, Inc.
Copyright (C) 2002-2009 John Fitzgibbons and others
Copyright (C) 2007-2008 Kristian Duske
Copyright (C) 2010-2014 QuakeSpasm developers
Copyright (C) 2020 Daniel Abbott

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// gl_menu.c -- OpenGL / Graphics options menu

#include "quakedef.h"
#include "cfgfile.h"

static char* texture_filter_modes[] = {
	"GL_NEAREST",
	"GL_NEAREST_MIPMAP_NEAREST",
	"GL_NEAREST_MIPMAP_LINEAR",
	"GL_LINEAR",
	"GL_LINEAR_MIPMAP_NEAREST",
	"GL_LINEAR_MIPMAP_LINEAR"
};

static int texture_filters_max = (int)(sizeof texture_filter_modes / sizeof texture_filter_modes[0]);

static char* particle_styles[] = {
	"None",
	"Smooth",
	"Classic"
};

static int particle_styles_max = (int)(sizeof particle_styles / sizeof particle_styles[0]);

/*
================
GL_Menu_ChooseNextFilter

chooses next texture filtering mode in order, then updates gl_texturemode cvar
================
*/
static void GL_Menu_ChooseNextFilter(int dir)
{
	int i;

	for (i = 0; i < texture_filters_max; i++)
	{
		if (Q_strcmp(texture_filter_modes[i], Cvar_VariableString("gl_texturemode")) == 0) {
			break;
		}
	}

	if (i == texture_filters_max)
	{
		i = 0;
	}
	else
	{
		i += dir;
		if (i >= texture_filters_max)
			i = 0;
		else if (i < 0)
			i = texture_filters_max - 1;
	}

	Cvar_Set("gl_texturemode", texture_filter_modes[i]);
}

/*
================
GL_Menu_Anisotropy

Select anisotropy mode with slider, 1-2-4-8-16
================
*/
static void GL_Menu_Anisotropy(int dir) {
	int v = Q_atoi(Cvar_VariableString("gl_texture_anisotropy"));

	v = (dir > 0) ? (v *= 2) : (v /= 2);

	if (v < 1)
		v = 16;
	if (v > 16)
		v = 1;
	Cvar_SetValue("gl_texture_anisotropy", v);
}

/*
================
GL_Menu_ParticleStyle

chooses style of particles, none > smooth > pixelated
================
*/
static void GL_Menu_ParticleStyle(int dir)
{
	int i;
	
	for (i = 0; i < particle_styles_max; i++)
	{
		if (i == Q_atoi(Cvar_VariableString("r_particles"))) {
			break;
		}
	}

	if (i == particle_styles_max)
	{
		i = 0;
	}
	else
	{
		i += dir;
		if (i >= particle_styles_max)
			i = 0;
		else if (i < 0)
			i = particle_styles_max - 1;
	}

	Cvar_SetValue("r_particles", (float)i);
}

//==========================================================================
//
//  OPENGL GRAPHICS OPTIONS MENU
//
//==========================================================================

enum {
  GL_OPT_FILTER,
  GL_OPT_ANISOTROPY,
  GL_OPT_PARTICLES,
  GL_OPTIONS_ITEMS
};

static int gl_options_cursor = 0;

void GL_MenuKey(int key) {
	switch (key) {
	case K_ESCAPE:
	case K_BBUTTON:
		M_Menu_Options_f();
		break;

	case K_UPARROW:
		S_LocalSound("misc/menu1.wav");
		gl_options_cursor--;
		if (gl_options_cursor < 0)
			gl_options_cursor = GL_OPTIONS_ITEMS - 1;
		break;

	case K_DOWNARROW:
		S_LocalSound("misc/menu1.wav");
		gl_options_cursor++;
		if (gl_options_cursor >= GL_OPTIONS_ITEMS)
			gl_options_cursor = 0;
		break;

	case K_LEFTARROW:
		S_LocalSound("misc/menu3.wav");
		switch (gl_options_cursor) {
		case GL_OPT_FILTER:
			GL_Menu_ChooseNextFilter(-1);
			break;
		case GL_OPT_ANISOTROPY:
			GL_Menu_Anisotropy(-1);
			break;
		case GL_OPT_PARTICLES:
			GL_Menu_ParticleStyle(-1);
			break;
		default:
			break;
		}
		break;

	case K_RIGHTARROW:
		S_LocalSound("misc/menu3.wav");
		switch (gl_options_cursor) {
		case GL_OPT_FILTER:
			GL_Menu_ChooseNextFilter(1);
			break;
		case GL_OPT_ANISOTROPY:
			GL_Menu_Anisotropy(1);
			break;
		case GL_OPT_PARTICLES:
			GL_Menu_ParticleStyle(1);
			break;
		default:
			break;
		}
		break;

	case K_ENTER:
	case K_KP_ENTER:
	case K_ABUTTON:
		m_entersound = true;
		switch (gl_options_cursor) {
		case GL_OPT_FILTER:
			GL_Menu_ChooseNextFilter(1);
			break;
		case GL_OPT_ANISOTROPY:
			GL_Menu_Anisotropy(1);
			break;
		case GL_OPT_PARTICLES:
			GL_Menu_ParticleStyle(1);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void GL_MenuDraw(void) {
	int i, y;
	float r;
	qpic_t *p;
	const char *title;

	y = 4;

	// plaque
	p = Draw_CachePic("gfx/qplaque.lmp");
	M_DrawTransPic(16, y, p);

	p = Draw_CachePic("gfx/p_option.lmp");
	M_DrawPic((320 - p->width) / 2, y, p);

	y += 28;

	title = "Graphics Options";
	M_PrintWhite((320 - 8 * strlen(title)) / 2, y, title);

	y += 16;

	for (i = 0; i < GL_OPTIONS_ITEMS; i++) {
		switch (i) {
		case GL_OPT_FILTER:
			M_Print(16, y, "     Filter mode");
			M_Print(184, y, ("%s", Cvar_VariableString("gl_texturemode")));
			break;
		case GL_OPT_ANISOTROPY:
			M_Print(16, y, "      Anisotropy");
			r = (float)Q_atof(Cvar_VariableString("gl_texture_anisotropy"));
			M_DrawSlider(184, y, r);
			// Not too clear on sliders yet
			M_Print(280, y, ("%d", Cvar_VariableString("gl_texture_anisotropy")));
			break;
		case GL_OPT_PARTICLES:
			M_Print(16, y, "       Particles");
			int v = Q_atoi(Cvar_VariableString("r_particles"));
			M_Print(184, y, ("%s", particle_styles[v]));
			break;
		}

		if (gl_options_cursor == i)
			M_DrawCharacter(168, y, 12 + ((int)(realtime * 4) & 1));
		
		y += 8;
	}
}

void GL_Menu_f(void) {
	IN_Deactivate(modestate == MS_WINDOWED);
	key_dest = key_menu;
	m_state = m_gl_options;
	m_entersound = true;
}