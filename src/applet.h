/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 *  USA.
 *
 *  Uptime Applet was writen by Assen Totin <assen.totin@gmail.com>
 *  
 */

#define APPLET_FACTORY "UptimeAppletFactory"
#define APPLET_ID "UptimeApplet"
#define APPLET_NAME "uptime"
#define APPLET_ICON "applet_uptime.png"
#define APPLET_VERSION "1"

#define APPLET_PROC_UPTIME "/proc/uptime"

typedef struct {
        MatePanelApplet *applet;
        GtkWidget *vbox, *label_top, label_bottom;
	int uptime;
        GtkDialog *about, *settings;
} UptimeApplet;

static const GtkActionEntry applet_menu_actions [] = {
	{ "About", GTK_STOCK_ABOUT, NULL, "_About", NULL, G_CALLBACK (about_cb) }
};

