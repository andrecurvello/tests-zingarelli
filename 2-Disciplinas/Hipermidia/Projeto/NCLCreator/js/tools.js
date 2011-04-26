$(function(){
	$("#desktop").click(function(){
		$(".ui-selected").removeClass("ui-selected").draggable('destroy').resizable('destroy');
	});
	
	$("#createRegion").click(function(){
		$("#selectable").append("<div class='box'>Drag and Resize me!</div>")
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
			var style = $(this).attr("style").split(';');
			for(var i = 0; i < style.length;i++){
				var attribute = style[i].split(':');
				switch('left'){//TODO MAKE THIS SWITCH WORK!
				case 'left':
					document.write('left'+attribute[1]+'<br />');
					break;
				case 'top':
					document.write('top'+attribute[1]+'<br />');
					break;
				case 'width':
					document.write('width'+attribute[1]+'<br />');
					break;
				case 'height':
					document.write('height'+attribute[1]+'<br />');
					break;
				default:
					document.write('None found<br />');
				}
				
			}
		});
		
		return false;
	});
});