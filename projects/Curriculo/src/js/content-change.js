// JavaScript Document
$(document).ready(function(){
	//hide content from the unselected tabs
	$("#text-acad").hide();
	$("#text-prof").hide();
});

//Changes the content based on which tab user clicked
function changeContent(number){
	switch(number){
		case 0: //geral selected
			$("#text-geral").show();
			$("#text-acad").hide();
			$("#text-prof").hide();
			if(!$("#sidebar-geral").hasClass('selected')){$("#sidebar-geral").addClass('selected');}
			if($("#sidebar-acad").hasClass('selected')){$("#sidebar-acad").removeClass('selected');}
			if($("#sidebar-prof").hasClass('selected')){$("#sidebar-prof").removeClass('selected');}
			break;
		case 1: //academic selected
			$("#text-geral").hide();
			$("#text-acad").show();			
			$("#text-prof").hide();	
			if($("#sidebar-geral").hasClass('selected')){$("#sidebar-geral").removeClass('selected');}
			if(!$("#sidebar-acad").hasClass('selected')){$("#sidebar-acad").addClass('selected');}
			if($("#sidebar-prof").hasClass('selected')){$("#sidebar-prof").removeClass('selected');}
			break;
		case 2: //professional selected
			$("#text-geral").hide();
			$("#text-acad").hide();
			$("#text-prof").show();	
			if($("#sidebar-geral").hasClass('selected')){$("#sidebar-geral").removeClass('selected');}
			if($("#sidebar-acad").hasClass('selected')){$("#sidebar-acad").removeClass('selected');}
			if(!$("#sidebar-prof").hasClass('selected')){$("#sidebar-prof").addClass('selected');}
			break;			
	}
}