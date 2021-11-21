
#include "GUI/gui.h"

GtkBuilder *builder;
gchar *filename;
GtkWidget *window = NULL;
GtkStack *stack;
GtkStack *stack_2;
pthread_t * thread;

char *get_filename_ext(const char *filename)
{
    // get the filename extension
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

void change_image(char * filename)
{
    GtkImage *imageWidget = GTK_IMAGE(gtk_builder_get_object(builder, "selected_image")); // get image
    GdkPixbuf *image = gdk_pixbuf_new_from_file(filename, NULL); // load image
    GdkPixbuf *resized_image = gdk_pixbuf_scale_simple(image, 500, 500, GDK_INTERP_BILINEAR); // resize image
    gtk_image_set_from_pixbuf(imageWidget, resized_image); // set image
    g_object_unref(image); // free image
    g_object_unref(resized_image); // free resized image
}

void edit_progress_bar(float progress, char * text)
{
    GtkProgressBar *progress_bar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar"));
    gtk_progress_bar_set_fraction(progress_bar, progress);
    gtk_progress_bar_set_text(progress_bar, text);
}

void on_file_set(GtkFileChooserButton *file_chooser, gpointer data)
{
    // select filename and update image
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    char *ext = get_filename_ext(filename);
    GtkButton *button = data;
    if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0
        || strcmp(ext, "jpeg") == 0 || strcmp(ext, "bmp") == 0)  // if image file is selected load image
    { 
       
        change_image(filename);

        // update label
        GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, "picture_path"));
        gtk_label_set_text(label, filename);
        

        gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
        gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE); // enable button to start processing
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE); // if not image file disable button
        gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(file_chooser), NULL); // reset filename


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
    GtkWidget * page = data;
    gtk_stack_set_visible_child(stack, page);
}
void change_panel(GtkWidget *widget, gpointer data)
{
    GtkWidget * page = data;
    gtk_stack_set_visible_child(stack_2, page);
}
void stop_processing()
{
    // get button
    GtkButton *button = GTK_BUTTON(gtk_builder_get_object(builder, "start"));
    gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE); // enable button to start processing
    gtk_button_set_label(button, "Start Process");
    gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2

}
void run_process(GtkButton *button, gpointer data)
{
    if (thread != NULL)
    {
        // change button text 
        // gtk_button_set_label(button, "Start");      

        // // cancel thread
        // pthread_cancel(*thread);
        // pthread_join(*thread, NULL);

        // thread = NULL;
       
    }
    else
    {
        // change text on button
        gtk_button_set_label(button, "Cancel");
        // show progress bar
        GtkProgressBar *progress_bar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar"));
        gtk_widget_show(GTK_WIDGET(progress_bar));
        // run the process
        char output[200];
        snprintf(output, sizeof(output), "%s%s/", "tmp",filename);

        thread = OCR_thread(filename,NULL,TRUE,TRUE,output,TRUE);
    }

    
}

void open_website()
{
   if (!system("firefox www.opticalloop.bugbear.com")) // TODO: change website
    {
        printf("Error opening website\n");
    }
}

void * init_gui()
{
    builder = NULL;
    GError *error = NULL;
    gchar *filename = NULL;

    gtk_init(NULL, NULL);

    builder = gtk_builder_new(); // initialize the builder
    filename = g_build_filename("src/GUI/main.glade", NULL); // build filename

    if (!gtk_builder_add_from_file(builder, filename, &error)) // load file
    {
        g_warning("%s", error->message);
        g_free(filename);
        g_error_free(error);
        pthread_exit(NULL);
    }
    g_free(filename); // free filename
    window =
        GTK_WIDGET(gtk_builder_get_object(builder, "window")); // get window

    gtk_builder_connect_signals(builder, NULL); // connect signals


    // Set title
    gtk_window_set_title(GTK_WINDOW(window), "opticalloop");

    // Load icon
    GdkPixbuf *icon =
        gdk_pixbuf_new_from_file_at_size("src/GUI/logo1.png", 128, 128, NULL);
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    // Load CSS
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "src/GUI/style.css", NULL);

    // Inject CSS
    GdkScreen *screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen,
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    // load widgets
    GtkButton *button_start = GTK_BUTTON(gtk_builder_get_object(builder, "start"));
    gtk_widget_set_sensitive(GTK_WIDGET(button_start), FALSE); // disable button

    stack = GTK_STACK(gtk_builder_get_object(builder, "window_pages"));
    stack_2 = GTK_STACK(gtk_builder_get_object(builder, "right_panel"));

    GtkProgressBar *progress_bar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar"));
    


    // load UI
    gtk_widget_show_all(window); // show window
    gtk_widget_hide(progress_bar); // hide progress bar
    gtk_main(); // start main loop

    pthread_exit(NULL);
}

void quit()
{
    if (thread != NULL)
    {
        // cancel thread
        pthread_cancel(*thread);
        pthread_join(*thread, NULL);
    }


    gtk_main_quit();
}