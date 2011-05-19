$(function(){
	$("#image-menu").hide();
	$("#text-menu").hide();
	
	idCounter = 0;
	
	$("#desktop").click(function(){
		$(".ui-selected").removeClass("ui-selected").draggable('destroy').resizable('destroy');
	});
	
	$("#createRegion").click(function(){
		$("#selectable").append("<div class='box' id='box-"+ idCounter++ +"'>Drag and Resize me!</div>")
	});
	
	/*Tratamendo para adição de imagem*/
	$("#createImage").click(function(){
		$("#image-menu").show('fast');
	});
	$("#cancelImage").click(function(){$("#image-menu").hide('fast');});
	$("#btnImage").click(function(){alert("oi");
		$("#image").append("<div style='width:1px;' class='image-wrapper'><img id='image-"+ idCounter++ +"' class='image' src='"+$("#imagePath").val()+"'></div>");
		$("#image div").draggable({
							containment:'#desktop', //define os limites de onde a região pode ser arrastada
							stack:'#desktop div', //define o z-index da imagem, que regiões ficarão abaixo dela
							opacity:0.40,
							grid: [10, 10]}); //nível de opacidade para quando se está arrastando a imagem
		$("#image img").resizable({grid: [10, 10]});
		//infelizmente, a imagem consegue ultrapassar a área delimitada para se trabalhar, pois quem é draggable é a div.
		//não é possível tornar a imagem draggable e resizable. Por isso é criado um div.
		//tutorial tirado em http://bugs.jqueryui.com/ticket/3446	
	});
	
	/*Tratamento para outros (que nome dar??)*/
	$("#selectable").selectable({
		selecting:function(event, ui){
			$(".ui-selecting")
				.draggable({
					containment:'#desktop',
					stack:'#desktop div',
					opacity:0.40,
					grid: [10, 10] })
				.resizable({grid: [10, 10]});
		},
		selected:function(event, ui){
			$(".ui-selected")
				.draggable({
					containment:'#desktop',
					stack:'#desktop div',
					opacity:0.40})
				.resizable({grid: [10, 10]});	
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
			arr[control] = ['region', $(this).attr("id"), $(this).css("left"), $(this).css("top"), $(this).css("width"), $(this).css("height"), "descriptor-" + $(this).attr("id")];
			control++;
		});
		$(".image").each(function(index){
			arr[control] = ['media',
			                "region-"+$(this).attr("id"),
			                $(this).parent().css("left"),
			                $(this).parent().css("top"), 
			                $(this).css("width"), 
			                $(this).css("height"), 
			                "descriptor-" + $(this).attr("id"), 
			                $(this).attr("id"), 
			                "image/jpeg",
			                $(this).attr("src")];
			control++;
		});
		//send post request 
		$.post('getNCL.php', {array: arr }, 
			function(data){
				if (data){
					$("#sub-menu").append("<a target='_blank' href='teste.xml'>Download do arquivo</a>");				
				}
				else{
					document.write('nada');					
				}
			});
		return false;
	});

});
