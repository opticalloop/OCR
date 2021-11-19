#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
// #include "GUI/load_image.h"

GtkWidget *image;
GtkFileChooserButton *file_chooser;
gchar *filename;
GtkWidget *window = NULL;

char *get_filename_ext(const char *filename)
{
    // get the filename extension
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}
void on_file_set(GtkFileChooserButton *file_chooser)
{
    // select filename and update image
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    char *ext = get_filename_ext(filename);

    if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0
        || strcmp(ext, "jpeg") == 0)
    { // if image file is selected load image
        gtk_image_set_from_file(GTK_IMAGE(image), filename);
    }
    else
    {
        // display error message
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "File is not an image");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog
    }
}

int main(int argc, char *argv[])
{
    GtkBuilder *builder = NULL;
    GError *error = NULL;
    gchar *filename = NULL;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new(); // initialize the builder
    filename = g_build_filename("main.glade", NULL); // build filename

    if (!gtk_builder_add_from_file(builder, filename, &error)) // load file
    {
        g_warning("%s", error->message);
        g_free(filename);
        g_error_free(error);
        return EXIT_FAILURE;
    }
    g_free(filename); // free filename
    window =
        GTK_WIDGET(gtk_builder_get_object(builder, "window")); // get window

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit),
                     NULL); // destroy window when clicked

    // Set title
    gtk_window_set_title(GTK_WINDOW(window), "opticalloop");

    // Load icon
    GdkPixbuf *icon =
        gdk_pixbuf_new_from_file_at_size("./logo1.png", 128, 128, NULL);
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    // Load CSS
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "./style.css", NULL);

    // Inject CSS
    GdkScreen *screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen,
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    // load widgets
    image = GTK_WIDGET(gtk_builder_get_object(builder, "sudoku_image"));
    file_chooser = GTK_WIDGET(gtk_builder_get_object(builder, "file_chooser"));

    // link signals to widgets
    g_signal_connect(file_chooser, "file-set", G_CALLBACK(on_file_set),
                     NULL); // choose file
    // load UI
    gtk_widget_show_all(window); // show window
    gtk_main(); // start main loop

    return EXIT_SUCCESS;
}