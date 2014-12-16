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
#include "string.h"

struct _BrowserDownloadFile {
    GtkDialog parent;

    GtkWidget *newDownloadDialog;
    GtkWidget *downloadList;
    GtkWidget *downloadDialog;

    GtkWidget *entry1, *entry2, *entry3;
    GtkWidget *dbtn, *btn;
};

struct _BrowserDownloadFileClass {
    GtkDialogClass parent;
};

G_DEFINE_TYPE(BrowserDownloadFile, browser_download_file, GTK_TYPE_DIALOG);

static void downloadBtnCallback(GtkWidget *btn, BrowserDownloadFile *dialogFile) {
    
    if(gtk_entry_get_text_length(GTK_ENTRY(dialogFile->entry1)) <= 0) 
        return;

    WebKitWebContext* m_webContext = webkit_web_context_get_default();
    const gchar *downloadUri = gtk_entry_get_text(GTK_ENTRY(dialogFile->entry1));

    const gchar *downloadDir = gtk_entry_get_text(GTK_ENTRY(dialogFile->entry2));
    if(!g_file_test(downloadDir, G_FILE_TEST_IS_DIR) && g_mkdir(downloadDir, 0700)) 
        downloadDir = g_get_home_dir ();
    //const gchar *filename = gtk_entry_get_text(GTK_ENTRY(dialogFile->entry3));
    gchar *destination = g_build_filename(downloadDir, gtk_entry_get_text(GTK_ENTRY(dialogFile->entry3)), NULL);
    //gchar *destinationUri = g_filename_to_uri(destination, 0, 0);
    WebKitDownload* download = webkit_web_context_download_uri(m_webContext, downloadUri);
    webkit_download_set_destination(download, g_filename_to_uri(destination, 0, 0));
    WebKitURIRequest* request = webkit_download_get_request(download);
    g_printerr("\t new download btn\n");
    gtk_widget_destroy(GTK_WIDGET(dialogFile));
}

static void cancelBtnCallback(GtkWidget *btn, BrowserDownloadFile *dialogFile) {
    gtk_widget_destroy(GTK_WIDGET(dialogFile));
}

static void filenameCallback(GtkWidget *btn, BrowserDownloadFile *dialogFile) {
    GFile *destFile = g_file_new_for_uri(gtk_entry_get_text(GTK_ENTRY(dialogFile->entry1)));
    gchar *filename = g_file_get_basename(destFile);
    gtk_entry_set_text(GTK_ENTRY(dialogFile->entry3), filename);
    g_free(filename);
    g_object_unref(destFile);
}


static void saveBtnCallback(GtkWidget *btn, BrowserDownloadFile * dialogFile) {
    GtkWidget *dialog;
    gchar *filename;
    dialog = gtk_file_chooser_dialog_new ("Save File As ...", GTK_WINDOW(dialogFile),
                                                                  GTK_FILE_CHOOSER_ACTION_SAVE,
                                                                  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                                  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                                                  NULL);

    const gchar *downloadsDir = g_get_user_special_dir(G_USER_DIRECTORY_DOWNLOAD);
    const gchar* downloadUrl = gtk_entry_get_text(GTK_ENTRY(dialogFile->entry1));
    if(downloadUrl)
    {
        GFile *destFile = g_file_new_for_uri(downloadUrl);
        filename = g_file_get_basename(destFile);
        gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER (dialog), filename);
    }
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (dialog), downloadsDir);
    gint result = gtk_dialog_run (GTK_DIALOG (dialog));
    if (result == GTK_RESPONSE_ACCEPT)
    {
        gchar *curDir = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (dialog));
        gtk_entry_set_text(GTK_ENTRY(dialogFile->entry2), curDir);
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        //gtk_entry_set_text(GTK_ENTRY(dialogFile->entry3), g_file_get_basename(g_file_new_for_uri(filename)));
        //g_printerr("filename:%s\n",filename);
        //g_printerr("path:%s\n", cur_dir);
        //gtk_button_set_label (button, filename);
        g_free(curDir);
    }
    g_free(filename);
    gtk_widget_destroy(dialog);
}

static void browser_download_file_init(BrowserDownloadFile *dialog)
{
    GtkBox *contentArea = GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog)));
    gtk_box_set_spacing(contentArea, 4);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 150);
    gtk_window_set_title(GTK_WINDOW(dialog), "下载文件");
    gtk_window_set_destroy_with_parent(GTK_WINDOW(dialog), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(dialog), 20);
	
    GtkWidget *h1box,*h2box,*h3box,*h4box,*vbox;
    GtkWidget *cbtn;
    GtkWidget *label1, *label2, *label3;

    h1box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    h2box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    h3box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    h4box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    label1 = gtk_label_new("网址:                  ");
    dialog->entry1 = gtk_entry_new();
    g_signal_connect (G_OBJECT (dialog->entry1), "changed", G_CALLBACK (filenameCallback), dialog);

    gtk_box_pack_start(GTK_BOX(h1box), label1, FALSE, FALSE, 0);  
    gtk_box_pack_start(GTK_BOX(h1box), dialog->entry1, TRUE, TRUE, 0); 

    label2 = gtk_label_new("保存至文件夹:     ");
    dialog->entry2 = gtk_entry_new();
    dialog->btn = gtk_button_new_with_label("...");
    const gchar *downloadsDir = g_get_user_special_dir(G_USER_DIRECTORY_DOWNLOAD);
    gtk_entry_set_text(GTK_ENTRY(dialog->entry2), downloadsDir);
    g_signal_connect (G_OBJECT (dialog->btn), "clicked", G_CALLBACK (saveBtnCallback), dialog);

    gtk_box_pack_start(GTK_BOX(h2box), label2, FALSE, FALSE, 0);  
    gtk_box_pack_start(GTK_BOX(h2box), dialog->entry2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(h2box), dialog->btn, FALSE, FALSE, 0); 

    label3 = gtk_label_new("文件名:               ");
    dialog->entry3 = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(h3box), label3, FALSE, FALSE, 0);  
    gtk_box_pack_start(GTK_BOX(h3box), dialog->entry3, TRUE, TRUE, 0);

    dialog->dbtn = gtk_button_new_with_label("下载");
    g_signal_connect(G_OBJECT(dialog->dbtn), "clicked", G_CALLBACK(downloadBtnCallback), dialog);

    cbtn = gtk_button_new_with_label("取消");
    g_signal_connect(G_OBJECT(cbtn), "clicked", G_CALLBACK(cancelBtnCallback), dialog);

    gtk_box_pack_start(GTK_BOX(h4box), dialog->dbtn, FALSE, FALSE, 0);  
    gtk_box_pack_start(GTK_BOX(h4box), cbtn, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(h1box),FALSE,FALSE,0);  
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(h2box),FALSE,FALSE,0); 
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(h3box),FALSE,FALSE,0); 
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(h4box),FALSE,FALSE,0);

    gtk_container_add(GTK_CONTAINER(contentArea),vbox);
    gtk_widget_show_all(GTK_WIDGET(contentArea));  
	
    //g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
    g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_widget_hide), NULL);
}

static void browserDownloadFileConstructed(GObject *object)
{
    BrowserDownloadFile *dialog = BROWSER_DOWNLOAD_FILE(object);
}

static void browser_download_file_class_init(BrowserDownloadFileClass *klass)
{
    GObjectClass *gobjectClass = G_OBJECT_CLASS(klass);
    gobjectClass->constructed = browserDownloadFileConstructed;
}

GtkWidget *browser_download_file_new()
{
    return GTK_WIDGET(g_object_new(BROWSER_TYPE_DOWNLOAD_FILE, NULL));
}


