#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "GUI/load_image.h"
int main(int argc, char *argv[])
{
    GtkWidget *window = NULL;
    GtkBuilder *builder = NULL;
    GError *error = NULL;
    gchar  *filename = NULL;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    filename = g_build_filename("main.glade", NULL);

    if (!gtk_builder_add_from_file(builder, filename, &error))
    {
        g_warning("%s", error->message);
        g_free(filename);
        g_error_free(error);
        return EXIT_FAILURE;
    }
    g_free(filename);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();


    return EXIT_SUCCESS;

}