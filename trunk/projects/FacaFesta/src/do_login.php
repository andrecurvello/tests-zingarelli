<?php
include 'config.php';

if(empty($_POST)){header("location:index.php");exit;}

//get data
$username = trim($_POST['username']);
$password = trim($_POST['password']);
$fromJavascript = trim($_POST['fromJavascript]']);

//TODO:validate data

//TODO:change to db connection
if(($username == $config_username) && ($password == $config_password))
{
	$_SESSION['logged'] = '1';
	if($fromJavascript == 'yes'){
		//print the welcome page
		print('
			<p>Olá, <strong><a href="#" title="Clique para ver seu perfil no site">Usuário A</a></strong> (<a href="logout.php" title="Clique aqui para fazer logoff" class="small">sair</a>)</p>
			<ul id="menu-logged-sub" class="right">
				<li><a href="#" title="Crie uma nova festa">Nova Festa</a></li>
				<li><a href="#" title="Edite ou divulge uma festa criada. Gerencie a sua lista de convidados">Gerenciar Festas</a></li>       
			</ul>
			<ul id="menu-logged-main" class="right">
				<li><a href="#" title="Edite as informações do seu cadastro">Minha Conta</a></li>
				<li><a href="#" title="Veja sua lista de fornecedores favoritos">Meus Favoritos</a></li>
				<li><a href="#" title="Veja sua lista de contatos">Meus Contatos</a></li>
				<li><a href="#" title="Crie uma nova festa ou gerencie festas já existentes">Minhas Festas</a></li>
			</ul>');
	}
	else{
		//user has come from a normal HTTP Post request, redirect to main page
		header("location:index.php");
	}
}
else
{
	echo 'N';
}
?>