#include "gtk/gtk.h"

typedef struct {
    GtkWidget *textview1;
    GtkWidget *textview2;
    GtkWidget *textview3;
} text_boxes;

text_boxes* text_boxes_new(GtkBuilder* builder);

void text_boxes_free(text_boxes* t);

int ui_run(int argc, char *argv[]);

char* get_textview_buffer(GtkWidget* text_view);

void ui_execute_diff(GtkButton *button, gpointer user_data);
