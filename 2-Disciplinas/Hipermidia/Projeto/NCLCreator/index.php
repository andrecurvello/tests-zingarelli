<html>
<head>
<title>Online NCL Creator</title>
<script type="text/javascript" src="js/jquery-1.5.1.min.js"></script>
<script type="text/javascript" src="js/jquery-ui-1.8.11.custom.min.js"></script>
<script type="text/javascript" src="js/tools.js"></script>
<script type="text/javascript" src="js/json2.js"></script>
<link type="text/css" href="css/jquery-ui-1.8.11.custom.css" rel="stylesheet" />	
<link type="text/css" href="css/main-drag.css" rel="stylesheet" />
</head>
<body>
	<div id="desktop">
		<!-- Elementos sem background -->
		<div id="selectable">
		</div>		
		<!-- Elementos de mídia (imagens, vídeo, flash) -->
		<div id="image"></div>
	</div>
	<div id="toolbar">
		<ul id="menu" class="left">
			<li><a href="#" id="createRegion" title="Adicionar região">Adicionar região</a></li>
			<li><a href="#" id="createTextBox" title="Adicionar caixa de texto">Adicionar caixa de texto</a></li>
			<li><a href="#" id="createVideo" title="Adicionar vídeo">Adicionar vídeo</a></li>
			<li><a href="#" id="createSound" title="Adicionar som">Adicionar som</a></li>
			<li>
				<a href="#" id="createImage" title="Adicionar imagem">Adicionar imagem</a>
			</li>		
			<li>
				<form action="getNCL.php" id="getNCL" method="post">
					<input id="btngetNCL" type="submit" title="Gerar NCL" value="Gerar NCL" />
				</form>
			</li>
		</ul>
		<div id="sub-menu" class="left">
			<div id="text-menu" >
				<textarea rows="6" cols="20"></textarea>
				<input type="button" id="btnText" value="Ok" />
			</div>
			<div id="image-menu" >
				<span>Localização da imagem: </span>
				<input type="file" id="imagePath"/>
				<input type="submit" id="btnImage" value="Ok" />
				<span><a href="#" title="Cancelar" id="cancelImage">Cancelar</a></span>
			</div>
		</div>
	</div>
</body>
</html>