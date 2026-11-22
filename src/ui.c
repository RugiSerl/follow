#include "ui.h"
#include "glib.h"
#include "plsc.h"
#include "tokenize.h"
#include <stdio.h>

text_boxes* text_boxes_new(GtkBuilder* builder) {
    text_boxes *widgets = g_new(text_boxes, 1);

    widgets->textview1 = GTK_WIDGET(gtk_builder_get_object(builder, "textview1"));
    widgets->textview2 = GTK_WIDGET(gtk_builder_get_object(builder, "textview2"));
    widgets->textview3 = GTK_WIDGET(gtk_builder_get_object(builder, "textview3"));

    gtk_text_view_set_editable(GTK_TEXT_VIEW(widgets->textview3), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(widgets->textview3), FALSE);

    return widgets;
}

void text_boxes_free(text_boxes *t) {
    g_free(t);
}

int ui_run(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;

    /* 1. Initialisation de GTK */
    gtk_init(&argc, &argv);

    /* 2. Création du GtkBuilder et chargement du fichier Glade / UI */
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "ui.glade", NULL)) {
        g_critical("Impossible de charger le fichier ui.glade");
        return 1;
    }

    GtkWidget *button = GTK_WIDGET(gtk_builder_get_object(builder, "execute"));

    /* 3. Récupération de la fenêtre principale par son ID */
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));

    text_boxes* textboxes = text_boxes_new(builder);

    g_signal_connect(button, "clicked", G_CALLBACK(ui_execute_diff), textboxes);

    /* Connecter la fermeture de la fenêtre à l'arrêt de la boucle principale */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), textboxes->textview1);

    /* 4. Affichage de la fenêtre et de tous ses widgets enfants */
    gtk_widget_show_all(window);

    /* 5. Libération du builder (il n'est plus nécessaire après la création) */
    g_object_unref(builder);

    /* 6. Lancement de la boucle principale GTK */
    gtk_main();

    return 0;
}


// Fonction appelée lorsque le bouton est cliqué
void ui_execute_diff(GtkButton *button, gpointer user_data) {
    g_print("Le bouton a été cliqué !\n");
    text_boxes *widgets = (text_boxes *)user_data;
    char* ref = get_textview_buffer(widgets->textview1);
    char* src = get_textview_buffer(widgets->textview2);

    token_list* ref_tokens = token_list_create(4);
    token_list* src_tokens = token_list_create(4);
    strhash* s = strhash_create(10);

    tokenize_text(ref, s, ref_tokens);
    tokenize_text(src, s, src_tokens);

    token_list* list_diff = diff(ref_tokens, src_tokens);
    char buf[2048];
    token_list_to_string(list_diff, buf);

    GtkTextBuffer *buffer3 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->textview3));
    gchar *message = g_strdup_printf("%s", buf);
    gtk_text_buffer_set_text(buffer3, message, -1);

    token_list_free(list_diff);
    token_list_free(ref_tokens);
    token_list_free(src_tokens);
    free(ref);
    free(src);

}

char* get_textview_buffer(GtkWidget* text_view) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    return text;
}
