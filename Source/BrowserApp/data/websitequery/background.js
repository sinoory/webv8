(function getBeian() {
var uri = 'http://www.beianbeian.com/search/baidu.com';
console.log("js#uri:" + uri);

var ajax = new XMLHttpRequest();
ajax.open("get", uri, false);
function HandleStateChange() {
console.log("onreadystatechange = " + ajax.readyState);
if (ajax.readyState == 4){
var data = ajax.responseText;
	var head = data.indexOf('#show_table td');
	var dead_index = data.indexOf('没有符合条件的记录');
	var source_code = data.substr(head+740, 1200);
   if (dead_index < 0 && (ajax.status == 200 || ajax.status == "200")) {
		    console.log("get success");
       var index1 = source_code.indexOf('<');
       var Organizer = source_code.substring(0, index1);
       source_code = source_code.substring(index1+92);
       index1 = source_code.indexOf('<');
       var nature = source_code.substring(0, index1);
       source_code = source_code.substring(index1+157);
       index1 = source_code.indexOf('<');
       var license = source_code.substring(0, index1);
       source_code = source_code.substring(index1+169);
       index1 = source_code.indexOf('\n');
       var sitename = source_code.substring(0, index1-1);
       index1 = source_code.indexOf('domain=');
       source_code = source_code.substring(index1+7);
       var index2 = source_code.indexOf('"');
       var homepage = source_code.substring(0, index2);
       index1 = source_code.indexOf('"pass_time">');
       source_code = source_code.substring(index1);
       index1 = source_code.indexOf('>');
       index2 = source_code.indexOf('<');
       var checktime = source_code.substring(index1+1, index2);

		    console.log("Organizer:"+Organizer+"len:"+Organizer.length);
		    if(Organizer.length == 0)
		    {
			    console.log("website_query_info" + "#"  + "unknown");
			    return;
		    }

        console.log("website_query_info" + "#"  + Organizer +  "#" + nature + "#"  + license  + "#"  + sitename + "#"  + homepage + "#"  + checktime );
	} else {  
		console.log("website_query_info" + "#"  + "unknown");
	} 
}
}

ajax.onreadystatechange= HandleStateChange;
ajax.send();

})();

