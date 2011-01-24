	<script type="text/javascript" src="js/jquery-1.4.3.min.js"></script>
	<script type="text/javascript" src="js/utils.js"></script>
</head>

<body>
	<div id="container-top"></div>
	<div id="container">
        <div id="header">
            <a href="index.php" title="Vá para a página inicial do site"><img class="left" src="img/faca-festa.png" alt="Faça Festa - Logo" title="Página inicial do site" /></a>
            <div id="menu" class="right">
                <?php if ($_SESSION['logged']==0) {//not logged ?>
                    <ul id="menu-main">
                        <li><a href="#" title="Clique aqui para se cadastrar no site" >Cadastrar</a></li>
                        <li>|</li>
                        <li><a href="#" title="Clique aqui para fazer login" onclick="showLogin();">Login</a></li>
                    </ul>
                    <form id="login-form" name="login-form" method="post" action="do_login.php">
						<div id="login-error">
		                   	<p class="error">Usuário ou senha incorretos, por favor, tente novamente.</p>
	                        <p class="small">(<a href="#" title="Clique aqui caso não se lembre de sua senha">Esqueci minha senha</a>)</p>
                        </div>
                        <label for="username">Usuário:</label>
                        <input type="text" id="username" name="username" size="15" />
                        <label for="password">Senha:</label>
                        <input type="password" id="password" name="password" size="10" />
                        <br />
                        <input type="submit" name="submit" id="login-submit" value="Ok" style="width:60px;"/>
                        <a href="#" title="Clique aqui para cancelar o login" class="small" onclick="hideLogin();">Cancelar</a>
                    </form>
                <?php 
					}
					else{
                    	include 'header-logged.php';
                 	}
				?>
				<div id="menu-logged"></div><!--menu-logged (for javascript validation)-->
            </div><!--menu-->
        </div><!--header-->
        <div id="search-bar" class="clear">
            <form id="search" action="#">
                <label for="search-input">Buscar</label>
                <input id="search-input" type="text" value="Digite o que está procurando" size="30" name="keyword"/>
                <label for="search-category">em</label>
                <select id="search-category" name="category">
                    <option title="Todas as Categorias" value="all" selected="selected">Todas as categorias</option>
                    <option title="Faça busca por locais" value="locais">Locais</option>            
                    <option title="Faça busca por bandas" value="bandas">Bandas</option>
                    <option title="Faça busca por buffet" value="buffe">Buffet</option>
                    <option title="Faça busca por decoracao" value="decoracao">Decoração</option>
                    <option title="Faça busca por bolo &amp; Doces" value="bolos">Bolo &amp; Doces</option>
                    <option title="Faça busca por bebidas" value="bebidas">Bebidas</option>
                    <option title="Faça busca por artigos para Festas" value="artigos">Artigos para Festas</option>
                    <option title="Faça busca por lembranças &amp; Brindes" value="lembrancas">Lembranças &amp; Brindes</option>
                    <option title="Faça busca por cozinheira" value="cozinheira">Cozinheira</option>
                    <option title="Faça busca por churrasqueiro" value="churrasqueiro">Churrasqueiro</option>
                    <option title="Faça busca por açougue" value="acougue">Açougue</option>
                    <option title="Faça busca por serviço de Limpeza" value="limpeza">Serviço de Limpeza</option>
                    <option title="Faça busca por animadores de Festa" value="animadores">Animadores de Festa</option>
                    <option title="Faça busca por DJ" value="dj">DJ</option>
                    <option title="Faça busca por seguranças" value="seguranca">Seguranças</option>  
                </select>
                <input type="submit" value="Ok" style="width:80px;height:31px"/>
            </form>
        </div>