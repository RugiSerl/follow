#include "gtk/gtk.h"
#include "tokenize.h"

// Struct qui permet de mettre nos 3 textview en paramètre
// de la fonction callback (on ne peut mettre qu'un seul paramètre).
typedef struct {
    GtkWidget *textview1;
    GtkWidget *textview2;
    GtkWidget *textview3;
} text_boxes;

// Création des tags pour surligner le texte dans le buffer.
void create_diff_tags(GtkTextBuffer *buffer);

// Convertit un token en string (comme token_to_string mais sans les ERASE et INSERT)
char *token_text(token *t);

// Affiche la différence sur le textview du bas avec les tags.
void display_diff(GtkTextBuffer *buffer, token_list *diff);

// Crée et initialise les trois textview affichées.
text_boxes* text_boxes_new(GtkBuilder* builder);

// Libère les trois textviews.
void text_boxes_free(text_boxes* t);

// Fonction principale de notre interface (équivalent à un main())
int ui_run(int argc, char *argv[]);

// Récupère la chaine de caractère dans un text_view.
char* get_textview_buffer(GtkWidget* text_view);

// Fonction de callback quand le bouton execute est cliqué.
void ui_execute_diff(GtkButton *button, gpointer user_data);
