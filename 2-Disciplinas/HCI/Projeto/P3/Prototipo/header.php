<!-- Header de todas as paginas contendo logo e area de login -->
<div id="header">
	<div id="logo"><a href="index.php"><img src="img/logo.png" height="130" width="500" title="Home" alt="Home"/></a></div>
    <div id="login">
		<div id="not-logged">
            <a href="#" title="Cadastrar">Cadastrar</a>
            &nbsp;|&nbsp;
            <a href="#" title="Fazer Login" onclick="showLogin();">Fazer Login</a>
		</div>
		<div id="login-section">
			<form>
				<input id="login-id" type="text" name="login" size="10" />
				<input id="login-pwd" type="password" name="password" size="10" />
				<input type="button" value="Login" onclick="checkLogin();" />
				<a href="#" title="Cancelar Login" onclick="showMenu();" class="small">Cancelar</a>
				<br />
				<div id="login-erro"><span class="red">Usuário ou senha incorretos.</span>&nbsp;&nbsp;(<a href="#" title="Esqueceu sua senha?" class="small">Esqueceu sua senha?</a>)</div>
			</form>
		</div>
        <div id="logged">
            Ol&aacute; <strong>Usu&aacute;rio</strong>!<span class="small">&nbsp;&nbsp;(<a href="#" onclick="logout()" title="Sair">Sair</a>)</span>
            <br />
            <div id="login-sub" class="right">
                <a href="#" title="Minha Conta">Minha Conta</a>
                <br />
                <a href="#" title="Meus Favoritos">Meus Favoritos</a>
                <br />
                <a href="#" title="Minhas Festas">Minhas Festas</a>
                <br />
                <a href="#" title="Meus Convidados">Meus Convidados</a>
                <br />
                <a href="#" title="Meus Contatos">Meus Contatos</a>
            </div>
            <div id="login-main" class="right">
                <a href="#" title="Criar Festa">Criar Festa</a>
                <br />
                <a href="#" title="Divulgar Festa">Divulgar Festa</a>
                <br />
                <a href="#" title="Gerenciar (clique para expandir)" onclick="toggleMenu();">Gerenciar</a>
                <img class="arrow-right" src="img/arrow-right.png" width="13" onclick="toggleMenu();" alt="Abrir op&ccedil;&otilde;es de gerenciamento" title="Abrir op&ccedil;&otilde;es de gerenciamento" />
                <img class="arrow-left" src="img/arrow-left.png" width="13" onclick="toggleMenu();" alt="Fechar op&ccedil;&otilde;es de gerenciamento" title="Fechar op&ccedil;&otilde;es de gerenciamento" />
            </div>
        </div><!--#logged-->
    </div>
</div>