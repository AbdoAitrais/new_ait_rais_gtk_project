//
// Created by abdo on 4/28/2022.
//

#ifndef MAIN_C_INIT_ENV_MACRO_H
#define MAIN_C_INIT_ENV_MACRO_H
#include <gtk/gtk.h>
#include "../constants.h"
#include "gobject_utils.h"
#include "structures.h"
#include "UI_individu_macros.h"
#include "Statistics.h"

typedef struct {
    gint	 baselinerow;	       /*The row to align the to the baseline
                                    when valign is GTK_ALIGN_BASELINE.Default value: 0*/

    gboolean columnhomogeneous	; /*If TRUE, the columns are all the same width.
                                        Default value:FALSE*/
    gint	 columnspacing	;         /*The amount of space between two consecutive columns.*/

    gboolean rowhomogeneous	 ;   ///If TRUE, the rows are all the same height.
    gint	 rowspacing  ;       ///he amount of space between two consecutive rows.

}GridProps;

GridProps set_grid_props(gboolean colhomo,gboolean rowhomo,gint colspace,gint rowspace)
{
    GridProps props;
    props.columnhomogeneous = colhomo;
    props.rowhomogeneous = rowhomo;
    props.columnspacing = colspace;
    props.rowspacing = rowspace;


    return props;
}


/**********************************
*initialiser les propriétés du Grid.
*@param Grid utilise
*@param props propriétés du Grid
*******************************/

void macro_applyGrid(GtkGrid *Grid,GridProps props){
    if( props.rowspacing)
        gtk_grid_set_row_spacing ( Grid, props.rowspacing);
    if(props.columnhomogeneous)
        gtk_grid_set_column_homogeneous (Grid, props.columnhomogeneous);
    if(props.rowhomogeneous)
        gtk_grid_set_row_homogeneous(Grid, props.rowhomogeneous);
    if(props.columnspacing)
        gtk_grid_set_column_spacing(Grid, props.columnspacing);
    if(props.baselinerow)
        gtk_grid_set_baseline_row(Grid, props.baselinerow);


}
/**********************************
*Creer un  grid widget
*@param props propriétés du Grid
*******************************/
///create new grid widget
GtkWidget *macro_createGrid(GridProps props) {
    GtkWidget *Grid;
    Grid=gtk_grid_new ();
    macro_applyGrid(GTK_GRID(Grid),props);
    return((GtkWidget *)Grid);
}

gboolean add_individu (GtkWidget *widget,GdkEvent *event,gpointer builder)
{


    if (gtk_bin_get_child((GTK_BIN(widget)))) {
        g_printerr("\nAlready has child\n");
        return FALSE;
    }
    GtkWidget *image = gtk_image_new_from_file ("person.png");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (builder, "MainWindow"));
    gtk_container_add(GTK_CONTAINER (widget),image);

    gtk_widget_show(image);
    gint left, top;
    gtk_container_child_get(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(widget))),
                            GTK_WIDGET(widget), "left-attach",
                            &left, "top-attach", &top, NULL);
    g_print("\nadded image  top = %d, left = %d.\n", top, left);

    Individu * indiv = lire_Indiv(builder);

    g_object_set_data((GObject *) image,DATA_KEY_INDIVIDU,indiv);
    inserer_data_GObject(G_OBJECT(window),DATA_KEY_LIST_INDIVIDU,image);

    calculate_stats(builder,((Stats *) g_object_get_data(builder,DATA_STATS)));
    //Stats * stat = calculate_stats(builder);
    afficher_Stats(((Stats *) g_object_get_data(builder,DATA_STATS)));



    //TODO :: Instead of running a timeout fro the individu add it to the list
    // g_timeout_add(2000,macro_moveGrid,image);
    return FALSE;
}

void create_backgroundBox(GtkGrid *grid,GtkBuilder *builder)
{
    for (int i = 0; i < DEFAULT_MAX_ROWS; ++i) {
        for (int j = 0; j < DEFAULT_MAX_COLS; ++j) {
            GtkWidget *box = gtk_event_box_new();
            g_signal_connect (box, "button-press-event", (GCallback)add_individu, builder);

            gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(box), j, i, 1, 1);
            gtk_style_context_add_class(gtk_widget_get_style_context(box), "box");
        }
    }
}

void init_background(GtkWidget *ViewPort2,gpointer builder)
{
    GtkWidget *grid = NULL;
    GridProps gprops = set_grid_props(TRUE, TRUE, 0, 0);
    grid = macro_createGrid(gprops);
    create_backgroundBox(GTK_GRID(grid),builder);
    gtk_container_add(GTK_CONTAINER(ViewPort2),grid);
}

#endif //MAIN_C_INIT_ENV_MACRO_H
