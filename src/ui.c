#include "ui.h"
#include "glib.h"
#include "plsc.h"
#include "tokenize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_diff_tags(GtkTextBuffer *buffer) {
    // Texte supprimé
    gtk_text_buffer_create_tag(buffer, "diff-erase",
        "background",    "#f8d7da",
        "foreground",    "#842029",
        "strikethrough", TRUE,
        NULL);
    // Texte ajouté
    gtk_text_buffer_create_tag(buffer, "diff-insert",
        "background", "#d1e7dd",
        "foreground", "#0f5132",
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

    for (size_t i = 0; i < diff->length; i++) {
        token *t = diff->tokens[i];
        char *text = token_text(t);

        if (t->type == ERASE) {
            gtk_text_buffer_insert_with_tags_by_name(buffer, &end, text, -1, "diff-erase", NULL);
        } else if (t->type == INSERT) {
            gtk_text_buffer_insert_with_tags_by_name(buffer, &end, text, -1, "diff-insert", NULL);
        } else {
            gtk_text_buffer_insert(buffer, &end, text, -1);
        }

        g_free(text);
    }

    gtk_text_buffer_end_user_action(buffer);
}

text_boxes* text_boxes_new(GtkBuilder* builder) {
    text_boxes *widgets = g_new(text_boxes, 1);

    widgets->textview1 = GTK_WIDGET(gtk_builder_get_object(builder, "textview1"));
    widgets->textview2 = GTK_WIDGET(gtk_builder_get_object(builder, "textview2"));
    widgets->textview3 = GTK_WIDGET(gtk_builder_get_object(builder, "textview3"));

    gtk_text_view_set_editable(GTK_TEXT_VIEW(widgets->textview3), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(widgets->textview3), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(widgets->textview3), TRUE);

    create_diff_tags(gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview1)));
    create_diff_tags(gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview2)));
    create_diff_tags(gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview3)));

    return widgets;
}

void text_boxes_free(text_boxes *t) {
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

    GtkWidget *button = GTK_WIDGET(gtk_builder_get_object(builder, "execute"));
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    text_boxes* textboxes = text_boxes_new(builder);

    g_signal_connect(button, "clicked", G_CALLBACK(ui_execute_diff), textboxes);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), textboxes->textview1);

    gtk_widget_show_all(window);
    g_object_unref(builder);
    gtk_main();
    return 0;
}


// Fonction appelée lorsque le bouton est cliqué
void ui_execute_diff(GtkButton *button, gpointer user_data) {
    text_boxes *widgets = (text_boxes *)user_data;
    char* ref = get_textview_buffer(widgets->textview1);
    char* src = get_textview_buffer(widgets->textview2);

    token_list* ref_tokens = token_list_create(10);
    token_list* src_tokens = token_list_create(10);
    strhash* s = strhash_create(10);

    tokenize_text(ref, s, ref_tokens);
    tokenize_text(src, s, src_tokens);

    token_list* list_diff = diff(ref_tokens, src_tokens);

    GtkTextBuffer *buffer3 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview3));
    display_diff(buffer3, list_diff);

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
