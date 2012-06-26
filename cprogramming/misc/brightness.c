#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

int GetIdleTime () {
	time_t idle_time;
	static XScreenSaverInfo *mit_info;
	Display *display;
	int screen;
	mit_info = XScreenSaverAllocInfo();
	if((display=XOpenDisplay(NULL)) == NULL) { return(-1); }
	screen = DefaultScreen(display);
	XScreenSaverQueryInfo(display, RootWindow(display,screen), mit_info);
	idle_time = (mit_info->idle) / 1000;
	XFree(mit_info);
	XCloseDisplay(display); 
	return idle_time;
}

int main() {
	time_t idle_time;
	while(1) {
		idle_time = GetIdleTime();
		if(idle_time > 5) {
			system("echo 25 > /sys/class/backlight/backlight/brightness");
		} else {
			system("echo 100 > /sys/class/backlight/backlight/brightness");
		}
	}
	return 0;
}
