#include "GUI/gui.h"

#define WEIGHTS_PATH "src/NeuralNetwork/Weights/w.data"
#define DATA_PATH "src/NeuralNetwork/data.txt"
#define IMAGE_PATH "src/Sudoku_Solver/Sudoku_Saver/Images"

#define IMAGE_SAVE_PATH "temp.bmp"
#define IMAGE_TEMP_PATH "temp2.bmp"

Image image;
Image temp_image;

GtkBuilder *builder;
gchar *filename;
GtkWidget *window = NULL;
GtkStack *stack;
GtkStack *stack_2;
pthread_t *thread;
pthread_t *thread_neural_network;
int processing = 0;
int is_weights_available = 0;
double rotation_value = 0;
double tmp_rotation_value = 0;

// Resize global variables
int resizing = 0;
Square resized_square;

#pragma region "Image_management"

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

    GtkStack *panel = GTK_STACK(gtk_builder_get_object(builder, "right_panel"));

    int width = clamp(gtk_widget_get_allocated_width(GTK_WIDGET(panel)), 0,
                      1000); // TODO: fix this
    int height =
        clamp(gtk_widget_get_allocated_height(GTK_WIDGET(panel)), 0, 1000);

    // int height = gtk_widget_get_allocated_height(GTK_WIDGET(panel));

    // get image size
    int image_width = gdk_pixbuf_get_width(pixbuf);
    int image_height = gdk_pixbuf_get_height(pixbuf);

    // get scale factor
    double scale_factor = 1;
    if (image_width > width || image_height > height)
    {
        scale_factor = (double)width / image_width;
        if (scale_factor * image_height > height)
            scale_factor = (double)height / image_height;
    }
    int new_width = image_width * scale_factor;
    int new_height = image_height * scale_factor;

    printf("%f %d %d\n", scale_factor, width, height);
    // resize the image
    GdkPixbuf *resized_image = gdk_pixbuf_scale_simple(
        pixbuf, new_width, new_height, GDK_INTERP_BILINEAR); // resize image

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

#pragma endregion "Image_management"

#pragma region "File_management"

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
    // GtkBox *box_1 = GTK_BOX(gtk_builder_get_object(builder, "options"));
    // GtkButton *button = data;

    // check if file is an image
    if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0
        || strcmp(ext, "jpeg") == 0 || strcmp(ext, "bmp") == 0)
    {
        // Create both image
        SDL_Surface *surface = IMG_Load(filename);
        image = newImage(surface, 0, surface->w, surface->h);
        SDL_FreeSurface(surface);
        temp_image = copyImage(&image, 0);

        printf("    🎨 Loaded %s\n", filename);

        // Display image
        change_image(&image, "selected_image");

        // update label
        GtkLabel *label =
            GTK_LABEL(gtk_builder_get_object(builder, "picture_path"));
        gtk_label_set_text(label, filename);

        gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
        set_buttons_options_status(TRUE); // enable buttons

        saveImage(&image, IMAGE_SAVE_PATH);
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

#pragma endregion "File_management"

#pragma region "GUI_interaction"

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

void show_page(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;

    GtkWidget *page = data;
    gtk_stack_set_visible_child(stack, page);
}

void change_panel(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;

    GtkWidget *page = data;
    gtk_stack_set_visible_child(stack_2, page);
}

void cancel_edit_option(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;

    resizing = 0;

    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(TRUE); // enable buttons
}

#pragma endregion "GUI_interaction"

#pragma region "Process"

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
        pthread_t t;
        t = OCR_thread(IMAGE_SAVE_PATH, NULL, TRUE, TRUE, "tmp", TRUE,
                       strcmp(dim, "9x9"));
        thread = &t;
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

void stop_processing()
{
    // get button
    GtkButton *button = GTK_BUTTON(gtk_builder_get_object(builder, "start"));
    gtk_widget_set_sensitive(GTK_WIDGET(button),
                             TRUE); // enable button to start processing
    gtk_button_set_label(button, "Start Process");
    gtk_stack_set_visible_child_name(stack_2, "page2"); // show page 2
}

#pragma endregion "Process"

#pragma region "Rotate"

void edit_rotation(GtkWidget *widget, gpointer data)
{
    // Don't get warning
    (void)widget;
    printf("    ❓ Starting rotation...\n");

    // Load image
    change_image(&image, "selected_image2");

    // reset scale to value 0
    GtkScale *scale =
        GTK_SCALE(gtk_builder_get_object(builder, "scale_rotation"));

    // If rotation value is not zero, the image have already been rotated
    gtk_range_set_value(GTK_RANGE(scale),
                        rotation_value != 0 ? rotation_value : 0);

    // Change page
    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(FALSE); // disable buttons
}

void on_rotation_finished(GtkWidget *widget, gpointer data)
{
    // Don't get warning
    (void)widget;

    printf("    👍 Finished rotation\n");

    // Change page
    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(TRUE); // enable buttons

    // get scale value
    GtkScale *scale =
        GTK_SCALE(gtk_builder_get_object(builder, "scale_rotation"));
    rotation_value = gtk_range_get_value(GTK_RANGE(scale));

    change_image(&image, "selected_image"); // change image of main page

    // Save
    saveImage(&image, IMAGE_SAVE_PATH);
    saveImage(&image, IMAGE_TEMP_PATH);
}

void rotate_img(GtkWidget *widget, gpointer data)
{
    // Don't get warning
    (void)data;

    // Get range value
    double value = gtk_range_get_value(GTK_RANGE(widget));

    cloneImage(&temp_image, &image);
    rotate(&image, value);

    // Visualize temp image
    change_image(&image, "selected_image2");
}

#pragma endregion "Rotate"

#pragma region "Resize"

void edit_resize(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;

    resizing = 1;
    printf("    🛠️ Starting resize...\n");
    set_leftPannel_status(FALSE); // disable buttons

    // get picture

    GtkImage *imageWidget = GTK_IMAGE(
        gtk_builder_get_object(builder, "selected_image3")); // get image

    // get image size
    GdkPixbuf *pixbuf = image_to_pixbuf(&image);

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);

    printf("    📷 Image size: %dx%d\n", width, height);

    resized_square.top.xStart = 0;
    resized_square.top.yStart = 0;
    resized_square.right.xStart = width - 1;
    resized_square.right.yStart = 0;
    resized_square.bottom.xStart = width - 1;
    resized_square.bottom.yStart = height - 1;
    resized_square.left.xStart = 0;
    resized_square.left.yStart = height - 1;

    // Change image
    selectionFilter(&image, &resized_square);
    // get drawing area
    GtkDrawingArea *drawing_area =
        GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawing_area"));
    // draw image on drawing area
    gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
    // set image
    change_image(&image, "selected_image3");

    // change page
    GtkWidget *page = data;
    change_panel(NULL, page);
}

void on_resize_finished(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;
    resizing = 0;
    printf("    👍 Finished resize\n");

    Image img = correctPerspective(&image, &resized_square, 1, "temp/");
    image = img;

    GtkWidget *page = data;
    change_panel(NULL, page);

    set_leftPannel_status(TRUE); // enable buttons

    // change image
    change_image(&image, "selected_image");
}

#pragma endregion "Resize"

#pragma region "Neural Network"

void start_nn(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;
    (void)data;

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
    int check_button_value =
        gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_button));

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
    reset_terminal("terminal_text");
    pthread_t t =
        train_thread(epoch_input_value, hidden_input_value, node_input_value, 1,
                     check_button_value ? WEIGHTS_PATH : "", WEIGHTS_PATH, 1);
    thread_neural_network = &t;
}

void reset_nn()
{
    // Delete file
    char cmd[200];
    snprintf(cmd, sizeof(cmd), "rm -f %s", WEIGHTS_PATH);
    if (!system(cmd))
    {
        printf("Error deleting file\n");
    }
}

void cancel_nn(GtkWidget *widget, gpointer data)
{
    // Avoid warning
    (void)widget;
    (void)data;

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

#pragma endregion "Neural Network"

#pragma region "Terminal"

void reset_terminal(char *terminal_id)
{
    // get text view
    GtkTextView *text_view =
        GTK_TEXT_VIEW(gtk_builder_get_object(builder, terminal_id));

    // get text buffer
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);

    // clear text buffer
    gtk_text_buffer_set_text(text_buffer, "", -1);
}

void edit_terminal(char *terminal_id, char *string)
{
    // get text view
    GtkTextView *text_view =
        GTK_TEXT_VIEW(gtk_builder_get_object(builder, terminal_id));
    // printf("%s\n", terminal_id);
    // get text buffer
    GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(text_view);

    // add string to text buffer
    gtk_text_buffer_insert_at_cursor(text_buffer, string, -1);

    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(text_buffer, &start); // get start iter
    gtk_text_buffer_get_end_iter(text_buffer, &end); // get end iter

    if (gtk_text_iter_get_line(&end) > 30) // if more than 25 lines
    {
        printf("too many lines\n");
        gtk_text_buffer_delete(text_buffer, &start, &end);
    }
}

#pragma endregion "Terminal"

#pragma region "Result"

void show_result(unsigned int **grid, int dimension, Image *res)
{
    GtkGrid *grid_result;
    GtkStack *stack_result;
    if (dimension == 9)
    {
        gtk_stack_set_visible_child_name(stack_2, "confirmation");

        // Load image
        change_image(res, "selected_image1");

        // get grid
        grid_result = GTK_GRID(gtk_builder_get_object(builder, "grid_result"));

        // stack_result = GTK_STACK(gtk_builder_get_object(builder, "stack1"));
    }
    else // dimension == 16
    {
        gtk_stack_set_visible_child_name(stack_2, "confirmation_hexa");

        // Load image
        change_image(res, "selected_image_hexa");

        // get grid
        grid_result =
            GTK_GRID(gtk_builder_get_object(builder, "grid_result_hexa"));

        // stack_result = GTK_STACK(gtk_builder_get_object(builder, "stack2"));
    }
    // gtk_stack_set_visible_child_name(stack_result, "Result");

    for (size_t i = 0; i < dimension; i++)
    {
        for (size_t j = 0; j < dimension; j++)
        {
            // get input at i,j and set value
            GtkEntry *entry =
                GTK_ENTRY(gtk_grid_get_child_at(grid_result, i, j));

            char ch[40] = " ";
            if (grid[i][j] > 9)
            {
                // Convert 10 to A
                ch[0] = 'A' + (grid[i][j] - 10);
            }
            else
            {
                ch[0] = '0' + grid[i][j];
            }
            gtk_entry_set_text(entry, ch);
        }
    }
    freeGrid(grid, dimension);
}

void confirm_result()
{
    GtkComboBox *combo_box =
        GTK_COMBO_BOX(gtk_builder_get_object(builder, "dim_input"));
    char *dim_text =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_box));

    int dim = !strcmp(dim_text, "9x9") ? 9 : 16;

    unsigned int **result = allocGrid(dim);

    GtkGrid *grid = dim == 9
        ? GTK_GRID(gtk_builder_get_object(builder, "grid_result"))
        : GTK_GRID(gtk_builder_get_object(builder, "grid_result_hexa"));

    char *str;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            GtkEntry *entry = GTK_ENTRY(gtk_grid_get_child_at(grid, i, j));

            str = gtk_entry_get_text(entry);
            int size = strlen(str);
            // Str length is 1 or 2
            if (size == 0)
                result[i][j] = 0;
            if (size == 1)
            {
                // Is it a digit
                if (str[0] >= '0' && str[0] <= '9')
                {
                    result[i][j] = (unsigned int)str[0] - '0';
                }
                else if (str[0] >= 'A' && str[0] <= 'G')
                {
                    result[i][j] = (unsigned int)str[0] - 'A' + 10;
                }
                else
                {
                    printf("Error: Invalid input\n");
                    return;
                }
            }
            else if (size == 2)
            {
                result[i][j] =
                    (unsigned int)(str[0] - '0') * 10 + (str[1] - '0');
            }
            else
            {
                printf("Error : should write 1 or 2 digit long (1, 2, 3, 4, 5, "
                       "6, 7, 8, 9, to 16 with heca or digit notation)\n");
                return;
            }
        }
    }

    if (!isSolvable(result, dim))
    {
        // display error message
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
            "The grid is not solvable, please correct the grid");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog

        gtk_stack_set_visible_child_name(stack_2,
                                         "page_result"); // TODO: remove this
        return;
    }

    unsigned int **copy = allocGrid(dim);
    // Copy array to have different color when saving the image
    copyArray(result, copy, dim);

    printf("\n    📊 Solving sudoku\n");
    printf("    ⏳ Please wait...\n");

    solveSuduko(result, 0, 0, dim);

    saveGrid(result, "grid.result", 1, dim);

    // Create, save and free the image
    Image sudoku_image;
    if (dim == 16)
    {
        sudoku_image = createHexaSudokuImage(result, copy, IMAGE_PATH);
    }
    else
    {
        sudoku_image = createSudokuImage(result, copy, IMAGE_PATH, dim);
    }

    freeGrid(result, dim); // Free grid
    freeGrid(copy, dim); // Free copy

    change_image(&sudoku_image, "selected_image");
    edit_progress_bar(1, "Result");

    // TODO: change the way we save image
    saveVerbose(1, &sudoku_image, "tmp", "0.0_grid", 1, 1);

    gtk_stack_set_visible_child_name(stack_2, "page_result");

    change_image(&sudoku_image, "result_image");
}

void on_resize(GtkWidget *widget, GdkRectangle *allocation, gpointer data)
{
    change_image(&image, "selected_image");
}

#pragma endregion "Result"

#pragma region "Main"
void open_file(GtkWidget *widget, gpointer data)
{
    // open image in explorer
    char *path = data;
    char command[100] = "xdg-open ";
    strcat(command, path);
    if (system(command))
    {
        printf("Error: Could not open file\n");

        // display error message
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Could not open file");

        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog
    }
}

void set_recents_files()
{
    GtkRecentManager *recent_manager = gtk_recent_manager_get_default();

    GList *items = gtk_recent_manager_get_items(recent_manager);

    GtkBox *box = GTK_BOX(gtk_builder_get_object(builder, "box_recents"));
    int i = 0;
    while (items != NULL && i < 5)
    {
        GtkRecentInfo *info = (GtkRecentInfo *)items->data;
        char *uri = gtk_recent_info_get_uri(info);
        char *ext = get_filename_ext(uri);
        // if file is not a picture, we don't add it to the list
        if (strcmp(ext, "png") && strcmp(ext, "jpg") && strcmp(ext, "jpeg")
            && strcmp(ext, "bmp"))
        {
            items = g_list_next(items);
            continue;
        }
        char *name = gtk_recent_info_get_display_name(info);
        char *path = g_filename_from_uri(uri, NULL, NULL);
        if (path == NULL)
        {
            printf("Error: Could not get path : %s\n", uri);
            items = g_list_next(items);
            continue;
        }
        GtkButton *button = gtk_button_new_with_label(name);
        gtk_widget_set_name(GTK_WIDGET(button), path);
        // set alignment horizontal to start and vertical to center
        gtk_button_set_alignment(button, 0, 0.5);
        g_signal_connect(button, "clicked", G_CALLBACK(open_file), path);
        GtkStyleContext *context =
            gtk_widget_get_style_context(GTK_WIDGET(button));

        gtk_box_pack_start(box, GTK_WIDGET(button), FALSE, FALSE, 0);

        // Free
        g_free(uri);
        g_free(name);

        items = g_list_next(items);
        i++;
    }
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

    set_recents_files();

    // on resize window
    // g_signal_connect(window, "size-allocate", G_CALLBACK(on_resize), NULL);
    // TODO: fix this

    // load UI
    gtk_widget_show_all(window); // show window
    gtk_widget_hide(GTK_WIDGET(progress_bar)); // hide progress bar
    gtk_main(); // start main loop

    // pthread_exit(NULL);
}
void open_website()
{
    // Check if the browser is installed
    if (g_find_program_in_path("firefox") != NULL)
    {
        // Open the website
        if (!system("firefox www.opticalloop.bugbear.fr"))
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
    freeImage(&image, 0);
    freeImage(&temp_image, 0);
    gtk_main_quit();

    // Free builder
    g_object_unref(builder);
}

void end_process()
{
    // ask user where he wants to save the image
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Save image", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE,
        "Cancel", GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_ACCEPT, NULL);

    // set filter
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "PNG image");
    gtk_file_filter_add_pattern(filter, "*.png");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // set default file name
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "image.png");

    // run dialog
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename =
            gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        // if the file already exists
        if (g_file_test(filename, G_FILE_TEST_EXISTS))
        {
            // ask user if he wants to overwrite the file
            GtkWidget *dialog_overwrite = gtk_message_dialog_new(
                GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_YES_NO, "The file already exists. Overwrite?");

            if (gtk_dialog_run(GTK_DIALOG(dialog_overwrite)) == GTK_RESPONSE_NO)
            {
                gtk_widget_destroy(dialog_overwrite);
                gtk_widget_destroy(dialog);
                return;
            }
            gtk_widget_destroy(dialog_overwrite);
        }
        saveImage(&image, filename); // TODO: change image by sudoku_image

        // save filename in recent files
        gtk_recent_manager_add_item(gtk_recent_manager_get_default(), filename);

        free(filename);
    }

    gtk_widget_destroy(dialog);

    // change to page1
    gtk_stack_set_visible_child_name(stack_2, "page1");

    // reset all GtkImage widgets to stock _id
    GtkImage *image_1 =
        GTK_IMAGE(gtk_builder_get_object(builder, "selected_image"));
    GtkImage *image_2 =
        GTK_IMAGE(gtk_builder_get_object(builder, "selected_image1"));
    GtkImage *image_3 =
        GTK_IMAGE(gtk_builder_get_object(builder, "selected_image_hexa"));
    GtkImage *image_4 =
        GTK_IMAGE(gtk_builder_get_object(builder, "selected_image3"));

    gtk_image_set_from_icon_name(image_1, "image-x-generic",
                                 GTK_ICON_SIZE_DIALOG);
    gtk_image_set_from_icon_name(image_2, "image-x-generic",
                                 GTK_ICON_SIZE_DIALOG);
    gtk_image_set_from_icon_name(image_3, "image-x-generic",
                                 GTK_ICON_SIZE_DIALOG);
    gtk_image_set_from_icon_name(image_4, "image-x-generic",
                                 GTK_ICON_SIZE_DIALOG);

    // get filechooser button
    GtkFileChooserButton *filechooser_button = GTK_FILE_CHOOSER_BUTTON(
        gtk_builder_get_object(builder, "filechooser_button"));
    // reset filechooser button
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(filechooser_button), NULL);
    printf("reset filechooser button\n");
    set_buttons_options_status(FALSE);
}

#pragma endregion "Main"