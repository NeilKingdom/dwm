#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <errno.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <xcb/res.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
#include <X11/Xlib-xcb.h>

#ifdef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif /* XINERAMA */

#ifdef __OpenBSD__
#include <sys/sysctl.h>
#include <kvm.h>
#endif /* __OpenBSD__ */

#endif /* COMMON_H */
