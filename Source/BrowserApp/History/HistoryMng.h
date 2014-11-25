#ifndef HISTORY_MNG_H
#define HISTORY_MNG_H

#include "BrowserWindow.h"

#define HS_DEBUG_ON 0
#define HS_DEBUG

#if HS_DEBUG_ON
#define HS_DEBUG g_print
#endif

#define HISTORY_DB_PATH "/tmp/history"
#define FAVICON_DEFAULT_PATH ("/tmp/history/default.png")

//#define HISTORY_DB_PATH "./history"
//#define FAVICON_DEFAULT_PATH ("./history/default.png")



void HS_showHistoryManagerWindow();

void HS_showHistoryClearWindow();

void HS_history_changed(WebKitBackForwardList *backForwadlist, WebKitBackForwardListItem *itemAdded);

void HS_history_new(BrowserWindow *window, WebKitWebView *webview);

void HS_init(BrowserWindow *window);

int sql_callback(void *arg, int nr, char **values, char **names) ;

#endif
