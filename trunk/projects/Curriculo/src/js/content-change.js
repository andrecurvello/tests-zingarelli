// JavaScript Document
$(document).ready(function(){
	//hide content from the unselected tabs
	$("#text-acad").hide();
	$("#text-prof").hide();
});

//Changes the content based on which tab user clicked
function changeContent(number){
	alert('here');
	switch(number){
		case 0: //geral selected
			$("#text-geral").show();
			$("#text-acad").hide();
			$("#text-prof").hide();			
			break;
		case 1: //academic selected
			$("#text-geral").hide();
			$("#text-acad").show();			
			$("#text-prof").hide();	
			break;
		case 2: //professional selected
			$("#text-geral").hide();
			$("#text-acad").hide();
			$("#text-prof").show();	
			break;			
	}
}