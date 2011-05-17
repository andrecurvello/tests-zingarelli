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
	
	$("#getNCL").submit(function(){
		//get width, height, top and left position of each region
		arr = new Array();
		$(".box").each(function(index){
			/*var left = $(this).css("left");
			var top = $(this).css("top");
			var width = $(this).css("width");
			var height = $(this).css("height");
			document.write(left);
			document.write(top);
			document.write(width);
			document.write(height);*/
			arr[0] = $(this).attr("id");
			arr[1] = $(this).css("left");
			arr[2] = $(this).css("top");
			arr[3] = $(this).css("width");
			arr[4] = $(this).css("height");
			//TODO:construir JSON com os dados e enviar para o PHP fazer funcionar
		});
		bigarr[]=arr;
		dados = [0,02,36,6];
		//send AJAX request 
		alert("entrando");
		$.post('getNCL.php', {array: bigarr }, 
			function(data){
				if (data){
					$("#toolbar").append("<a href='teste.xml'>Download do arquivo</a>");				
				}
				else{
					document.write('nada');					
				}
			});
		alert("saindo");
		return false;
	});

});