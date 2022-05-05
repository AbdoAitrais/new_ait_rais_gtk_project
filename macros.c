
//
// Created by abdo on 3/8/2022.
//


#include "by Abderrahman/structures.h"


/*********** project functions ****************/

void loadCSS(GtkWidget *window) {
    GFile *css_gFile = g_file_new_for_path("style.css");
    GtkCssProvider *cssProvider = gtk_css_provider_new();

    gtk_css_provider_load_from_file(cssProvider, css_gFile, 0);

    gtk_style_context_add_provider_for_screen(gtk_widget_get_screen(window),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void set_css(gpointer builder) {
    GtkWidget *fenetre_principale = NULL;
    GtkWidget *menu = NULL;
    GtkWidget *stack = NULL;
    GtkWidget *switcher = NULL;
    GtkWidget *comboBox1 = NULL;
    GtkWidget *comboBox2 = NULL;
    GtkWidget *comboBox3 = NULL;
    GtkWidget *comboBox4 = NULL;
    GtkWidget *comboBox5 = NULL;

    GtkWidget *scale2 = NULL;
    GtkWidget *button = NULL;


    fenetre_principale = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
    menu = GTK_WIDGET(gtk_builder_get_object(builder, "MenuBar"));

    button = GTK_WIDGET(gtk_builder_get_object(builder, "subutton"));
    stack = GTK_WIDGET(gtk_builder_get_object(builder, "stack"));
    switcher = GTK_WIDGET(gtk_builder_get_object(builder, "switch"));
    comboBox1 = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxGenitiques"));
    comboBox2 = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxTension"));
    comboBox3 = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxDiabete"));
    comboBox4 = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxCardiaque"));
    comboBox5 = GTK_WIDGET(gtk_builder_get_object(builder, "comboBoxPoumons"));
    scale2 = GTK_WIDGET(gtk_builder_get_object(builder, "scale2"));


    loadCSS(fenetre_principale);

    gtk_style_context_add_class(gtk_widget_get_style_context(fenetre_principale), "class1");
    gtk_style_context_add_class(gtk_widget_get_style_context(menu), "menu");
    gtk_style_context_add_class(gtk_widget_get_style_context(stack), "stack");
    gtk_style_context_add_class(gtk_widget_get_style_context(switcher), "switch");
    gtk_style_context_add_class(gtk_widget_get_style_context(comboBox1), "comboBox");
    gtk_style_context_add_class(gtk_widget_get_style_context(comboBox2), "comboBox");
    gtk_style_context_add_class(gtk_widget_get_style_context(comboBox3), "comboBox");
    gtk_style_context_add_class(gtk_widget_get_style_context(comboBox4), "comboBox");
    gtk_style_context_add_class(gtk_widget_get_style_context(comboBox5), "comboBox");

    gtk_style_context_add_class(gtk_widget_get_style_context(scale2), "scale");


    gtk_style_context_add_class(gtk_widget_get_style_context(button), "button2");
}


gboolean macro_moveGrid(GtkImage *image) {

    gboolean returnVal = TRUE;

    GtkEventBox *boxSrc = GTK_EVENT_BOX (gtk_widget_get_parent(image));
    gint top, left;

    gtk_container_child_get(GTK_CONTAINER (gtk_widget_get_parent(GTK_WIDGET(boxSrc))),
                            GTK_WIDGET (boxSrc), "left-attach",
                            &left, "top-attach", &top, NULL);


    g_assert(GTK_IS_EVENT_BOX(boxSrc));

    GtkWidget *child = gtk_bin_get_child(GTK_BIN(boxSrc));
    if (!child) {
        g_printerr("child does not exist");
        return returnVal;
    }
    gint pas = g_random_int_range(0, 4);
    GtkWidget *boxDst;


    /*
				    0
				    ^
               1 <     > 3
				    v
				    2
    */

    switch (pas) {
        case 0: {
            if ((top - 1) >= 0) {
                top--;
                break;
            }
        }
        case 1: {
            if ((left - 1) >= 0) {
                left--;
                break;
            }
        }
        case 2: {
            if ((top + 1) < DEFAULT_MAX_ROWS) {
                top++;
                break;
            }
        }
        case 3: {
            if ((left + 1) < DEFAULT_MAX_COLS) {
                left++;
                break;
            } else
                return returnVal;
        }
        default:
            return returnVal;
    }

    boxDst = gtk_grid_get_child_at(
            GTK_GRID(gtk_widget_get_parent(GTK_WIDGET(boxSrc))), left, top);
    g_assert(GTK_IS_EVENT_BOX(boxDst));

    /// si la case ne contient pas bnadem on ajoute
    if (boxDst && !(gtk_bin_get_child(GTK_BIN(boxDst)))) {
        g_object_ref(child);
        gtk_container_remove(GTK_CONTAINER(boxSrc), child);
        gtk_container_add(GTK_CONTAINER(boxDst), child);
        g_object_unref(child);
    }
    return returnVal;

}


Individu *lire_Indiv(gpointer builder);




void iterateSingleIndividu(gpointer data, gpointer builder) {

    Stats *stat = ((Stats *) g_object_get_data(builder, DATA_KEY_STATS));

    if (PLAY_MODE) {
        Individu *individu = (Individu *) g_object_get_data(G_OBJECT(data), DATA_KEY_INDIVIDU);

        if ((individu->hp <= 0) && (individu->hp != -99)) {
            GError * err = NULL;

            GdkPixbuf * gdkPixbuf = gdk_pixbuf_new_from_file("../pic/9ber50.png",&err);
            gtk_image_set_from_pixbuf(data,gdkPixbuf);

            stat->deaths++;
            show_Stats(builder,stat);
            individu->hp = -99;

        } else if (individu->hp > 0){
            macro_moveGrid(data);

            individu->hp += individu->damageTaken;
            damage_to_Virus(individu);

            g_list_foreach(individu->virusList, (GFunc) contaminate_indivCercleSingleVrs, data);
        }


    }
}

gboolean iterateIndividusList(gpointer builder) {
    GTK_IS_WIDGET(builder);
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, BUILDER_ID_MAIN_WINDOW));

    GList *pers = g_object_get_data(G_OBJECT(window), DATA_KEY_LIST_INDIVIDU);

    g_list_foreach(pers, iterateSingleIndividu, builder);

    g_timeout_add(PLAY_SPEED, iterateIndividusList, builder);

    return FALSE;
}


