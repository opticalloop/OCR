#include "GUI/gui.h"

#define WEIGHTS_PATH "src/NeuralNetwork/Weights/w.data"
#define DATA_PATH "src/NeuralNetwork/data.txt"
#define SAVE_PATH "temp.bmp"

GtkBuilder *builder;
gchar *filename;
GtkWidget *window = NULL;
GtkStack *stack;
GtkStack *stack_2;
pthread_t *thread;
pthread_t *thread_neural_network;
int processing = 0;
int is_weights_available = 0;
float rotation_value = 0;
float tmp_rotation_value = 0;

Image *image;
Image *temp_image;

// Resize global variables
int resizing = 0;
int resized_x = -1;
int resized_y = -1;
int resized_w = -1;
int resized_h = -1;

char *get_filename_ext(const char *filename)
{
    // get the filename extension
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

GdkPixbuf *image_to_pixbuf(Image *image)
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8,
                                       image->width, image->height);

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);

    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            guchar *p = pixels + y * rowstride + x * n_channels;
            p[0] = image->pixels[x][y].r;
            p[1] = image->pixels[x][y].g;
            p[2] = image->pixels[x][y].b;
            p[3] = 255;
        }
    }

    return pixbuf;
}

void set_selected_image(GdkPixbuf *pixbuf, char *GtkimageID)
{
    GtkImage *imageWidget =
        GTK_IMAGE(gtk_builder_get_object(builder, GtkimageID)); // get image

    // resize the image
    GdkPixbuf *resized_image = gdk_pixbuf_scale_simple(
        pixbuf, 500, 500, GDK_INTERP_BILINEAR); // resize image

    // set the image
    gtk_image_set_from_pixbuf(imageWidget, resized_image);

    // free
    g_object_unref(pixbuf);
    g_object_unref(resized_image);
}

void change_image(Image *_image, char *GtkimageID)
{
    GdkPixbuf *pixbuf = image_to_pixbuf(_image);

    set_selected_image(pixbuf, GtkimageID);
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
        SDL_Surface *surface = IMG_Load(filename);
        Image img_temp = newImage(surface, 0, surface->w, surface->h);
        temp_image = &img_temp;

        Image tmp = copyImage(&img_temp, 0);
        image = &tmp;

        change_image(image, "selected_image");

        // update label
        GtkLabel *label =
            GTK_LABEL(gtk_builder_get_object(builder, "picture_path"));
        gtk_label_set_text(label, filename);

        gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
        set_buttons_options_status(TRUE); // enable buttons

        saveImage(image, SAVE_PATH);
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

        printf("Processing...\n");
        // Run processing
        thread = OCR_thread(SAVE_PATH, NULL, TRUE, TRUE, "tmp", TRUE,
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

void cancel_edit_option(GtkWidget *widget, gpointer data)
{
    resizing = 0;

    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(TRUE); // enable buttons
}

// ------ ROTATION ------

void edit_rotation(GtkWidget *widget, gpointer data)
{
    // Load image
    GtkImage *image =
        GTK_IMAGE(gtk_builder_get_object(builder, "selected_image2"));
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(image);

    set_selected_image(pixbuf, "selected_image2");

    // reset scale to value 0
    GtkScale *scale =
        GTK_SCALE(gtk_builder_get_object(builder, "scale_rotation"));

    // If rotation value is not zero, the image have already been rotated
    gtk_range_set_value(GTK_RANGE(scale),
                        rotation_value != 0 ? rotation_value : 0);

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
    saveImage(SAVE_PATH, image); // save image
}

void rotate_img(GtkWidget *widget, gpointer data)
{
    // Copy temp to img
    // Free image pixels
    freeImage(image, 0);

    image->pixels = copyPixelsArray(temp_image, 0);

    // Get range value
    float value = gtk_range_get_value(widget);

    rotate(image, value);
    change_image(image, "selected_image2");
}

// ------ /ROTATION ------

// ------ RESIZE ------

void edit_resize(GtkWidget *widget, gpointer data)
{
    resizing = 1;
    printf("Resizing\n");

    // change image
    change_image(image, "selected_image3");

    // change page
    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(FALSE); // disable buttons
}

void on_resize_finished(GtkWidget *widget, gpointer data)
{
    resizing = 0;

    SDL_Rect rect;
    rect.x = resized_x;
    rect.y = resized_y;
    rect.w = resized_w;
    rect.h = resized_h;
    Image img = cropImage(image, &rect);
    image = &img;

    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(TRUE); // enable buttons

    // change image
    change_image(image, "selected_image");
}

void start_nn(GtkWidget *widget, gpointer data)
{
    // get spin button value
    GtkSpinButton *epoch_input =
        GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "epoch_input"));
    int epoch_input_value = gtk_spin_button_get_value_as_int(epoch_input);

    GtkSpinButton *hidden_input =
        GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "hidden_input"));
    int hidden_input_value = gtk_spin_button_get_value_as_int(hidden_input);

    GtkSpinButton *node_input =
        GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "node_input"));
    int node_input_value = gtk_spin_button_get_value_as_int(node_input);

    // get check button value
    GtkCheckButton *check_button =
        GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "train_image"));
    int check_button_value = gtk_toggle_button_get_active(check_button);

    // Check if file exists
    FILE *file;
    file = fopen(WEIGHTS_PATH, "r");
    if (file != NULL && !check_button_value)
    {
        fclose(file);
        // ask user if he wants to cancel
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
            "At the end of the process the current neural network will be "
            "overriden. \nDo you want to continue?");
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (response != GTK_RESPONSE_YES)
        {
            return;
        }
    }

    // start training
    // train_nn(image, epoch_input_value, hidden_input_value, node_input_value
    pthread_t t =
        train_thread(epoch_input_value, hidden_input_value, node_input_value, 1,
                     check_button_value ? WEIGHTS_PATH : "", WEIGHTS_PATH, 1);
    thread_neural_network = &t;
}

void reset_nn()
{}

void cancel_nn(GtkWidget *widget, gpointer data)
{
    if (thread_neural_network != NULL)
    {
        // ask user if he wants to cancel
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
            "Are you sure you want to cancel?");
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (response == GTK_RESPONSE_YES)
        {
            // cancel thread
            pthread_cancel(*thread_neural_network);
            pthread_join(*thread_neural_network, NULL);
        }
        else
        {
            return;
        }
    }
    GtkWidget *page = data;
    show_page(NULL, page);

    set_leftPannel_status(TRUE); // enable buttons
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
    freeImage(image, 0);
    freeImage(temp_image, 0);
    quit();
    pthread_exit(NULL);
}
void reset_terminal()
{
    // get text view
    GtkTextView *text_view =
        GTK_TEXT_VIEW(gtk_builder_get_object(builder, "terminal_text"));

    // get text buffer
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);

    // clear text buffer
    gtk_text_buffer_set_text(text_buffer, "", -1);
}
void edit_terminal(char *string)
{
    // get text view
    GtkTextView *text_view =
        GTK_TEXT_VIEW(gtk_builder_get_object(builder, "terminal_text"));

    // get text buffer
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);

    // add string to text buffer
    gtk_text_buffer_insert_at_cursor(text_buffer, string, -1);

    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(text_buffer, &start); // get start iter
    gtk_text_buffer_get_end_iter(text_buffer, &end); // get end iter

    if (gtk_text_iter_get_line(&end) > 30) // if more than 25 lines
    {
        gtk_text_buffer_delete(text_buffer, &start, &end);
    }
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