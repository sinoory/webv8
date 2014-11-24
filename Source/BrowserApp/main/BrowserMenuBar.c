#include "cmakeconfig.h"
#include "BrowserWindow.h"
#include "BrowserMenuBar.h"

#include <glib/gi18n.h>

//自定义信号量
enum {
    MENU_FIND_SIGNAL,
    MENU_ZOOM_IN_SIGNAL,
    MENU_ZOOM_OUT_SIGNAL,
    MENU_ZOOM_FIT_SIGNAL,
    MENU_PAGESTYLE_NOSTYLE_SIGNAL,
    MENU_PAGESTYLE_WEBSTYLE_SIGNAL,
    MENU_FULLSCREEN_SIGNAL,
    MENU_BOOKMARKBAR_SIGNAL,
    MENU_HISTORY_MANAGER_SIGNAL,                //[8] 管理历史记录 add by zlf 2014.11.12
    MENU_HISTORY_CLEAR_SIGNAL,                        //[9]删除历史记录  add by zlf 2014.11.12
    
    MENU_QUIT_SIGNAL,
    
    LAST_SIGNAL
};
static gint menu_signals[LAST_SIGNAL] = { 0 };

struct _BrowserMenuBar {
    GtkMenuBar parent;

    GtkWidget* filemenu;
    GtkWidget* menuitem_new_page;
    GtkWidget* menuitem_new_window;
    GtkWidget* menuitem_open;
    GtkWidget* menuitem_save_as;
    GtkWidget* menuitem_print_preview;
    GtkWidget* menuitem_print;
    GtkWidget* menuitem_quit;
    
    GtkWidget* editmenu;
    GtkWidget* menuitem_undo;
    GtkWidget* menuitem_redo;
    GtkWidget* menuitem_copy;
    GtkWidget* menuitem_cut;
    GtkWidget* menuitem_paste;
    GtkWidget* menuitem_delete;
    GtkWidget* menuitem_find;
    
    GtkWidget* viewmenu;
    GtkWidget* menuitem_toolbar;
    GtkWidget* menuitem_zoom_in;
    GtkWidget* menuitem_zoom_out;
    GtkWidget* menuitem_zoom_fit;
    GtkWidget* menuitem_pagestyle;
    GtkWidget* menuitem_fullscreen;
    
    //视图中工具栏的二级菜单
    GtkWidget* menuitem_toolbar_menubar;
    GtkWidget* menuitem_toolbar_toolbar;
    GtkWidget* menuitem_toolbar_bookmarkbar;
    
    //页面风格二级菜单
    GtkWidget* menuitem_pagestyle_nostyle;
    GtkWidget* menuitem_pagestyle_webstyle;
    
    GtkWidget* historymenu;
    GtkWidget* menuitem_managerecords;
    GtkWidget* menuitem_deleterecords;
    
    GtkWidget* bookmarkmenu;
    GtkWidget* menuitem_managebookmarks;
    GtkWidget* menuitem_addthispage;
    
    GtkWidget* toolmenu;
    GtkWidget* menuitem_viewsourcecode;
    GtkWidget* menuitem_pageinfo;
    GtkWidget* menuitem_clearcookie;
    GtkWidget* menuitem_clearcache;   
    
    GtkWidget* helpmenu;
    GtkWidget* menuitem_help;
    GtkWidget* menuitem_about;
};

struct _BrowserMenuBarClass {
    GtkMenuBarClass parentClass;
};

G_DEFINE_TYPE(BrowserMenuBar, browser_menu_bar, GTK_TYPE_MENU_BAR)

static void on_menu_activate(GtkMenuItem* item,gpointer data)
{
    g_print("menuitem %s is pressed.\n",(gchar*)data);
}

static void on_clear_cache_activate(GtkMenuItem* item,gpointer data)
{
    webkit_web_context_clear_cache(webkit_web_context_get_default());
}

static void on_clear_cookie_activate(GtkMenuItem* item, gpointer data)
{
    WebKitCookieManager* cookiemanager = webkit_web_context_get_cookie_manager(webkit_web_context_get_default());
    if(cookiemanager){
        webkit_cookie_manager_delete_all_cookies(cookiemanager);
    }
}

static void on_open_new_window_cb(gpointer data)
{
    createBrowserWindow("", NULL);
}

static void on_open_file_cb(gpointer data)
{
/*
    GtkWidget *FileSelection;
    FileSelection = gtk_file_selection_new("选择文件");
    gtk_file_selection_set_filename(GTK_FILE_SELECTION(FileSelection), "*.*");
    gtk_widget_show(FileSelection);
    */
}

static void on_menu_find_cb(GtkMenuItem *item, gpointer data)
{
    g_signal_emit(G_OBJECT(data),menu_signals[MENU_FIND_SIGNAL],0);
}

static void on_menu_bookmarkbar_cb(GtkMenuItem *item, gpointer data)
{
    g_signal_emit(G_OBJECT(data), menu_signals[MENU_BOOKMARKBAR_SIGNAL], 0);
}

static void on_menu_zoomin_cb(GtkMenuItem *item, gpointer data)
{
    g_signal_emit(G_OBJECT(data),menu_signals[MENU_ZOOM_IN_SIGNAL],0);
}

static void on_menu_zoomout_cb(GtkMenuItem *item, gpointer data)
{
    g_signal_emit(G_OBJECT(data),menu_signals[MENU_ZOOM_OUT_SIGNAL],0);
}

static void on_menu_zoomfit_cb(GtkMenuItem *item, gpointer data)
{
    g_signal_emit(G_OBJECT(data),menu_signals[MENU_ZOOM_FIT_SIGNAL],0);
}

static void on_menu_nostyle_cb(GtkMenuItem *item, gpointer data)
{
    g_signal_emit(G_OBJECT(data),menu_signals[MENU_PAGESTYLE_NOSTYLE_SIGNAL],0);
}

static void on_menu_webstyle_cb(GtkMenuItem *item, gpointer data)
{
    g_signal_emit(G_OBJECT(data),menu_signals[MENU_PAGESTYLE_WEBSTYLE_SIGNAL],0);
}

static void on_menu_fullscreen_cb(GtkMenuItem *item, gpointer data)
{
    g_signal_emit(G_OBJECT(data),menu_signals[MENU_FULLSCREEN_SIGNAL],0);
}

static void on_menu_quit_cb(GtkMenuItem *item, gpointer data)
{
    g_signal_emit(G_OBJECT(data), menu_signals[MENU_QUIT_SIGNAL], 0);
}

//add by zlf start 2014.11.12
static void on_menu_history_manager_cb(GtkMenuItem *item, gpointer data)
{
//    g_print("file:%s\n  func:%s\n  line[%d]\n",__FILE__,__func__,__LINE__);
    g_signal_emit(G_OBJECT(data),menu_signals[MENU_HISTORY_MANAGER_SIGNAL],0);
}


static void on_menu_history_clear_cb(GtkMenuItem *item, gpointer data)
{
//    g_print("file:%s\n  func:%s\n  line[%d]\n",__FILE__,__func__,__LINE__);
    g_signal_emit(G_OBJECT(data),menu_signals[MENU_HISTORY_CLEAR_SIGNAL],0);
}
//add by zlf end 2014.11.12
static void browser_menu_bar_init(BrowserMenuBar *menubar)
{
    GtkWidget* menu;
    GtkWidget* menu_2;  //用于创建二级菜单
    GtkWidget* rootmenu;
    GtkWidget* menuitem;
    GtkWidget* menuitem_2;  //用于创建二级菜单项
    GtkWidget* image;
    GSList* group = NULL;
    
    gtk_container_set_border_width(GTK_CONTAINER(menubar), 0);

    menu = gtk_menu_new();
    menuitem = gtk_image_menu_item_new_with_label("新建标签页");
    menubar->menuitem_new_page = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect_swapped(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate), (gpointer)(" 新建标签页 "));
    
    image = gtk_image_new_from_resource("");
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW,NULL);
//    menuitem = gtk_image_menu_item_new_with_label("新建窗口");
//    gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM(menuitem), image);
//    gtk_image_menu_item_set_image(menuitem, TRUE);
    menubar->menuitem_new_window = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect_swapped(G_OBJECT(menuitem),"activate",G_CALLBACK(on_open_new_window_cb), (gpointer)(" 新建窗口 "));
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN,NULL);
    menubar->menuitem_open = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect_swapped(G_OBJECT(menuitem),"activate",G_CALLBACK(on_open_file_cb),(gpointer)(" 打开 "));

    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS,NULL);
    menubar->menuitem_save_as = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)(" 页面另存为 "));
    
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_PRINT_PREVIEW,NULL);
    menubar->menuitem_print_preview = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)(" 打印预览 "));
    
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_PRINT,NULL);
    menubar->menuitem_print = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)(" 打印 "));
    
    menuitem=gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    menuitem=gtk_image_menu_item_new_from_stock( GTK_STOCK_QUIT,NULL);
    menubar->menuitem_quit = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_quit_cb),menubar);
//zgh  rootmenu=gtk_menu_item_new_with_label(" 文件 "); 
//  rootmenu=gtk_menu_item_new_from_stock(GTK_STOCK_FILE,accel_group);
    rootmenu=gtk_menu_item_new_with_mnemonic("文件(_F)");
    menubar->filemenu = rootmenu;
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),menu);
//    menubar=gtk_menu_bar_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),rootmenu);
    rootmenu=gtk_menu_item_new_with_mnemonic("编辑(_E)");
    menubar->editmenu = rootmenu;
    menu=gtk_menu_new();
    
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_UNDO,NULL);
    menubar->menuitem_undo = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)(" 撤消 "));
    
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_REDO,NULL);
    menubar->menuitem_redo = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)(" 重做 "));
    
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT,NULL);
    menubar->menuitem_cut = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)(" 剪切 "));
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY,NULL);
    menubar->menuitem_copy = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)("复制 "));
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE,NULL);
    menubar->menuitem_paste = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)(" 粘贴 "));
    
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_DELETE,NULL);
    menubar->menuitem_delete = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)(" 删除 "));
    
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_FIND,NULL);
    menubar->menuitem_find = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_find_cb), menubar);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),rootmenu);
    
    //视图
    rootmenu = gtk_menu_item_new_with_mnemonic("视图(_V)");
    menubar->viewmenu = rootmenu;
    menu = gtk_menu_new();
    
    menuitem = gtk_menu_item_new_with_label("工具栏 ");
    menubar->menuitem_toolbar = menu;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    
//二级开关菜单
    menu_2 = gtk_menu_new();
    menuitem_2 = gtk_check_menu_item_new_with_mnemonic("菜单栏(_M)");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem_2),TRUE);
    menubar->menuitem_toolbar_menubar = menuitem_2;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_2), menuitem_2);
    g_signal_connect(G_OBJECT(menuitem_2), "activate", G_CALLBACK(on_menu_activate), (gpointer)("菜单栏显示隐藏"));
    
    menuitem_2 = gtk_check_menu_item_new_with_mnemonic("工具栏(_T)");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem_2),TRUE);
    menubar->menuitem_toolbar_menubar = menuitem_2;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_2), menuitem_2);
    g_signal_connect(G_OBJECT(menuitem_2), "activate", G_CALLBACK(on_menu_activate), (gpointer)("工具栏显示隐藏"));
    
    menuitem_2 = gtk_check_menu_item_new_with_mnemonic("书签栏(_B)");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem_2),FALSE);
    menubar->menuitem_toolbar_menubar = menuitem_2;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_2), menuitem_2);
    g_signal_connect(G_OBJECT(menuitem_2), "activate", G_CALLBACK(on_menu_bookmarkbar_cb), menubar);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu_2);
// ----工具栏二级菜单
    
    //放大
    menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
    menubar->menuitem_zoom_in = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_zoomin_cb), menubar);
    //缩小
    menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
    menubar->menuitem_zoom_out = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_zoomout_cb), menubar);
    //恢复缩放
    menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_FIT, NULL);
    menubar->menuitem_zoom_fit = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_zoomfit_cb), menubar);
    
    menuitem = gtk_menu_item_new_with_label( "页面风格 " );
    menubar->menuitem_pagestyle = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    
    
//页面风格二级菜单
    menu_2 = gtk_menu_new();
    
    menuitem_2 = gtk_radio_menu_item_new_with_label(NULL, "无风格");
    group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (menuitem_2));
    menubar->menuitem_pagestyle_nostyle = menuitem_2;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_2), menuitem_2);
    g_signal_connect(G_OBJECT(menuitem_2), "activate", G_CALLBACK(on_menu_nostyle_cb), menubar);
    
    menuitem_2 = gtk_radio_menu_item_new_with_label(group, "网页风格");
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuitem_2),TRUE);
    menubar->menuitem_pagestyle_webstyle = menuitem_2;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_2), menuitem_2);
    g_signal_connect(G_OBJECT(menuitem_2), "activate", G_CALLBACK(on_menu_webstyle_cb), menubar);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), menu_2);
//-----页面风格二级菜单
    
    //全屏
    menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_FULLSCREEN, NULL);
    menubar->menuitem_fullscreen = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_fullscreen_cb), menubar);
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), rootmenu);
    // -视图
    
    //历史记录
    rootmenu = gtk_menu_item_new_with_mnemonic("历史记录(_S)");
    menubar->historymenu = rootmenu;
    menu = gtk_menu_new();
    
    menuitem = gtk_menu_item_new_with_label("管理所有历史记录 ");
    menubar->menuitem_managerecords = menu;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_history_manager_cb), menubar); //modify by zlf 2014.11.12
//    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_activate), (gpointer)( "管理所有历史记录" ));
    
    menuitem = gtk_menu_item_new_with_label( "删除所有历史记录 " );
    menubar->menuitem_deleterecords = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_history_clear_cb),menubar);   //modify by zlf 2014.11.12
//    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_activate), (gpointer)(" 删除所有历史记录 "));
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), rootmenu);
    // -历史记录
    
    //书签
    rootmenu = gtk_menu_item_new_with_mnemonic("书签(_B)");
    menubar->bookmarkmenu = rootmenu;
    menu = gtk_menu_new();
    
    menuitem = gtk_menu_item_new_with_label("管理所有书签 ");
    menubar->menuitem_managebookmarks = menu;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_activate), (gpointer)( "管理所有书签" ));
    
    menuitem = gtk_menu_item_new_with_label( "删除所有书签 " );
    menubar->menuitem_addthispage = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_activate), (gpointer)(" 删除所有书签 "));
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), rootmenu);
    // -书签
    
    //工具
    rootmenu = gtk_menu_item_new_with_mnemonic("工具(_B)");
    menubar->toolmenu = rootmenu;
    menu = gtk_menu_new();
    
    menuitem = gtk_menu_item_new_with_label("查看源代码 ");
    menubar->menuitem_viewsourcecode = menu;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_activate), (gpointer)( "查看源代码" ));
    
    menuitem = gtk_menu_item_new_with_label( "页面信息 " );
    menubar->menuitem_pageinfo = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_menu_activate), (gpointer)(" 页面信息 "));

    menuitem = gtk_menu_item_new_with_label( "清除缓存 " );
    menubar->menuitem_clearcache = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_clear_cache_activate), (gpointer)(" 清除缓存 "));

    menuitem = gtk_menu_item_new_with_label( "清除cookie " );
    menubar->menuitem_clearcookie = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
    g_signal_connect(G_OBJECT(menuitem), "activate", G_CALLBACK(on_clear_cookie_activate), (gpointer)(" 清除cookie "));
    
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), rootmenu);
    // -工具
    
    rootmenu=gtk_menu_item_new_with_mnemonic("帮助(_H)");
    menubar->helpmenu = rootmenu;
    menu=gtk_menu_new();
    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_HELP,NULL);
    menubar->menuitem_help = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)( " 帮助 "));
    menuitem=gtk_menu_item_new_with_label(" 关于");
//    menuitem=gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);
    menubar->menuitem_about = menuitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
    g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(on_menu_activate),(gpointer)(" 关于 "));
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),rootmenu);
//    gtk_widget_show(menu);
//    gtk_widget_show(editmenu);
    gtk_widget_show_all(GTK_WIDGET(menubar));
}

static void browser_menu_bar_class_init(BrowserMenuBarClass *klass)
{
    GObjectClass *objectclass = G_OBJECT_CLASS(klass);
/*
    menu_signals[MENU_CONTROL_F_SIGNAL] = g_signal_new("search_something",
							G_TYPE_FROM_CLASS(objectclass),
							G_SIGNAL_RUN_FIRST,
							0, NULL, NULL,
							g_cclosure_marshal_VOID__VOID,
							G_TYPE_NONE, 0, NULL);
*/
                     
    menu_signals[MENU_FIND_SIGNAL] =
        g_signal_new("menu_find",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);
    
    menu_signals[MENU_ZOOM_IN_SIGNAL] =
        g_signal_new("menu_zoom_in",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);
                     
    menu_signals[MENU_ZOOM_OUT_SIGNAL] =
        g_signal_new("menu_zoom_out",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);
    
    menu_signals[MENU_ZOOM_FIT_SIGNAL] =
        g_signal_new("menu_zoom_fit",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);
    
    menu_signals[MENU_PAGESTYLE_NOSTYLE_SIGNAL] =
        g_signal_new("menu_nopagestyle",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);
                     
    menu_signals[MENU_PAGESTYLE_WEBSTYLE_SIGNAL] =
        g_signal_new("menu_webpagestyle",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);    
    
    menu_signals[MENU_FULLSCREEN_SIGNAL] =
        g_signal_new("menu_fullscreen",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);   
    //add by zlf start. 2014.11.12
    menu_signals[MENU_HISTORY_MANAGER_SIGNAL] =
        g_signal_new("menu_history_manager",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);   
    menu_signals[MENU_HISTORY_CLEAR_SIGNAL] =
        g_signal_new("menu_history_clear",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);   
    //add by zlf end. 2014.11.12
    
    menu_signals[MENU_QUIT_SIGNAL] =
        g_signal_new("menu_quit",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);   
                  
    menu_signals[MENU_BOOKMARKBAR_SIGNAL] =
        g_signal_new("menu_bookmarkbar",
                     G_TYPE_FROM_CLASS(objectclass),
                     G_SIGNAL_RUN_LAST,
                     0, NULL, NULL,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0); 
}

GtkWidget *browser_menu_bar_new()
{
    GtkMenuBar *menuBar = GTK_MENU_BAR(g_object_new(BROWSER_TYPE_MENU_BAR, NULL));
    return GTK_WIDGET(menuBar);
}

void browser_menu_bar_add_accelerators(BrowserMenuBar *menuBar, GtkAccelGroup *accelGroup)
{
    g_return_if_fail(BROWSER_IS_MENU_BAR(menuBar));
    g_return_if_fail(GTK_IS_ACCEL_GROUP(accelGroup));

    gtk_widget_add_accelerator(menuBar->menuitem_new_window, "activate", accelGroup, GDK_KEY_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(menuBar->menuitem_open, "activate", accelGroup, GDK_KEY_O, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
//    gtk_widget_add_accelerator(menuBar->menuitem_save_as, "activate", accelGroup, GDK_KEY_A, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
//    gtk_widget_add_accelerator(menuBar->menuitem_print_preview, "activate", accelGroup, GDK_KEY_V, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(menuBar->menuitem_quit, "activate", accelGroup, GDK_KEY_Q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    
//    gtk_widget_add_accelerator(menuBar->menuitem_undo, "activate", accelGroup, GDK_KEY_Z, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
//    gtk_widget_add_accelerator(menuBar->menuitem_redo, "activate", accelGroup, GDK_KEY_Z, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
//    gtk_widget_add_accelerator(menuBar->menuitem_cut, "activate", accelGroup, GDK_KEY_X, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
//    gtk_widget_add_accelerator(menuBar->menuitem_copy, "activate", accelGroup, GDK_KEY_C, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
//    gtk_widget_add_accelerator(menuBar->menuitem_paste, "activate", accelGroup, GDK_KEY_P, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(menuBar->menuitem_delete, "activate", accelGroup, GDK_KEY_Delete, 0, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(menuBar->menuitem_find, "activate", accelGroup, GDK_KEY_F, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    gtk_widget_add_accelerator(menuBar->menuitem_fullscreen, "activate", accelGroup, GDK_KEY_F11, 0, GTK_ACCEL_VISIBLE);
    
    gtk_widget_add_accelerator(menuBar->menuitem_help, "activate", accelGroup, GDK_KEY_H, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

}

