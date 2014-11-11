
#include "TabMng.h"

#define mlog g_print

int generateTabId_(TabMng* tbmng){
    mlog("generateTabId_ totalTab=%d MAX_TAB=%d\n",tbmng->totalTab,MAX_TAB);
    if(!tbmng->canAddTab(tbmng)){
        return -1;
    }
    tbmng->incedId++;
    return tbmng->incedId;
}

bool canAddTab_(TabMng* tbmng){
    if(tbmng->totalTab >= MAX_TAB){
        return false;
    }
    return true;
}

int addTab_(TabMng* tbmng,int tabid,GtkWidget* viewoverlay,WebKitWebView *webView){
    int i=0;
    mlog("addTab tabid=%d totalTab=%d\n",tabid,tbmng->totalTab);
    if(!tbmng->canAddTab(tbmng)){
        return -1;
    }
    if(!viewoverlay || !webView){
        g_print("xxxxxWarning : addTab viewoverlay=%p webView=%p\n",viewoverlay,webView);
    }
    for(;i<MAX_TAB;i++){
        if(tbmng->tabviews[i].tabId==-1){
            tbmng->tabviews[i].tabId=tabid;
            tbmng->tabviews[i].viewOverlay=viewoverlay;
            tbmng->tabviews[i].webView=webView;
            tbmng->totalTab++;
            mlog("addedTab totalTab=%d\n",tbmng->totalTab);
            return 0;
        }
    }
    return -1;
}

int rmTab_(TabMng* tbmng,int tabid){
    int i=0;
    for(;i<MAX_TAB;i++){
        if(tbmng->tabviews[i].tabId==tabid){
            tbmng->tabviews[i].tabId=-1;
            tbmng->tabviews[i].viewOverlay=0;
            tbmng->totalTab--;
            return 0;
        }
    }
    return -1;
}
TabView getTabView_(TabMng* tbmng,int tabid){
    int i=0;
    for(;i<MAX_TAB;i++){
        if(tbmng->tabviews[i].tabId==tabid){
            return tbmng->tabviews[i];
        }
    }
}

void initTabMng(TabMng* tbmng){
    tbmng->totalTab=0;
    tbmng->incedId=MIN_ID_INDEX;
    tbmng->generateTabId=generateTabId_;
    tbmng->addTab=addTab_;
    tbmng->rmTab=rmTab_;
    tbmng->getTabView=getTabView_;
    tbmng->canAddTab=canAddTab_;
    int i=0;
    for(;i<MAX_TAB;i++){
        tbmng->tabviews[i].tabId=-1;
    }
}



