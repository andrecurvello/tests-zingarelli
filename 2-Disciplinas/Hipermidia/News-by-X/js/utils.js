// JavaScript Document
// Developed by: Matheus Ricardo Uihara Zingarelli - nUSP 5377855
$(document).ready(function(){
	$(".new-comment").hide();
	$("#tag-input").hide();	
});

//Comments
function addComment(){	
	$(".txt-comment").val("");
	$(".new-comment").show("slow");
	
}

function postComment(){
	$(".new-comment").hide("fast");
	//show comment on comments board
	var newComment = "<p class=\"show-slow\"><a href=\"#\" title=\"Veja o perfil do usuário\">Usuário</a><span class=\"small\"> (5 segundos atrás): </span></p><p>" + $(".txt-comment").val() + "</p><hr />";
	$(".comment-box").prepend(newComment);
	$(".show-slow").hide();
	$(".show-slow").show("slow");
	$(".txt-comment").val("");
}

function cancelComment(){
	$(".new-comment").hide("slow");
	$(".txt-comment").val("");
}

//Tags
function addTag(){
	$(".txt-tag").val("");
	$("#tag-input").show("slow");
}

function postTag(){
	$("#tag-input").hide("fast");
	//add tag to tag list
	var newTag = "<li class=\"show-slow\"><a href=\"#\" title=\"veja outras notícias que possuem esta palavra-chave\"> " + $(".txt-tag").val() + ",</li>";
	$("#tag-list").prepend(newTag);
	$(".show-slow").hide();
	$(".show-slow").show("slow");
	$(".txt-tag").val("");
}

function cancelTag(){
	$("#tag-input").hide("slow");
	$(".txt-tag").val("");	
}