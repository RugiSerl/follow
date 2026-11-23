#include "ui.h"
#include "glib-object.h"
#include "glib.h"
#include "gtk/gtk.h"
#include "plsc.h"
#include "tokenize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_diff_tags(GtkTextBuffer *buffer) {
    // Texte supprimé
    gtk_text_buffer_create_tag(buffer, "diff-erase",
        "background",    "#f8d7da",
        "foreground",    "#e90c1e",
        "strikethrough", TRUE,
        NULL);
    // Texte ajouté
    gtk_text_buffer_create_tag(buffer, "diff-insert",
        "background", "#d1e7dd",
        "foreground", "#0aee84",
        "underline",  FALSE,
        NULL);
    // Texte remplacé
    gtk_text_buffer_create_tag(buffer, "diff-replace",
        "background", "#ffffff",
        "foreground", "#0a3fee",
        "underline",  FALSE,
        NULL);
}

char *token_text(token *t) {
    switch (t->type) {
        case WORD:
        case SPACE:
            return g_strdup(t->data.word);

        case SHORT_SPACE: {
            char tmp[5] = { 0 };
            memcpy(tmp, t->data.space, 4);
            return g_strdup(tmp);
        }

        case ERASE:
        case INSERT:
        case REPLACE:
            return token_text(t->data.token_referenced);

        default:
            return g_strdup("");
    }
}

void display_diff(GtkTextBuffer *buffer, token_list *diff) {
    gtk_text_buffer_set_text(buffer, "", 0);

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);

    // Regroupe toutes les insertions en une seule action (undo + perf).
    gtk_text_buffer_begin_user_action(buffer);
    printf("length : %p\n", diff);
    for (size_t i = 0; i < diff->length; i++) {
        token *t = diff->tokens[i];
        char *text = token_text(t);

        if (t->type == ERASE) {
            gtk_text_buffer_insert_with_tags_by_name(buffer, &end, text, -1, "diff-erase", NULL);
        } else if (t->type == INSERT) {
            gtk_text_buffer_insert_with_tags_by_name(buffer, &end, text, -1, "diff-insert", NULL);
        } else if (t->type == REPLACE) {
            gtk_text_buffer_insert_with_tags_by_name(buffer, &end, text, -1, "diff-replace", NULL);
        } else {
            gtk_text_buffer_insert(buffer, &end, text, -1);
        }

        g_free(text);
    }

    gtk_text_buffer_end_user_action(buffer);
}

ui_state* ui_state_new(GtkBuilder* builder, GtkWindow* window) {
    ui_state *widgets = g_new(ui_state, 1);

    widgets->textview1 = GTK_WIDGET(gtk_builder_get_object(builder, "textview1"));
    widgets->textview2 = GTK_WIDGET(gtk_builder_get_object(builder, "textview2"));
    widgets->textview3 = GTK_WIDGET(gtk_builder_get_object(builder, "textview3"));
    widgets->window = window;

    gtk_text_view_set_editable(GTK_TEXT_VIEW(widgets->textview3), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(widgets->textview3), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(widgets->textview3), TRUE);

    create_diff_tags(gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview1)));
    create_diff_tags(gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview2)));
    create_diff_tags(gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview3)));

    return widgets;
}

void ui_state_free(ui_state *t) {
    g_free(t);
}

int ui_run(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "ui.glade", NULL)) {
        g_critical("Impossible de charger le fichier ui.glade");
        return 1;
    }

    GtkWidget *execute_button = GTK_WIDGET(gtk_builder_get_object(builder, "execute"));
    GtkWidget *loadref_button = GTK_WIDGET(gtk_builder_get_object(builder, "loadref"));
    GtkWidget *loadsrc_button = GTK_WIDGET(gtk_builder_get_object(builder, "loadsrc"));
    GtkWidget *apropos_button = GTK_WIDGET(gtk_builder_get_object(builder, "apropos"));
    GtkWidget *quit_button = GTK_WIDGET(gtk_builder_get_object(builder, "quit"));
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    ui_state* state = ui_state_new(builder, GTK_WINDOW(window));

    g_signal_connect(execute_button, "clicked", G_CALLBACK(ui_execute_diff), state);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(quit_button, "activate", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(loadref_button, "activate", G_CALLBACK(load_ref), state);
    g_signal_connect(loadsrc_button, "activate", G_CALLBACK(load_src), state);
    g_signal_connect(apropos_button, "activate", G_CALLBACK(open_apropos), window);

    gtk_widget_show_all(window);
    g_object_unref(builder);
    gtk_main();
    return 0;
}


// Fonction appelée lorsque le bouton est cliqué
void ui_execute_diff(GtkButton *button, gpointer user_data) {
    ui_state *widgets = (ui_state *)user_data;
    char* ref = get_textview_buffer(widgets->textview1);
    char* src = get_textview_buffer(widgets->textview2);

    token_list* ref_tokens = token_list_create(10);
    token_list* src_tokens = token_list_create(10);
    strhash* s = strhash_create(10);

    tokenize_text(ref, s, ref_tokens);
    tokenize_text(src, s, src_tokens);

    token_list* list_diff = diff(ref_tokens, src_tokens);
    GtkTextBuffer *buffer3 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview3));

    if (list_diff == NULL) {
        gtk_text_buffer_set_text(buffer3, "Erreur :(", -1);
        token_list_free(list_diff);
        token_list_free(ref_tokens);
        token_list_free(src_tokens);
        g_free(ref);
        g_free(src);
        return;
    }

    display_diff(buffer3, list_diff);

    (void) button; //pour enlever le warning unused
    token_list_free(list_diff);
    token_list_free(ref_tokens);
    token_list_free(src_tokens);
    g_free(ref);
    g_free(src);

}

char* get_textview_buffer(GtkWidget* text_view) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    return text;
}

char* open_file_dialog(GtkWindow* window) {
    GtkFileChooserNative *native;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    native = gtk_file_chooser_native_new("Open File", window, action, "_Open", "_Cancel");
    res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(native);
        filename = gtk_file_chooser_get_filename(chooser);
        return filename;
    }

    g_object_unref (native);
    return NULL;
}

void load_ref(GtkButton *button, gpointer user_data) {
    ui_state *widgets = (ui_state *)user_data;
    char* filename = open_file_dialog(widgets->window);
    if (filename == NULL) return;
    char* ref = openfile(filename);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview1));
    gtk_text_buffer_set_text(buffer, ref, (gint) strlen(ref));

    (void) button; //pour enlever le warning unused 
    g_free(filename);
    free(ref);
}

void load_src(GtkButton *button, gpointer user_data) {
    ui_state *widgets = (ui_state *)user_data;
    char* filename = open_file_dialog(widgets->window);
    if (filename == NULL) return;
    char* src = openfile(filename);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview2));
    gtk_text_buffer_set_text(buffer, src, (gint) strlen(src));

    (void) button; //encore pour enlever le warning unused 
    g_free(filename);
    free(src);
}

void open_apropos(GtkButton *button, gpointer parent) {
    const gchar *authors[] = { "Raphaël Glet", "Kylian Bougeard", NULL };
    gtk_show_about_dialog(
        GTK_WINDOW(parent),
        "program-name", "Follow",
        "version", "0.5.0",
        "comments", "Application qui permet de calculer les différences entre deux textes.",
        "authors", authors,
        "license-type", GTK_LICENSE_MIT_X11,
        NULL
    );
    (void) button;  //toujours pour enlever le warning unused 
}
