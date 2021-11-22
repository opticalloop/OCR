#include <gtk/gtk.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkBuilder *builder;
gchar *filename;
GtkWidget *window = NULL;
GtkStack *stack;
GtkStack *stack_2;

char *get_filename_ext(const char *filename)
{
    // get the filename extension
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}
void on_file_set(GtkFileChooserButton *file_chooser, gpointer data)
{
    // select filename and update image
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    char *ext = get_filename_ext(filename);
    GtkButton *button = data;
    if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0
        || strcmp(ext, "jpeg") == 0)
    { // if image file is selected load image

        gtk_stack_set_visible_child_name(stack_2, "page2");

        GtkImage *image = GTK_IMAGE(gtk_builder_get_object(builder, "sudoku"));
        gtk_image_set_from_file(GTK_IMAGE(image), filename);

        gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(button),
                                 FALSE); // if not image file disable button
        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(file_chooser),
                                      NULL); // reset filename

        // display error message
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "File is not an image");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog
    }
}

void show_page(GtkWidget *widget, gpointer data)
{
    GtkWidget *page = data;
    gtk_stack_set_visible_child(stack, page);
}
void change_panel(GtkWidget *widget, gpointer data)
{
    GtkWidget *page = data;
    gtk_stack_set_visible_child(stack_2, page);
}

void run_process(GtkButton *button, gpointer data)
{
    // run OCR process
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_OTHER,
        GTK_BUTTONS_CLOSE, "Processing...");
    gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
    gtk_widget_destroy(dialog); // destroy dialog
}

void open_website()
{
    system("firefox www.google.com");
}

int main(int argc, char *argv[])
{
    builder = NULL;
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

    gtk_builder_connect_signals(builder, NULL); // connect signals

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
    GtkButton *button_start =
        GTK_WIDGET(gtk_builder_get_object(builder, "start"));
    gtk_widget_set_sensitive(GTK_WIDGET(button_start), FALSE); // disable button

    stack = GTK_STACK(gtk_builder_get_object(builder, "window_pages"));
    stack_2 = GTK_STACK(gtk_builder_get_object(builder, "right_panel"));

    // load UI
    gtk_widget_show_all(window); // show window
    gtk_main(); // start main loop

    return EXIT_SUCCESS;
}