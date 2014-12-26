
(function getBeian() {
//alert("js start:1");
var uri = '%s';
console.log("js#uri:" + uri);
//$.ajaxSetup({ 
    //async : true 
//});
$.get(uri, function(data,status) {
	console.log("get" + uri);
	//alert("js start:2");
	
	if (status == "success") {  
		console.log("get success");
//console.log("get success" + data);
		var $tr = $(data).find('#show_table tr:eq(1)');
		var Organizer = $tr.find('td:eq(1)').text();
		var nature = $tr.find('td:eq(2)').text();
		var license = $tr.find('td:eq(3) a:eq(0)').text();
		var sitename = $tr.find('td:eq(4)').text();
		var homepage = $tr.find('td:eq(5)').text();
		var checktime = $tr.find('td:eq(6)').text();

		console.log("Organizer:"+Organizer+"len:"+Organizer.length);
		if(Organizer.length == 0)
		{
			console.log("website_query_info" + "#"  + "unknown");
			//alert("unknown");
			return;
		}

		console.log("website_query_info" + "#"  + $.trim(Organizer) +  "#" + $.trim(nature) + "#"  + $.trim(license)  + "#"  + $.trim(sitename) + "#"  + $.trim(homepage) + "#"  + $.trim(checktime) );
	} else {  
		console.log("website_query_info" + "#"  + "unknown");
		alert("###unknown");
	} 
			//alert("js start:2");
  	})
})();
//alert("background ok");

	//var urlReg = /[a-zA-Z0-9][-a-zA-Z0-9]{0,62}(\.[a-zA-Z0-9][-a-zA-Z0-9]{0,62})+\.?/;
	//url=urlReg.exec(url);

/*
			Organizer =$.trim(Organizer);
			nature = $.trim(nature);
			license = $.trim(license);
			sitename $.trim(sitename);
			homepage =$.trim(homepage);
			checktime =$.trim(checktime);
			//console.log("Organizer[" + Organizer + "]");
			//console.log("Organizer2[" + $.trim(Organizer) + "]");

			Organizer = Organizer.replace(/(^\s*)|(\s*$)/g,'');
			nature = nature.replace(/(^\s*)|(\s*$)/g,'');
			license = license.replace(/(^\s*)|(\s*$)/g,'');
			sitename = sitename.replace(/(^\s*)|(\s*$)/g,'');
			homepage = homepage.replace(/(^\s*)|(\s*$)/g,'');
			checktime = checktime.replace(/(^\s*)|(\s*$)/g,'');

			console.log("Organizer[" +  Organizer + "]");
			console.log("nature[" +  nature + "]");
			console.log("license[" +  license + "]");
			console.log("sitename[" +  sitename + "]");
			console.log("homepage[" +  homepage + "]");
			console.log("checktime[" +  checktime + "]");
			alert("get ok");
*/