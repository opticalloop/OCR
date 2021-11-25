
#include "GUI/gui.h"

#define WEIGHTS_PATH "src/NeuralNetwork/Weights/w.txt"

GtkBuilder *builder;
gchar *filename;
GtkWidget *window = NULL;
GtkStack *stack;
GtkStack *stack_2;
GtkRange *rotation_range;
pthread_t *thread;
int processing = 0;
int is_weights_available = 0;
SDL_Surface *image;

char *get_filename_ext(const char *filename)
{
    // get the filename extension
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

void change_image(SDL_Surface *surface)
{
    GtkImage *imageWidget = GTK_IMAGE(
        gtk_builder_get_object(builder, "selected_image")); // get image
    // convert SDL surface to GTK image
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
        surface->pixels, GDK_COLORSPACE_RGB, FALSE, 8, surface->w, surface->h,
        surface->pitch, NULL, NULL);

    // resize the image
    GdkPixbuf *resized_image = gdk_pixbuf_scale_simple(
        pixbuf, 500, 500, GDK_INTERP_BILINEAR); // resize image

    // set the image
    gtk_image_set_from_pixbuf(imageWidget, resized_image);

    // free
    g_object_unref(pixbuf);
    g_object_unref(resized_image);
}

void edit_progress_bar(float progress, char *text)
{
    // get progress bar
    GtkProgressBar *progress_bar =
        GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar"));

    // set progress bar
    gtk_progress_bar_set_fraction(progress_bar, progress);

    // set progress bar text
    gtk_progress_bar_set_text(progress_bar, text);
}

void on_file_set(GtkFileChooserButton *file_chooser, gpointer data)
{
    // select filename and update image
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    char *ext = get_filename_ext(filename);
    GtkButton *button = data;

    // check if file is an image
    if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0
        || strcmp(ext, "jpeg") == 0 || strcmp(ext, "bmp") == 0)
    {
        // load image
        image = IMG_Load(filename);
        change_image(image);

        // update label
        GtkLabel *label =
            GTK_LABEL(gtk_builder_get_object(builder, "picture_path"));
        gtk_label_set_text(label, filename);

        gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
        gtk_widget_set_sensitive(GTK_WIDGET(button),
                                 TRUE); // enable button to start processing
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

void stop_processing()
{
    // get button
    GtkButton *button = GTK_BUTTON(gtk_builder_get_object(builder, "start"));
    gtk_widget_set_sensitive(GTK_WIDGET(button),
                             TRUE); // enable button to start processing
    gtk_button_set_label(button, "Start Process");
    gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
}

void run_process(GtkButton *button)
{
    // Check if file exist opening it
    FILE *file = fopen(WEIGHTS_PATH, "r");
    if (file == NULL)
    {
        is_weights_available = 0;
    }
    else
    {
        is_weights_available = 1;
        fclose(file);
    }


    if (processing)
    {
        processing = 0;
        stop_processing();

        // // Cancel processing
        // pthread_cancel(*thread);
        // pthread_join(*thread, NULL);
        // thread = NULL;
    }
    else if (is_weights_available)
    {
        processing = 1;
        // change text on button
        gtk_button_set_label(button, "Cancel");
        // show progress bar
        GtkProgressBar *progress_bar =
            GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar"));
        gtk_widget_show(GTK_WIDGET(progress_bar));

        // Get signal from combo box
        GtkComboBox *combo_box =
            GTK_COMBO_BOX(gtk_builder_get_object(builder, "dim_input"));
        // Get string from combo box
        char *dim =
            gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));

        // Run processing
        thread = OCR_thread(image, NULL, TRUE, TRUE, "tmp", TRUE,
                            strcmp(dim, "9x9"));
    }
    else
    {
        // display error message
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
            "No weights available. Please train the network first.");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog
    }
}

void open_website()
{
    // Check if the browser is installed
    if (g_find_program_in_path("firefox") != NULL)
    {
        // Open the website
        if (!system("firefox www.opticalloop.bugbear.com"))
        {
            printf("Error opening website\n");
        }
    }
}

void edit_rotation()
{
    // Show widget
    gtk_widget_show(GTK_WIDGET(rotation_range));
}

void rotate_img()
{
    // Get range value
    float value = gtk_range_get_value(rotation_range);
    rotateSurface(image, value);
    change_image(image);

}

void *init_gui()
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
    GtkButton *button_start =
        GTK_BUTTON(gtk_builder_get_object(builder, "start"));
    gtk_widget_set_sensitive(GTK_WIDGET(button_start), FALSE); // disable button

    stack = GTK_STACK(gtk_builder_get_object(builder, "window_pages"));
    stack_2 = GTK_STACK(gtk_builder_get_object(builder, "right_panel"));

    GtkProgressBar *progress_bar =
        GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar"));
    
    // Rotation range
    rotation_range = GTK_RANGE(gtk_builder_get_object(builder, "Rotation_scale"));
    gtk_range_set_range(rotation_range, -180, 180);
    gtk_range_set_show_fill_level(rotation_range, TRUE);
    gtk_range_set_value(rotation_range, 0);
    gtk_widget_hide(GTK_WIDGET(rotation_range));
    

    // load UI
    gtk_widget_show_all(window); // show window
    gtk_widget_hide(GTK_WIDGET(progress_bar)); // hide progress bar
    gtk_main(); // start main loop

    // End program
    SDL_FreeSurface(image);
    quit();
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

void resetNeuralNetwork()
{
    // Delete file
    char cmd[200];
    snprintf(cmd, sizeof(cmd), "rm -f %s", WEIGHTS_PATH);
    if (!system(cmd))
    {
        printf("Error deleting file\n");
    }
}