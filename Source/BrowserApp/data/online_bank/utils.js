var Utils=(Utils!==undefined)?Utils:{};

(function(a){
    function b(c){
        //chrome.windows.getLastFocused(function(d){chrome.tabs.getAllInWindow(d.id,function(e){c(e)})})
    }
    a.openPage=function(c){
        var d=null;
        window.open(c, "_blank");
/*
        b(function(f){
            for(var e=0;e<f.length;e++){
                var g=f[e];
                if(g.url===c){
                    (function(h){chrome.tabs.getSelected(null,function(i){
                      if(i.url===c){chrome.tabs.update(i.id,{selected:true})}
                      else{chrome.tabs.update(h.id,{selected:true})}
                      })})(g);
                    return
                }
                else{
                    if(g.url==="chrome://newtab/"){d=g}
                }
            }
            if(d){chrome.tabs.update(d.id,{url:c,selected:true})}
            else{chrome.windows.getCurrent(function(i){var h=i.id;chrome.tabs.create({windowId:h,url:c})})}
        })
*/
    }
})(Utils);
//Object.preventExtensions(Utils);

