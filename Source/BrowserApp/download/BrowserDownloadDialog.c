/*
 * Copyright (C) 2011 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "BrowserDownloadFile.h"
#include "BrowserDownloadDialog.h"
//#include "cmakeconfig.h"

#include <glib/gi18n.h>

//#include <webkit2/webkit2.h>

enum {
    DOWNLOAD_DIALOG_COLUMN_FILENAME,
    DOWNLOAD_DIALOG_COLUMN_PERCENT,
    DOWNLOAD_DIALOG_COLUMN_FILESIZE,
    DOWNLOAD_DIALOG_COLUMN_REMAININGTIME,
    DOWNLOAD_DIALOG_COLUMN_CREATETIME,
    DOWNLOAD_DIALOG_COLUMN_WEBSIZE,
    DOWNLOAD_DIALOG_COLUMN_PATH,
    DOWNLOAD_DIALOG_COLUMN_WEBKITDOWNLOAD,

    DOWNLOAD_DIALOG_N_COLUMN
};

#define DOWNLOAD_LIST_MAX 20

struct _BrowserDownloadDialog {
    GtkDialog parent;
    
    GtkWidget *downloadList;
    BrowserDownloadFile *downloadDialog;
    WebKitDownload *download;

    GtkListStore *model;
    GtkTreeIter iter;
    GtkTreeSelection *select;

    //download list
    char *filename[DOWNLOAD_LIST_MAX];
    char *percent[DOWNLOAD_LIST_MAX];
    char *filesize[DOWNLOAD_LIST_MAX];
    char *remaining[DOWNLOAD_LIST_MAX];
    char *createtime[DOWNLOAD_LIST_MAX];
    char *website[DOWNLOAD_LIST_MAX];
    char *path[DOWNLOAD_LIST_MAX];

    GtkWidget *newDownload;
    GtkWidget *openFile;
    GtkWidget *openDir;
    GtkWidget *cancelDownload;
};

struct _BrowserDownloadDialogClass {
    GtkDialogClass parent;
};

G_DEFINE_TYPE(BrowserDownloadDialog, browser_download_dialog, GTK_TYPE_DIALOG);

static void newDownloadCallback(GtkWidget *button, BrowserDownloadDialog *dialog)
{
    
    if (dialog->downloadDialog) {
        gtk_window_present(GTK_WINDOW(dialog->downloadDialog));
        return;
    }

    dialog->downloadDialog = BROWSER_DOWNLOAD_FILE(browser_download_file_new());
    gtk_window_set_transient_for(GTK_WINDOW(dialog->downloadDialog), GTK_WINDOW(dialog));
    g_object_add_weak_pointer(G_OBJECT(dialog->downloadDialog), (gpointer *)&dialog->downloadDialog);
    gtk_widget_show(GTK_WIDGET(dialog->downloadDialog));
}

static void cancelDownloadCallback(GtkWidget *button, BrowserDownloadDialog *dialog)
{
    GtkTreeModel * model;
    //store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(dialog->downloadList)));
    GtkTreeIter iter;
    WebKitDownload *tmpDownload;
    gchar *path;
    gint columns=gtk_tree_model_get_n_columns(GTK_TREE_MODEL(dialog->model));
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(dialog->select), &model, &iter)) 
    {
        gtk_tree_model_get (GTK_TREE_MODEL(dialog->model), &iter, 7, &tmpDownload, -1);
        if(tmpDownload)
            webkit_download_cancel(WEBKIT_DOWNLOAD(tmpDownload));
        gtk_tree_model_get (GTK_TREE_MODEL(dialog->model), &iter, 6, &path, -1);
        if(path)
        {
        }
        char *treeviewIndex = g_strdup_printf("%s", gtk_tree_model_get_string_from_iter(GTK_TREE_MODEL(dialog->model),&iter)); 
        if(treeviewIndex)
        {
            size_t listIndex = atoi(treeviewIndex);
            DeleteList(dialog->download,listIndex);
        }
        gtk_list_store_remove(dialog->model, &iter);
    }
    g_free(path);
    //g_printerr("cancelDownloadCallback:::columns:%d::path:%s\n",columns);
    //gtk_widget_destroy(GTK_WIDGET(dialog));
}


static void openFileCallback(GtkWidget *button, BrowserDownloadDialog *dialog)
{
    GtkTreeModel * model;
    GtkTreeIter iter;
    //WebKitDownload *tmpDownload;
    gchar *path_file;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(dialog->select), &model, &iter)) 
    {
        gtk_tree_model_get (GTK_TREE_MODEL(dialog->model), &iter, 6, &path_file, -1);
        if(path_file)
            gtk_show_uri(gtk_widget_get_screen(GTK_WIDGET(dialog)),
                                path_file,
                                gtk_get_current_event_time(), 
                                NULL);
    }
    g_free(path_file);
    //gtk_widget_hide(GTK_WIDGET(dialog));
}

static void openDirCallback(GtkWidget *button, BrowserDownloadDialog *dialog)
{
    GtkTreeModel * model;
    GtkTreeIter iter;
    //WebKitDownload *tmpDownload;
    gchar *path_file;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(dialog->select), &model, &iter)) 
    {
        gtk_tree_model_get (GTK_TREE_MODEL(dialog->model), &iter, 6, &path_file, -1);
        if(path_file)
        {
            int path_len = strlen(path_file) - strlen(g_strrstr(path_file , "/"));
            gchar *download_path = (char*)malloc(path_len*sizeof(char)+1);
            memset(download_path, 0x00, path_len+1);
            memcpy(download_path, path_file, path_len);
            
            char *argv[] = { "xdg-open", download_path, NULL} ;//{ "xdg-open", "/home/sunh", NULL };

            /** use a cast to shut up gcc **/
            g_spawn_async( NULL, (gchar **)argv, NULL, (GSpawnFlags)(G_SPAWN_SEARCH_PATH |
            G_SPAWN_STDOUT_TO_DEV_NULL |
            G_SPAWN_STDERR_TO_DEV_NULL |
            G_SPAWN_STDERR_TO_DEV_NULL),
            NULL, NULL, NULL, NULL );
            g_free(download_path);
        }
    }
}


/*
static void tree_selection_changed(GtkTreeSelection *select,BrowserDownloadDialog *dialog)
{
    //gtk_widget_set_sensitive(dialog->openFile, true);
}
*/

static void download_list_init(BrowserDownloadDialog *dialog)
{
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();

    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(dialog->downloadList),
                        0, "文件名", renderer,
                        "text", DOWNLOAD_DIALOG_COLUMN_FILENAME,
                        NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(dialog->downloadList),
                        1, "百分比", renderer,
                        "text", DOWNLOAD_DIALOG_COLUMN_PERCENT,
                        NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(dialog->downloadList),
                        2, "大小", renderer,
                        "text", DOWNLOAD_DIALOG_COLUMN_FILESIZE,
                        NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(dialog->downloadList),
                        3, "剩余时间", renderer,
                        "text", DOWNLOAD_DIALOG_COLUMN_REMAININGTIME,
                        NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(dialog->downloadList),
                        4, "创建日期", renderer,
                        "text", DOWNLOAD_DIALOG_COLUMN_CREATETIME,
                        NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(dialog->downloadList),
                        5, "网址", renderer,
                        "text", DOWNLOAD_DIALOG_COLUMN_WEBSIZE,
                        NULL);

    dialog->model = gtk_list_store_new(DOWNLOAD_DIALOG_N_COLUMN, 
                                             G_TYPE_STRING, 
                                             G_TYPE_STRING, 
                                             G_TYPE_STRING,
    						 G_TYPE_STRING, 
    						 G_TYPE_STRING,
    						 G_TYPE_STRING,
    						 G_TYPE_STRING,
                                             G_TYPE_OBJECT);
    //gtk_list_store_append(dialog->model, &dialog->iter);	
    gtk_tree_view_set_model(GTK_TREE_VIEW(dialog->downloadList), GTK_TREE_MODEL(dialog->model));
    g_object_unref(dialog->model);
}

static void browser_download_dialog_init(BrowserDownloadDialog *dialog)
{
    GtkBox *contentArea = GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog)));
    gtk_box_set_spacing(contentArea, 2);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 500, 300);
    gtk_window_set_title(GTK_WINDOW(dialog), "下载管理");
    //gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(dialog), 5);
    gtk_dialog_add_button(GTK_DIALOG(dialog), GTK_STOCK_CLOSE, GTK_RESPONSE_CANCEL);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_CANCEL);

    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    dialog->downloadList = gtk_tree_view_new();

    dialog->select = gtk_tree_view_get_selection(GTK_TREE_VIEW(dialog->downloadList));
    gtk_tree_selection_set_mode(dialog->select, GTK_SELECTION_SINGLE);
    //g_signal_connect(dialog->select, "changed", G_CALLBACK(tree_selection_changed),dialog);

    GtkWidget *toolbar = gtk_toolbar_new();

    //gtk_orientable_set_orientation(GTK_ORIENTABLE(toolbar), GTK_ORIENTATION_HORIZONTAL);
    //gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH_HORIZ);

    GtkToolItem *item = gtk_tool_button_new_from_stock(GTK_STOCK_ADD);
    dialog->newDownload = GTK_WIDGET(item);
    g_signal_connect(dialog->newDownload, "clicked", G_CALLBACK(newDownloadCallback), (gpointer)dialog);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
    gtk_widget_show(GTK_WIDGET(item));

    item = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PLAY);
    GtkWidget *startDownload = GTK_WIDGET(item);

    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
    gtk_widget_show(GTK_WIDGET(item));

    item = gtk_tool_button_new_from_stock(GTK_STOCK_MEDIA_PAUSE);
    GtkWidget *stopDownload = GTK_WIDGET(item);

    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
    gtk_widget_show(GTK_WIDGET(item));

    item = gtk_tool_button_new_from_stock(GTK_STOCK_CANCEL);
    dialog->cancelDownload = GTK_WIDGET(item);
    g_signal_connect(dialog->cancelDownload, "clicked", G_CALLBACK(cancelDownloadCallback), (gpointer)dialog);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
    gtk_widget_show(GTK_WIDGET(item));

    item = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
    dialog->openFile = GTK_WIDGET(item);
    //gtk_widget_set_sensitive(dialog->openFile, false);
    g_signal_connect(dialog->openFile, "clicked", G_CALLBACK(openFileCallback), (gpointer)dialog);

    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
    gtk_widget_show(GTK_WIDGET(item));

    item = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
    dialog->openDir = GTK_WIDGET(item);
    g_signal_connect(dialog->openDir, "clicked", G_CALLBACK(openDirCallback), (gpointer)dialog);

    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
    gtk_widget_show(GTK_WIDGET(item));

    gtk_box_pack_start(contentArea, toolbar, FALSE, FALSE, 0);
    gtk_widget_show(toolbar);

    download_list_init(dialog);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), dialog->downloadList);
    gtk_widget_show(dialog->downloadList);

    gtk_box_pack_start(contentArea, scrolledWindow, TRUE, TRUE, 0);
    gtk_widget_show(scrolledWindow);

    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_hide), NULL);
    g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_widget_hide), NULL);
}

static void browserDownloadDialogConstructed(GObject *object)
{
    BrowserDownloadDialog *dialog = BROWSER_DOWNLOAD_DIALOG(object);
}

static void browser_download_dialog_class_init(BrowserDownloadDialogClass *klass)
{
    GObjectClass *gobjectClass = G_OBJECT_CLASS(klass);

    gobjectClass->constructed = browserDownloadDialogConstructed;
}

struct _BrowserDownload {
    GtkBox parent;

    WebKitDownload *download;
    WebKitDownload *tmpDownload;
    guint64 contentLength;
    guint64 downloadedSize;
    gboolean finished;

    GtkListStore *model;
    GtkTreeIter iter;
    GtkWidget *downloadList;

    //download list
    char *filename;
    char *percent;
    char *filesize;
    char *remaining;
    char *createtime;
    char *website;
    char *path;

    BrowserDownloadDialog *browserDownloadDialog;
};

struct _BrowserDownloadClass {
    GtkBoxClass parentClass;
};

G_DEFINE_TYPE(BrowserDownload, browser_download, GTK_TYPE_BOX)

static void browser_download_init(BrowserDownload *dialog)
{
    //g_printerr("BrowserDownload init:\n");
}

static void browser_download_class_init(BrowserDownloadClass *klass)
{
    GObjectClass *objectClass = G_OBJECT_CLASS(klass);
}

static gchar *filesizeCalc(BrowserDownload *dialog)
{
    guint64 contentLength = dialog->contentLength;
    guint64 tmpContent;

    if(contentLength <= 0) return NULL;

    tmpContent = contentLength /1024;
    if(tmpContent <= 0)
        return g_strdup_printf ("%lld B", contentLength);
    tmpContent = tmpContent /1024;
    if(tmpContent <= 0)
        return g_strdup_printf ("%lld KB", contentLength/1024);
    tmpContent = tmpContent /1024;
    if(tmpContent <= 0)
        return g_strdup_printf ("%lld MB", contentLength/(1024*1024));
    return g_strdup_printf ("%lld MB", contentLength/(1024*1024*1024));
}

 static gchar *remainingTime(BrowserDownload *dialog)
 {
     guint64 total = dialog->contentLength;
     guint64 current = dialog->downloadedSize;
     gdouble elapsedTime = webkit_download_get_elapsed_time(dialog->download);
 
     if (current <= 0)
         return NULL;
 
     gdouble perByteTime = elapsedTime / current;
     gdouble interval = perByteTime * (total - current);
 
     int hours = (int) (interval / 3600);
     interval -= hours * 3600;
     int mins = (int) (interval / 60);
     interval -= mins * 60;
     int secs = (int) interval;
 
     if (hours > 0) {
         if (mins > 0)
             return g_strdup_printf (ngettext ("%u:%02u h", "%u:%02u h", hours), hours, mins);
         return g_strdup_printf (ngettext ("%u h", "%u h", hours), hours);
     }
 
     if (mins > 0)
         return g_strdup_printf (ngettext ("%u:%02u m", "%u:%02u m", mins), mins, secs);
     return g_strdup_printf (ngettext ("%u s", "%u s", secs), secs);
 }

 static void download_list_add(BrowserDownloadDialog *dialog, size_t listsize)
{
    dialog->model = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(dialog->downloadList)));

    int i;
    for(i = 0; i < listsize; i++)
    {
        gtk_list_store_append(dialog->model, &dialog->iter);
        gtk_list_store_set(dialog->model, &dialog->iter,
        	                            DOWNLOAD_DIALOG_COLUMN_FILENAME,    dialog->filename[i],         -1);
        gtk_list_store_set(dialog->model, &dialog->iter,
        		                     DOWNLOAD_DIALOG_COLUMN_FILESIZE,      dialog->filesize[i],                 -1);
        gtk_list_store_set(dialog->model, &dialog->iter,
        				      DOWNLOAD_DIALOG_COLUMN_CREATETIME, dialog->createtime[i],           -1);
        gtk_list_store_set(dialog->model, &dialog->iter,
        		                     DOWNLOAD_DIALOG_COLUMN_WEBSIZE,      dialog->website[i],              -1);
        gtk_list_store_set(dialog->model, &dialog->iter,
        		                     DOWNLOAD_DIALOG_COLUMN_PATH,      dialog->path[i],              -1);
        gtk_list_store_set(dialog->model, &dialog->iter,
        		                     DOWNLOAD_DIALOG_COLUMN_PERCENT,      "--" ,              -1);
        gtk_list_store_set(dialog->model, &dialog->iter,
        		                     DOWNLOAD_DIALOG_COLUMN_REMAININGTIME,      "--" ,              -1);
    }
}

 static void download_list_add1(BrowserDownload *dialog, WebKitURIResponse *response)
{   //webkit_download_get_destination
    GFile *destFile = g_file_new_for_uri(webkit_download_get_destination(dialog->download));
    //GFile *destFile = g_file_new_for_uri(webkit_uri_response_get_uri(response));
    dialog->filename = g_file_get_basename(destFile);
    dialog->website = g_strdup_printf("%s", webkit_uri_response_get_uri(response)); 
    dialog->filesize = filesizeCalc(dialog);

    //gtk_list_store_get_value
    dialog->model = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(dialog->downloadList)));
    
    gtk_list_store_set(dialog->model, &dialog->iter,
    	                            DOWNLOAD_DIALOG_COLUMN_FILENAME,     dialog->filename, -1);
    gtk_list_store_set(dialog->model, &dialog->iter,
    		                     DOWNLOAD_DIALOG_COLUMN_FILESIZE,      dialog->filesize,    -1);
    //gtk_list_store_set(dialog->model, &dialog->iter,
    	//			      DOWNLOAD_DIALOG_COLUMN_CREATETIME, ct,                        -1);
    gtk_list_store_set(dialog->model, &dialog->iter,
    		                     DOWNLOAD_DIALOG_COLUMN_WEBSIZE,      dialog->website , -1);
    //gtk_list_store_set(dialog->model, &dialog->iter,
    //		                     DOWNLOAD_DIALOG_COLUMN_PATH,            dialog->path ,      -1);
    gtk_list_store_set(dialog->model, &dialog->iter,
    		                     DOWNLOAD_DIALOG_COLUMN_WEBKITDOWNLOAD,  dialog->download ,      -1);
    //gtk_list_store_set(dialog->model, &dialog->iter,
    	//	                     DOWNLOAD_DIALOG_COLUMN_PATH,      dialog->download ,     -1);
    g_object_unref(destFile);
}

static void download_list_add2(BrowserDownload *dialog)
{
    gdouble per = webkit_download_get_estimated_progress(dialog->download);
    dialog->remaining = remainingTime(dialog);
    dialog->percent = g_strdup_printf("%.1lf%%", per*100);

    gtk_list_store_set(dialog->model, &dialog->iter, DOWNLOAD_DIALOG_COLUMN_PERCENT, dialog->percent,-1);
    gtk_list_store_set(dialog->model, &dialog->iter, DOWNLOAD_DIALOG_COLUMN_REMAININGTIME, dialog->remaining,-1);
}

static void downloadReceivedResponse(WebKitDownload *download, GParamSpec *paramSpec, BrowserDownload *dialog)
{
    WebKitURIResponse *response = webkit_download_get_response(download);
    dialog->contentLength = webkit_uri_response_get_content_length(response);
    gtk_list_store_append(dialog->model, &dialog->iter);
    //download_list_add1(dialog, response);
} 

static void createdDestinationCallback(WebKitDownload *download, gchar* param,BrowserDownload *dialog)
{
    //g_printerr("createdDestinationCallback\n");
    WebKitURIResponse *response = webkit_download_get_response(download);
    download_list_add1(dialog, response);
}


static void decideDestinationCallback(WebKitDownload *download, gchar* param, BrowserDownload *dialog)
{
    //g_printerr("decideDestinationCallback:param:%s\n", param);
    /*
    const gchar *downloadDir = g_get_home_dir ();
    gchar *destination = g_build_filename(downloadDir, param, NULL);
    gchar *destinationUri = g_filename_to_uri(destination, 0, 0);
    webkit_download_set_destination(dialog->download, destinationUri);
    */
    const gchar *dir = webkit_download_get_destination(dialog->download);
    if(dir)
    {
        WebKitURIResponse *response = webkit_download_get_response(download);
        download_list_add1(dialog, response);
    }
    //g_printerr("decideDestinationCallback:home:%s\n", dir);
    //const gchar *dir = webkit_download_get_destination(dialog->download);
    //g_printerr("decideDestinationCallback\n");
    //g_printerr("decideDestinationCallback:home:%s\n", dir);
}


static void downloadProgress(WebKitDownload *download, GParamSpec *paramSpec, BrowserDownload *dialog)
{
    download_list_add2(dialog);
}

static void downloadReceivedData(WebKitDownload *download, guint64 dataLength, BrowserDownload *dialog)
{
    dialog->downloadedSize += dataLength;
}

static void downloadFinished(WebKitDownload *download, BrowserDownload *dialog)
{
    //WebKitURIResponse *response = webkit_download_get_response(download);
    //download_list_add1(dialog, response);
    time_t t = time(0);
    char ct[32];
    strftime(ct, sizeof(ct), "%Y-%m-%d %H:%M:%S", localtime(&t));
    dialog->path = g_strdup_printf("%s", webkit_download_get_destination(dialog->download));

    gtk_list_store_set(dialog->model, &dialog->iter,
    				      DOWNLOAD_DIALOG_COLUMN_CREATETIME, ct,                        -1);
    gtk_list_store_set(dialog->model, &dialog->iter,
    		                     DOWNLOAD_DIALOG_COLUMN_PATH,            dialog->path ,      -1);

    //InitDownloadList(dialog->download); 
    SaveValueToDownloadList(dialog->tmpDownload, 
                                            dialog->filename, 
                                            dialog->filesize,
                                            ct,
                                            dialog->website,
                                            dialog->path);

    dialog->finished = TRUE;
}

static void downloadFailed(WebKitDownload *download, GError *error, BrowserDownload *dialog)
{
    gtk_list_store_remove(dialog->model, &dialog->iter);
    g_signal_handlers_disconnect_by_func(dialog->download, downloadFinished, dialog);
    if (g_error_matches(error, WEBKIT_DOWNLOAD_ERROR, WEBKIT_DOWNLOAD_ERROR_CANCELLED_BY_USER)) {
        gtk_widget_destroy(GTK_WIDGET(dialog));
        return;
    }
    
}

GtkWidget *browserDownloadNew(WebKitDownload *download, BrowserDownloadDialog *dialog)
{
    BrowserDownload *browserDownload = BROWSER_DOWNLOAD(g_object_new(BROWSER_TYPE_DOWNLOAD,
                                                                     NULL));

    browserDownload->model = dialog->model;
    browserDownload->iter = dialog->iter;
    browserDownload->downloadList = dialog->downloadList;
    browserDownload->tmpDownload = dialog->download;
    
    browserDownload->download = g_object_ref(download);
    g_signal_connect(browserDownload->download, "notify::response", G_CALLBACK(downloadReceivedResponse), browserDownload);
    g_signal_connect(browserDownload->download, "created-destination", G_CALLBACK(createdDestinationCallback), browserDownload);
    //g_signal_connect(browserDownload->download, "decide-destination", G_CALLBACK(decideDestinationCallback), browserDownload);
    g_signal_connect(browserDownload->download, "notify::estimated-progress", G_CALLBACK(downloadProgress), browserDownload);
    g_signal_connect(browserDownload->download, "received-data", G_CALLBACK(downloadReceivedData), browserDownload);
    g_signal_connect(browserDownload->download, "finished", G_CALLBACK(downloadFinished), browserDownload);
    g_signal_connect(browserDownload->download, "failed", G_CALLBACK(downloadFailed), browserDownload);
    g_signal_connect(browserDownload->download, "decide-destination", G_CALLBACK(decideDestinationCallback), browserDownload);
    //g_signal_connect(browserDownload->download, "created-destination", G_CALLBACK(createdDestinationCallback), browserDownload);
    

    return GTK_WIDGET(browserDownload);
}

GtkWidget *browser_download_dialog_new(WebKitDownload *download)
{
    BrowserDownloadDialog *dialog = BROWSER_DOWNLOAD_DIALOG(g_object_new(BROWSER_TYPE_DOWNLOAD_DIALOG,
                                                                     NULL));

    dialog->download = WEBKIT_DOWNLOAD(g_object_new(WEBKIT_TYPE_DOWNLOAD, NULL));
    //InitDownloadList(dialog->download); 
    size_t listsize;
    gboolean ret = SetDownloadList(dialog->download, dialog->filename, dialog->filesize, dialog->createtime, dialog->website, dialog->path, &listsize);
    if(ret) 
    {
        //g_printerr("SetDownloadList\n");
        download_list_add(dialog, listsize);
    }
    
    if(download) 
    {
        GtkWidget *browserDownload = browserDownloadNew(download, dialog);
    }
    return GTK_WIDGET(dialog);
}


