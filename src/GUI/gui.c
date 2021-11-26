#include "GUI/gui.h"

#define WEIGHTS_PATH "src/NeuralNetwork/Weights/w.data"

GtkBuilder *builder;
gchar *filename;
GtkWidget *window = NULL;
GtkStack *stack;
GtkStack *stack_2;
pthread_t *thread;
int processing = 0;
int is_weights_available = 0;
float rotation_value = 0;
float tmp_rotation_value = 0;

SDL_Surface *image;

char *get_filename_ext(const char *filename)
{
    // get the filename extension
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

void change_image(SDL_Surface *surface, char *GtkimageID)
{
    GtkImage *imageWidget =
        GTK_IMAGE(gtk_builder_get_object(builder, GtkimageID)); // get image
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
void set_leftPannel_status(gboolean status)
{
    // set the left pannel status
    GtkWidget *leftPannel =
        GTK_WIDGET(gtk_builder_get_object(builder, "left_panel"));
    gtk_widget_set_sensitive(leftPannel, status);
}
void set_buttons_options_status(gboolean status)
{
    GtkButton *button_start =
        GTK_BUTTON(gtk_builder_get_object(builder, "start"));
    gtk_widget_set_sensitive(GTK_WIDGET(button_start),
                             status); // disable button

    GtkBox *box_1 = GTK_BOX(gtk_builder_get_object(builder, "options"));
    gtk_widget_set_sensitive(GTK_WIDGET(box_1), status); // disable box
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
    GtkBox *box_1 = GTK_BOX(gtk_builder_get_object(builder, "options"));
    char *ext = get_filename_ext(filename);
    GtkButton *button = data;

    // check if file is an image
    if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0
        || strcmp(ext, "jpeg") == 0 || strcmp(ext, "bmp") == 0)
    {
        // load image
        image = IMG_Load(filename);
        change_image(image, "selected_image");

        // update label
        GtkLabel *label =
            GTK_LABEL(gtk_builder_get_object(builder, "picture_path"));
        gtk_label_set_text(label, filename);

        gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
        set_buttons_options_status(TRUE); // enable buttons
    }
    else
    {
        set_buttons_options_status(FALSE); // disable buttons
        // gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(file_chooser),
        //                               NULL); // reset filename

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
void edit_rotation(GtkWidget *widget, gpointer data)
{
    // change image
    change_image(image, "selected_image2");
    // reset scale to value 0
    GtkScale *scale =
        GTK_SCALE(gtk_builder_get_object(builder, "scale_rotation"));

    // if scale value is not 0
    if (gtk_range_get_value(GTK_SCALE(scale)) != 0)
    {
        // TODO : reset image
    }

    gtk_range_set_value(GTK_RANGE(scale), 0);

    // change page

    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(FALSE); // disable buttons
}

void on_rotation_finished(GtkWidget *widget, gpointer data)
{
    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(TRUE); // enable buttons

    // get scale value
    GtkScale *scale =
        GTK_SCALE(gtk_builder_get_object(builder, "scale_rotation"));
    rotation_value = gtk_range_get_value(GTK_RANGE(scale));

    change_image(image, "selected_image"); // change image of main page
}

void rotate_img(GtkWidget *widget, gpointer data)
{
    // Get range value
    float value = gtk_range_get_value(widget);
    value -= tmp_rotation_value; // get difference between old and new value
    // if (value < 0) // if value is negative
    //     value = 360 + value; // convert to positive value

    tmp_rotation_value = value; // save new value
    rotateSurface(image, value);
    change_image(image, "selected_image2");
}

void edit_resize(GtkWidget *widget, gpointer data)
{
    SDL_Rect rect;
    rect.x = 100;
    rect.y = 100;
    rect.w = 1000;
    rect.h = 1000;
    selectionFilter(image, &rect);

    // change image
    change_image(image, "selected_image3");

    // change page
    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(FALSE); // disable buttons
}
void cancel_edit_option(GtkWidget *widget, gpointer data)
{
    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(TRUE); // enable buttons
}

void on_resize_finished(GtkWidget *widget, gpointer data)
{
    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(TRUE); // enable buttons

    // change image
    change_image(image, "selected_image");
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
    set_buttons_options_status(FALSE);

    stack = GTK_STACK(gtk_builder_get_object(builder, "window_pages"));
    stack_2 = GTK_STACK(gtk_builder_get_object(builder, "right_panel"));

    GtkProgressBar *progress_bar =
        GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progress_bar"));

    // load UI
    gtk_widget_show_all(window); // show window
    gtk_widget_hide(GTK_WIDGET(progress_bar)); // hide progress bar
    gtk_main(); // start main loop

    // End program
    SDL_FreeSurface(image);
    quit();
    pthread_exit(NULL);
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