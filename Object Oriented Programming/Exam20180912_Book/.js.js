

/**
 * Language switch functions
 *
 * Version: 1.1
 * Date: 13/7/2018
 *
 **/

window.addEventListener("load",function(){
    var t = document.getElementsByTagName("h1");
    var t = t[t.length-1];
    var switcher = document.createElement("p");
    switcher.appendChild(document.createTextNode("Lingua / Language"));
    
    switcher.classList.add("noprint");
    switcher.style.textAlign="center";
    switcher.style.backgroundColor="salmon"
    var itBt = document.createElement("button");
    var enBt = document.createElement("button");
    
    itBt.setAttribute("type","button")
    enBt.setAttribute("type","button")
    itBt.appendChild(document.createTextNode("Italiano"));
    enBt.appendChild(document.createTextNode("English"));
    switcher.append(itBt);
    switcher.append(enBt);
    
    t.parentNode.insertBefore(switcher,t.nextSibling);

	var it = document.getElementsByClassName("it");
	var en = document.getElementsByClassName("en");
	
	var toggle = function(enter,out){
		var i;
		for(i=0; i < enter.length; ++i){
			enter[i].style.display="block";
		} 
		for(i=0; i < out.length; ++i){
			out[i].style.display="none";
		} 
	}
	
	itBt.onclick = function(){
		document.title = "Requisiti";
		itBt.disabled=true
		enBt.disabled=false
		toggle(it,en);
	}
	enBt.onclick = function(){
		document.title = "Requirements";
		itBt.disabled=false
		enBt.disabled=true
		toggle(en,it);
	}
	
	itBt.onclick();
});
