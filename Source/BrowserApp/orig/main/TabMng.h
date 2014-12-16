#ifndef TAB_MNG_H
#define TAB_MNG_H
#include "BrowserWindow.h"

typedef struct _TabView TabView ;

struct _TabView{
    int tabId;
    GtkWidget *viewOverlay;
    WebKitWebView *webView;
};

#define MAX_TAB 20
#define MIN_ID_INDEX 0

typedef struct _TabMng TabMng;

struct _TabMng {
    int curId;
    int incedId; //id increase from here
    int totalTab;
    TabView tabviews[MAX_TAB];
    int (*generateTabId)(TabMng* tbmng);
    int (*getCurId)();

    int (*addTab)(TabMng* tbmng,int tabid,GtkWidget* viewoverlay,WebKitWebView *webView);
    int (*rmTab)(TabMng* tbmng,int tabid);
    void (*showTab)(TabMng* tbmng);
    TabView (*getTabView)(TabMng* tbmng ,int tabid);
    bool (*canAddTab)(TabMng* tbmng);
};



void initTabMng(TabMng* tbmng);
//void AddTab(struct TabMng* tbmng);
#endif
