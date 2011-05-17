$(function(){
	idCounter = 0;
	
	$("#desktop").click(function(){
		$(".ui-selected").removeClass("ui-selected").draggable('destroy').resizable('destroy');
	});
	
	$("#createRegion").click(function(){
		$("#selectable").append("<div class='box' id='box-"+ idCounter++ +"'>Drag and Resize me!</div>")
	});
	
	$("#createImage").click(function(){
		sourcePath = prompt("Caminho da imagem");
		$("#image").append("<img class='image' src='"+sourcePath+"'>");
		$("#image img").resizable();
		//infelizmente, a imagem consegue ultrapassar a área delimitada para se trabalhar...
		//tutorial tirado em http://bugs.jqueryui.com/ticket/3446
	});
	
	/*Tratamento para mídias*/
	$("#image").css("width","1px");
	$("#image")
		.draggable({
			containment:'#desktop',
			stack:'#desktop div',
			opacity:0.40});
	
	
	/*Tratamento para outros (que nome dar??)*/
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
		control = 0;
		$(".box").each(function(index){
			/*var left = $(this).css("left");
			var top = $(this).css("top");
			var width = $(this).css("width");
			var height = $(this).css("height");
			document.write(left);
			document.write(top);
			document.write(width);
			document.write(height);
			arr[0] = $(this).attr("id");
			arr[1] = $(this).css("left");
			arr[2] = $(this).css("top");
			arr[3] = $(this).css("width");
			arr[4] = $(this).css("height"); */
			
			//Esse foi o jeito que consegui de fazer o troço...
			//Não tenho certeza sobre o "region" se e preciso ou não... De qualquer forma, aí está....
			//Zinga: Ta, entendi que o region é pra vc controlar que tipo está sendo passado, blz...
			arr[control] = ['region', $(this).attr("id"), $(this).css("left"), $(this).css("top"), $(this).css("width"), $(this).css("height"), "descriptor-" + $(this).attr("id")];
			control++;
		});
		//send post request 
		$.post('getNCL.php', {array: arr }, 
			function(data){
				if (data){
					$("#toolbar").append("<a href='teste.xml'>Download do arquivo</a>");				
				}
				else{
					document.write('nada');					
				}
			});
		return false;
	});

});
