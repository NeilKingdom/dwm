#ifndef MPDCONTROL_H
#define MPDCONTROL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <mpd/client.h>

#include "dwm.h"

#define MPDHOST "localhost"
#define MPDPORT 6600

struct mpd_connection *get_conn();
void mpdchange(const Arg *direction);
char *get_regerror(int errcode, regex_t *compiled);
void mpdcontrol();

#endif /* MPDCONTROL_H */
