#ifndef DWM_H
#define DWM_H

#include "common.h"
#include "drw.h"
#include "util.h"

/*** Macros ***/

/* Used to save DWM session after reboot */
#define SESSION_FILE            "/tmp/dwm-session"

#define MODKEY                  Mod4Mask /* (Super key) */
#define BUTTONMASK              (ButtonPressMask | ButtonReleaseMask)
#define CLEANMASK(mask)         (mask & ~(numlockmask | LockMask) & \
                                    (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask))
#define INTERSECT(x,y,w,h,m)    (MAX(0, MIN((x)+(w),(m)->wx+(m)->ww) - MAX((x),(m)->wx)) \
                                    * MAX(0, MIN((y)+(h),(m)->wy+(m)->wh) - MAX((y),(m)->wy)))
#define ISVISIBLE(C)            ((C->tags & C->mon->tagset[C->mon->seltags]))
#define LENGTH(X)               (sizeof X / sizeof X[0])
#define MOUSEMASK               (BUTTONMASK|PointerMotionMask)
#define WIDTH(X)                ((X)->w + 2 * (X)->bw)
#define HEIGHT(X)               ((X)->h + 2 * (X)->bw)
#define TAGMASK                 ((1 << LENGTH(tags)) - 1)
#define TEXTW(X)                (drw_fontset_getwidth(drw, (X)) + lrpad)
#define ColFloat                3

/*** Enums ***/

enum DwmCursor {
    CurNormal,
    CurResize,
    CurMove,
    CurLast
};

enum ColorScheme {
    SchemeNorm,
    SchemeSel
};

enum EWMHAtom {
    NetSupported,
    NetWMName,
    NetWMState,
    NetWMCheck,
    NetWMFullscreen,
    NetActiveWindow,
    NetWMWindowType,
    NetWMWindowTypeDialog,
    NetClientList,
    NetLast
};

enum DefaultAtom {
    WMProtocols,
    WMDelete,
    WMState,
    WMTakeFocus,
    WMLast
};

enum Click {
    ClkTagBar,
    ClkLtSymbol,
    ClkStatusText,
    ClkWinTitle,
    ClkClientWin,
    ClkRootWin,
    ClkLast
};

enum LayoutItem {
    Tile = 0,
    Monicle,
    BStack,
    BStackHoriz,
    CenteredM,
    CenteredMFloat,
    Col,
    Grid,
    Floating,
    LayoutSize
};

/*** Structs/Unions ***/

typedef union {
    int i;              /* Integer */
    unsigned ui;        /* Unsigned int */
    float f;            /* Float */
    float sf;           /* "Signed" float */
    const void *v;      /* Vector/array */
} Arg;

typedef struct {
	unsigned int click;
	unsigned int mask;
	unsigned int button;
	void (*func)(const Arg *arg);
	const Arg arg;
} Button;

typedef struct Monitor Monitor;
typedef struct Client Client;

struct Client {
    char name[256];
    float mina, maxa;
    int x, y, w, h;
    int sfx, sfy, sfw, sfh; /* Stored float geometry, used on mode revert */
    int oldx, oldy, oldw, oldh;
    int basew, baseh, incw, inch, maxw, maxh, minw, minh, hintsvalid;
    int bw, oldbw;
    unsigned int tags;
    int isfixed, isfloating, isurgent, neverfocus, oldstate, isfullscreen, isterminal, noswallow;
    pid_t pid;
    int issteam;
    Client *next;
    Client *snext;
    Client *swallowing;
    Monitor *mon;
    Window win;
};

typedef struct {
	unsigned int mod;               /* Modifier */
	KeySym keysym;                  /* Key */
	void (*func)(const Arg *);      /* Callback */
	const Arg arg;                  /* Arguments */
} Key;

typedef struct {
	const char *symbol;             /* String representation of layout */
	void (*arrange)(Monitor *);     /* The associated layout function */
} Layout;

struct Monitor {
    char ltsymbol[16];
    float mfact;
    float smfact;
    int nmaster;
    int num;
    int by;               /* Bar geometry */
    int mx, my, mw, mh;   /* Screen size */
    int wx, wy, ww, wh;   /* Window area  */
    int gappx;            /* Gaps between windows */
    unsigned int seltags;
    unsigned int sellt;
    unsigned int tagset[2];
    int showbar;
    int topbar;
    Client *clients;
    Client *sel;
    Client *stack;
    Monitor *next;
    Window barwin;
    const Layout *lt[2];
    unsigned int alttag;
};

typedef struct {
    const char *class;
    const char *instance;
    const char *title;
    unsigned int tags;
    int isfloating;
    int isterminal;
    int noswallow;
    int monitor;
} Rule;

/*** Function declarations ***/

static void         applyrules(Client *c);
static int          applysizehints(Client *c, int *x, int *y, int *w, int *h, int interact);
static void         arrange(Monitor *m);
static void         arrangemon(Monitor *m);
static void         attach(Client *c);
static void         attachstack(Client *c);
static void         buttonpress(XEvent *e);
static void         checkotherwm(void);
static void         cleanup(void);
static void         cleanupmon(Monitor *mon);
static void         clientmessage(XEvent *e);
static void         configure(Client *c);
static void         configurenotify(XEvent *e);
static void         configurerequest(XEvent *e);
static Monitor     *createmon(void);
static void         destroynotify(XEvent *e);
static void         detach(Client *c);
static void         detachstack(Client *c);
static Monitor     *dirtomon(int dir);
static void         drawbar(Monitor *m);
static void         drawbars(void);
static void         enternotify(XEvent *e);
static void         expose(XEvent *e);
static void         focus(Client *c);
static void         focusin(XEvent *e);
static void         focusmon(const Arg *arg);
static void         focusstack(const Arg *arg);
static Atom         getatomprop(Client *c, Atom prop);
static int          getrootptr(int *x, int *y);
static long         getstate(Window w);
static int          gettextprop(Window w, Atom atom, char *text, unsigned int size);
static void         grabbuttons(Client *c, int focused);
static void         grabkeys(void);
static void         incnmaster(const Arg *arg);
static void         keypress(XEvent *e);
static void         keyrelease(XEvent *e);
static void         killclient(const Arg *arg);
static void         manage(Window w, XWindowAttributes *wa);
static void         mappingnotify(XEvent *e);
static void         maprequest(XEvent *e);
static void         monocle(Monitor *m);
static void         motionnotify(XEvent *e);
static void         movemouse(const Arg *arg);
static Client       *nexttiled(Client *c);
static void         pop(Client *c);
static void         propertynotify(XEvent *e);
static void         quit(const Arg *arg);
static Monitor     *recttomon(int x, int y, int w, int h);
static void         resize(Client *c, int x, int y, int w, int h, int interact);
static void         resizeclient(Client *c, int x, int y, int w, int h);
static void         resizemouse(const Arg *arg);
static void         restack(Monitor *m);
static void         run(void);
static void         scan(void);
static int          sendevent(Client *c, Atom proto);
static void         sendmon(Client *c, Monitor *m);
static void         setclientstate(Client *c, long state);
static void         setfocus(Client *c);
static void         setfullscreen(Client *c, int fullscreen);
static void         setgaps(const Arg *arg);
static void         setlayout(const Arg *arg);
static void         setmfact(const Arg *arg);
static void         setsmfact(const Arg *arg);
static void         setup(void);
static void         seturgent(Client *c, int urg);
static void         showhide(Client *c);
static void         sigchld(int unused);
static void         sighup(int unused);
static void         sigterm(int unused);
static void         spawn(const Arg *arg);
static void         tag(const Arg *arg);
static void         tagmon(const Arg *arg);
static void         tile(Monitor *m);
static void         togglealttag(const Arg *arg);
static void         togglebar(const Arg *arg);
static void         togglefloating(const Arg *arg);
static void         toggletag(const Arg *arg);
static void         toggleview(const Arg *arg);
static void         unfocus(Client *c, int setfocus);
static void         unmanage(Client *c, int destroyed);
static void         unmapnotify(XEvent *e);
static void         updatebarpos(Monitor *m);
static void         updatebars(void);
static void         updateclientlist(void);
static int          updategeom(void);
static void         updatenumlockmask(void);
static void         updatesizehints(Client *c);
static void         updatestatus(void);
static void         updatetitle(Client *c);
static void         updatewindowtype(Client *c);
static void         updatewmhints(Client *c);
static void         view(const Arg *arg);
static Client      *wintoclient(Window w);
static Monitor     *wintomon(Window w);
static int          xerror(Display *dpy, XErrorEvent *ee);
static int          xerrordummy(Display *dpy, XErrorEvent *ee);
static int          xerrorstart(Display *dpy, XErrorEvent *ee);
static void         zoom(const Arg *arg);
static pid_t        getparentprocess(pid_t p);
static int          isdescprocess(pid_t p, pid_t c);
static Client      *swallowingclient(Window w);
static Client      *termforwin(const Client *c);
static pid_t        winpid(Window w);
static void         bstack(Monitor *m);
static void         bstackhoriz(Monitor *m);
static void         centeredmaster(Monitor *m);
static void         centeredfloatingmaster(Monitor *m);
static void         col(Monitor *m);
static void         grid(Monitor *m);
static void         saveSession(void);

#endif /* DWM_H */
