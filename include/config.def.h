#include "mpdcontrol.h"

#define SESSION_FILE "/tmp/dwm-session" /* Used to save DWM session after reboot */

/*
   The default configuration file for DWM.
   Patches from suckless.org will be applied to this file.
   The Makefile copies the contents of this file to config.h when building.
   config.h is removed with make clean, so edits should be applied to config.def.h.
*/

/*** Appearance ***/

static const unsigned int borderpx = 7;  /* Pixel thickness of window borders */
static const unsigned int gappx    = 5;  /* Gaps between windows */
static const unsigned int snap     = 30; /* Pixel spacing for floating windows to snap */
static const unsigned int minwsz   = 20; /* Minimum height of a client for smfact */
static const int swallowfloating   = 0;  /* 1 means swallow floating windows by default */
static const int nmaxmaster        = 3;  /* Maximum number of windows allowed in master area */
static const int showbar           = 1;  /* 0 = don't show bar, 1 = show bar */
static const int topbar            = 1;  /* 0 = bar on bottom of screen, 1 = bar on top of screen */
static const int vertpad           = 5;  /* Vertical padding of bar */
static const int sidepad           = 5;  /* Horizontal padding of bar */

/*
   Fonts for bar (these are overriden by dmenu).
   Additional fonts are used as fallbacks.
   Check man fonts-conf(1) to see attributes.
   See my ST config.def.h for more info on the
   specific fonts that were chosen.
*/
static const char *fonts[2] = {
   "Terminus:style=Bold:size=14:antialias=true",
   "Noto Color Emoji:style=Regular:pixelsize=14"
};

/* The font used for the dmenu cmd shortcut later in this config */
static const char dmenucmd_font[] = "Terminus:style=Bold:size=14:antialias=true";

/* Color pallet (Gruvbox colors) */
static const char col_black1[]   = "#1D2021";
static const char col_red1[]     = "#CC241D";
static const char col_yellow1[]  = "#98971A";
static const char col_green1[]   = "#D79921";
static const char col_blue1[]    = "#458588";
static const char col_cyan1[]    = "#B16286";
static const char col_violet1[]  = "#689D6A";
static const char col_white1[]   = "#A89984";
static const char col_black2[]   = "#928374";
static const char col_red2[]     = "#FB4934";
static const char col_yellow2[]  = "#B8BB26";
static const char col_green2[]   = "#FABD2F";
static const char col_blue2[]    = "#83A598";
static const char col_cyan2[]    = "#D3869B";
static const char col_violet2[]  = "#8EC07C";
static const char col_white2[]   = "#EBDBB2";

/* Color schemes */

/*
   NOTE: FG and BG refer to the foreground and background colors for
   the bar respectively.
*/
static const char *colors[][4] = {

/*                   FG          BG          Border      Border (Floating) */
	[SchemeNorm] = { col_white2, col_black1, col_black1, col_violet2 },   /* When not focused */
	[SchemeSel]  = { col_white1, col_black1, col_blue1,  col_violet2 },   /* When focused */
};

/* Tag symbols */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tagsalt[] = { "🌐", "📱", "📨", "📑", "", "", "", "", "🗑️" };
static const int momentaryalttags = 0; /* Set to 1 to only show alt tags when key is held down */

/*
   Rules for specific applications

   From man pages for xprop(1):
       WM_CLASS(STRING) = Instance, Class
       WM_NAME(STRING) = Title
*/
static const Rule rules[] = {
    /* Class      Instance    Title           Tags Mask     IsFloating   IsTerminal   NoSwallow   Monitor */
    { "Gimp",     NULL,       NULL,           0,            1,           0,           0,          -1 },
    { "Firefox",  NULL,       NULL,           (1 << 8),     0,           0,           0,          -1 },
    { "st",       NULL,       NULL,           0,            0,           1,           0,          -1 },
    { NULL,       NULL,       "Event Tester", 0,            0,           0,           1,          -1 }, /* xev */
};

/*** Layout(s) ***/

static const float mfact        = 0.5f; /* Default master:slave vertical split ratio */
static const float smfact       = 0.0f; /* Default slave:slave horizontal split ratio (0 is centered) */
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
    { "HHH",      grid },
};

/*** Extra key definitions ***/

#define MODKEY Mod4Mask /* Mod4Mask = Super key */
#define TAGKEYS(KEY, TAG) \
	{ MODKEY,                             KEY,      view,           { .ui = (1 << TAG) }}, \
	{ MODKEY | ControlMask,               KEY,      toggleview,     { .ui = (1 << TAG) }}, \
	{ MODKEY | ShiftMask,                 KEY,      tag,            { .ui = (1 << TAG) }}, \
	{ MODKEY | ControlMask|ShiftMask,     KEY,      toggletag,      { .ui = (1 << TAG) }},

/*** Commands ***/

/*
   The initial string in the list is the command to run.
   All subsequent strings are a comma-separated list of
   options that can be applied to the initial command
   (list must be NULL-terminated).
*/
static char dmenumon[2] = "0"; /* Used in dwm.c and dmenucmd below to select which monitor dmenu should appear on */
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon, "-fn", dmenucmd_font, "-nb", col_black1, "-nf", col_white2, "-sb", col_black1, "-sf", col_white1, NULL };
static const char *termcmd[]    = { "st", NULL };
static const char *capture[]    = { "capture", NULL };
static const char *firefoxcmd[] = { "firefox", NULL };

/*
   .v   = Vector (array/string)
   .i   = Integer
   .ui  = Unsigned integer
   .f   = Float
*/
static const Key keys[] = {
    /* Modifier                     Key                Function          Argument */
    { MODKEY,                       XK_d,              spawn,            { .v = dmenucmd }},     /* Run dmenu */
    { MODKEY,                       XK_Return,         spawn,            { .v = termcmd }},      /* Spawn new ST instance */
    { MODKEY|ShiftMask,             XK_s,              spawn,            { .v = capture }},      /* Takes a screen capture */
    { MODKEY,                       XK_w,              spawn,            { .v = firefoxcmd }},   /* Run firefox */
    { MODKEY,                       XK_b,              togglebar,        { 0 }},                 /* Toggle bar */
    { MODKEY,                       XK_j,              focusstack,       { .i = +1 }},           /* Cycle to next window in the stack */
    { MODKEY,                       XK_k,              focusstack,       { .i = -1 }},           /* Cycle to the previous window in the stack */
    { MODKEY,                       XK_a,              incnmaster,       { .i = +1 }},           /* Add another master window */
    { MODKEY,                       XK_s,              incnmaster,       { .i = -1 }},           /* Subtract/demote most recent master to a slave */
    { MODKEY,                       XK_h,              setmfact,         { .f = -0.05f }},       /* Move vertical split left */
    { MODKEY,                       XK_l,              setmfact,         { .f = +0.05f }},       /* Move vertical split right */
    { MODKEY|ShiftMask,             XK_k,              setsmfact,        { .f = +0.05f }},       /* Move horizontal split up */
    { MODKEY|ShiftMask,             XK_j,              setsmfact,        { .f = -0.05f }},       /* Move horizontal split down */
    { MODKEY,                       XK_space,          zoom,             { 0 }},                 /* Swap active window with master */
    { MODKEY,                       XK_Tab,            view,             { 0 }},                 /* Cycle between current tag and previously selected tag */
    { MODKEY,                       XK_q,              killclient,       { 0 }},                 /* Kill active window */
    { MODKEY|ShiftMask,             XK_a,              togglealttag,     { 0 }},                 /* Toggle between normal tags and alternative tags */
    { MODKEY|ControlMask,           XK_q,              quit,             { 0 }},                 /* Kill DWM */
    { MODKEY|ShiftMask,             XK_q,              quit,             { 1 }},                 /* Restart DWM */
    /* Layouts */
    { MODKEY,                       XK_t,              setlayout,        { .v = &layouts[0] }},  /* Tiling */
    { MODKEY,                       XK_f,              setlayout,        { .v = &layouts[1] }},  /* Floating */
    { MODKEY,                       XK_m,              setlayout,        { .v = &layouts[2] }},  /* Monicle */
    { MODKEY,                       XK_u,              setlayout,        { .v = &layouts[3] }},  /* Bottom stack (vertical) */
    { MODKEY,                       XK_i,              setlayout,        { .v = &layouts[4] }},  /* Bottom stack (horizontal) */
    { MODKEY,                       XK_o,              setlayout,        { .v = &layouts[5] }},  /* Centered master */
    { MODKEY,                       XK_p,              setlayout,        { .v = &layouts[6] }},  /* Centered floating master */
    { MODKEY,                       XK_c,              setlayout,        { .v = &layouts[7] }},  /* Columns */
    { MODKEY,                       XK_g,              setlayout,        { .v = &layouts[8] }},  /* Grid */
    /* Layouts end */
    { MODKEY|ShiftMask,             XK_f,              togglefloating,   { 0 }},                 /* Snap a floating window back to its position according to the current layout */
    { MODKEY,                       XK_0,              view,             { .ui = ~0 }},          /* Display all tags at once */
    { MODKEY|ShiftMask,             XK_0,              tag,              { .ui = ~0 }},          /* Duplicate active window handle to all tags */
    { MODKEY,                       XK_comma,          focusmon,         { .i = -1 }},           /* Switch active monitor to the left */
    { MODKEY,                       XK_period,         focusmon,         { .i = +1 }},           /* Switch active monitor to the right */
    { MODKEY|ShiftMask,             XK_comma,          tagmon,           { .i = -1 }},
    { MODKEY|ShiftMask,             XK_period,         tagmon,           { .i = +1 }},
    { MODKEY|ShiftMask,             XK_equal,          setgaps,          { .i = -5 }},           /* Decrease gaps */
    { MODKEY,                       XK_equal,          setgaps,          { .i = 0 }},            /* Reset gaps */
    { MODKEY,                       XK_minus,          setgaps,          { .i = +5 }},           /* Increase gaps */
    { MODKEY,                       XK_bracketleft,    mpdchange,        { .i = -1 }},           /* Go to previous song */
    { MODKEY|ShiftMask,             XK_bracketright,   mpdchange,        { .i = +1 }},           /* Go to next song */
    { MODKEY,                       XK_bracketright,   mpdcontrol,       { 0 }},                 /* Stop song */
    /* Switch between tags */
    TAGKEYS(                        XK_1,                                0)
    TAGKEYS(                        XK_2,                                1)
    TAGKEYS(                        XK_3,                                2)
    TAGKEYS(                        XK_4,                                3)
    TAGKEYS(                        XK_5,                                4)
    TAGKEYS(                        XK_6,                                5)
    TAGKEYS(                        XK_7,                                6)
    TAGKEYS(                        XK_8,                                7)
    TAGKEYS(                        XK_9,                                8)
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
   Button 4 = Scroll up
   Button 5 = Scroll down
   Button 6 = Scroll left
   Button 7 = Scroll right
   Button 8 = Back button
   Button 9 = Forwards button
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
