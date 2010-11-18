// JavaScript Document
$(document).ready(function(){
	//esconde o menu de opcoes para usuario logado e o texto de login
	$("#login-section").hide();
	$("#login-sub").hide();
	$("#login-erro").hide();
	
	//logica para tratamento de usuario logado
	//pega o cookie e valida
	var c_name = "usuario=";
	var login_cookie = "";
	if (document.cookie.length>0){
		var c_start = document.cookie.indexOf(c_name);
		if (c_start!=-1){
			c_start = c_start + c_name.length;
			c_end = document.cookie.indexOf(";",c_start);
			if (c_end==-1) c_end = document.cookie.length;
			login_cookie = document.cookie.substring(c_start,c_end);
		}
	}	
	if(login_cookie == "Usuario"){
		$("#not-logged").hide();
	} else{
		$("#logged").hide();
	}
	
	//esconde a imagem de flecha para voltar no menu de opcoes de usuario logado
	$(".arrow-left").hide();
	//esconde secoess de interacao com detalhes do fornecedor
	$(".avaliacao-1").hide();
	$(".fav-1-1").hide();
	$(".spam-1-1").hide();
	$(".new-comment-1").hide();
	$(".avaliacao-2").hide();
	$(".fav-1-2").hide();
	$(".spam-1-2").hide();
	$(".new-comment-2").hide();
	$(".avaliacao-3").hide();
	$(".fav-1-3").hide();
	$(".spam-1-3").hide();
	$(".new-comment-3").hide();
	$(".avaliacao-4").hide();
	$(".fav-1-4").hide();
	$(".spam-1-4").hide();
	$(".new-comment-4").hide();
	$(".avaliacao-5").hide();
	$(".fav-1-5").hide();
	$(".spam-1-5").hide();
	$(".new-comment-5").hide();
	$(".avaliacao-6").hide();
	$(".fav-1-6").hide();
	$(".spam-1-6").hide();
	$(".new-comment-6").hide();
	//esconde o conteudo de detalhes de um fornecedor
	$(".inner-content-1").hide();
	$(".inner-content-2").hide();
	$(".inner-content-3").hide();
	$(".inner-content-4").hide();
	$(".inner-content-5").hide();
	$(".inner-content-6").hide();	
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
	$(".star-5").hover(
		function(){
			$(this).attr("src","img/star.png");
			$(".star-4").attr("src","img/star.png");
			$(".star-3").attr("src","img/star.png");
			$(".star-2").attr("src","img/star.png");
			$(".star-1").attr("src","img/star.png");},
		function(){
			$(this).attr("src","img/star-empty.png");
			$(".star-4").attr("src","img/star-empty.png");
			$(".star-3").attr("src","img/star-empty.png");
			$(".star-2").attr("src","img/star-empty.png");
			$(".star-1").attr("src","img/star-empty.png");}
	);
	$(".star-4").hover(
		function(){
			$(this).attr("src","img/star.png");
			$(".star-3").attr("src","img/star.png");
			$(".star-2").attr("src","img/star.png");
			$(".star-1").attr("src","img/star.png");},
		function(){
			$(this).attr("src","img/star-empty.png");
			$(".star-3").attr("src","img/star-empty.png");
			$(".star-2").attr("src","img/star-empty.png");
			$(".star-1").attr("src","img/star-empty.png");}
	);
	$(".star-3").hover(
		function(){
			$(this).attr("src","img/star.png");
			$(".star-2").attr("src","img/star.png");
			$(".star-1").attr("src","img/star.png");},
		function(){
			$(this).attr("src","img/star-empty.png");
			$(".star-2").attr("src","img/star-empty.png");
			$(".star-1").attr("src","img/star-empty.png");}
	);		
	$(".star-2").hover(
		function(){
			$(this).attr("src","img/star.png");
			$(".star-1").attr("src","img/star.png");},
		function(){
			$(this).attr("src","img/star-empty.png");
			$(".star-1").attr("src","img/star-empty.png");}
	);
	$(".star-1").hover(
		function(){$(this).attr("src","img/star.png");},
		function(){$(this).attr("src","img/star-empty.png");}
	);	
});

//aparece e esconde o submenu de login (gerenciar)
function toggleMenu(){
	$("#login-sub").toggle('slow');
	changeArrow();
}

//troca a flecha da area de login (quando clica em gerenciar)
function changeArrow(){
	$(".arrow-right").toggle();
	$(".arrow-left").toggle()
}

//aparece os detalhes de um fornecedor
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
	if(number==3){
		$(".inner-content-6").hide();
		$(".inner-content-5").hide();
		$(".inner-content-4").hide();
		$(".inner-content-3").show('fast');
	}
	if(number==4){
		$(".inner-content-6").hide();
		$(".inner-content-5").hide();
		$(".inner-content-3").hide();
		$(".inner-content-4").show('fast');
	}
	if(number==5){
		$(".inner-content-6").hide();
		$(".inner-content-3").hide();
		$(".inner-content-4").hide();
		$(".inner-content-5").show('fast');
	}
	if(number==6){
		$(".inner-content-3").hide();
		$(".inner-content-4").hide();
		$(".inner-content-5").hide();
		$(".inner-content-6").show('fast');
	}
}

//cria uma janela popup para mostrar as fotos
function popup(url){
	popupWindow = window.open(
		url,'popUpWindow','height=600,width=800,left=10,top=10,resizable=yes,scrollbars=yes,toolbar=no,menubar=no,location=no,directories=no,status=yes')
}

//mostra os input para usuario logar
function showLogin(){
	$("#not-logged").hide();
	$("#login-section").show();
	$("#login-id").val("");
	//esconde as mensagens de erro caso o usuario tenha visto uma
	$(".em-1-1").hide("slow");
	$(".em-2-1").hide("slow");
	$(".em-1-2").hide("slow");
	$(".em-2-2").hide("slow");	
	$(".em-1-3").hide("slow");
	$(".em-2-3").hide("slow");
	$(".em-1-4").hide("slow");
	$(".em-2-4").hide("slow");	
	$(".em-1-5").hide("slow");
	$(".em-2-5").hide("slow");
	$(".em-1-6").hide("slow");
	$(".em-2-6").hide("slow");
}

//mostra links de cadastro ou login se usuario cancela a acao de logar
function showMenu(){
	$("#not-logged").show();
	$("#login-section").hide();
}

//verificacao de login hardcoded
function checkLogin(){
	if($("#login-id").val() == "usuario" && $("#login-pwd").val() == "usuario"){
		//seta cookie de usuario logado
		document.cookie = "usuario=Usuario;expires=3060";
		//mostra menu de usuario logado
		$("#logged").show();
		//esconde menu de login
		$("#login-section").hide();
		
	}
	else{
		//mostra mensagem de erro
		$("#login-erro").show();
	}
}

//destroi cookie de login
function logout(){
	//destroi cookie
	var d = new Date();
	document.cookie = "usuario=Usuario;expires=" + d.toGMTString();
	//mostra o menu de login/cadastro
	$("#not-logged").show();
	//esconde menu de usuario logado
	$("#logged").hide();
}

//verifica se usuario esta logado. type qual mensagem de erro ou sucesso mostrar. Number qual dos detalhes deve ser mostrado 
function checkLogged(type, number){
	//pega o cookie e valida
	var c_name = "usuario=";
	var login_cookie = "";
	if (document.cookie.length>0){
		var c_start = document.cookie.indexOf(c_name);
		if (c_start!=-1){
			c_start = c_start + c_name.length;
			c_end = document.cookie.indexOf(";",c_start);
			if (c_end==-1) c_end = document.cookie.length;
			login_cookie = document.cookie.substring(c_start,c_end);
		}
	}	
	if(login_cookie == "Usuario"){
		//executa alguma acao de sucesso dependendo da area do conteudo
		if(type == "avaliar"){
			//mostra mensagem de agradecimento
			$(".avaliacao-"+number).show();
			//tira as estrelas de avaliacao
			$(".stars-"+number).hide();			
		} else if(type == "favoritos"){
			//mostra mensagem de agradecimento
			$(".fav-"+number).hide();
			$(".fav-1-"+number).show();
		} else if(type == "spam"){
			//mostra mensagem de agradecimento
			$(".spam-"+number).hide();
			$(".spam-1-"+number).show();
		} else if(type == "comentar"){
			//apaga texto se houver algum
			$(".txt-comment-"+number).val("");
			//mostra area de comentario
			$(".new-comment-"+number).show("slow");
		}
	} else{
		//mostra mensagem de erro adequada
		if(type == "comentar"){ $(".em-2-"+number).show("slow");}
		else{ $(".em-1-"+number).show("slow");}
	}
}

//cancela acao de comentario
function cancelComment(number){
	//esconde parte de comentario
	$(".new-comment-"+number).hide("slow");
	$(".txt-comment-"+number).val("");
}

//simula a postagem de comentario
function postComment(number){
	//esconde a parte de comentario
	$(".new-comment-"+number).hide();
	//mostra o comentario no board
	var newComment = "<div class=\"inner-comment show-slow-" + number + "\"><a href=\"usuario.php\" title=\"Veja o perfil do usuário\">Usuário</a><span class=\"small\"> (5 segundos atrás): </span>" + $(".txt-comment-"+number).val() + "</div>"
	$(".comment-board-"+number).prepend(newComment);
	//faz animacao do novo comentario
	$(".show-slow-"+number).hide("fast");
	$(".show-slow-"+number).show("slow");
	//limpa area de texto
	$(".txt-comment-"+number).val("");
}

//simula deletar um favorito
function deleteFav(number){
	var r = confirm("Está certo de que deseja excluir este fornecedor da lista de favoritos?");
	if (r==true){
		$("#fav-"+number).hide();
	}	
}