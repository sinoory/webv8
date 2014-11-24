#include <gtk/gtk.h>
#include "HistoryMng.h"
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>

sqlite3 *db;  
extern sqlite3 *db;  

int timeindex = 0;

enum{
    PIXBUF_COL,
    TEXT_COL1
};

typedef struct _HS_HistoryItem{
    gint index;
    gchar uri[256];
    gchar  title[256];
    gchar favicon_path[256];
//    HS_timer timer;
} HS_HistoryItem;

typedef struct _HS_HistoryData{
    HS_HistoryItem history_item_arrary[256];
    gint current_index;
} HS_HistoryData;

int sql_callback(void *arg, int nr, char **values, char **names);  

void HS_init(BrowserWindow *window)
{
    char tmp_str[256] = {0};  

    sprintf(tmp_str,"mkdir -p %s",HISTORY_DB_PATH);
    system(tmp_str);
    
    memset(tmp_str, '\0', 256);  
    sprintf(tmp_str,"%s/history.db",HISTORY_DB_PATH);
    sqlite3_open(tmp_str, &db); 

    memset(tmp_str, '\0', 256);  
    strcpy(tmp_str, "create table tb(id INTEGER PRIMARY KEY, url TEXT, title TEXT, favicon_path TEXT)");  
    sqlite3_exec(db, tmp_str, NULL, NULL, NULL);  
}

void HS_uninit()
{
    sqlite3_close(db); 
}


void HS_showHistoryManagerWindow()
{
    HS_test_treelist();
    
    return;
}
void HS_showHistoryClearWindow()
{
    HS_DEBUG("*************Clear All History*************\n");

    char sql[256];
    memset(sql, '\0', 256);  
    sprintf(sql,"delete from tb");
    sqlite3_exec(db, sql, sql_callback, NULL, NULL);  
}

void HS_history_changed(WebKitBackForwardList *backForwadlist, WebKitBackForwardListItem *itemAdded)
{
    HS_DEBUG("file:%s\n  func:%s\n  line[%d]\n",__FILE__,__func__,__LINE__);
}

void HS_get_favicon_path(gchar *favicon_path)
{
    static gint index = 0;    
    gchar path[128] = {0};
    
    sprintf(path,"%s/favicon_%d.png", HISTORY_DB_PATH, index++);

    strcpy(favicon_path, path);
}

void HS_history_new(BrowserWindow *window, WebKitWebView *webview)
{

    char sql[1024] = {0};
    gchar favicon_path[256] = {0};
    gint favicon_width = 0;
    gint favicon_height = 0;
    gint favicon_stride = 0;
    unsigned char *favicon_data = 0;

    const char *title = webkit_web_view_get_title(webview);
    if(strcmp("",title) == 0){
        HS_DEBUG("Warning:title is NUll\n");
    }else {
        const char *uri = webkit_web_view_get_uri(webview);
        cairo_surface_t *favicon=webkit_web_view_get_favicon(webview); 
        if(favicon != NULL) {
            //favicon_width = cairo_image_surface_get_width(favicon);
            //favicon_height = cairo_image_surface_get_height(favicon);
            //favicon_stride = cairo_image_surface_get_stride(favicon);    
            //HS_DEBUG("\t 1 favicon w[%d] X h[%d] X s[%d]\n",favicon_width, favicon_height, favicon_stride);
            //cairo_surface_t size is 16X16
            //if(favicon_width = 16 && favicon_height == 16){
            if(1){
                HS_get_favicon_path(favicon_path);
                HS_DEBUG("favicon_path is[%s]\n", favicon_path);
                cairo_surface_write_to_png(favicon, favicon_path);
            }else {
                strcpy(favicon_path, FAVICON_DEFAULT_PATH);
                //TODO convert
                //convert to 16X16
                //favicon_data = cairo_image_surface_get_data(favicon);
                //favicon = cairo_image_surface_create_for_data(favicon_data, CAIRO_FORMAT_ARGB32, 16, 16,  128);

                //favicon_width = cairo_image_surface_get_width(favicon);
                //favicon_height = cairo_image_surface_get_height(favicon);
                //favicon_stride = cairo_image_surface_get_stride(favicon);
                //HS_DEBUG("\t 2 favicon w[%d] X h[%d] X s[%d]\n",favicon_width, favicon_height, favicon_stride);
                //cairo_surface_write_to_png(favicon, favicon_path);
            }
        }else {
            HS_DEBUG("Warning:favicon is NUll\n");
            strcpy(favicon_path, FAVICON_DEFAULT_PATH);
        }

        HS_DEBUG("===========add a history===================\n");
        HS_DEBUG("\turl[%s]\n\ttitle[%s]\t\nfavicon[%s]\n", uri, title, favicon_path );
        HS_DEBUG("======================================\n");
    
        memset(sql, '\0', 1024);
        sprintf(sql,"insert into tb(url, title, favicon_path) values(\"%s\", \"%s\", \"%s\")",uri, title, favicon_path);
        int error_num;
        gchar error_str[256];
        error_num = sqlite3_exec(db, sql, NULL, NULL, error_str);  
        HS_DEBUG("error [%d]:[%s]", error_num, error_str);
    }
}

void HS_load_all_history(HS_HistoryData * history_data, int size)
{
    char sql[256];
    memset(sql, '\0', 256);  
    sprintf(sql,"select * from tb");
    sqlite3_exec(db, sql, sql_callback, history_data, NULL);  

    return;
}

GtkTreeModel* createModel()
{
    GdkPixbuf *pixbuf;
    GtkTreeIter iter;
    GtkListStore *store;
    gint i;
    gint size;
    
    HS_HistoryData history_data;

    memset( &history_data, 0, sizeof(HS_HistoryData));

    store = gtk_list_store_new(2,GDK_TYPE_PIXBUF, G_TYPE_STRING);  

    //load history record
    HS_load_all_history(&history_data,256);
    
    //insert a history record
    for(i=0;i<history_data.current_index;i++)
    {
        pixbuf = gdk_pixbuf_new_from_file(history_data.history_item_arrary[i].favicon_path, NULL);
        gtk_list_store_append(store,&iter);
        gtk_list_store_set(store,&iter,PIXBUF_COL,pixbuf,TEXT_COL1,history_data.history_item_arrary[i].title,-1);
        gdk_pixbuf_unref(pixbuf);
    }
    
    return GTK_TREE_MODEL(store);
}
gboolean selection_changed(GtkTreeSelection *selection, GtkLabel *label){
    GtkTreeView *treeView;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *active;
    treeView = gtk_tree_selection_get_tree_view(selection);
    model=gtk_tree_view_get_model(treeView);
    gtk_tree_selection_get_selected(selection,&model,&iter);
    gtk_tree_model_get(model,&iter,1,&active,-1);    
    gtk_label_set_text(label,active);
}


void HS_test_treelist(void)
{
    GtkWidget *window;
    GtkWidget *treeView;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkWidget *label;
    GtkWidget *vbox;
    GtkTreeSelection *selection;

    gtk_init (0, 0);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_title(GTK_WINDOW(window),"历史记录");
    gtk_window_set_default_size(GTK_WINDOW(window),600,200);


    GtkTreeModel* model = createModel();

    treeView = gtk_tree_view_new_with_model(model);

    renderer = gtk_cell_renderer_pixbuf_new();
    column = gtk_tree_view_column_new_with_attributes("favicon",renderer, "pixbuf", PIXBUF_COL, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("history-title",renderer, "text", TEXT_COL1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeView), column);

    label = gtk_label_new("");
    vbox = gtk_vbox_new(FALSE,5);

    gtk_box_pack_start(GTK_BOX(vbox),treeView, TRUE,TRUE,5);
    gtk_box_pack_start(GTK_BOX(vbox),label, TRUE,TRUE,5);
    gtk_container_add(GTK_CONTAINER(window),vbox);
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeView));
    g_signal_connect(G_OBJECT(selection), "changed",G_CALLBACK(selection_changed),label);
    g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return;
}

int sql_callback(void *arg, int nr, char **values, char **names)  
{
    gint i;  
    gint current_index;
    
    HS_HistoryData *history_data = (HS_HistoryData*)arg;
    current_index = history_data->current_index;

    HS_DEBUG( "%d\t%s\t%s\t%s\t\n",current_index, values[1],values[2],values[3]);  

    history_data->history_item_arrary[current_index].index = current_index;

    strcpy(history_data->history_item_arrary[current_index].uri, values[1]);
    strcpy(history_data->history_item_arrary[current_index].title, values[2]);
    strcpy(history_data->history_item_arrary[current_index].favicon_path, values[3]);

    history_data->current_index++;

    return 0;
}

