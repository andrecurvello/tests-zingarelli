$(function(){
	idCounter = 0;
	
	$("#desktop").click(function(){
		$(".ui-selected").removeClass("ui-selected").draggable('destroy').resizable('destroy');
	});
	
	$("#createRegion").click(function(){
		$("#selectable").append("<div class='box' id='box-"+ idCounter++ +"'>Drag and Resize me!</div>")
	});
	
	$("#selectable").selectable({
		selecting:function(event, ui){
			$(".ui-selecting")
				.draggable({
					containment:'#desktop',
					stack:'#desktop div',
					opacity:0.40})
				.resizable();
		},
		selected:function(event, ui){
			$(".ui-selected")
				.draggable({
					containment:'#desktop',
					stack:'#desktop div',
					opacity:0.40})
				.resizable();
		},
		unselected:function(event, ui){
			$(".ui-selectee").draggable('destroy').resizable('destroy');
		}
	});
	
	$("#getNCL").click(function(){
		//get width, height, top and left position of each region
		$(".box").each(function(index){
			style = $(this).attr("style").split(' ');
			regId = $(this).attr("id");
			document.write('ID: ' + regId + '<br />');
			for(i = 0; i < style.length; i++){//for each attribute, store the ones we're interested in
				switch(style[i]){
					case "left:":
						document.write('left: '+style[i+1]+'<br />');
						break;
					case 'top:':
						document.write('top: '+style[i+1]+'<br />');
						break;
					case 'width:':
						document.write('width: '+style[i+1]+'<br />');
						break;
					case 'height:':
						document.write('height: '+style[i+1]+'<br />');
						break;
				}
			}
			//TODO:construid JSON com os dados e enviar para o PHP
		});
		return false;
	});
});