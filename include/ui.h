#include "gtk/gtk.h"
#include "tokenize.h"

// Struct qui permet de mettre nos 3 textview en paramètre
// de la fonction callback (on ne peut mettre qu'un seul paramètre).
typedef struct {
    GtkWidget *textview1;
    GtkWidget *textview2;
    GtkWidget *textview3;
    GtkWindow* window;
} ui_state;

// Création des tags pour surligner le texte dans le buffer.
void create_diff_tags(GtkTextBuffer *buffer);

// Convertit un token en string (comme token_to_string mais sans les ERASE et INSERT)
char *token_text(token *t);

// Affiche la différence sur le textview du bas avec les tags.
void display_diff(GtkTextBuffer *buffer, token_list *diff);

// Crée et initialise les trois textview affichées.
ui_state* ui_state_new(GtkBuilder* builder, GtkWindow* window);

// Libère les trois textviews.
void ui_state_free(ui_state* t);

// Fonction principale de notre interface (équivalent à un main())
int ui_run(int argc, char *argv[]);

// Récupère la chaine de caractère dans un text_view.
char* get_textview_buffer(GtkWidget* text_view);

// Fonction de callback quand le bouton execute est cliqué.
void ui_execute_diff(GtkButton *button, gpointer user_data);

// Ouvre un contexte pour sélectionner un fichier et renvoie le chemin du fichier sélectionné.
// Si le choix a été abandonné, retourne NULL.
char* open_file_dialog(GtkWindow* window);

// Fonction de callback quand le bouton loadref est cliqué.
void load_ref(GtkButton *button, gpointer user_data);

// Fonction de callback quand le bouton loadsrc est cliqué.
void load_src(GtkButton *button, gpointer user_data);

// Fonction de callback quand le bouton À propos est cliqué.
void open_apropos(GtkButton *button, gpointer user_data);
