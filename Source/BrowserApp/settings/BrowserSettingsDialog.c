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

#include "BrowserSettingsDialog.h"
#include "BrowserCellRendererVariant.h"

static void initOnStartupRadioButton(WebKitSettings *settings)
{
    int ivalue;
    g_object_get(settings,
             key[PROP_ON_STARTUP], &ivalue,
             NULL);
    switch(ivalue) {
    case 0:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton1_general), TRUE);
        break;
    case 1:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton2_general), TRUE);
        break;
    case 2:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton3_general), TRUE);
        break;
    default:
        printf("error PROP_ON_STARTUP ivalue = %i\n", ivalue);
        break;
    }
}

static void initHomePageEntry(WebKitSettings *settings)
{
    gchar *strval;
    g_object_get(settings,
             key[PROP_HOME_PAGE], &strval,
             NULL);
    gtk_entry_set_text(GTK_ENTRY(settings->entry1_general), strval);
    g_free (strval);
}

static void initOpenNewpageRadioButton(WebKitSettings *settings)
{
    int ivalue;
    g_object_get(settings,
             key[PROP_OPEN_NEWPAGE], &ivalue,
             NULL);
    switch(ivalue) {
    case 0:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton4_general), TRUE);
        break;
    case 1:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton5_general), TRUE);
        break;
    case 2:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton6_general), TRUE);
        break;
    default:
        printf("error PROP_OPEN_NEWPAGE ivalue = %i\n", ivalue);
        break;
    }
}

static void initMuchTabWarningCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_MUCH_TAB_WARNING], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton1_general), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton1_general), FALSE); 
    }
}

static void initShowHomepageCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_SHOW_HOMEPAGE_BUTTON], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton2_general), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton2_general), FALSE); 
    }
}

static void initShowBookmarkbarCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_SHOW_BOOKMARKBAR], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton3_general), TRUE);      
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton3_general), FALSE); 
    }
}

static void initShowTitlebarAndMenubarCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_SHOW_TITLEBAR_AND_MENUBAR], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton4_general), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton4_general), FALSE); 
    }
}

static void initShowFullscreenCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_SHOW_FULLSCREEN], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton5_general), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton5_general), FALSE); 
    }
}

static void initFontComboBox(WebKitSettings *settings)
{
    int index = 0;
    gchar *strval;
    bool bfound = FALSE;
    g_object_get(settings,
             key[PROP_DEFAULT_FONT_FAMILY], &strval,
             NULL);

    for(index = 0; index < settings->fontNum; index++) {
      if(!g_strcmp0(font[index], strval)) {
        bfound = TRUE;
        break;    
      }
    }
    if(bfound) {    
      gtk_combo_box_set_active(GTK_COMBO_BOX(settings->comboboxtext1_content), index);
    } else {
      printf("error, undefined string: %s to init font combobox\n", strval);
      gtk_combo_box_set_active(GTK_COMBO_BOX(settings->comboboxtext1_content), 0);
    } 
    g_free (strval);
}

static void initFontSizeComboBox(WebKitSettings *settings)
{
    int index = 0;
    bool bfound = FALSE;
    gint ivalue;
    g_object_get(settings,
             key[PROP_DEFAULT_FONT_SIZE], &ivalue,
             NULL);
    for(index = 0; index < settings->fontSizeNum; index++) {
      if(font_size[index] == ivalue) {
        bfound = TRUE;
        break;    
      }
    }
    if(bfound) {    
      gtk_combo_box_set_active(GTK_COMBO_BOX(settings->comboboxtext2_content), index);
    } else {
      printf("error, undefined font size: %i to init font_size combobox\n", ivalue);
      gtk_combo_box_set_active(GTK_COMBO_BOX(settings->comboboxtext2_content), 0);
    } 
}

static void initPageZoomComboBox(WebKitSettings *settings)
{
    int index = 0;
    bool bfound = FALSE;
    gdouble dvalue;
    g_object_get(settings,
             key[PROP_PAGE_ZOOM], &dvalue,
             NULL);
    for(index = 0; index < settings->pageZoomNum; index++) {
      if(zoom_factor[index] == dvalue) {
        bfound = TRUE;
        break;    
      }
    }
    if(bfound) {  
      gtk_combo_box_set_active(GTK_COMBO_BOX(settings->comboboxtext3_content), index);
    } else {
      printf("error, undefined page zoom: %f to init page_zoom combobox\n", dvalue);
      gtk_combo_box_set_active(GTK_COMBO_BOX(settings->comboboxtext3_content), 0);
    } 
}

static void initZoomTextOnlyCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_ZOOM_TEXT_ONLY], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton1_content), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton1_content), FALSE); 
    }
}

static void initShowImageRadioButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_AUTO_LOAD_IMAGES], &bvalue,
             NULL);   
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton1_content), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton2_content), TRUE); 
    }
}

static void initRunJavascriptRadioButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_ENABLE_JAVASCRIPT], &bvalue,
             NULL);   
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton3_content), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton4_content), TRUE); 
    }
}

static void initPageContentCacheCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_PAGE_CONTENT_CACHE], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton3_privacy), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton3_privacy), FALSE); 
    }
}

static void initHistorySettingComboBox(WebKitSettings *settings)
{
    gint ivalue;
    g_object_get(settings,
             key[PROP_HISTORY_SETTING], &ivalue,
             NULL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(settings->comboboxtext1_privacy), ivalue);
}

static void initClearBrowseRecordCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_CLEAR_BROWSE_RECORD], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton4_privacy), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton4_privacy), FALSE); 
    }
}

static void initClearDownloadRecordCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_CLEAR_DOWNLOAD_RECORD], &bvalue,
             NULL);
    if(TRUE == bvalue) { 
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton5_privacy), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton5_privacy), FALSE); 
    }
}

static void initClearCookieAndOthersCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_CLEAR_COOKIE_AND_OTHERS], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton6_privacy), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton6_privacy), FALSE); 
    }
}

static void initClearCachedImagesAndFilesCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_CLEAR_CACHED_IMAGES_AND_FILES], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton7_privacy), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton7_privacy), FALSE); 
    }
}

static void initClearPasswordsCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_CLEAR_PASSWORDS], &bvalue,
             NULL);
    if(TRUE == bvalue) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton8_privacy), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton8_privacy), FALSE); 
    }
}

static void initCookieSettingRadioButton(WebKitSettings *settings)
{
    int ivalue;
    g_object_get(settings,
             key[PROP_COOKIE_SETTING], &ivalue,
             NULL);   
    switch(ivalue) {
    case 0:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton1_privacy), TRUE);
        break;
    case 1:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton2_privacy), TRUE);
        break;
    case 2:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton3_privacy), TRUE);
        break;
    default:
        printf("error PROP_COOKIE_SETTING ivalue = %i\n", ivalue);
        break;
    }
}


static void initTrackLocationRadioButton(WebKitSettings *settings)
{
    int ivalue;
    g_object_get(settings,
             key[PROP_TRACK_LOCATION], &ivalue,
             NULL);   
    switch(ivalue) {
    case 0:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton4_privacy), TRUE);
        break;
    case 1:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton5_privacy), TRUE);
        break;
    case 2:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton6_privacy), TRUE);
        break;
    default:
        printf("error PROP_TRACK_LOCATION ivalue = %i\n", ivalue);
        break;
    }

}

static void initCertificateRevocationCheckButton(WebKitSettings *settings)
{
    bool bvalue;
    g_object_get(settings,
             key[PROP_CERTIFICATE_REVOCATION], &bvalue,
             NULL);
    if(TRUE == bvalue) { 
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton1_security), TRUE);     
    } else {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->checkbutton1_security), FALSE); 
    }

}

static void initMediaAccessRadioButton(WebKitSettings *settings)
{
    int ivalue;
    g_object_get(settings,
             key[PROP_MEDIA_ACCESS], &ivalue,
             NULL);   
    switch(ivalue) {
    case 0:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton1_security), TRUE);
        break;
    case 1:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(settings->radiobutton2_security), TRUE);
        break;
    default:
        printf("error PROP_MEDIA_ACCESS ivalue = %i\n", ivalue);
        break;
    } 

}

/*Get new state, init corresponding widgets. sunhaiming add.*/
static void browserSettingsWindowConstructed(WebKitSettings *settings)
{
    //put widget init func here.
    initOnStartupRadioButton(settings);
    initHomePageEntry(settings);
    initOpenNewpageRadioButton(settings);
    initMuchTabWarningCheckButton(settings);
    initShowHomepageCheckButton(settings);
    initShowBookmarkbarCheckButton(settings);
    initShowTitlebarAndMenubarCheckButton(settings);
    initShowFullscreenCheckButton(settings);
    initFontComboBox(settings);
    initFontSizeComboBox(settings);
    initPageZoomComboBox(settings);
    initZoomTextOnlyCheckButton(settings);
    initShowImageRadioButton(settings);
    initRunJavascriptRadioButton(settings);
    initPageContentCacheCheckButton(settings);
    initHistorySettingComboBox(settings);
    initClearBrowseRecordCheckButton(settings);
    initClearDownloadRecordCheckButton(settings);
    initClearCookieAndOthersCheckButton(settings);
    initClearCachedImagesAndFilesCheckButton(settings);
    initClearPasswordsCheckButton(settings);
    initCookieSettingRadioButton(settings);
    initTrackLocationRadioButton(settings);
    initCertificateRevocationCheckButton(settings);
    initMediaAccessRadioButton(settings);  
}

static void homePageCallback(GtkEntry *entry, WebKitSettings *settings) 
{
    const gchar * entry_content =  gtk_entry_get_text(GTK_ENTRY(settings->entry1_general));
    g_object_set(settings,
             key[PROP_HOME_PAGE], entry_content,
             NULL);
}

static void currentPageToHomePageCallback(GtkButton *button, WebKitSettings *settings) 
{
    const gchar * locationbar =  gtk_entry_get_text(GTK_ENTRY(settings->parent_uriEntry));
    gtk_entry_set_text(GTK_ENTRY(settings->entry1_general),locationbar);
}

static void onStartupCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    int ivalue;
    if(gtk_toggle_button_get_active(togglebutton)) {
      if((void *)settings->radiobutton1_general == (void *)togglebutton) {
        ivalue = 0;
      }
      else if((void *)settings->radiobutton2_general == (void *)togglebutton) {
        ivalue = 1;
      }
      else if((void *)settings->radiobutton3_general == (void *)togglebutton) {
        ivalue = 2;
      } 
      else {
        printf("error onStartupCallback\n"); 
        return;
      }
      g_object_set(settings,
               key[PROP_ON_STARTUP], ivalue,
               NULL);
    }
} 

static void openNewpageCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    int ivalue;
    if(gtk_toggle_button_get_active(togglebutton)) {
      if((void *)settings->radiobutton4_general == (void *)togglebutton) {
        ivalue = 0;
      }
      else if((void *)settings->radiobutton5_general == (void *)togglebutton) {
        ivalue = 1;
      }
      else if((void *)settings->radiobutton6_general == (void *)togglebutton) {
        ivalue = 2;
      } 
      else {
        printf("error openNewpageCallback\n"); 
        return;
      }
      g_object_set(settings,
               key[PROP_OPEN_NEWPAGE], ivalue,
               NULL);
    }
} 

static void muchTabWarningCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_MUCH_TAB_WARNING], bvalue,
             NULL);
} 


static void showHomepageButtonCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_SHOW_HOMEPAGE_BUTTON], bvalue,
             NULL);
} 

static void showBookmarkbarCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_SHOW_BOOKMARKBAR], bvalue,
             NULL);
} 

static void showTitlebarAndMenubarCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_SHOW_TITLEBAR_AND_MENUBAR], bvalue,
             NULL);
} 


static void showFullscreenCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_SHOW_FULLSCREEN], bvalue,
             NULL);
} 


static void defaultFontCallback(GtkComboBox *widget, WebKitSettings *settings)
{
    int CurrentSelect = gtk_combo_box_get_active(widget); 
    g_object_set(settings,
             key[PROP_DEFAULT_FONT_FAMILY], font[CurrentSelect],
             NULL);
}

static void defaultFontsizeCallback(GtkComboBox *widget, WebKitSettings *settings)
{ 
    int CurrentSelect = gtk_combo_box_get_active(widget); 
    g_object_set(settings,
             key[PROP_DEFAULT_FONT_SIZE], font_size[CurrentSelect],
             NULL);
}

static void pageZoomCallback(GtkComboBox *widget, WebKitSettings *settings)
{
    int CurrentSelect = gtk_combo_box_get_active(widget);
    g_object_set(settings,
             key[PROP_PAGE_ZOOM], zoom_factor[CurrentSelect],
             NULL);
}

static void zoomTextOnlyCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_ZOOM_TEXT_ONLY], bvalue,
             NULL);
}

static void showImageCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue;
    if(gtk_toggle_button_get_active(togglebutton)) {
      if((void *)settings->radiobutton1_content == (void *)togglebutton) {
        bvalue = TRUE;
      }
      else if((void *)settings->radiobutton2_content == (void *)togglebutton) {
        bvalue = FALSE;
      }
      else {
        printf("error showImageCallback\n"); 
        return;
      }
      g_object_set(settings,
               key[PROP_AUTO_LOAD_IMAGES], bvalue,
               NULL);
    }
} 

static void runJavascriptCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue;
    if(gtk_toggle_button_get_active(togglebutton)) {
      if((void *)settings->radiobutton3_content == (void *)togglebutton) {
        bvalue = TRUE;
      }
      else if((void *)settings->radiobutton4_content == (void *)togglebutton) {
        bvalue = FALSE;
      }
      else {
        printf("error runJavascriptCallback\n"); 
        return;
      }
      g_object_set(settings,
               key[PROP_ENABLE_JAVASCRIPT], bvalue,
               NULL);
    }
} 

static void pageContentCacheCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_PAGE_CONTENT_CACHE], bvalue,
             NULL);
} 

static void historySettingCallback(GtkComboBox *widget, WebKitSettings *settings)
{
    int CurrentSelect = gtk_combo_box_get_active(widget); 
    g_object_set(settings,
             key[PROP_HISTORY_SETTING], CurrentSelect,
             NULL);
}


static void clearBrowseRecordCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_CLEAR_BROWSE_RECORD], bvalue,
             NULL);
} 


static void clearDownloadRecordCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_CLEAR_DOWNLOAD_RECORD], bvalue,
             NULL);
} 


static void clearCookieAndOthersCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_CLEAR_COOKIE_AND_OTHERS], bvalue,
             NULL);
}

static void clearCachedImagesAndFilesCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_CLEAR_CACHED_IMAGES_AND_FILES], bvalue,
             NULL);
}

static void clearPasswordsCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_CLEAR_PASSWORDS], bvalue,
             NULL);
}

static void cookieSettingCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    int ivalue;
    if(gtk_toggle_button_get_active(togglebutton)) {
      if((void *)settings->radiobutton1_privacy == (void *)togglebutton) {
        ivalue = 0;
      }
      else if((void *)settings->radiobutton2_privacy == (void *)togglebutton) {
        ivalue = 1;
      }
      else if((void *)settings->radiobutton3_privacy == (void *)togglebutton) {
        ivalue = 2;
      }
      else {
        printf("error cookieSettingCallback\n"); 
        return;
      }
      g_object_set(settings,
               key[PROP_COOKIE_SETTING], ivalue,
               NULL);
    }
} 

static void trackLocationCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    int ivalue;
    if(gtk_toggle_button_get_active(togglebutton)) {
      if((void *)settings->radiobutton4_privacy == (void *)togglebutton) {
        ivalue = 0;
      }
      else if((void *)settings->radiobutton5_privacy == (void *)togglebutton) {
        ivalue = 1;
      }
      else if((void *)settings->radiobutton6_privacy == (void *)togglebutton) {
        ivalue = 2;
      }
      else {
        printf("error trackLocationCallback\n"); 
        return;
      }
      g_object_set(settings,
               key[PROP_TRACK_LOCATION], ivalue,
               NULL);
    }
} 

static void certificateRevocationCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    bool bvalue = gtk_toggle_button_get_active(togglebutton); 
    g_object_set(settings,
             key[PROP_CERTIFICATE_REVOCATION], bvalue,
             NULL);
}

static void mediaAccessCallback(GtkToggleButton *togglebutton, WebKitSettings *settings)
{
    int ivalue;
    if(gtk_toggle_button_get_active(togglebutton)) {
      if((void *)settings->radiobutton1_security == (void *)togglebutton) {
        ivalue = 0;
      }
      else if((void *)settings->radiobutton2_security == (void *)togglebutton) {
        ivalue = 1;
      }
      else {
        printf("error mediaAccessCallback\n"); 
        return;
      }
      g_object_set(settings,
               key[PROP_MEDIA_ACCESS], ivalue,
               NULL);
    }
} 

/*Build settings window from the glade file. Connecting widgets we interest. sunhaiming add.*/
GtkWidget * browser_settings_window_new(WebKitSettings *settings)
{
   GtkBuilder *builder;
   GtkWidget *window;
   char exe_path[MAXBUFSIZE];
   int count;
   char *pstr;
   count = readlink("/proc/self/exe", exe_path, MAXBUFSIZE);
   exe_path[count] = '\0';
   pstr = exe_path + strlen(exe_path) - strlen("CuprumBrowser");
   *pstr = '\0';
   strcat(exe_path, "resources/layout/cuprum-design.glade");
   builder = gtk_builder_new();
   gtk_builder_add_from_file(builder, exe_path, NULL);

   window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
   
   settings->entry1_general = GTK_WIDGET(gtk_builder_get_object(builder, "entry1_general"));
   g_signal_connect(G_OBJECT(settings->entry1_general), "changed", G_CALLBACK(homePageCallback), settings);

   settings->button1_general = GTK_WIDGET(gtk_builder_get_object(builder, "button1_general"));
   g_signal_connect(G_OBJECT(settings->button1_general), "clicked", G_CALLBACK(currentPageToHomePageCallback), settings);

   settings->radiobutton1_general = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton1_general"));
   g_signal_connect(G_OBJECT(settings->radiobutton1_general), "toggled", G_CALLBACK(onStartupCallback), settings);

   settings->radiobutton2_general = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton2_general"));
   g_signal_connect(G_OBJECT(settings->radiobutton2_general), "toggled", G_CALLBACK(onStartupCallback), settings);

   settings->radiobutton3_general = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton3_general"));
   g_signal_connect(G_OBJECT(settings->radiobutton3_general), "toggled", G_CALLBACK(onStartupCallback), settings);

   settings->radiobutton4_general = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton4_general"));
   g_signal_connect(G_OBJECT(settings->radiobutton4_general), "toggled", G_CALLBACK(openNewpageCallback), settings);

   settings->radiobutton5_general = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton5_general"));
   g_signal_connect(G_OBJECT(settings->radiobutton5_general), "toggled", G_CALLBACK(openNewpageCallback), settings);

   settings->radiobutton6_general = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton6_general"));
   g_signal_connect(G_OBJECT(settings->radiobutton6_general), "toggled", G_CALLBACK(openNewpageCallback), settings);

   settings->checkbutton1_general = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton1_general"));
   g_signal_connect(G_OBJECT(settings->checkbutton1_general), "toggled", G_CALLBACK(muchTabWarningCallback), settings);

   settings->checkbutton2_general = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton2_general"));
   g_signal_connect(G_OBJECT(settings->checkbutton2_general), "toggled", G_CALLBACK(showHomepageButtonCallback), settings);

   settings->checkbutton3_general = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton3_general"));
   g_signal_connect(G_OBJECT(settings->checkbutton3_general), "toggled", G_CALLBACK(showBookmarkbarCallback), settings);

   settings->checkbutton4_general = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton4_general"));
   g_signal_connect(G_OBJECT(settings->checkbutton4_general), "toggled", G_CALLBACK(showTitlebarAndMenubarCallback), settings);

   settings->checkbutton5_general = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton5_general"));
   g_signal_connect(G_OBJECT(settings->checkbutton5_general), "toggled", G_CALLBACK(showFullscreenCallback), settings);

   settings->comboboxtext1_content = GTK_WIDGET(gtk_builder_get_object(builder, "comboboxtext1_content"));
   g_signal_connect(G_OBJECT(settings->comboboxtext1_content), "changed", G_CALLBACK(defaultFontCallback), settings);

   settings->comboboxtext2_content = GTK_WIDGET(gtk_builder_get_object(builder, "comboboxtext2_content"));
   g_signal_connect(G_OBJECT(settings->comboboxtext2_content), "changed", G_CALLBACK(defaultFontsizeCallback), settings);


   settings->comboboxtext3_content = GTK_WIDGET(gtk_builder_get_object(builder, "comboboxtext3_content"));
   g_signal_connect(G_OBJECT(settings->comboboxtext3_content), "changed", G_CALLBACK(pageZoomCallback), settings);

   settings->checkbutton1_content = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton1_content"));
   g_signal_connect(G_OBJECT(settings->checkbutton1_content), "toggled", G_CALLBACK(zoomTextOnlyCallback), settings);

   settings->radiobutton1_content = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton1_content"));
   g_signal_connect(G_OBJECT(settings->radiobutton1_content), "toggled", G_CALLBACK(showImageCallback), settings);

   settings->radiobutton2_content = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton2_content"));
   g_signal_connect(G_OBJECT(settings->radiobutton2_content), "toggled", G_CALLBACK(showImageCallback), settings);

   settings->radiobutton3_content = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton3_content"));
   g_signal_connect(G_OBJECT(settings->radiobutton3_content), "toggled", G_CALLBACK(runJavascriptCallback), settings);

   settings->radiobutton4_content = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton4_content"));
   g_signal_connect(G_OBJECT(settings->radiobutton4_content), "toggled", G_CALLBACK(runJavascriptCallback), settings);

   settings->checkbutton3_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton3_privacy"));
   g_signal_connect(G_OBJECT(settings->checkbutton3_privacy), "toggled", G_CALLBACK(pageContentCacheCallback), settings);

   settings->comboboxtext1_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "comboboxtext1_privacy"));
   g_signal_connect(G_OBJECT(settings->comboboxtext1_privacy), "changed", G_CALLBACK(historySettingCallback), settings);

   settings->checkbutton4_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton4_privacy"));
   g_signal_connect(G_OBJECT(settings->checkbutton4_privacy), "toggled", G_CALLBACK(clearBrowseRecordCallback), settings);

   settings->checkbutton5_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton5_privacy"));
   g_signal_connect(G_OBJECT(settings->checkbutton5_privacy), "toggled", G_CALLBACK(clearDownloadRecordCallback), settings);

   settings->checkbutton6_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton6_privacy"));
   g_signal_connect(G_OBJECT(settings->checkbutton6_privacy), "toggled", G_CALLBACK(clearCookieAndOthersCallback), settings);

   settings->checkbutton7_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton7_privacy"));
   g_signal_connect(G_OBJECT(settings->checkbutton7_privacy), "toggled", G_CALLBACK(clearCachedImagesAndFilesCallback), settings);

   settings->checkbutton8_privacy = GTK_WIDGET(gtk_builder_get_object (builder, "checkbutton8_privacy"));
   g_signal_connect(G_OBJECT(settings->checkbutton8_privacy), "toggled", G_CALLBACK(clearPasswordsCallback), settings);

   settings->radiobutton1_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton1_privacy"));
   g_signal_connect(G_OBJECT(settings->radiobutton1_privacy), "toggled", G_CALLBACK(cookieSettingCallback), settings);

   settings->radiobutton2_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton2_privacy"));
   g_signal_connect(G_OBJECT(settings->radiobutton2_privacy), "toggled", G_CALLBACK(cookieSettingCallback), settings);

   settings->radiobutton3_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton3_privacy"));
   g_signal_connect(G_OBJECT(settings->radiobutton3_privacy), "toggled", G_CALLBACK(cookieSettingCallback), settings);

   settings->radiobutton4_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton4_privacy"));
   g_signal_connect(G_OBJECT(settings->radiobutton4_privacy), "toggled", G_CALLBACK(trackLocationCallback), settings);

   settings->radiobutton5_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton5_privacy"));
   g_signal_connect(G_OBJECT(settings->radiobutton5_privacy), "toggled", G_CALLBACK(trackLocationCallback), settings);

   settings->radiobutton6_privacy = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton6_privacy"));
   g_signal_connect(G_OBJECT(settings->radiobutton6_privacy), "toggled", G_CALLBACK(trackLocationCallback), settings);


   settings->checkbutton1_security = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton1_security"));
   g_signal_connect(G_OBJECT(settings->checkbutton1_security), "toggled", G_CALLBACK(certificateRevocationCallback), settings);


   settings->radiobutton1_security = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton1_security"));
   g_signal_connect(G_OBJECT(settings->radiobutton1_security), "toggled", G_CALLBACK(mediaAccessCallback), settings);

   settings->radiobutton2_security = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton2_security"));
   g_signal_connect(G_OBJECT(settings->radiobutton2_security), "toggled", G_CALLBACK(mediaAccessCallback), settings);

   browserSettingsWindowConstructed(settings);
   g_object_unref (G_OBJECT (builder));
   return window;
}

