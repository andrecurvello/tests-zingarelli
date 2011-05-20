$(function(){
	$("#media-menu").hide();
	/*$("#flash-menu").hide();
	$("#image-menu").hide();*/
	
	idCounter = 0;
	
	$("#desktop").click(function(){
		$(".ui-selected").removeClass("ui-selected").draggable('destroy').resizable('destroy');
	});
	
	$("#createRegion").click(function(){
		$("#selectable").append("<div class='box' style='position:absolute;top:0;left:0' id='box-"+ idCounter++ +"'>Região</div>")
	});
	
	/*Tratamendo para mídias*/
	//RESTRIÇÃO: mídias devem se encontrar na mesma pasta que o código! TODO: fazer upload da mídia
	//imagem
	tipoMidia = 'none';
	$("#createVideo").click(function(){
		$("#media-menu").show('fast');
		tipoMidia = 'video';
	});
	$("#createFlash").click(function(){
		$("#media-menu").show('fast');
		tipoMidia = 'flash';
	});
	$("#createImage").click(function(){
		$("#media-menu").show('fast');
		tipoMidia = 'image';
	});	
	$("#cancelMedia").click(function(){$("#media-menu").hide('fast');});
	$("#btnMedia").click(function(){
		switch(tipoMidia){
			case 'video':
				$("#media").append("<div style='position:absolute;top:0;left:0;width:1px;' class='video-wrapper'><img width=100 height=100 id='video-"+ idCounter++ +"' class='video' src='img/video.jpg'></div>");
				srcVideo = $("#mediaPath").val();
				break;
			case 'flash':
				$("#media").append("<div style='position:absolute;top:0;left:0;width:1px;' class='flash-wrapper'><img width=100 height=100 id='flash-"+ idCounter++ +"' class='flash' src='img/flash.jpg'></div>");
				srcFlash = $("#mediaPath").val();
				break;
			case 'image':
				$("#media").append("<div style='position:absolute;top:0;left:0;width:1px;' class='image-wrapper'><img width=100 height=100 id='image-"+ idCounter++ +"' class='image' src='"+$("#mediaPath").val()+"'></div>");
				break;
		}
		$("#media div").draggable({
							containment:'#desktop', //define os limites de onde a região pode ser arrastada
							stack:'#desktop div', //define o z-index da imagem, que regiões ficarão abaixo dela
							opacity:0.40,
							grid: [10, 10]}); //nível de opacidade para quando se está arrastando a imagem
		$("#media img").resizable({grid: [10, 10]});
		//infelizmente, a imagem consegue ultrapassar a área delimitada para se trabalhar, pois quem é draggable é a div.
		//não é possível tornar a imagem draggable e resizable. Por isso é criado um div.
		//tutorial tirado em http://bugs.jqueryui.com/ticket/3446	
		$("#media-menu").hide();
		return false;
	});
	
	/*Tratamento para region)*/
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
			                $(this).parent().parent().css("left"),
			                $(this).parent().parent().css("top"), 
			                $(this).css("width"), 
			                $(this).css("height"), 
			                "descriptor-" + $(this).attr("id"), 
			                $(this).attr("id"), 
			                "image/jpeg",
			                $(this).attr("src")];
			control++;
		});
		$(".video").each(function(index){
			arr[control] = ['media',
			                "region-"+$(this).attr("id"),
			                $(this).parent().parent().css("left"),
			                $(this).parent().parent().css("top"), 
			                $(this).css("width"), 
			                $(this).css("height"), 
			                "descriptor-" + $(this).attr("id"), 
			                $(this).attr("id"), 
			                "video/mpeg", //colocar descritor de video
			                srcVideo];
			control++;
		});
		$(".flash").each(function(index){
			arr[control] = ['media',
			                "region-"+$(this).attr("id"),
			                $(this).parent().parent().css("left"),
			                $(this).parent().parent().css("top"), 
			                $(this).css("width"), 
			                $(this).css("height"), 
			                "descriptor-" + $(this).attr("id"), 
			                $(this).attr("id"), 
			                "image/jpeg", //colocar descritor de flash
			                srcFlash];
			control++;
		});
		//send post request 
		$.post('getNCL.php', {array: arr }, 
			function(data){
				if (data){
					$("#sub-menu").append("<a target='_blank' href='teste.ncl'>Download do arquivo</a>");				
				}
				else{
					alert('error');					
				}
			});
		return false;
	});

});
