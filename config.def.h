/*
   The default configuration file for DWM.
   Patches from suckless.org will be applied to this file.
   The Makefile copies the contents of this file to config.h when building.
   config.h is removed with make clean, so edits should be applied to config.def.h.
*/

/*** Appearance ***/

static const unsigned int borderpx  = 5;  /* Pixel thickness of window borders */
static const unsigned int snap      = 30; /* Pixel spacing for floating windows to snap */
static const int showbar            = 1;  /* 0 = don't show bar, 1 = show bar */
static const int topbar             = 1;  /* 0 = bar on bottom of screen, 1 = bar on top of screen */
static const int vertpad            = 5;  /* Vertical padding of bar */
static const int sidepad            = 5;  /* Horizontal padding of bar */

/*
   Fonts for bar.
   Additional fonts are used as fallbacks.
   Check man fonts-conf(1) to see attributes.
*/
static const char *fonts[] = {
   "Terminus:pixelsize=32:style=bold:antialias=true:autohint=true",
   "NotoColorEmoji:pixelsize=32:antialias=true:autohint=true"
};

/* Color pallet */
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#BBBBBB";
static const char col_gray4[]       = "#EEEEEE";
static const char col_cyan[]        = "#005577";
static const char col_purple[]      = "#9544FF";

/* Color schemes */
static const char *colors[][4]      = {
/*                  FG         BG          Border      Float     */
	[SchemeNorm] = { col_gray3, col_gray1,  col_gray2,  col_gray2 },
	[SchemeSel]  = { col_gray4, col_purple, col_purple, col_cyan  },
};

/* Tag symbols */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/*
   From man pages for xprop(1):
	WM_CLASS(STRING) = Instance, Class
	WM_NAME(STRING) = Title
*/
static const Rule rules[] = {
	/* Class      Instance    Title       Tags Mask     IsFloating   Monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           -1      },
	{ "Firefox",  NULL,       NULL,       (1 << 8),     0,           -1      },
};

/*** Layout(s) ***/

static const float mfact        = 0.5f; /* Default master:slave vertical split ratio */
static const int nmaster        = 1;    /* Default number of windows in master area */
static const int resizehints    = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;    /* 1 will force focus on the fullscreen window */

/* NOTE: The first entry is the default layout */
static const Layout layouts[] = {
	/* Symbol     Layout Function (NULL = floating behavior) */
	{ "[]=",      tile },
	{ "><>",      NULL },
	{ "[M]",      monocle },
   { "TTT",      bstack },
   { "===",      bstackhoriz },
   { "|M|",      centeredmaster },
   { ">M>",      centeredfloatingmaster },
   { "|||",      col },
};

/*** Extra key definitions ***/

#define MODKEY Mod4Mask /* Mod4Mask = Super key */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                           KEY,      view,           { .ui = (1 << TAG) }}, \
	{ MODKEY|ControlMask,               KEY,      toggleview,     { .ui = (1 << TAG) }}, \
	{ MODKEY|ShiftMask,                 KEY,      tag,            { .ui = (1 << TAG) }}, \
	{ MODKEY|ControlMask|ShiftMask,     KEY,      toggletag,      { .ui = (1 << TAG) }},

/*** Commands ***/

/*
   The initial string in the list is the command to run.
   All subsequent strings are a comma-separated list of
   options that can be applied to the initial command.
*/
static const char *dmenucmd[]   = { "dmenu_run", "-fn", "Terminus:pixelsize=32:style=bold:antialias=true:autohint=true", "-nb", col_gray1, "-nf", col_gray3, "-sb", col_purple, "-sf", col_gray4, NULL };
static const char *termcmd[]    = { "st", NULL };
static const char *firefoxcmd[] = { "firefox", NULL };

/*
   .v = Vector (array/string)
   .i = Integer
   .ui = Unsigned Integer
   .f = Float
*/
static const Key keys[] = {
	/* Modifier                     Key             Function          Argument */
   { MODKEY,                       XK_w,           spawn,            { .v = firefoxcmd }},   /* Run firefox */
	{ MODKEY,                       XK_d,           spawn,            { .v = dmenucmd }},     /* Run dmenu */
	{ MODKEY,                       XK_Return,      spawn,            { .v = termcmd }},      /* Spawn new ST instance */
	{ MODKEY,                       XK_b,           togglebar,        { 0 }},                 /* Toggle bar */
	{ MODKEY,                       XK_j,           focusstack,       { .i = +1 }},           /* Cycle to next window in the stack */
	{ MODKEY,                       XK_k,           focusstack,       { .i = -1 }},           /* Cycle to the previous window in the stack */
	{ MODKEY,                       XK_a,           incnmaster,       { .i = +1 }},           /* Add another master window */
	{ MODKEY,                       XK_s,           incnmaster,       { .i = -1 }},           /* Subtract/demote most recent master to a slave */
	{ MODKEY,                       XK_h,           setmfact,         { .f = -0.05f }},       /* Move vertical split left */
	{ MODKEY,                       XK_l,           setmfact,         { .f = +0.05f }},       /* Move vertical split right */
	{ MODKEY,                       XK_space,       zoom,             { 0 }},                 /* Swap active window with master */
	{ MODKEY,                       XK_Tab,         view,             { 0 }},                 /* Cycle between current tag and previously selected tag */
	{ MODKEY,                       XK_q,           killclient,       { 0 }},                 /* Kill active window */
   /* Layouts */
	{ MODKEY,                       XK_t,           setlayout,        { .v = &layouts[0] }},  /* Tiling */
	{ MODKEY,                       XK_f,           setlayout,        { .v = &layouts[1] }},  /* Floating */
	{ MODKEY,                       XK_m,           setlayout,        { .v = &layouts[2] }},  /* Monicle */
   { MODKEY,                       XK_u,           setlayout,        { .v = &layouts[3] }},  /* Bottom stack (vertical) */
   { MODKEY,                       XK_i,           setlayout,        { .v = &layouts[4] }},  /* Bottom stack (horizontal) */
   { MODKEY,                       XK_o,           setlayout,        { .v = &layouts[5] }},  /* Centered master */
   { MODKEY,                       XK_p,           setlayout,        { .v = &layouts[6] }},  /* Centered floating master */
   { MODKEY,                       XK_c,           setlayout,        { .v = &layouts[7] }},  /* Columns */
   /* Layouts end */
	{ MODKEY|ShiftMask,             XK_f,           togglefloating,   { 0 }},                 /* Snap a floating window back to its position according to the current layout */
	{ MODKEY,                       XK_0,           view,             { .ui = ~0 }},          /* Display all tags at once */
	{ MODKEY|ShiftMask,             XK_0,           tag,              { .ui = ~0 }},          /* Duplicate active window handle to all tags */
	{ MODKEY,                       XK_comma,       focusmon,         { .i = -1 }},           /* Switch active monitor to the left */
	{ MODKEY,                       XK_period,      focusmon,         { .i = +1 }},           /* Switch active monitor to the right */
	{ MODKEY|ShiftMask,             XK_comma,       tagmon,           { .i = -1 }},
	{ MODKEY|ShiftMask,             XK_period,      tagmon,           { .i = +1 }},
   /* Switch between tags */
	  TAGKEYS(                      XK_1,                             0)
	  TAGKEYS(                      XK_2,                             1)
	  TAGKEYS(                      XK_3,                             2)
	  TAGKEYS(                      XK_4,                             3)
	  TAGKEYS(                      XK_5,                             4)
	  TAGKEYS(                      XK_6,                             5)
	  TAGKEYS(                      XK_7,                             6)
	  TAGKEYS(                      XK_8,                             7)
	  TAGKEYS(                      XK_9,                             8)
	{ MODKEY|ShiftMask,             XK_q,           quit,             { 0 }},                   /* Kill DWM */
};

/*** Button events ***/

/*
   Click options:

   ClkTagBar      = Click on tags
   ClkLtSymbol    = Click on the layout symbol
   ClkStatusText  =
   ClkWinTitle    = Click on the window's title
   ClkClientWin   = Click on a slave window
   ClkRootWin     = Click on the master window

   Button options:

   Button 1 = Left click
   Button 2 = Scroll wheel
   Button 3 = Right click
*/
static const Button buttons[] = {
	/* Click                Event Mask      Button          Function        Argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      { 0 }},
	{ ClkLtSymbol,          0,              Button3,        setlayout,      { .v = &layouts[2] }},
	{ ClkWinTitle,          0,              Button2,        zoom,           { 0 }},
	{ ClkStatusText,        0,              Button2,        spawn,          { .v = termcmd }},
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      { 0 }},
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, { 0 }},
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    { 0 }},
	{ ClkTagBar,            0,              Button1,        view,           { 0 }},
	{ ClkTagBar,            0,              Button3,        toggleview,     { 0 }},
	{ ClkTagBar,            MODKEY,         Button1,        tag,            { 0 }},
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      { 0 }},
};

