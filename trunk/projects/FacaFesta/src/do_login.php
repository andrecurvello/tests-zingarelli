<?php
include 'config.php';

if(empty($_POST)){header("location:index.php");exit;}

//get data
$username = trim($_POST['username']);
$password = trim($_POST['password']);
$fromJavascript = trim($_POST['fromJavascript']);

//Prevent SQL Injection
$username = mysql_escape_string($username);
$password = mysql_escape_string($password);
$fromJavascript = mysql_escape_string($fromJavascript);

//Encrypts password
$password = md5($password);

//connect to MySQL
$db = mysql_connect(DB_SERVER, DB_USER, DB_PASS,false, 65536) or die('Falha na conexao. Verifique as credenciais.' . mysql_error());
mysql_select_db(DB_NAME, $db) or die ('Nao foi possivel conectar no banco' . DB_NAME);

//call stored procedure to check login
$query = sprintf("CALL verifyLogin('%s', '%s', @nome);", $username, $password);
$result = mysql_query($query);
$result = mysql_query("SELECT @nome");

$row = mysql_fetch_array($result);
if(!empty($row['@nome'])){ //login verified	
	$_SESSION['logged'] = '1';
	if($fromJavascript == 'yes'){
		//print the welcome page
		echo '
			<p>Olá, <strong><a href="#" title="Clique para ver seu perfil no site">'.$row['@nome'].'</a></strong> (<a href="logout.php" title="Clique aqui para fazer logoff" class="small">sair</a>)</p>
			<ul id="menu-logged-sub" class="right">
				<li><a href="#" title="Crie uma nova festa">Nova Festa</a></li>
				<li><a href="#" title="Edite ou divulge uma festa criada. Gerencie a sua lista de convidados">Gerenciar Festas</a></li>       
			</ul>
			<ul id="menu-logged-main" class="right">
				<li><a href="#" title="Edite as informações do seu cadastro">Minha Conta</a></li>
				<li><a href="#" title="Veja sua lista de fornecedores favoritos">Meus Favoritos</a></li>
				<li><a href="#" title="Veja sua lista de contatos">Meus Contatos</a></li>
				<li><a href="#" title="Crie uma nova festa ou gerencie festas já existentes">Minhas Festas</a></li>
			</ul>';
	}
	else{
		//user has come from a normal HTTP Post request, redirect to main page
		header("location:index.php");
	}
}
else //wrong login information
{
	$_SESSION['login-error'] = 1;
	if($fromJavascript == 'yes'){
		echo "N";
	}
	else{
		header("location:index.php");
	}
}
?>