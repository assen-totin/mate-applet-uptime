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
 *  Lock Keys (formerly Led) Applet was writen by Jörgen Scheibengruber <mfcn@gmx.de>
 *  Modifed for MATE by Assen Totin <assen.totin@gmail.com>
 *
 */

#include <mate-panel-applet.h>
#include <libintl.h>
#include <string.h>

#include "../config.h"
#include "applet.h"

#define _(String) gettext (String)


/* Free the memory of the applet struct
 * Save the current state of the applet
 */
static void applet_destroy(MatePanelApplet *applet_widget, UptimeApplet *applet) {
	g_assert(applet);
	g_free(applet);
	return;
}

/* Just a boring about box
 */

static void quitDialogOK( GtkWidget *widget, gpointer data ){
        GtkWidget *quitDialog = data;
        gtk_widget_destroy(quitDialog);
}


static void about_cb (GtkAction *action, UptimeApplet *applet) {
	char msg1[1024];

	sprintf(&msg1[0], "%s\n\n%s\n\n%s", _("Uptime Applet"), _("An applet that shows the system uptime"), ("Assen Totin <assen.totin@gmail.com>"));

        GtkWidget *label = gtk_label_new (&msg1[0]);

        GtkWidget *quitDialog = gtk_dialog_new_with_buttons (_("Keyboard Lock Keys"), GTK_WINDOW(applet), GTK_DIALOG_MODAL, NULL);
        GtkWidget *buttonOK = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_OK, GTK_RESPONSE_OK);

        gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG(quitDialog)->vbox), label);
        g_signal_connect (G_OBJECT(buttonOK), "clicked", G_CALLBACK (quitDialogOK), (gpointer) quitDialog);

        gtk_widget_show_all (GTK_WIDGET(quitDialog));
}


static void applet_back_change (MatePanelApplet *a, MatePanelAppletBackgroundType type, GdkColor *color, GdkPixmap *pixmap, UptimeApplet *applet) {
        /* taken from the TrashApplet */
        GtkRcStyle *rc_style;
        GtkStyle *style;

        /* reset style */
        gtk_widget_set_style (GTK_WIDGET (applet->applet), NULL);
        rc_style = gtk_rc_style_new ();
        gtk_widget_modify_style (GTK_WIDGET (applet->applet), rc_style);
        g_object_unref (rc_style);

        switch (type) {
                case PANEL_COLOR_BACKGROUND:
                        gtk_widget_modify_bg (GTK_WIDGET (applet->applet), GTK_STATE_NORMAL, color);
                        break;

                case PANEL_PIXMAP_BACKGROUND:
                        style = gtk_style_copy (gtk_widget_get_style (GTK_WIDGET (applet->applet)));
                        if (style->bg_pixmap[GTK_STATE_NORMAL])
                                g_object_unref (style->bg_pixmap[GTK_STATE_NORMAL]);
                        style->bg_pixmap[GTK_STATE_NORMAL] = g_object_ref(pixmap);
                        gtk_widget_set_style (GTK_WIDGET (applet->applet), style);
                        g_object_unref (style);
                        break;

                case PANEL_NO_BACKGROUND:
                default:
                        break;
        }

}


/*
 * Get the uptime
 */

void get_uptime(UptimeApplet *applet) {
	FILE *fp = fopen(APPLET_PROC_UPTIME, "r");
	if (!fp)
		return "0:0";
	char tmp1[32];
	fgets(&tmp1[0], sizeof(tmp1), fp);
	fclose(fp);
	
	char *tmp2 = strtok(&tmp1[0], " ");
	char *tmp3 = strtok(tmp2, ".");
	applet->uptime = atoi(tmp3);
}


/* 
 * Format uptime
 */
void show_uptime(UptimeApplet *applet, char *s) {
	int d = applet->uptime / 86400;
	int h = (applet->uptime - d * 86400) / 3600;
	int m = (applet->uptime - (h * 3600)) / 60;
	if (d > 0)
		sprintf(s, "%uD%uH", d, h);
	else
		sprintf(s, "%uH%uM", h, m);
}

/*
 * Update the applet
 */
void applet_check_uptime(UptimeApplet *applet) {
	get_uptime(applet);
	show_uptime(applet);
}

/* The "main" function
 */
static gboolean uptime_applet_factory(MatePanelApplet *applet_widget, const gchar *iid, gpointer data) {
	GdkDrawable *drawable;
	char *buf;
	UptimeApplet *applet;
	GdkPixbuf *icon;
	GList *iconlist = NULL;

	if (strcmp (iid, APPLET_ID) != 0) 
		return FALSE;

	// i18n
        setlocale (LC_ALL, "");
        bindtextdomain (PACKAGE_NAME, LOCALEDIR);
        bind_textdomain_codeset(PACKAGE_NAME, "utf-8");
        textdomain (PACKAGE_NAME);

	/* Set an icon for all windows */
/*
	char image_file[1024];
        sprintf(&image_file[0], "%s/%s", APPLET_ICON_PATH, APPLET_ICON);
	icon = gdk_pixbuf_new_from_file(&image_file[0], NULL);
	iconlist = g_list_append(NULL, (gpointer)icon);
	gtk_window_set_default_icon_list(iconlist);
	g_object_unref(icon);
*/	
	applet = g_malloc0(sizeof(UptimeApplet));
	applet->applet = applet_widget;
	
	applet->vbox = gtk_vbox_new(FALSE, 0);

	applet->label_top = gtk_label_new (_("uptime"));

	char msg_bottom[32];
	get_uptime(applet);
	format_uptime(applet, &msg_bottom[0]);
	applet->label_bottom = gtk_label_new (&msg_bottom[0]);

	gtk_box_pack_start(GTK_BOX(applet->vbox), applet->label_top, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(applet->vbox), applet->label_bottom, FALSE, FALSE, 0);
	
	gtk_container_add(GTK_CONTAINER(applet_widget), applet->vbox);

        GtkActionGroup *action_group = gtk_action_group_new ("Lockkeys Applet Actions");
        gtk_action_group_set_translation_domain (action_group, GETTEXT_PACKAGE);
        gtk_action_group_add_actions (action_group, applet_menu_actions, G_N_ELEMENTS (applet_menu_actions), applet);
	//mate_panel_applet_setup_menu_from_file(applet->applet, "/usr/share/mate-2.0/ui/lockkeys-applet-menu.xml", action_group);

	gtk_widget_show_all(GTK_WIDGET(applet_widget));

	g_signal_connect(G_OBJECT(applet_widget), "destroy", G_CALLBACK(applet_destroy), (gpointer)applet);
	g_signal_connect(G_OBJECT(applet_widget), "change_background", G_CALLBACK (applet_back_change), (gpointer)applet);

	g_timeout_add(60000, (GtkFunction) applet_check_uptime, (gpointer)applet);

	return TRUE;
}

MATE_PANEL_APPLET_OUT_PROCESS_FACTORY (APPLET_FACTORY, PANEL_TYPE_APPLET, APPLET_NAME, uptime_applet_factory, NULL)



