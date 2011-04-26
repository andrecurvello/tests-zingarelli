<html>
<head>
<title>Testing draggable and resizable</title>
<script type="text/javascript" src="js/jquery-1.5.1.min.js"></script>
<script type="text/javascript" src="js/jquery-ui-1.8.11.custom.min.js"></script>
<script type="text/javascript" src="js/tools.js"></script>
<link type="text/css" href="css/jquery-ui-1.8.11.custom.css" rel="stylesheet" />	
<link type="text/css" href="css/main-drag.css" rel="stylesheet" />
</head>
<body>
	<div id="desktop">
		<div id="selectable">
		</div>		
	</div>
	<div id="toolbar">
		<a href="#" id="createRegion">Criar região</a><br />
		<a href="#" id="createTextBox">Criar caixa de texto</a><br />
		<a href="#" id="createVideo">Adicionar vídeo</a><br />
		<a href="#" id="createSound">Adicionar som</a><br />
		<form action="getNCL.php">
			<input id="getNCL" type="submit" title="Gerar Arquivo NCL" value="Gerar Arquivo NCL" />
		</form>
	</div>
</body>
</html>