/*
* Created by: Matheus Ricardo Uihara Zingarelli
* Date: January, 2011
* E-mail: zingarelli.m@gmail.com
*
* Last change date: January, 2011
* By: Matheus Ricardo Uihara Zingarelli
*/

$(function(){
	//hide login form
	$("#login-form").hide();
	
	//hide error from wrong login
	$("#login-error").hide();
	
	//action to be done when login button is pressed
	$("#login-form").submit(function(){
		//hide any error previoulsy raised
		$("#login-error").hide();
		
		//get input
		var username = $("input#username").val();
		if(username == ""){
			$("#login-error").html("<p class=\"error\">Usuário ou senha incorretos, por favor, tente novamente.");
			$("#login-error").show();
			$("input#username").focus();
			return false;
		}
		
		var password = $("input#password").val();
		if(password == ""){
			$("#login-error").html("<p class=\"error\">Usuário ou senha incorretos, por favor, tente novamente.");
			$("#login-error").show();
			$("input#username").focus();
			return false;
		}
		
		//send AJAX request
		var dataString = 'username=' + username + '&password=' + password + '&fromJavascript=yes';
		$.ajax({
			type: "POST",
			url: "do_login.php",
			data: dataString,
			success: function(data){
				if(data == 'N'){ //invalid login
					//show error message
					$("#login-error").html("<p class=\"error\">Usuário ou senha incorretos, por favor, tente novamente.");
					$("#login-error").show();
				}
				else{
					//remove error message if any
					$("#login-error").hide;
					//hide login menu
					$("#menu-main").hide();
					$("#login-form").hide();
					//display welcome data
					$("#menu-logged").html(data);
				}
			}
		})
		
		//avoid page reload from submit click
		return false;
		
	}); //submit button clicked
});

//show login form
function showLogin(){
	$("#menu-main").hide();
	$("#login-form").show();
}

//hide login form
function hideLogin(){
	$("#login-form").hide();
	$("#menu-main").show();
}















//deprecated
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