<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Projeto Faça a Festa - Busca de fornecedores</title>
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
    <?php if (strtolower($_GET["fornecedor"]) == 'enfeites' || strtolower($_GET["fornecedor"]) == 'decoracao'){ ?>
    <div id="content-list" class="left">
    	<br />
        <p>Resultados da busca para: <strong><?php echo $_GET["fornecedor"]?></strong></p>
    	<br />
        <ol>
        <li class="clear">
        	<img width="70" height="70" src="img/mickey.jpg" class="left photo" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails(1);">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails(1);" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Suzanna Enfeites e Decorações</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />           
            <p class="small">Deixamos sua festa linda com nossas decorações personalizadas! Trabalhamos também com presentinhos de lembrança de batizados, festas, casamentos em geral. Venha nos visitar!</p>
        </li>
        <li class="clear">
        	<img width="70" height="70" src="img/backyardigans.jpg" class="left photo" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails(2);">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails(2);" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Kids Festas Infantis</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />           
            <p class="small">Portal com todo tipo de informação para a organizações de festas infantis. Kides Festas Infantis é o melhor lugar para você organizar a festa infantil. Trabalhamos também com decorações atuais como Backyardigans, Pinky Dinky Do e Hi-5</p>
        </li>
        </ol>
	</div>
    <div id="content-details" class="right">
		<div id="details-empty"><em>Clique em um dos fornecedores ao lado e veja aqui mais detalhes sobre ele.</em></div>
        <div class="inner-content-1">
            <h1>Suzanna Enfeites e Decorações</h1>
			<span class="avaliacao-1 small"><em>Fornecedor avaliado!</em></span>
			<span class="stars-1">
            <a href="#" onclick="checkLogged('avaliar', 1);"><img class="star-1" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 1 estrela" /></a>
            <a href="#" onclick="checkLogged('avaliar', 1);"><img class="star-2" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 2 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 1);"><img class="star-3" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 3 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 1);"><img class="star-4" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 4 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 1);"><img class="star-5" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 5 estrelas" /></a>
			</span>
            &nbsp;&nbsp;&nbsp;
            <a class="small fav-1" onclick="checkLogged('favoritos', 1);" href="#" title="Adicionar a favoritos" nowrap>Adicionar a favoritos</a>
			<span class="small fav-1-1"><em>Salvo aos favoritos!</em></span>
            &nbsp;&nbsp;&nbsp;
            <a class="small spam-1"onclick="checkLogged('spam', 1);" href="#" title="Sinalizar como spam">Sinalizar como spam</a>
            <span class="small spam-1-1"><em>Obrigado, iremos verificar.</em></span>
			
            <div class="error-message em-1-1"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            
            <p class="small">Deixamos sua festa linda com nossas decorações personalizadas! Trabalhamos também com presentinhos de lembrança de batizados, festas, casamentos em geral. Venha nos visitar!</p>
            <p><strong>Dados para contato:</strong></p>
			<p>Rua Centenário, 1278 - Sinibaldi - São Carlos/SP</p>
            <p>(17) 3011 2624</p>
            
            <hr />
            
            <p>Fotos <span class="small">(clique na foto para vê-la em um tamanho maior)</span></p>
            <div id=photo-gallery>
            	<a href="#" title="Ver foto em tamanho maior" onclick="popup('img/mickey-1.jpg');"><img src="img/mickey-1-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/mickey-2.jpg');"><img src="img/mickey-2-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/mickey-3.jpg');"><img src="img/mickey-3-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/hello-kitty-1.jpg');"><img src="img/hello-kitty-1-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/hello-kitty-2.jpg');"><img src="img/hello-kitty-2-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
            </div>
            
            <hr />
            
            <p>Comentários <span class="small">(<a href="#" onclick="checkLogged('comentar', 1);" title="Deixe seu comentário">comente</a>)</span></p>
            <div class="error-message em-2-1"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            <div class="new-comment-1">
				<textarea class="txt-comment-1" rows="3" cols="45"/></textarea>
				<input type="submit" value="Postar" onclick="postComment(1);" />
				<a href="#" onclick="cancelComment(1)" title="Cancelar comentário">Cancelar</a>
			</div>
			<div id="comments" class="comment-board-1">
				<div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Marcia</a><span class="small"> (1 dia atrás): </span> Fiz a festa do meu filho com eles com a decoração do Ben 10, ficou divino!! Fora que eles cuidam de tudo, na hora de arrumar e de retirar a decoração. Nota 1000!!!</div>
				<div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Ana Cláudia</a><span class="small"> (2 dias atrás): </span>Olá amiga vocês trabalham com decoração para outras cidades? Moro em Ibaté. Obrigada Ana.</div>
				<div class="bottom-comment"><a href="usuario.php" title="Veja o perfil do usuário">Carlos</a><span class="small"> (3 semanas atrás): </span>O atendimento é ótimo, mas o preço... podia melhorar né? Paguei 2 reais por cada lembrancinha de batizado.</div>
            </div><!--#comments-->
        </div><!--.inner-content-->
        <div class="inner-content-2">
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
        </div><!--.inner-content-->        
    </div><!--#content-details-->
    <?php } else if (strtolower($_GET["fornecedor"]) == 'locais'){ ?>
    <div id="content-list" class="left">
    	<br />
        <p>Resultados da busca para: <strong>Locais</strong></p>
    	<br />
        <ol>
        <li class="clear">
        	<img width="70" height="70" src="img/barao-1-thumbs.jpg" class="left photo" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails(3);">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails();" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Chácara do Barão</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />            
            <p class="small">Ideal para todo o tipo de eventos como Casamentos, Formaturas, Aniversários, Confraternizações de Empresas, Reunião de Amigos, etc... AMPLO SALÃO com som, forno industrial, churrasqueiras, freezer, mesas e cadeiras, videoquê. QUIOSQUE com bar, televisão, máquina de gelo, churrasqueira, fogão, geladeira e forno caipira. LAZER COMPLETO: piscina com toboágua, campo de futebol, playground, chafariz e pomar. Estrutura completa de higiene inclusive berçário reservado.</p>
        </li>
        <li class="clear">
        	<img width="70" height="70" src="img/vale-1-thumbs.jpg" class="left photo" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails(4);">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails();" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Chácara Portal do Vale</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />            
            <p class="small">Chácara em Água Vermelha (Distrito de São Carlos). A chácara tem 1000m2, casa com dois dormitórios (acomoda 13 pessoas para dormir), um banheiro, uma sala, uma ampla cozinha com duas mesas, dez cadeiras, armário com talheres, pratos, copos, panelas, etc, um fogão de seis bocas, uma geladeira e uma pia, varanda com churraqueira, grelhas e espetos, uma piscina, espaço gramado e pomar no fundo.</p>
        </li>
        <li class="clear">
        	<img width="70" height="70" src="img/thiago-1-thumbs.jpg" class="left photo" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails(5);">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails();" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Salão de Festas San'Tiago</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />           
            <p class="small">Salão de festas dentro da cidade: piscina, mesas, freezer, mesa de pebolim, churrasqueira, forno a lenha. Casa mobiliada com três dormitórios sendo uma suíte com hidromassagem, sala ampla varandas, garagem coberta para oito carros, estacionamento para vários carros.</p>
        </li>
        <li class="clear">
        	<img width="70" height="70" src="img/valley-1-thumbs.jpg" class="left photo" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails(6);">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails();" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Big Valley</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />          
            <p class="small">Chácara com 5.232 m2, casa sede tem 3 quartos com 3 ventiladores c/pedestal, guarda roupa, cômoda, com capacidade para pernoite de 15 pessoas, sala com 2 sofás, cozinha completa com fogão de 6 bocas, 2 geladeiras, 1 freezer horizontal, mesa e 6 cadeiras, armários e utensílios à granel, tais como panelas, talhares, pratos, copos, etc., 2 WCs, 1 AS com tanque em inox, 140 m2 de varandas com 20 ganchos p/até 10 redes, 25 cadeiras e 7 mesas de PVC, 4 bancos de madeira-artesanato de 3 metros cada, 1 mesa de fórmica, churrasqueira de tijolinhos à vista, piscina com 40 mil litros d'água, mini campo de futebol todo gramado, mesas de bilhar e de tênis de mesa.</p>
        </li>
        </ol>
	</div>
    <div id="content-details" class="right">
		<div id="details-empty"><em>Clique em um dos fornecedores ao lado e veja aqui mais detalhes sobre ele.</em></div>
        <div class="inner-content-3">
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
        </div><!--.inner-content-->        		
		<div class="inner-content-4">
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
        </div><!--.inner-content-->        
		<div class="inner-content-5">
            <h1>Salão de Festas San'Tiago</h1>
			<span class="avaliacao-5 small"><em>Fornecedor avaliado!</em></span>         
            <span class="stars-5">
			<a href="#" onclick="checkLogged('avaliar', 5);"><img class="star-1" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 1 estrela" /></a>
			<a href="#" onclick="checkLogged('avaliar', 5);"><img class="star-2" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 2 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 5);"><img class="star-3" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 3 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 5);"><img class="star-4" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 4 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 5);"><img class="star-5" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 5 estrelas" /></a>
			</span>
            &nbsp;&nbsp;&nbsp;
            <a class="small fav-5" onclick="checkLogged('favoritos', 5);" href="#" title="Adicionar a favoritos" nowrap>Adicionar a favoritos</a>
			<span class="small fav-1-5"><em>Salvo aos favoritos!</em></span>
&nbsp;&nbsp;&nbsp;
<a class="small spam-5"onclick="checkLogged('spam', 5);" href="#" title="Sinalizar como spam">Sinalizar como spam</a>
<span class="small spam-1-5"><em>Obrigado, iremos verificar.</em></span>
			
			<div class="error-message em-1-5"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
          
			<p class="small">Salão de festas dentro da cidade: piscina, mesas, freezer, mesa de pebolim, churrasqueira, forno a lenha. Casa mobiliada com três dormitórios sendo uma suíte com hidromassagem, sala ampla varandas, garagem coberta para oito carros, estacionamento para vários carros.</p>
			<p><strong>Dados para contato:</strong></p>
            <p>Jardim Tangará  - São Carlos/SP</p>
            <p>(16) 9991-7847</p>

            <hr />
            
            <p>Fotos <span class="small">(clique na foto para vê-la em um tamanho maior)</span></p>
            <div id=photo-gallery>
            	<a href="#" title="Ver foto em tamanho maior" onclick="popup('img/thiago-2.jpg');"><img src="img/thiago-2-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/thiago-3.jpg');"><img src="img/thiago-3-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/thiago-4.jpg');"><img src="img/thiago-4-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/thiago-5.jpg');"><img src="img/thiago-5-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/thiago-6.jpg');"><img src="img/thiago-6-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
            </div>
            
            <hr />
            
            <p>Comentários <span class="small">(<a href="#" onclick="checkLogged('comentar', 5);" title="Deixe seu comentário">comente</a>)</span></p>
            <div class="error-message em-2-5"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            <div class="new-comment-5">
				<textarea class="txt-comment-5" rows="3" cols="45"/></textarea>
				<input type="submit" value="Postar" onclick="postComment(5);" />
				<a href="#" onclick="cancelComment(5)" title="Cancelar comentário">Cancelar</a>
			</div>
			<div id="comments" class="comment-board-5">
            <div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Maria Teodora</a><span class="small"> (2 dia atrás): </span> Os enfeites e a decoração é muito bonita, porém cuidado, quebram fácil e tive que pagar multa.</div>
            <div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Jacqueline</a><span class="small"> (1 mês atrás): </span>Não gostei do serviço, a decoração veio em uma caixa de papelão com alguns bonecos de isopor quebrados. Quando liguei para eles reporem os bonecos quebrados eles falaram que viriam trocar na festa e nem apareceram. Já na hora de pagar a história foi outra...</div>
            <div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Jéssica</a><span class="small"> (1 mês atrás): </span>Eles trabalham muito bem com tudo detalhado no contrato minha festa ficou maravilhosa!!</div>
            <div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Thamires Cristina</a><span class="small"> (3 meses atrás): </span>Conheço o trabalho deles e recomendo!</div>
            <div class="bottom-comment"><a href="usuario.php" title="Veja o perfil do usuário">Andréia</a><span class="small"> (1 ano atrás): </span>Eles podiam melhorar e começar a fazer decorações mais atuais o que acham????</div>
            </div><!--#comments-->
        </div><!--.inner-content-->        
		<div class="inner-content-6">
            <h1>Big Valley</h1>
			<span class="avaliacao-6 small"><em>Fornecedor avaliado!</em></span>         
            <span class="stars-6">
			<a href="#" onclick="checkLogged('avaliar', 6);"><img class="star-1" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 1 estrela" /></a>
			<a href="#" onclick="checkLogged('avaliar', 6);"><img class="star-2" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 2 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 6);"><img class="star-3" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 3 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 6);"><img class="star-4" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 4 estrelas" /></a>
			<a href="#" onclick="checkLogged('avaliar', 6);"><img class="star-5" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 5 estrelas" /></a>
			</span>
            &nbsp;&nbsp;&nbsp;
            <a class="small fav-6" onclick="checkLogged('favoritos', 6);" href="#" title="Adicionar a favoritos" nowrap>Adicionar a favoritos</a>
			<span class="small fav-1-6"><em>Salvo aos favoritos!</em></span>
&nbsp;&nbsp;&nbsp;
<a class="small spam-6"onclick="checkLogged('spam', 6);" href="#" title="Sinalizar como spam">Sinalizar como spam</a>
<span class="small spam-1-6"><em>Obrigado, iremos verificar.</em></span>
			
			<div class="error-message em-1-6"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            
			<p class="small">Chácara com 5.232 m2, casa sede tem 3 quartos com 3 ventiladores c/pedestal, guarda roupa, cômoda, com capacidade para pernoite de 15 pessoas, sala com 2 sofás, cozinha completa com fogão de 6 bocas, 2 geladeiras, 1 freezer horizontal, mesa e 6 cadeiras, armários e utensílios à granel, tais como panelas, talhares, pratos, copos, etc., 2 WCs, 1 AS com tanque em inox, 140 m2 de varandas com 20 ganchos p/até 10 redes, 25 cadeiras e 7 mesas de PVC, 4 bancos de madeira-artesanato de 3 metros cada, 1 mesa de fórmica, churrasqueira de tijolinhos à vista, piscina com 40 mil litros d'água, mini campo de futebol todo gramado, mesas de bilhar e de tênis de mesa.</p>
			<p><strong>Dados para contato:</strong></p>
            <p>Loteamento: Vale da Sta. Felicidade - Varjão - São Carlos/SP</p>
            <p>(16) 3371-6178</p>
            <p><a href="mailto:fernandotreviso@hotmail.com " title="Email: fernandotreviso@hotmail.com ">fernandotreviso@hotmail.com </a></p>
			
            <hr />
            
            <p>Fotos <span class="small">(clique na foto para vê-la em um tamanho maior)</span></p>
            <div id=photo-gallery>
            	<a href="#" title="Ver foto em tamanho maior" onclick="popup('img/valley-2.jpg');"><img src="img/valley-2-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/valley-3.jpg');"><img src="img/valley-3-thumbs.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
            </div>
            
            <hr />
            
            <p>Comentários <span class="small">(<a href="#" onclick="checkLogged('comentar', 6);" title="Deixe seu comentário">comente</a>)</span></p>
            <div class="error-message em-2-6"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            <div class="new-comment-2">
				<textarea class="txt-comment-6" rows="3" cols="45"/></textarea>
				<input type="submit" value="Postar" onclick="postComment(6);" />
				<a href="#" onclick="cancelComment(6)" title="Cancelar comentário">Cancelar</a>
			</div>
			<div id="comments" class="comment-board-6">
            <div class="inner-comment"><a href="usuario.php" title="Veja o perfil do usuário">Teófilo</a><span class="small"> (2 dias atrás): </span>Cuidado! Essa chácara não é nada do que dizem. muito ruim!!!!</div>
            <div class="bottom-comment"><a href="usuario.php" title="Veja o perfil do usuário">Rodrigo</a><span class="small"> (1 mês atrás): </span>Já fui lá, tudo muito mal cuidado e sujo. Além de ser o olho da cara.</div>
            </div><!--#comments-->
        </div><!--.inner-content-->        
    </div><!--#content-details-->
    <?php } else { ?>
	<div id="empty-search">
	    <p><em>Sua busca não retorno nenhum resultado. Por favor, tente utilizar outras palavras ou outra categoria.</em></p>
		<p><em>Você pode também alterar a área de localização da busca.</em></p>
	    <p><em>A busca realizada foi por: </em><strong><?php echo $_GET["fornecedor"]; ?></strong></p>
    </div>
    <?php } ?>   
    </div><!--#content-->
</div>
</body>
</html>