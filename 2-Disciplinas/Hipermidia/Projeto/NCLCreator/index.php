<html>
<head>
<title>Online NCL Creator</title>
<script type="text/javascript" src="js/jquery-1.5.1.min.js"></script>
<script type="text/javascript" src="js/jquery-ui-1.8.11.custom.min.js"></script>
<script type="text/javascript" src="js/tools.js"></script>
<script type="text/javascript" src="js/jquery.fileupload-ui.js"></script>
<script type="text/javascript" src="js/jquery.fileupload.js"></script>
<link type="text/css" href="css/jquery-ui-1.8.11.custom.css" rel="stylesheet" />	
<link type="text/css" href="css/main-drag.css" rel="stylesheet" />
</head>
<body>
	<h1>NCL Creator</h1>
    <div id="desktop">
		<!-- Elementos sem background -->
		<div id="selectable">
		</div>		
		<!-- Elementos de mídia (imagens, vídeo, flash) -->
		<div id="media"></div>
	</div>
	<div id="toolbar">
		<ul id="menu" class="left">
			<li><a href="#" id="createRegion" title="Adicionar região">Adicionar região</a></li>
			<li><a href="#" id="createVideo" title="Adicionar vídeo">Adicionar vídeo</a></li>
			<li><a href="#" id="createImage" title="Adicionar imagem">Adicionar imagem</a></li>		
			<li>
				<form action="getNCL.php" id="getNCL" method="post">
					<input id="btngetNCL" type="submit" title="Gerar NCL" value="Gerar NCL" />
				</form>
			</li>
		</ul>
		<div id="sub-menu" class="left">
            <form id="media-menu" method="post">
                <span>Localização da mídia: </span>
                <input type="file" id="mediaPath" name="mediaPath"/>
                <input type="submit" id="btnMedia" value="Ok" />
                <span><a href="#" title="Cancelar" id="cancelMedia">Cancelar</a></span>
            </form>
		</div>
	</div>
</body>
</html>