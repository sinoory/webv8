var LifeAssistantConfig=(LifeAssistantConfig!==undefined)?LifeAssistantConfig:{};(function(a){a.isDebug=false;a.list=[{logo:"工行.jpg",name:"工商银行",url:"http://www.icbc.com.cn/icbc"},{logo:"建行.jpg",name:"建设银行",url:"http://www.ccb.com"},{logo:"中行.jpg",name:"中国银行",url:"http://www.boc.cn"},{logo:"农行.jpg",name:"农业银行",url:"http://www.abchina.com/cn/"},{logo:"邮政.gif",name:"邮政储蓄",url:"http://www.psbc.com/portal/zh_CN/index.html"},{logo:"招行.jpg",name:"招商银行",url:"http://www.cmbchina.com/"},{logo:"交行.jpg",name:"交通银行",url:"http://www.bankcomm.com"},{logo:"民生.jpg",name:"民生银行",url:"http://www.cmbc.com.cn/"},{logo:"光大.jpg",name:"光大银行",url:"http://www.cebbank.com"},{logo:"浦发.png",name:"浦发银行",url:"http://www.spdb.com.cn/"},{logo:"中信.png",name:"中信银行",url:"http://bank.ecitic.com/"},{logo:"兴业.jpg",name:"兴业银行",url:"http://www.cib.com.cn/"},{logo:"广发.jpg",name:"广发银行",url:"http://www.cgbchina.com.cn/"},{logo:"华夏.png",name:"华夏银行",url:"http://www.hxb.com.cn/"},{logo:"平安.jpg",name:"平安银行",url:"http://bank.pingan.com/"},{logo:"上海.jpg",name:"上海银行",url:"http://www.bankofshanghai.com/"},{logo:"支付宝.jpg",name:"支付宝",url:"https://www.alipay.com/"},{logo:"网银.png",name:"更多银行",url:"http://licai.360.cn/bank"}]})(LifeAssistantConfig);//Object.preventExtensions(LifeAssistantConfig);

var imgDotting=null;
function dotting(a){imgDotting=imgDotting||document.createElement("img");imgDotting.src=a};

$(function(){
//  var b=chrome.extension.getBackgroundPage();
//  var a=b.LifeAssistantConfig;
  var a=LifeAssistantConfig;
 (function(){
//    b.dotting("http://dd.browser.360.cn/static/a/48.8514.gif?_t="+Math.random());
//    dotting("http://dd.browser.360.cn/static/a/48.8514.gif?_t="+Math.random());
    var e=a.list;
    var f=$(".items");
    for(var c=0;c<e.length;c++){
      var d=e[c];
      f.append($('<div class="item"></div>').append($('<img alt=""/>').attr({src:d.logo})).append($('<a target="_blank"></a>').attr({href:d.url}).html(d.name)))
    }
    $(document).on("click",".item",function(h){
                 //h.preventDefault();
                 var g=$(this).children("a").attr("href");
                 if(g==="https://kyfw.12306.cn/otn/leftTicket/init"){
                   //b.dotting("http://dd.browser.360.cn/static/a/48.2603.gif?_t="+Math.random())
                   dotting("http://dd.browser.360.cn/static/a/48.2603.gif?_t="+Math.random())
                 }
                 else{
                   if(g==="http://app.148365.com/Default.aspx"){
                       //b.dotting("http://dd.browser.360.cn/static/a/48.4855.gif?_t="+Math.random())
                       dotting("http://dd.browser.360.cn/static/a/48.4855.gif?_t="+Math.random())
                   }
                 }
                 Utils.openPage(g)
    });

    //$(document).on("click",".btn-close",function(){window.close()})
})();
a.isDebug||(console.log=function(){});
document.oncontextmenu=function(){return a.isDebug}
});
