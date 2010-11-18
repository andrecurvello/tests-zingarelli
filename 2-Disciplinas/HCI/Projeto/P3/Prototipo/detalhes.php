<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Projeto Faça a Festa - Meus Favoritos</title>
<link rel="stylesheet" type="text/css" href="http://yui.yahooapis.com/3.2.0/build/cssreset/reset-min.css" />
<link rel="stylesheet" type="text/css" href="css/main.css"/>
<script type="text/javascript" src="js/jquery-1.4.3.min.js"></script>
<script type="text/javascript" src="js/utils.js"></script>
</head>

<body>
<?php include 'header.php'; ?>
<div id="container">
	<?php include 'left-bar.php'; ?>
    <div id="content">
		<?php if ($_GET["ID"] == 1){ ?>
		<div id="fornecedor-detalhes">
            <h1>Kids Festas Infantis</h1>
			<span class="avaliacao-2 small"><em>Fornecedor avaliado!</em></span>         
            <span class="stars-2">
			<a href="#" onclick="checkLogged('avaliar', 2);"><img class="star-1" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 1 estrela" /></a>
			<a href="#" onclick="checkLogged('avaliar', 2);"><img class="star-2" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 2 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 2);"><img class="star-3" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 3 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 2);"><img class="star-4" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 4 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 2);"><img class="star-5" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 5 estrelas" /></a>
			</span>
            &nbsp;&nbsp;&nbsp;
            <a class="small fav-2" onclick="checkLogged('favoritos', 2);" href="#" title="Adicionar a favoritos" nowrap>Adicionar a favoritos</a>
			<span class="small fav-1-2"><em>Salvo aos favoritos!</em></span>
&nbsp;&nbsp;&nbsp;
<a class="small spam-2"onclick="checkLogged('spam', 2);" href="#" title="Sinalizar como spam">Sinalizar como spam</a>
<span class="small spam-1-2"><em>Obrigado, iremos verificar.</em></span>
			
			<div class="error-message em-1-2"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            
			<p class="small">Portal com todo tipo de informação para a organizações de festas infantis. Kides Festas Infantis é o melhor lugar para você organizar a festa infantil. Trabalhamos também com decorações atuais como Backyardigans, Pinky Dinky Do e Hi-5</p>
			<p><strong>Dados para contato:</strong></p>
            <p>Avenida Santos Dumont, 1915 - Jd. Boa Esperança - São Carlos/SP</p>
            <p>(13) 3342-3232</p>
            <p><a href="mailto:contato@kidsfestas.com.br" title="Email: contato@kidsfestas.com.br">contato@kidsfestas.com.br</a></p>
            <p><a href="#" title="http://www.kidsfestas.com">http://www.kidsfestas.com</a></p>
            
            <hr />
            
            <p>Fotos <span class="small">(clique na foto para vê-la em um tamanho maior)</span></p>
            <div id=photo-gallery>
            	<a href="#" title="Ver foto em tamanho maior" onclick="popup('img/backyardigans-1.jpg');"><img src="img/backyardigans-1-thumb.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/backyardigans-2.jpg');"><img src="img/backyardigans-2-thumb.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/pooh-1.jpg');"><img src="img/pooh-1-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/pooh-2.jpg');"><img src="img/pooh-2-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/pooh-3.jpg');"><img src="img/pooh-3-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
            </div>
            
            <hr />
            
            <p>Comentários <span class="small">(<a href="#" onclick="checkLogged('comentar', 2);" title="Deixe seu comentário">comente</a>)</span></p>
            <div class="error-message em-2-2"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            <div class="new-comment-2">
				<textarea class="txt-comment-2" rows="3" cols="45"/></textarea>
				<input type="submit" value="Postar" onclick="postComment(2);" />
				<a href="#" onclick="cancelComment(2)" title="Cancelar comentário">Cancelar</a>
			</div>
			<div id="comments" class="comment-board-2">
            <div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Maria Teodora</a><span class="small"> (2 dia atrás): </span> Os enfeites e a decoração é muito bonita, porém cuidado, quebram fácil e tive que pagar multa.</div>
            <div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Jacqueline</a><span class="small"> (1 mês atrás): </span>Não gostei do serviço, a decoração veio em uma caixa de papelão com alguns bonecos de isopor quebrados. Quando liguei para eles reporem os bonecos quebrados eles falaram que viriam trocar na festa e nem apareceram. Já na hora de pagar a história foi outra...</div>
            <div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Jéssica</a><span class="small"> (1 mês atrás): </span>Eles trabalham muito bem com tudo detalhado no contrato minha festa ficou maravilhosa!!</div>
            <div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Thamires Cristina</a><span class="small"> (3 meses atrás): </span>Conheço o trabalho deles e recomendo!</div>
            <div class="bottom-comment"><a href="usuario.php" title="Veja o perfil do usuário">Andréia</a><span class="small"> (1 ano atrás): </span>Eles podiam melhorar e começar a fazer decorações mais atuais o que acham????</div>
            </div><!--#comments-->
		</div><!--#fornecedor-detalhes-->
		<?php } else if ($_GET["ID"] == 2){ ?>		
		<div id="fornecedor-detalhes">
			<h1>Chácara do Barão</h1>
				<span class="avaliacao-3 small"><em>Fornecedor avaliado!</em></span>         
				<span class="stars-3">
				<a href="#" onclick="checkLogged('avaliar', 3);"><img class="star-1" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 1 estrela" /></a>
				<a href="#" onclick="checkLogged('avaliar', 3);"><img class="star-2" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 2 estrelas" /></a>
				<a href="#" onclick="checkLogged('avaliar', 3);"><img class="star-3" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 3 estrelas" /></a>
				<a href="#" onclick="checkLogged('avaliar', 3);"><img class="star-4" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 4 estrelas" /></a>
				<a href="#" onclick="checkLogged('avaliar', 3);"><img class="star-5" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 5 estrelas" /></a>
				</span>
				&nbsp;&nbsp;&nbsp;
				<a class="small fav-3" onclick="checkLogged('favoritos', 3);" href="#" title="Adicionar a favoritos" nowrap>Adicionar a favoritos</a>
				<span class="small fav-1-3"><em>Salvo aos favoritos!</em></span>
	&nbsp;&nbsp;&nbsp;
	<a class="small spam-3"onclick="checkLogged('spam', 3);" href="#" title="Sinalizar como spam">Sinalizar como spam</a>
	<span class="small spam-1-3"><em>Obrigado, iremos verificar.</em></span>
				
				<div class="error-message em-1-3"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>

				<p class="small">Ideal para todo o tipo de eventos como Casamentos, Formaturas, Aniversários, Confraternizações de Empresas, Reunião de Amigos, etc... AMPLO SALÃO com som, forno industrial, churrasqueiras, freezer, mesas e cadeiras, videoquê. QUIOSQUE com bar, televisão, máquina de gelo, churrasqueira, fogão, geladeira e forno caipira. LAZER COMPLETO: piscina com toboágua, campo de futebol, playground, chafariz e pomar. Estrutura completa de higiene inclusive berçário reservado.</p>
				
				<p><strong>Dados para contato:</strong></p>
				<p>Rua Caetano Moruzzi,111 – Jd. São João Batista - São Carlos/SP</p>
				<p>(16) 3361-3920</p>
				<p><a href="mailto:chacaradobarao@ig.com.br" title="Email: chacaradobarao@ig.com.br">chacaradobarao@ig.com.br</a></p>
				<p><a href="#" title="http://www.chacaradobarao.com.br">http://www.chacaradobarao.com.br</a></p>
				
				<hr />
				
				<p>Fotos <span class="small">(clique na foto para vê-la em um tamanho maior)</span></p>
				<div id=photo-gallery>            	
					<a href="#" title="Ver foto em tamanho maior" onclick="popup('img/barao-2.jpg');"><img src="img/barao-2-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
					<a href="#" title="Ver foto em tamanho maior" onclick="popup('img/barao-3.jpg');"><img src="img/barao-3-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
					<a href="#" title="Ver foto em tamanho maior" onclick="popup('img/barao-4.jpg');"><img src="img/barao-4-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
				</div>
				
				<hr />
				
				<p>Comentários <span class="small">(<a href="#" onclick="checkLogged('comentar', 3);" title="Deixe seu comentário">comente</a>)</span></p>
				<div class="error-message em-2-3"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
				<div class="new-comment-3">
					<textarea class="txt-comment-3" rows="3" cols="45"/></textarea>
					<input type="submit" value="Postar" onclick="postComment(3);" />
					<a href="#" onclick="cancelComment(3)" title="Cancelar comentário">Cancelar</a>
				</div>
				<div id="comments" class="comment-board-3">
					<div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">André CAASO</a><span class="small"> (2 dia atrás): </span>Faço sempre as festas da rep lá!!</div>
					<div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Amanda</a><span class="small"> (1 mês atrás): </span>Tem uma cabra muito divertida na chácara!! Rsrsrs</div>
					<div class="bottom-comment"><a href="usuario.php" title="Veja o perfil do usuário">Samuel</a><span class="small"> (1 ano atrás): </span>Passamos o ano novo lá e deu tudo certo!</div>
				</div><!--#comments-->
		</div>
		<?php } else if ($_GET["ID"] == 3){ ?>
		<div id="fornecedor-detalhes">
			<h1>Chácara Portal do Vale</h1>
			<span class="avaliacao-4 small"><em>Fornecedor avaliado!</em></span>         
            <span class="stars-4">
			<a href="#" onclick="checkLogged('avaliar', 4);"><img class="star-1" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 1 estrela" /></a>
			<a href="#" onclick="checkLogged('avaliar', 4);"><img class="star-2" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 2 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 4);"><img class="star-3" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 3 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 4);"><img class="star-4" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 4 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 4);"><img class="star-5" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 5 estrelas" /></a>
			</span>
            &nbsp;&nbsp;&nbsp;
            <a class="small fav-4" onclick="checkLogged('favoritos', 4);" href="#" title="Adicionar a favoritos" nowrap>Adicionar a favoritos</a>
			<span class="small fav-1-4"><em>Salvo aos favoritos!</em></span>
&nbsp;&nbsp;&nbsp;
<a class="small spam-4"onclick="checkLogged('spam', 4);" href="#" title="Sinalizar como spam">Sinalizar como spam</a>
<span class="small spam-1-4"><em>Obrigado, iremos verificar.</em></span>
			
			<div class="error-message em-1-4"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>

			<p class="small">Chácara em Água Vermelha (Distrito de São Carlos). A chácara tem 1000m2, casa com dois dormitórios (acomoda 13 pessoas para dormir), um banheiro, uma sala, uma ampla cozinha com duas mesas, dez cadeiras, armário com talheres, pratos, copos, panelas, etc, um fogão de seis bocas, uma geladeira e uma pia, varanda com churraqueira, grelhas e espetos, uma piscina, espaço gramado e pomar no fundo.</p>
            <p><strong>Dados para contato:</strong></p>
			<p>Água Vermelha – Distrito de São Carlos </p>
            <p>(16) 9221-3995</p>
            <p><a href="mailto:nutri_sabrina@yahoo.com.br" title="Email: nutri_sabrina@yahoo.com.br">nutri_sabrina@yahoo.com.br</a></p>
            
            <hr />
            
            <p>Fotos <span class="small">(clique na foto para vê-la em um tamanho maior)</span></p>
            <div id=photo-gallery>
            	<a href="#" title="Ver foto em tamanho maior" onclick="popup('img/vale-2.jpg');"><img src="img/vale-2-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/vale-3.jpg');"><img src="img/vale-3-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/vale-4.jpg');"><img src="img/vale-4-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/vale-5.jpg');"><img src="img/vale-5-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
            </div>
            
            <hr />
            
            <p>Comentários <span class="small">(<a href="#" onclick="checkLogged('comentar', 4);" title="Deixe seu comentário">comente</a>)</span></p>
            <div class="error-message em-2-4"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            <div class="new-comment-4">
				<textarea class="txt-comment-4" rows="3" cols="45"/></textarea>
				<input type="submit" value="Postar" onclick="postComment(4);" />
				<a href="#" onclick="cancelComment(4)" title="Cancelar comentário">Cancelar</a>
			</div>
			<div id="comments" class="comment-board-4">
            <div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Giovanna</a><span class="small"> (2 dia atrás): </span>Achei a chácara um pouco pequena, mas deu pra se divertir bastante.</div>
            <div class="bottom-comment"><a href="usuario.php" title="Veja o perfil do usuário">Luiz</a><span class="small"> (1 ano atrás): </span>Tava tudo mto bom ate a policia chegar e estragar...</div>
            </div><!--#comments-->
		</div>
		<?php } else { ?>
		<p><em>Fornecedor não cadastrado.</em></p>
		<?php } ?>
	</div><!--#content-->
</div>
</body>
</html>