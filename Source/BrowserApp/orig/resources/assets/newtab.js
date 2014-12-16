var createNodeId = function()
{
    var i = 0, j = 0;
    var nodes = document.getElementsByClassName("shortcut");
    for(i=1; i<nodes.length; i++)
    {
        var nodeId = "s" + i;
        var tmpid = "";
        for (j=0; j<nodes.length; j++)
        {
            tmpid = nodes[j].id;
            if (nodeId == tmpid)
                break;
        }
        if (j == nodes.length)
            return nodeId;
    }
}
function renameShortcut(ele)
{
    var shortcutNode = ele.parentNode;
    var renameNode = shortcutNode.lastElementChild;
    var old_name =renameNode.textContent;

    var name = prompt ("输入快捷方式标题", old_name);
    if (!name) return;
    renameNode.textContent = name;
    
    
    //todo database opt
}


var clearShortcut = function (ele)
{
    if(!confirm("您确定要删除此快捷方式吗？"))
        return;
        
    var shortcutNode = ele.parentNode.parentNode;
    var id = shortcutNode.id;

    shortcutNode.parentNode.removeChild(shortcutNode);
    
    var csNode = document.getElementById("cs");
    csNode.hidden = false;
    /*   zgh database opt
    try{
    var db = openDatabase("cupromhomepage", "1.0", "主页表", 1 * 1024, function () { });
    }
    catch(err)
    {
//        alert(err);
    }
    db.transaction(function (tx) {
    tx.executeSql(
    "delete from testtable where id= ?",
    [id],
    function (tx, result) {
 //   alert("ok");
    },
    function (tx, error) {
    alert('删除失败: ' + error.message);
    });
    });
*/
}

function load()
{
     //添加shortcut
        var tmplatNode = document.getElementById("s");   //获得模板结点
        var parentNode = tmplatNode.parentNode;
        var ids = parentNode.childNodes.length - 10;
        var clonNode = tmplatNode.cloneNode(true);
        clonNode.id = createNodeId();//"s" + ids;
        clonNode.hidden = false;
        clonNode.className = "shortcut";
        
        var csNode = document.getElementById("cs");
        var clearNode = clonNode.firstElementChild.firstElementChild;

        var tableNode = clearNode.nextElementSibling.firstElementChild;
        tableNode.hidden = true;
      
        var urlNode = clonNode.firstElementChild.lastElementChild;
        urlNode.href = "http://www.casiot.com/";
        var imgNode = urlNode.firstElementChild;
        imgNode.src = "../img/casiot.png";
        var renameNode = clonNode.lastElementChild;
        renameNode.textContent = "江苏中科物联网科技创业投资有限公司";
        
        parentNode.insertBefore(clonNode, csNode);
        /*
    try {
    var dataBase = openDatabase("cupromhomepage", "1.0", "学生表", 1 * 1024, function () { });
    }
    catch (error)
    {
       // alert(error);
    }
    
    //shortcutpages
    dataBase.transaction( function(tx) { 
                                        tx.executeSql(
                                        "create table if not exists testtable (id TEXT, url TEXT, titlename TEXT, icon TEXT)", 
                                        [], 
                                        null,//function(tx,result){ alert('创建shortcutpages表成功'); }, 
                                        null//function(tx, error){ alert('创建shortcutpages表失败:' + error.message);} 
                                            );
                                    });
                                    
                                    // 删除表语句
//    dataBase.transaction(function (tx) {
//    tx.executeSql('drop table shortcutpages');
    });
                                
    dataBase.transaction(function (tx) {
        tx.executeSql(
        "select * from testtable", [],
        function (tx, result) { 
//            alert(result.rows.length);
            var count = result.rows.length;
            if (8 < count)
            {
                count =8;
                csNode.hidden = true;
            }
                
            for (var i=0; i<count; i++){
                var dataNode = tmplatNode.cloneNode(true);
                dataNode.id = result.rows.item(i).id;
                 var clrNode = dataNode.firstElementChild.firstElementChild;

                var tbNode = clrNode.nextElementSibling.firstElementChild;
                tbNode.hidden = true;
              
                var ulNode = dataNode.firstElementChild.lastElementChild;
                var imgeNode = ulNode.firstElementChild;
                var rnNode = dataNode.lastElementChild;
                ids = parentNode.childNodes.length - 10;
                 dataNode.id = result.rows.item(i).id;//dataNode.id = "s" + ids;
                dataNode.hidden = false;
                dataNode.className = "shortcut";
                imgeNode.src = "../img/2.png";  //todo 资源名改为对应id的名字.png
                ulNode.href = result.rows.item(i).url;
                rnNode.textContent = result.rows.item(i).titlename;
                parentNode.insertBefore(dataNode,csNode);
            }
        //执行成功的回调函数
        //在这里对result 做你想要做的事情吧...........
    },
    function (tx, error) {
    alert('查询失败: ' + error.message);
    } );
    });
    */
}

var handMouse = function () {
	this.style.cursor = "hand"; 	
}

var defaultMouse = function () {
	this.style.cursor = "default";
}

var getAction = function (id)
    {
        var s = document.getElementById(id).childNodes[0];
        if (s.className == 'preview')
            return true;

/*
        var url = prompt ("输入快捷方式地址", "http://");
        if (!url) return false;
        if (url.indexOf ("://") == -1)
            url = "http://" + url;
*/           
        //添加shortcut
        var tmplatNode = document.getElementById(id);   //获得模板结点
        var parentNode = tmplatNode.parentNode;
//        var ids = parentNode.childNodes.length - 10;
        var ids = createNodeId();
        var clonNode = tmplatNode.cloneNode(true);
        clonNode.id = ids;//id + ids;
        clonNode.hidden = false;
        clonNode.classname = "shortcut";
        
        var csNode = document.getElementById("cs");
        var clearNode = clonNode.firstElementChild.firstElementChild;

        var tableNode = clearNode.nextElementSibling.firstElementChild;
        var urltextNode = tableNode.firstElementChild.lastElementChild.firstElementChild;
        urltextNode.id = "urltext" + ids;
        var titletextNode = tableNode.firstElementChild.nextElementSibling.lastElementChild.firstElementChild;
        titletextNode.id = "titletext" + ids;
      
        var urlNode = clonNode.firstElementChild.lastElementChild;
        var renameNode = clonNode.lastElementChild;
        var urltext = "";
        var titletext = "";
          
	     var okbtnNode = tableNode.lastElementChild.firstElementChild.firstElementChild;
	     okbtnNode.onclick = function () {
								urltext = urltextNode.value;
								titletext = titletextNode.value;	
								urlNode.href = urltext;
								renameNode.textContent = titletext;
								tableNode.hidden = true;
/*								
								//写入数据
								try{
								    var dataBase = openDatabase("cupromhomepage", "1.0", "快捷主页", 1*1024, null);
								}
								catch(error)
								{
								    alert(error);
								}
								
                                if (!dataBase) {
                                    alert("数据库创建失败！");
                                } 
                                
                                dataBase.transaction( function(tx) { 
                                        tx.executeSql(
                                        "create table if not exists testtable (id TEXT, url TEXT, titlename TEXT, icon TEXT)", 
                                        [], 
                                        null,//function(tx,result){ alert('创建shortcutpages表成功'); }, 
                                        function(tx, error){ alert('创建shortcutpages表失败:' + error.message); 
                                            });
                                    });
                                    
                                dataBase.transaction(function (tx) {
                                        tx.executeSql(
                                        "insert into testtable (id, url, titlename) values(?, ?, ?)",
                                        [ids, urltext, titletext],
                                        null,//function () { alert(urltext); },
                                        function (tx, error) { alert('添加数据失败: ' + error.message); 
                                            } );
                                    });
                                    */
	     					 }
	     							
	     var cancelbtnNode = tableNode.lastElementChild.firstElementChild.lastElementChild;
	     cancelbtnNode.onclick = function () {
	     										clonNode.parentNode.removeChild(clonNode);
	     										csNode.hidden = false;
	     								  }

        parentNode.insertBefore(clonNode, csNode);
        
        if (document.getElementsByClassName("shortcut").length >= 11 ) {
        		csNode.hidden = true;
        	}

        return false;
    }

