/* See LICENSE file for copyright and license details. */

/* appearance */
#define ICONSIZE 16
#define USE_ARCS
// #define WARP_ON_RESIZE
static const unsigned int gappx     = 4;
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 8;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono-Regular:size=8", "SymbolsNerdFontMono-Regular:size=6" };

static const char *colors[][3]      = {
	/*               fg      bg      border   */
	[SchemeNorm] = { "#FFFFFF", "#202020", "#202020" },
	[SchemeSel]  = { "#202020", "#40E0D0", "#40E0D0" },
	[SchemeInv]  = { "#202020", "#FFFFFF", NULL },
};

/* tagging */
static const char *tags[] = { "!1", "@2", "#3", "$4", "%5", "^6", "&7", "*8", "(9" };

#include "shift-tools.c"
#include "layouts.c"

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "float", NULL, NULL, 0, 1, -1 }
};

/* layout(s) */
static const float mfact = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1; /* number of clients in master area */
static const int resizehints = 0; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "T",        tile },    /* first entry is default */
	{ "F",        NULL },    /* no layout function means floating behavior */
	{ "M",        monocle },
	{ "S",        spiral },
};

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, topbar ? NULL : "-b", NULL };

#define MODKEY Mod4Mask
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_Delete, quit,           {0} },
	{ MODKEY,                       XK_x,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v=(const char *[]){"terminal", NULL}}},
	{ MODKEY,                       XK_c,      spawn,          {.v=(const char *[]){"lock", NULL}}},
	{ MODKEY,                       XK_z,      spawn,          {.v=(const char *[]){"smenu", NULL}}},
	{ MODKEY,                       XK_Delete, spawn,          {.v=(const char *[]){"dconfirm", "poweroff", "systemctl poweroff", NULL}}},
	{ MODKEY,                       XK_BackSpace, spawn,       {.v=(const char *[]){"dconfirm", "reboot", "systemctl reboot", NULL}}},
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v=(const char *[]){"screenshot", NULL}}},

	{ MODKEY,                       XK_space,  togglebar,      {0} },
	{ MODKEY|ControlMask,           XK_space,  toggletopbar,   {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_q,      shiftview,      { -1 } },
	{ MODKEY,                       XK_e,      shiftview,      { +1 } },

	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[3]} },
#define TAG(k,t) \
	{ MODKEY,                       k,         view,           {.ui = 1 << t} }, \
	{ MODKEY|ControlMask,           k,         toggleview,     {.ui = 1 << t} }, \
	{ MODKEY|ShiftMask,             k,         tag,            {.ui = 1 << t} }, \
	{ MODKEY|ControlMask|ShiftMask, k,         toggletag,      {.ui = 1 << t} },
	TAG(XK_1,0)TAG(XK_2,1)TAG(XK_3,2)TAG(XK_4,3)TAG(XK_5,4)TAG(XK_6,5)TAG(XK_7,6)TAG(XK_8,7)TAG(XK_9,8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[0]} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

