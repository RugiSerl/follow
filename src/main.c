#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
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

    /* 3. Récupération de la fenêtre principale par son ID */
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));

    /* Connecter la fermeture de la fenêtre à l'arrêt de la boucle principale */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* 4. Affichage de la fenêtre et de tous ses widgets enfants */
    gtk_widget_show_all(window);

    /* 5. Libération du builder (il n'est plus nécessaire après la création) */
    g_object_unref(builder);

    /* 6. Lancement de la boucle principale GTK */
    gtk_main();

    return 0;
}
