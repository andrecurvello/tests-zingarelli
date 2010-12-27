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
    <?php if (strtolower($_GET["fornecedor"]) == 'enfeites'){ ?>
    <div id="content-list" class="left">
    	<br />
        <p>Resultados da busca para: <strong>enfeites</strong></p>
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
            <img class="avaliar" id="star-1" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 1 estrela" />
            <img class="avaliar" id="star-2" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 2 estrelas" title="Avalie o fornecedor - 2 estrelas" />
            <img class="avaliar" id="star-3" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 3 estrelas" title="Avalie o fornecedor - 3 estrelas" />
            <img class="avaliar" id="star-4" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 4 estrelas" title="Avalie o fornecedor - 4 estrelas" />
            <img class="avaliar" id="star-5" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 5 estrelas" title="Avalie o fornecedor - 5 estrelas" />
            &nbsp;&nbsp;&nbsp;
            <a class="small" href="#" title="Adicionar a favoritos">Adicionar a favoritos</a>
            &nbsp;&nbsp;&nbsp;
            <a class="small" href="#" title="Sinalizar como spam">Sinalizar como spam</a>
            
            <div class="error-message" id="em-1"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            
            <p class="small">Deixamos sua festa linda com nossas decorações personalizadas! Trabalhamos também com presentinhos de lembrança de batizados, festas, casamentos em geral. Venha nos visitar!</p>
            <p>Rua Centenário, 1278 - Sinibaldi - S.J.Rio Preto/SP</p>
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
            
            <p>Comentários <span class="small">(<a href="#" title="Deixe seu comentário">comente</a>)</span></p>
            <div class="error-message" id="em-2"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            <div id="comments">
            <div class="inner-comment"><a href="#" title="Veja o perfil do usuário">Marcia</a><span class="small"> (1 dia atrás): </span> Fiz a festa do meu filho com eles com a decoração do Ben 10, ficou divino!! Fora que eles cuidam de tudo, na hora de arrumar e de retirar a decoração. Nota 1000!!!</div>
            <div class="inner-comment"><a href="#" title="Veja o perfil do usuário">Ana Cláudia</a><span class="small"> (2 dias atrás): </span>Olá amiga vocês trabalham com decoração para outras cidades? Moro em Mirassol. Obrigada Ana.</div>
            <div class="bottom-comment"><a href="#" title="Veja o perfil do usuário">Carlos Eustáquio</a><span class="small"> (3 semanas atrás): </span>O atendimento é ótimo, mas o preço... podia melhorar né? Paguei 2 reais por cada lembrancinha de batizado.</div>
            </div><!--#comments-->
        </div><!--.inner-content-->
        <div class="inner-content-2">
            <h1>Kids Festas Infantis</h1>            
            <img class="avaliar" id="star-1" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 1 estrela" title="Avalie o fornecedor - 1 estrela" />
            <img class="avaliar" id="star-2" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 2 estrelas" title="Avalie o fornecedor - 2 estrelas" />
            <img class="avaliar" id="star-3" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 3 estrelas" title="Avalie o fornecedor - 3 estrelas" />
            <img class="avaliar" id="star-4" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 4 estrelas" title="Avalie o fornecedor - 4 estrelas" />
            <img class="avaliar" id="star-5" width="15" height="15" src="img/star-empty.png" alt="Avalie o fornecedor - 5 estrelas" title="Avalie o fornecedor - 5 estrelas" />
            &nbsp;&nbsp;&nbsp;
            <a class="small" href="#" title="Adicionar a favoritos">Adicionar a favoritos</a>
            &nbsp;&nbsp;&nbsp;
            <a class="small" href="#" title="Sinalizar como spam">Sinalizar como spam</a>
            
            <div class="error-message" id="em-1"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            
            <p>Lorem ipsum dolor sit amet</p>
            <p>Lorem ipsum, 235 - Lorem Ipsum/SP</p>
            <p>(xx) xxxx - xxxx</p>
            <p><a href="mailto:lorem@ipsum.com.br" title="Email: lorem@ipsum.com.br">lorem@ipsum.com.br</a></p>
            <p><a href="#" title="Acessar o site do fornecedor">http://www.lipsum.com.br</a></p>
            
            <hr />
            
            <p>Fotos <span class="small">(clique na foto para vê-la em um tamanho maior)</span></p>
            <div id=photo-gallery>
            	<a href="#" title="Ver foto em tamanho maior" onclick="popup('img/bob-esponja.jpg');"><img src="img/bob-esponja.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/bob-esponja.jpg');"><img src="img/bob-esponja.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/bob-esponja.jpg');"><img src="img/bob-esponja.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/bob-esponja.jpg');"><img src="img/bob-esponja.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/bob-esponja.jpg');"><img src="img/bob-esponja.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
            </div>
            
            <hr />
            
            <p>Comentários <span class="small">(<a href="#" title="Deixe seu comentário">comente</a>)</span></p>
            <div class="error-message" id="em-2"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            <div id="comments">
            <div class="inner-comment"><a href="#" title="Veja o perfil do usuário">Usuário A</a><span class="small"> (1 dia atrás): </span> Consectetur adipiscing elit. Nulla id neque ipsum, eu porta purus. Vivamus id tellus ac ipsum elementum consectetur. Sed viverra mi enim, vitae vestibulum libero.</div>
            <div class="inner-comment"><a href="#" title="Veja o perfil do usuário">Usuário A</a><span class="small"> (1 dia atrás): </span> Curabitur vel orci nunc. Praesent eget massa purus, sed tempor nulla. Integer at sapien non metus lacinia dapibus nec non eros.</div>
            <div class="inner-comment"><a href="#" title="Veja o perfil do usuário">Usuário A</a><span class="small"> (1 dia atrás): </span> uis pharetra enim a magna gravida sollicitudin. Phasellus semper eros in augue tincidunt luctus. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Duis ut diam ac metus varius scelerisque a in dui.</div>
            <div class="inner-comment"><a href="#" title="Veja o perfil do usuário">Usuário A</a><span class="small"> (1 dia atrás): </span> Vestibulum eget urna vitae felis sodales congue a ut metus. Nulla interdum commodo metus quis interdum. Maecenas porttitor consectetur arcu, eu mollis nulla tincidunt at. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.</div>
            <div class="bottom-comment"><a href="#" title="Veja o perfil do usuário">Usuário A</a><span class="small"> (1 dia atrás): </span> Sed sit amet velit eget eros lobortis sagittis vitae sed metus. Quisque vitae nunc nibh. Mauris vitae mauris arcu, non porttitor velit. Aliquam dictum justo ut enim volutpat eget dictum diam consectetur. Fusce lorem odio, accumsan ut lobortis ac, vulputate ac lectus.</div>
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
        	<img width="70" height="70" src="img/bob-esponja.jpg" class="left photo" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails();">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails();" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Fornecedor 1</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />            
            <p class="small">Lorem ipsum dolor sit amet, consectetur adipiscing elit. In scelerisque faucibus enim vitae aliquam. Duis luctus rutrum risus. Nullam vitae blandit sem. Vestibulum viverra auctor tempor. Nunc ac massa leo, ac tempor nisl. Etiam aliquet convallis pulvinar. Proin nulla neque, varius posuere viverra quis, semper a ligula. Nulla tortor nunc, tempus nec fermentum nec, elementum quis lacus. Suspendisse eu nisi eu leo consectetur semper.</p>
        </li>
        <li class="clear">
        	<img width="70" height="70" src="img/bob-esponja.jpg" class="left" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails();">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails();" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Fornecedor com um nome extremamente grande para testar se o layout fica muito quebrado</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />            
            <p class="small">Lorem ipsum dolor sit amet</p>
        </li>
        <li class="clear">
        	<img width="70" height="70" src="img/bob-esponja.jpg" class="left" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails();">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails();" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Fornecedor 1</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />            
            <p class="small">Lorem ipsum dolor sit amet</p>
        </li>
        <li class="clear">
        	<img width="70" height="70" src="img/bob-esponja.jpg" class="left" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails();">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails();" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Fornecedor 1</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />            
            <p class="small">Lorem ipsum dolor sit amet</p>
        </li>
        <li class="clear">
        	<img width="70" height="70" src="img/bob-esponja.jpg" class="left" />
			<p class="small right">
            	<a href="#" title="mais detalhes" onclick="showDetails();">mais detalhes</a>
               	<img class="arrow-details" src="img/arrow-right.png" width="13" onclick="showDetails();" alt="ver mais detalhes do fornecedor" title="ver mais detalhes do fornecedor" />
            </p>
            <p>Fornecedor 1</p>
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />
            <img width="12" height="12" src="img/star.png" />            
            <p class="small">Lorem ipsum dolor sit amet</p>
        </li>
        </ol>
	</div>
    <div id="content-details" class="right">
		<div id="details-empty"><em>Clique em um dos fornecedores ao lado e veja aqui mais detalhes sobre ele.</em></div>
        <div class="inner-content">
            <h1>Fornecedor 1</h1>
            <img class="avaliar" id="star-1" width="15" height="15" src="img/star-empty.png" />
            <img class="avaliar" id="star-2" width="15" height="15" src="img/star-empty.png" />
            <img class="avaliar" id="star-3" width="15" height="15" src="img/star-empty.png" />
            <img class="avaliar" id="star-4" width="15" height="15" src="img/star-empty.png" />
            <img class="avaliar" id="star-5" width="15" height="15" src="img/star-empty.png" />
            &nbsp;&nbsp;&nbsp;
            <a class="small" href="#" title="Adicionar a favoritos">Adicionar a favoritos</a>
            &nbsp;&nbsp;&nbsp;
            <a class="small" href="#" title="Sinalizar como spam">Sinalizar como spam</a>
            
            <div class="error-message" id="em-1"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            
            <p>Lorem ipsum dolor sit amet</p>
            <p>Lorem ipsum, 235 - Lorem Ipsum/SP</p>
            <p>(xx) xxxx - xxxx</p>
            <p><a href="mailto:lorem@ipsum.com.br" title="Email: lorem@ipsum.com.br">lorem@ipsum.com.br</a></p>
            <p><a href="#" title="Acessar o site do fornecedor">http://www.lipsum.com.br</a></p>
            
            <hr />
            
            <p>Fotos <span class="small">(clique na foto para vê-la em um tamanho maior)</span></p>
            <div id=photo-gallery>
            	<a href="#" title="Ver foto em tamanho maior" onclick="popup('img/bob-esponja.jpg');"><img src="img/bob-esponja.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/bob-esponja.jpg');"><img src="img/bob-esponja.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/bob-esponja.jpg');"><img src="img/bob-esponja.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/bob-esponja.jpg');"><img src="img/bob-esponja.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
                <a href="#" title="Ver foto em tamanho maior" onclick="popup('img/bob-esponja.jpg');"><img src="img/bob-esponja.jpg" width="50" alt="Foto fornecedor" title="Foto fornecedor"/></a>
            </div>
            
            <hr />
            
            <p>Comentários <span class="small">(<a href="#" title="Deixe seu comentário">comente</a>)</span></p>
            <div class="error-message" id="em-2"><a href="#" title="Cadastrar">Cadastre-se</a> ou <a href="#" title="Fazer Login">faça login</a> para realizar esta ação!</div>
            <div id="comments">
            <div class="inner-comment"><a href="#" title="Veja o perfil do usuário">Usuário A</a><span class="small"> (1 dia atrás): </span> Consectetur adipiscing elit. Nulla id neque ipsum, eu porta purus. Vivamus id tellus ac ipsum elementum consectetur. Sed viverra mi enim, vitae vestibulum libero.</div>
            <div class="inner-comment"><a href="#" title="Veja o perfil do usuário">Usuário A</a><span class="small"> (1 dia atrás): </span> Curabitur vel orci nunc. Praesent eget massa purus, sed tempor nulla. Integer at sapien non metus lacinia dapibus nec non eros.</div>
            <div class="inner-comment"><a href="#" title="Veja o perfil do usuário">Usuário A</a><span class="small"> (1 dia atrás): </span> uis pharetra enim a magna gravida sollicitudin. Phasellus semper eros in augue tincidunt luctus. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Duis ut diam ac metus varius scelerisque a in dui.</div>
            <div class="inner-comment"><a href="#" title="Veja o perfil do usuário">Usuário A</a><span class="small"> (1 dia atrás): </span> Vestibulum eget urna vitae felis sodales congue a ut metus. Nulla interdum commodo metus quis interdum. Maecenas porttitor consectetur arcu, eu mollis nulla tincidunt at. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.</div>
            <div class="bottom-comment"><a href="#" title="Veja o perfil do usuário">Usuário A</a><span class="small"> (1 dia atrás): </span> Sed sit amet velit eget eros lobortis sagittis vitae sed metus. Quisque vitae nunc nibh. Mauris vitae mauris arcu, non porttitor velit. Aliquam dictum justo ut enim volutpat eget dictum diam consectetur. Fusce lorem odio, accumsan ut lobortis ac, vulputate ac lectus.</div>
            </div><!--#comments-->
        </div><!--.inner-content-->
    </div><!--#content-details-->
    <?php } else { ?>
	<div id="empty-search">
	    <p><em>Sua busca não retorno nenhum resultado. Por favor, tente utilizar outras palavras ou utilize a nossa busca por categoria.</em></p>
	    <p><em>A busca realizada foi por: </em><strong><?php echo $_GET["fornecedor"]; ?></strong></p>
    </div>
    <?php } ?>   
    </div><!--#content-->
</div>
</body>
</html>