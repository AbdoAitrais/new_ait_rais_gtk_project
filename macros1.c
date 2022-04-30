//
// Created by abdo on 3/8/2022.
//


#include <gtk/gtk.h>
#include "constants.h"

///la structure bouton
typedef struct {

    const gchar *icon_name;///nom d'icone d'apres le icon theme set
    //TODO::impliment ImageProps instead of GtkWidget *image and use its macro
    GtkWidget *image;///pour ajouter un image d'apres un lien donn�
    gboolean alwaysShowImage;///Toujours afficher l'image si elle existe
    GtkPositionType ImagePos;///La position de l'image par rapport au text


    const gchar *label;///l'�tiquette du bouton
    ///L'un de ses trois premiers element va etre appliqu�.

    ///La taille du bouton
    gint width;
    gint height;


    gboolean mnemonicLabel;///si le label est mn�monique ou pas
    gboolean underline;///si utilis� un sous-ligne dans le texte indiquera que la prochaine lettre est mnemonique

    GtkReliefStyle relief; /// le style du bordure de bouton.
} ButtonProps;


void macro_ApplyButton(GtkButton *button, ButtonProps *props) {
    if (props->mnemonicLabel && props->underline)
        gtk_button_set_use_underline(GTK_BUTTON(button), props->underline);
    if (props->label && !props->mnemonicLabel)
        gtk_button_set_label(button, props->label);
    if (props->image)
        gtk_button_set_image(button, props->image);
    if (props->alwaysShowImage)
        gtk_button_set_always_show_image(GTK_BUTTON(button), props->alwaysShowImage);
    if (props->ImagePos)
        gtk_button_set_image_position(GTK_BUTTON(button), props->ImagePos);
    if (props->relief)
        gtk_button_set_relief(GTK_BUTTON(button), props->relief);
    if ((props->width > 0) && (props->height > 0))
        gtk_widget_set_size_request(GTK_WIDGET(button), props->width, props->height);

}

GtkWidget *macro_createButton(ButtonProps *props) {
    GtkWidget *button;

    if (props->mnemonicLabel && props->label)
        button = gtk_button_new_with_mnemonic(props->label);
    else if (props->icon_name)
        button = gtk_button_new_from_icon_name(props->icon_name, GTK_ICON_SIZE_BUTTON);
    else
        button = gtk_button_new();


    macro_ApplyButton(GTK_BUTTON(button), props);
    return button;
}


typedef struct {

    gboolean active;
    ButtonProps *ButtonProps;
} ToggleButtonProps;

void macro_ApplyToggleButton(GtkWidget *togglebutton, ToggleButtonProps props) {
    if (props.active)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(togglebutton), props.active);

}

GtkWidget *macro_createToggleButton(ToggleButtonProps props) {
    GtkWidget *togglebutton = gtk_toggle_button_new();
    //TODO::Problem with mnemonic labels bcz we can't set them only can be added in creation (Gotta fix it if needed)
    if (props.ButtonProps)
        macro_ApplyButton(GTK_BUTTON(togglebutton), props.ButtonProps);

    macro_ApplyToggleButton(togglebutton, props);

    return togglebutton;
}

typedef struct {

    ToggleButtonProps props;

} CheckButtonProps;


GtkWidget *macro_createCheckButton(CheckButtonProps props) {
    GtkWidget *checkbutton = gtk_check_button_new();
    macro_ApplyButton(GTK_BUTTON(checkbutton), props.props.ButtonProps);
    macro_ApplyToggleButton(checkbutton, props.props);


    return checkbutton;
}

typedef struct {

    GtkWidget *radioGroup;///definit le groupe des bouton radio dont on ajoute la bouton (NULL par defaut)
    ButtonProps *ButtonProps;

} RadioButtonProps;

void macro_ApplyRadioButton(GtkWidget *radiobutton, RadioButtonProps props) {
    if (props.radioGroup)
        gtk_radio_button_join_group(GTK_RADIO_BUTTON(radiobutton), GTK_RADIO_BUTTON(props.radioGroup));
}

GtkWidget *macro_createRadioButton(RadioButtonProps props) {
    GtkWidget *radiobutton = gtk_radio_button_new(NULL);
    if (props.ButtonProps)
        macro_ApplyButton(GTK_BUTTON(radiobutton), props.ButtonProps);

    macro_ApplyRadioButton(radiobutton, props);
    return radiobutton;
}

void create_radio_buttons(void) {

    GtkWidget *window, *radio1, *radio2, *box, *entry;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_box_set_homogeneous(GTK_BOX (box), TRUE);

    /// Create a radio button with a label
    radio1 = gtk_radio_button_new_with_label_from_widget(NULL, "Homme");

    /// Create a radio button with a label
    /// radioGroup : an existing radio button group, or NULL if you are creating a new group.
    radio2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON (radio1), "Femme");

    /// Pack them into a box, then show all the widgets
    gtk_box_pack_start(GTK_BOX (box), radio1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX (box), radio2, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER (window), box);
    gtk_widget_show_all(window);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

}

ButtonProps *
set_button_props(const gchar *label, gboolean underline, gboolean alwaysshow, GtkPositionType imgpos, gint width,
                 gint height,
                 GtkWidget *img, GtkReliefStyle relief, const gchar *icon_name, gboolean mnemonic) {
    ButtonProps *props = (ButtonProps *) g_malloc(sizeof(ButtonProps));
    props->label = label;
    props->underline = underline;
    props->alwaysShowImage = alwaysshow;
    props->ImagePos = imgpos;
    props->width = width;
    props->height = height;
    // gtk_image_new_from_file ("facebook.png");
    props->image = img;
    props->relief = relief;
    props->icon_name = icon_name;
    props->mnemonicLabel = mnemonic;
    return props;
}

//
// Created by W0L1D on 2/15/2022.
//
#include <gtk/gtk.h>

//Cellule contient un string
typedef struct SCel {
    gchar *str;         /// string
    struct SCel *next;  /// Cellule suivante
} StrCel;

//Cellule du provider des classes CSS
typedef struct PrvCel {
    GtkStyleProvider *provider;  /// Provider du style
    guint priority;   /// Priorite de ce provider.
    struct PrvCel *next;      /// Cellule suivante
} PrvCel;


typedef struct {
    gint start;     /// margin gauche
    gint end;       /// margin droit
    gint bottom;    /// margin du bas
    gint top;       /// margin du haut
} MarginProps;


typedef struct {
    /*
     * propiete boolXXX designe que
     * la propriete XXX doit etre initialise
     */
    gboolean boolTooltipTxt;
    gboolean boolAlign;
    gboolean boolOpacity;

    MarginProps *marginPrp;    /// les margin de la fenetre
    const gchar *tooltipText;  /// text a afficher en hover
    const gchar *name;         /// nom du widget
    GtkAlign align;        /// alignement du widget
    double opacity;      /// visibilite entre 0 et 1
} WidgetProps;


typedef struct {
    /*
     * propiete boolXXX designe que
     * la propriete XXX doit etre initialise
     */
    gboolean boolTitle;
    gboolean boolSize;
    gboolean boolResizable;
    gboolean boolIcon;
    gboolean boolPosition;
    gboolean boolGravity;

    WidgetProps *widgetProps; ///propietes h�ritee du widget
    const gchar *title;      /// titre de la fenetre
    gint width;       /// la largeur de la fenetre
    gint height;      /// la hauteur de la fenetre
    gboolean resizable;   /// users can resize the window
    GdkPixbuf *icon;       /// l'icon de la fenetre
    GtkWindowPosition position;    /// position initiale de la fenetre
    GdkGravity gravity;     /// gravite de la fenetre
} WindowProps;

typedef struct {
    /*
     * propiete boolXXX designe que
     * la propriete XXX doit etre initialise
     */
    gboolean boolHomogeneous;
    gboolean boolBaselinePos;

    gboolean homogeneous;         /// the number of pixels to place by default between children
    GtkOrientation orientation;         /// orientation du Box (obligatoir)
    gint spacing;             /// propriete obligatoire
    GtkBaselinePosition baselinePosition;    /// where to put the baseline inside the extra available space.
} BoxProps;

typedef struct {
    /*
     * propiete boolXXX designe que
     * la propriete XXX doit etre initialise
     */
    gboolean boolBoxPrp;
    gboolean boolBBStyle;

    BoxProps boxPrp;         /// proprietees herite du Box
    GtkOrientation orientation;   /// orientation du box
    GtkButtonBoxStyle bbStyle;      /// style du Box button
} BtnBoxProps;


typedef struct {
    StrCel *classes;    /// liste des classes attribues
    PrvCel *providers; /// liste des providers du style
} StyleContextProps;


typedef struct {
    /*
     * propiete boolXXX designe que
     * la propriete XXX doit etre initialise
     */
    gboolean boolPackDirect;
    gboolean boolChdPackDirect;

    /// proprietees herite du widget
    WidgetProps *widgetPrp;

    /// Determines how widgets should be packed
    /// inside menubars and menuitems contained in menubars.
    GtkPackDirection packDirect;
    /// Determines how children widgets should be packed
    GtkPackDirection chdPackDirect;
} MenuBarProps;

typedef struct {
    WidgetProps *widgetPrp;        /// proprietees herite du widge
    const gchar *label;            /// label du menu item
    gboolean mnemonicLabel;     /// si le label est mn�monique ou pas
    //ADDED ::
} MenuItemProps;


typedef union {
    gchar *str; // file | ressource | Stock | Icon name (depricated)
    GtkIconSet *iconSet; //(depricated)
    GdkPixbuf *pixbuf;
    GdkPixbufAnimation *animation;
    cairo_surface_t *cairoSurface;
    GIcon *gIcon;
} ImageContentUN;

//starting value 1000
typedef enum {
    GTK_IMAGE_RESOURCE = 1000,   /// image from resource (1000)
    GTK_IMAGE_FILE              /// image from file (1001)
} MoreImageType;

typedef union {
    int type;               // lecture du type
    GtkImageType bType;     // types d'image predefinies dans Gtk
    MoreImageType mType;    // types customise
} ImageTypeUN;

typedef struct {
    /// proprietees herite du widget
    WidgetProps *widgetPrp;
    // type d'image a creer
    ImageTypeUN storageType;
    //source d'Image
    ImageContentUN imgContent;
    //taille predifinie si necessaire
    GtkIconSize iconSize;
} ImageProps;


//ADDED ::
typedef struct {
    MenuItemProps itemProps;
    ImageProps imagePrp;
} ImageMnItemProps;


/************************************************************
 * initialisation des margin d'une widget
 * @param widget widget utilisee
 * @param props les margin
 */
void macro_applyMargin(GtkWidget *widget, MarginProps props) {
    gtk_widget_set_margin_top(widget, props.top);
    gtk_widget_set_margin_start(widget, props.start);
    gtk_widget_set_margin_top(widget, props.bottom);
    gtk_widget_set_margin_end(widget, props.end);
}

/*********************************************************
 * initialiser la widget
 * @param widget widget a initialiser
 * @param props prpriete du widget
 */
void macro_applyWidget(GtkWidget *widget, WidgetProps props) {
    if (props.marginPrp)
        macro_applyMargin(widget, *props.marginPrp);
    if (props.boolTooltipTxt)
        gtk_widget_set_tooltip_text(widget, props.tooltipText);
    if (props.boolOpacity)
        gtk_widget_set_opacity(widget, props.opacity);
    if (props.name)
        gtk_widget_set_name(widget, props.name);
}

/****************************************************************
 * initialiser le style context par les classes et providers du style css
 * @param pContext context a initialiser
 * @param props listes des classes et providers
 */
void macro_applyStyle(GtkStyleContext *pContext, StyleContextProps props) {
    StrCel *curr = props.classes;
    while (curr) {
        gtk_style_context_add_class(pContext, curr->str);
        curr = curr->next;
    }
    PrvCel *prvCurr = props.providers;
    while (prvCurr) {
        gtk_style_context_add_provider(pContext,
                                       prvCurr->provider,
                                       prvCurr->priority);
        prvCurr = prvCurr->next;
    }
}

/*****************************************************************
 * creer un style context initialiser
 * @param props proprietes du style cre
 * @return StyleContext initialise
 */
GtkStyleContext *macro_createStyle(StyleContextProps props) {
    GtkStyleContext *style = gtk_style_context_new();
    macro_applyStyle(style, props);
    return style;
}


/***************************************************************
 * initialiser une image
 * @param img image a initialiser
 * @param props proprietes de l'image (source de l'image...)
 */
void macro_applyImage(GtkImage *img, ImageProps props) {
    if (props.widgetPrp)
        macro_applyWidget(GTK_WIDGET(img), *props.widgetPrp);
    switch (props.storageType.type) {
        case GTK_IMAGE_PIXBUF:
            gtk_image_set_from_pixbuf(img, props.imgContent.pixbuf);
            break;
        case GTK_IMAGE_ANIMATION:
            gtk_image_set_from_animation(img, props.imgContent.animation);
            break;
        case GTK_IMAGE_GICON:
            gtk_image_set_from_gicon(img,
                                     props.imgContent.gIcon, props.iconSize);
            break;
        case GTK_IMAGE_SURFACE:
            gtk_image_set_from_surface(img, props.imgContent.cairoSurface);
            break;
        case GTK_IMAGE_ICON_NAME:
            gtk_image_set_from_icon_name(img,
                                         props.imgContent.str, props.iconSize);
            break;
        case GTK_IMAGE_STOCK:
            gtk_image_set_from_stock(img, props.imgContent.str, props.iconSize);
            break;
        case GTK_IMAGE_ICON_SET:
            gtk_image_set_from_icon_set(img,
                                        props.imgContent.iconSet, props.iconSize);
            break;
        case GTK_IMAGE_FILE:
            gtk_image_set_from_file(img, props.imgContent.str);
            break;
        case GTK_IMAGE_RESOURCE:
            gtk_image_set_from_resource(img, props.imgContent.str);
            break;
    }
}

/**********************************************
 * creer et initialiser l'image
 * @param props proprietes de l'image
 * @return l'image cree
 */
GtkWidget *macro_createImage(ImageProps props) {
    GtkWidget *img = gtk_image_new();
    macro_applyImage(GTK_IMAGE(img), props);
    return img;
}


void macro_applyMenuBar(GtkMenuBar *bar, MenuBarProps props) {
    if (props.widgetPrp)
        macro_applyWidget(GTK_WIDGET(bar), *props.widgetPrp);
    if (props.boolPackDirect)
        gtk_menu_bar_set_pack_direction(GTK_MENU_BAR(bar), props.packDirect);
    if (props.boolChdPackDirect)
        gtk_menu_bar_set_child_pack_direction(GTK_MENU_BAR(bar), props.chdPackDirect);
}

GtkWidget *macro_createMenuBar(MenuBarProps props) {
    GtkWidget *bar = gtk_menu_bar_new();
    macro_applyMenuBar(GTK_MENU_BAR(bar), props);
    return bar;
}


void macro_applyMenuItem(GtkMenuItem *pItem, MenuItemProps props) {
    if (props.widgetPrp)
        macro_applyWidget(GTK_WIDGET(pItem), *props.widgetPrp);
    if (props.label)
        gtk_menu_item_set_label(pItem, props.label);
    gtk_menu_item_set_use_underline(pItem, props.mnemonicLabel);
}

GtkWidget *macro_createMenuItem(MenuItemProps props) {
    GtkWidget *item = gtk_menu_item_new();
    macro_applyMenuItem(GTK_MENU_ITEM(item), props);
    return item;
}


static
void macro_addImageToMenuItem(GtkMenuItem *pItem, ImageProps imgProps) {

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    GtkWidget *label = gtk_label_new(gtk_menu_item_get_label(pItem));

    gtk_container_foreach(GTK_CONTAINER (pItem), (void *) gtk_widget_destroy, NULL);

    gtk_box_pack_start(GTK_BOX (box), macro_createImage(imgProps), FALSE, FALSE, 2);
    gtk_box_pack_end(GTK_BOX (box), label, TRUE, TRUE, 2);

    gtk_container_add(GTK_CONTAINER (pItem), box);
}


GtkWidget *macro_createMenuitemWithImage(ImageMnItemProps props) {
    GtkWidget *menuItem = macro_createMenuItem(props.itemProps);
    if (props.imagePrp.storageType.type != GTK_IMAGE_EMPTY)
        macro_addImageToMenuItem(GTK_MENU_ITEM(menuItem), props.imagePrp);
    return menuItem;
}


/*************************************************************
 * initialiser une fenetre
 * @param window fenetre a initialiser
 * @param props propriete de l'image
 */
void macro_applyWindow(GtkWindow *window, WindowProps props) {
    if (props.widgetProps) /// initialisation des propietes heritees du widget
        macro_applyWidget(GTK_WIDGET(window), *props.widgetProps);
    if (props.boolTitle) /// initialisation du Titre
        gtk_window_set_title(GTK_WINDOW(window), props.title);
    if (props.boolSize) /// initialisation du largeur et hauteur
        gtk_window_set_default_size(GTK_WINDOW(window),
                                    props.width, props.height);
    if (props.boolResizable) /// initialisation redimensionnabilite du fenetre
        gtk_window_set_resizable(GTK_WINDOW(window), props.resizable);
    if (props.boolIcon) /// initialisation de l'icon du fenetre
        gtk_window_set_icon(GTK_WINDOW(window), props.icon);
    if (props.boolPosition) /// initialisation du position de la fenetre
        gtk_window_set_position(GTK_WINDOW(window), props.position);
    if (props.boolGravity) /// initialisation de la gravite
        gtk_window_set_gravity(GTK_WINDOW(window), props.gravity);
}

/**********************************************************************
 * creer et initialiser une fenetre
 * @param app application utilisee pour creer la fentre
 * @param props propietes de la fenetre
 * @return fenetre cree
 */
GtkWidget *macro_createWindow(GtkApplication *app, WindowProps props) {
    GtkWidget *window;
    window = gtk_application_window_new(app);
    macro_applyWindow(GTK_WINDOW(window), props);
    return window;
}

/****************************************************
 * initialiser le box
 * @param box box a initialiser
 * @param props propietes du box
 */
void macro_applyBox(GtkBox *box, BoxProps props) {
    if (props.boolHomogeneous)
        gtk_box_set_homogeneous(GTK_BOX(box),
                                props.homogeneous);
    if (props.boolBaselinePos)
        gtk_box_set_baseline_position(GTK_BOX(box),
                                      props.baselinePosition);
}

/******************************************
 * creer et initialiser le box
 * @param props propietes du box
 * @return box cree
 */
GtkWidget *macro_createBox(BoxProps props) {
    GtkWidget *box = gtk_box_new(props.orientation, props.spacing);
    macro_applyBox(GTK_BOX(box), props);
    return box;
}


/*********************************************************
 * initialiser le ButtonBox
 * @param bbox ButtonBox a initialiser
 * @param props propietes du ButtonBox
 */
void macro_applyBtnBox(GtkButtonBox *bbox, BtnBoxProps props) {
    if (props.boolBoxPrp) /// initialisation des propietes heritees du Box
        macro_applyBox(GTK_BOX(bbox), props.boxPrp);
    if (props.boolBBStyle)
        gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),
                                  props.bbStyle);
}


/*************************************************
 * creer et initialiser le ButtonBox
 * @param props propietes du ButtonBox
 * @return ButtonBox cree
 */
GtkWidget *macro_createBtnBox(BtnBoxProps props) {
    GtkWidget *bbox = gtk_button_box_new(props.orientation);
    macro_applyBtnBox(GTK_BUTTON_BOX(bbox), props);
    return bbox;
}

typedef struct wdCel {
    GtkWidget *widget;
    gint x, y;
    struct wdCel *next;
} ChildCel;


typedef struct MenuCel {
    ImageMnItemProps itemProps;// etiquette de cellule
    GCallback gCallback;

    struct MenuCel *sousMenu;//pointeur sur le sousmenu du item il m�me
    struct MenuCel *svt;//pointeur sur l'�l�ment suivant de menu entier
} MenuCel;// definition de item




GtkWidget *macro_autoCreateSousMenu(MenuCel *menuCel) {
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *menuItem;
    MenuCel *curr = menuCel;

    while (curr) {
        //init menu item
        menuItem = macro_createMenuitemWithImage(curr->itemProps);
        g_print("%s\n", curr->itemProps.itemProps.label);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
        g_signal_connect(menuItem, "activate", G_CALLBACK(curr->gCallback), NULL);

        //create submenu if it exists
        if (curr->sousMenu)
            gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem),
                                      macro_autoCreateSousMenu(curr->sousMenu));
        curr = curr->svt; // go to the next menu item
    }

    return menu;
}


GtkWidget *macro_autoCreateMenuBar(MenuCel *menuCel) {
    MenuBarProps props = {FALSE, FALSE, NULL, 0, 0};
    GtkWidget *menuBar = macro_createMenuBar(props);
    MenuCel *curr = menuCel;

    GtkWidget *sMenu = NULL, //Sous menu
    *mItem = NULL; // menu item

    while (curr) {
        //init menu item
        mItem = macro_createMenuitemWithImage(curr->itemProps);
        //si menu item a sous menu
        if (curr->sousMenu) {
            sMenu = macro_autoCreateSousMenu(curr->sousMenu);

            gtk_menu_item_set_submenu(GTK_MENU_ITEM(mItem), sMenu);
        }
        gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), mItem);
        //passer a la cellule suivant
        curr = curr->svt;
    }

    return menuBar;
}

void macro_initMenuBarPrp(MenuBarProps *props) {
    props->widgetPrp = NULL;
    props->boolPackDirect = TRUE;
    props->boolPackDirect = GTK_PACK_DIRECTION_LTR;
    props->boolChdPackDirect = FALSE;
}


/********** structures ************/


typedef struct cel {
    gint Id;//Indentifiant du virus(peut �tre coleur par exemple rgb Ox......) // it can be gchar* too choose one
    const gchar *nom;// le nom associ� � le virus
    gfloat prctContam;// le pourcentage de contamination de virus
    gfloat prctMortel; // le pourcentage de mortalite de virus
    guint cercleDeContam;// le rayon ou bien la cercle de contamination
} Virus;

typedef struct {
    gint x;// la position selon les lignes
    gint y;// la position selon les colonnes
} Coord;// les cooordonn�es d'une entit� dans l'interface

typedef enum {
    GENETIQUEMENT_FAIBLE,
    GENETIQUEMENT_FRAGILE,
    GENETIQUEMENT_MOYEN,
    GENETIQUEMENT_FORT
} Genitique;

typedef enum {
    ARTERIELLE_NORMAL,
    ARTERIELLE_HYPERTENDU,
    ARTERIELLE_HYPERTENSION_FORTE,
} Tension;

typedef enum {
    DIABETE_NORMAL,
    DIABETE_MODERE,
    DIABETE_AVANCE
} Diabete;

typedef enum {
    CARDIAQUE_NORMAL,
    CARDIAQUE_MALADE,
    CARDIAQUE_SEVERE
} Cardiaque;

typedef enum {
    POUMONS_SEIN,
    POUMONS_MALADE,
    POUMONS_GRAVE
} Poumons;

typedef struct {
    Genitique genetic;
    Tension tension;
    Diabete diabete;
    Cardiaque cardiac;
    Poumons poumons;
} Sante;

typedef enum {
    GENRE_UNSPECIFIED,
    GENRE_MALE,
    GENRE_FEMALE
} Genre;

typedef enum {
    AGE_KIDS,
    AGE_TEENS,
    AGE_YOUTH,
    AGE_ADULT,
    AGE_OLD
} Age;

typedef enum {
    AGE_INF12,
    AGE_BETWEEN1225,
    AGE_BETWEEN2540,
    AGE_BETWEEN4065,
    AGE_SUP65
} Age2;

// si vous voulez faire deux entites (personne,animal) mais je pense c'est pas le peine
typedef struct {

    Genre gender;// le sexe de Individu
    ///gint age;//l'�ge de Individu
    Age categorie;//la categorie associ� � le Individu selon son �ge
    Coord pos;//les coordonn�es o� se pr�sente le Individu
    Sante health;// l'�tat sanitaire associ� � le Individu
    GList *VirusList;// liste des virus qui a le Individu
} Individu;// peut �tre animal,personne...


//struct struct used for some elements of toolbar

typedef struct{
    gint check;// if check=1 mode play=TRUE
    gint speed;//
    guint id;// id of g_timeout_add

}maStr;


/*********** project functions ****************/

void loadCSS(GtkWidget *window)
{
    GFile *css_gFile = g_file_new_for_path("style.css");
    GtkCssProvider *cssProvider = gtk_css_provider_new();

    gtk_css_provider_load_from_file(cssProvider, css_gFile, 0);

    gtk_style_context_add_provider_for_screen(gtk_widget_get_screen(window),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void set_css(gpointer builder)
{
    GtkWidget *fenetre_principale = NULL;
    GtkWidget *menu = NULL;
    GtkWidget *stack = NULL;
    GtkWidget *toolbar = NULL;
    GtkWidget *switcher = NULL;
    GtkWidget *comboBox1 = NULL;
    GtkWidget *comboBox2 = NULL;
    GtkWidget *comboBox3 = NULL;
    GtkWidget *comboBox4 = NULL;
    GtkWidget *comboBox5 = NULL;

    GtkWidget *textView1 = NULL;

    GtkWidget *scale1 = NULL;
    GtkWidget *scale2 = NULL;
    GtkWidget *button = NULL;



    fenetre_principale = GTK_WIDGET(gtk_builder_get_object (builder, "MainWindow"));
    menu = GTK_WIDGET(gtk_builder_get_object (builder, "MenuBar"));

    button = GTK_WIDGET(gtk_builder_get_object (builder, "subutton"));
    stack = GTK_WIDGET(gtk_builder_get_object (builder, "stack"));
    switcher = GTK_WIDGET(gtk_builder_get_object (builder, "switch"));
    comboBox1 = GTK_WIDGET(gtk_builder_get_object (builder, "comboBoxGenitiques"));
    comboBox2 = GTK_WIDGET(gtk_builder_get_object (builder, "comboBoxTension"));
    comboBox3 = GTK_WIDGET(gtk_builder_get_object (builder, "comboBoxDiabete"));
    comboBox4 = GTK_WIDGET(gtk_builder_get_object (builder, "comboBoxCardiaque"));
    comboBox5 = GTK_WIDGET(gtk_builder_get_object (builder, "comboBoxPoumons"));
    scale1 = GTK_WIDGET(gtk_builder_get_object (builder, "scale1"));
    scale2 = GTK_WIDGET(gtk_builder_get_object (builder, "scale2"));
    textView1 = GTK_WIDGET(gtk_builder_get_object (builder, "TextView"));
    toolbar = GTK_WIDGET(gtk_builder_get_object (builder, "toolbar"));



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

    gtk_style_context_add_class(gtk_widget_get_style_context(scale1), "scale");
    gtk_style_context_add_class(gtk_widget_get_style_context(scale2), "scale");
    gtk_style_context_add_class(gtk_widget_get_style_context(textView1), "textView");

    gtk_style_context_add_class(gtk_widget_get_style_context(button), "button2");
}


gboolean macro_moveGrid(gpointer image) {

    gboolean returnVal = TRUE;

    GtkEventBox *boxSrc = GTK_EVENT_BOX (gtk_widget_get_parent(image));
    gint top, left;

    gtk_container_child_get(GTK_CONTAINER (gtk_widget_get_parent(GTK_WIDGET(boxSrc))),
                            GTK_WIDGET (boxSrc), "left-attach",
                            &left, "top-attach", &top, NULL)    ;

//    printf("\ninit pos : top = %d left = %d\n", top, left);

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
            if ((top + 1) < MAXrow) {
                top++;
                break;
            }
        }
        case 3: {
            if ((left + 1) < MAXcol) {
                left++;
                break;
            } else
                return returnVal;
        }
        default:
            return returnVal;
    }

//    printf("\nnew pos : top = %d left = %d. ", top, left);
    boxDst = gtk_grid_get_child_at(
            GTK_GRID(gtk_widget_get_parent(GTK_WIDGET(boxSrc))), left, top);
    g_assert(GTK_IS_EVENT_BOX(boxDst));
    if (boxDst && !(gtk_bin_get_child(GTK_BIN(boxDst))))// si la case ne contient pas bnadem on ajoute
    {
        /*
         * GObject is a reference counted type. Reference counting is a form of garbage collection.
         * Every time you take ownership of an object instance, you must acquire a reference to it�using g_object_ref().
         * Once you drop the ownership of that instance, you must release the reference you acquired�using g_object_unref().
         * If you get an object back from a function, and the documentation says �transfer full� or �newly allocated� or �a new reference�,
         * then you need to call g_object_unref() to release the reference you�re given.
         * The API reference will always tell you if you�re dealing with something you own, or just a pointer to something that
         * is owned by something else.
         */
//        printf("is valid\n\n");
        g_object_ref(child);
        gtk_container_remove(GTK_CONTAINER(boxSrc), child);
        ///child->pos.x = top;
        ///child->pos.y=left;
        gtk_container_add(GTK_CONTAINER(boxDst), child);
        g_object_unref(child);
    }
    return returnVal;

}

void afficher_virus(Virus *vir)
{
    printf("L'id = %d\n",vir->Id);
    printf("Le nom = %s\n",vir->nom);
    printf("Le pourcentage de contamination = %f\n",vir->prctContam);
    printf("Le taux de mortalite = %f\n",vir->prctMortel);
    printf("Le cercle de contamination = %d\n",vir->cercleDeContam);
}

void inserer_data_GObject(GObject * object,gchar * key,gpointer data)
{
    GList * l = g_object_get_data(object,key);

    l = g_list_append(l,data);

    g_object_set_data(object,key,l);

}

void Afficher_VirusList(GList * VirusList)
{
    GList *crt = VirusList;
    while (crt)
    {
        afficher_virus((Virus *)crt->data);
        crt = crt->next;
    }
}

void Afficher_individu(Individu * indiv)
{
    if(indiv)
    {
        g_print("individu gender = %d\n",indiv->gender);
        g_print("individu genetic = %d\n",indiv->health.genetic);
        g_print("individu tension = %d\n",indiv->health.tension);
        g_print("individu diabete = %d\n",indiv->health.diabete);
        g_print("individu cardiac = %d\n",indiv->health.cardiac);
        g_print("individu poumons = %d\n",indiv->health.poumons);
        g_print("individu categorie = %d\n",indiv->categorie);
        Afficher_VirusList(indiv->VirusList);
    }
}



Individu *lire_Indiv(gpointer builder);
gboolean add_individu (GtkWidget *widget,GdkEvent *event,gpointer builder)
{

    const gchar *labels[] = {
            ("Show Bookmarks"),
            ("Edit Bookmarks"),
            ("Previous Item"),
            ("Next Item"),
    };
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


    //TODO :: Instead of running a timeout fro the individu add it to the list
   // g_timeout_add(2000,macro_moveGrid,image);
    return FALSE;
}


void iterateSingleIndividu( gpointer data, gpointer user_data) {
    if (PLAY_MODE)
        macro_moveGrid(data);
}



gboolean iterateIndividusList(gpointer data) {
    GTK_IS_WIDGET(data);

    GList *pers = g_object_get_data(data, DATA_KEY_LIST_INDIVIDU);

    g_list_foreach(pers, iterateSingleIndividu, NULL);
    //note -->same function of star_game

    // TODO :: test if the play mode is started
    //  GET PLAY MODE BOOLEAN FROM TOOLBAR
   /* if (PLAY_MODE)
        g_timeout_add(*speed, iterateIndividusList, data);
    return FALSE;*/
}




void pause_game(GtkWidget *btn, maStr *info)
{
    if (info->check==0) {
        PLAY_MODE=FALSE;
        g_source_remove (info->id);
        PLAY_MODE=FALSE;
        info->check=1;
        //gtk_widget_set_sensitive(btn,FALSE);
    }


}

void start_game(  GtkWidget *btn, maStr *info)
{
        if (info->check){
        PLAY_MODE=TRUE;
        GtkWidget *fenetre_principale= GTK_WINDOW(gtk_widget_get_toplevel(btn));
        info->id=g_timeout_add(info->speed, iterateIndividusList, fenetre_principale );
        info->check=0;

        //gtk_widget_set_sensitive(btn,FALSE);

    }
}

void about_game(GtkWidget *btn,gpointer data)
{
        GtkWidget *content_area,*dialog=gtk_dialog_new_with_buttons("About",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,
                                                                        GTK_RESPONSE_OK,NULL);
        content_area=gtk_dialog_get_content_area(dialog);
        GtkWidget *image_about=gtk_image_new_from_file ("About.png");
        gtk_widget_set_size_request(dialog,300,300);
        gtk_window_set_position(dialog,GTK_WIN_POS_CENTER);
        gtk_container_add(GTK_CONTAINER(content_area),image_about);
        gtk_widget_show_all(dialog);
        gint reponse=gtk_dialog_run(dialog);
        gtk_widget_destroy(dialog);
}

void limit_speed()
{
     GtkWidget *limit;
        gpointer data=NULL;
     limit = gtk_message_dialog_new (GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_CLOSE,
        "limit of speed \n  please decrease Speed ");

   /* Affichage de la boite de message */
    gtk_dialog_run(GTK_DIALOG(limit));

    /* Destruction de la boite de message */
    gtk_widget_destroy(limit);

    return ;


}
void decreaseSpeed( GtkWidget *btn,maStr *info)
{

    gtk_widget_set_sensitive(btn,TRUE);
     if(info->speed > 0 ){
        info->speed =  info->speed - (gint)100;
        pause_game(  btn, info);
        start_game(  btn, info );
     }
     else{
          g_print("\n limit of speed\n" );
          limit_speed();
     }
}

void increaseSpeed(  GtkWidget *btn, maStr *info )
{
        info->speed =  info->speed + (gint)100;
        pause_game(  btn, info);
        start_game(  btn, info);


}




void mvToolBar(GtkWidget *btn1,GtkWidget *btn3,GtkWidget *btn4,GtkWidget *btn5 ,maStr *info){


 g_signal_connect (GTK_TOOL_BUTTON(btn3), "clicked", (GCallback)pause_game, info);
 g_signal_connect (GTK_TOOL_BUTTON(btn1), "clicked", (GCallback)start_game, info);
 g_signal_connect (GTK_TOOL_BUTTON(btn4), "clicked", (GCallback) increaseSpeed, info);
 g_signal_connect (GTK_TOOL_BUTTON(btn5), "clicked", (GCallback) decreaseSpeed, info);
}

 void quit_game(GtkWidget* widget)
{

        gpointer data=NULL;
        GtkWidget *ask = gtk_message_dialog_new (GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        " Do you really want\ndrop the game ?");

    /*   answer */
    switch(gtk_dialog_run(GTK_DIALOG(ask)))
    {
        case GTK_RESPONSE_YES:
            /* yes ->   quit application */
            gtk_main_quit();
            break;
        case GTK_RESPONSE_NO:
            /* no -> on destroy dialog */
            gtk_widget_destroy(ask);
            break;
    }

    return ;
}