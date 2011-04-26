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
});