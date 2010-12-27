// JavaScript Document
$(document).ready(function(){
	//esconde o menu de opcoes para usuario logado
	$("#login-sub").hide();
	//esconde a imagem de flecha para voltar no menu de opcoes de usuario logado
	$(".arrow-left").hide();
	//esconde o conteudo de detalhes de um fornecedor
	$(".inner-content-1").hide();
	$(".inner-content-2").hide();
	//esconde as mensagens de erro de usuario nao logado
	$(".error-message").hide();
	//faz animacao das flechas em login e lista de resultados
	$("img.arrow-details").hover(
		function(){$(this).attr("src", "img/arrow-right-selected.png");},
		function(){$(this).attr("src", "img/arrow-right.png");}
	);
	$("img.arrow-right").hover(
		function(){$(this).attr("src", "img/arrow-right-selected.png");},
		function(){$(this).attr("src", "img/arrow-right.png");}
	);
	$("img.arrow-left").hover(
		function(){$(this).attr("src", "img/arrow-left-selected.png");},
		function(){$(this).attr("src", "img/arrow-left.png");}
	);
	//animacao das estrelas para votar
	$("#star-5").hover(
		function(){
			$(this).attr("src","img/star.png");
			$("#star-4").attr("src","img/star.png");
			$("#star-3").attr("src","img/star.png");
			$("#star-2").attr("src","img/star.png");
			$("#star-1").attr("src","img/star.png");},
		function(){
			$(this).attr("src","img/star-empty.png");
			$("#star-4").attr("src","img/star-empty.png");
			$("#star-3").attr("src","img/star-empty.png");
			$("#star-2").attr("src","img/star-empty.png");
			$("#star-1").attr("src","img/star-empty.png");}
	);
	$("#star-4").hover(
		function(){
			$(this).attr("src","img/star.png");
			$("#star-3").attr("src","img/star.png");
			$("#star-2").attr("src","img/star.png");
			$("#star-1").attr("src","img/star.png");},
		function(){
			$(this).attr("src","img/star-empty.png");
			$("#star-3").attr("src","img/star-empty.png");
			$("#star-2").attr("src","img/star-empty.png");
			$("#star-1").attr("src","img/star-empty.png");}
	);
	$("#star-3").hover(
		function(){
			$(this).attr("src","img/star.png");
			$("#star-2").attr("src","img/star.png");
			$("#star-1").attr("src","img/star.png");},
		function(){
			$(this).attr("src","img/star-empty.png");
			$("#star-2").attr("src","img/star-empty.png");
			$("#star-1").attr("src","img/star-empty.png");}
	);		
	$("#star-2").hover(
		function(){
			$(this).attr("src","img/star.png");
			$("#star-1").attr("src","img/star.png");},
		function(){
			$(this).attr("src","img/star-empty.png");
			$("#star-1").attr("src","img/star-empty.png");}
	);
	$("#star-1").hover(
		function(){$(this).attr("src","img/star.png");},
		function(){$(this).attr("src","img/star-empty.png");}
	);	
});

function toggleMenu(){
	$("#login-sub").toggle('slow');
	changeArrow();
}

function changeArrow(){
	$(".arrow-right").toggle();
	$(".arrow-left").toggle()
}

function showDetails(number){
	$("#details-empty").hide();
	if(number==1){
		$(".inner-content-1").show('fast');
		$(".inner-content-2").hide();
	}
	if(number==2){
		$(".inner-content-1").hide();
		$(".inner-content-2").show('fast');
	}
}

function popup(url){
	popupWindow = window.open(
		url,'popUpWindow','height=600,width=800,left=10,top=10,resizable=yes,scrollbars=yes,toolbar=no,menubar=no,location=no,directories=no,status=yes')
}