<!-- Barra de Pesquisa -->
<div id="left-bar">
    <h1 class="title">Pesquisar Fornecedores</h1>
    <br />
    <p class="small">Resultados em: <strong>São Carlos/SP</strong>&nbsp;&nbsp;<span class="small">(<a href="#" title="Alterar localização">Alterar</a>)</span></p>
    <div class="search">
        <p>Pesquisa por palavra chave:</p>
        <form action="fornecedores.php" method="get">
	        <input type="text" id="keysearch" name="fornecedor" />
		    <input type="submit" value="Pesquisar" />
        </form>
    </div>
    <hr />
    <div class="search">
        <p>Pesquisa por categoria:</p>
        <form action="fornecedores.php" method="get">
        <select name="fornecedor">
            <option title="Locais" value="Locais" >Locais</option>
            <option value="Bandas">Bandas</option>
            <option value="Buffet">Buffet</option>
            <option value="Decoração">Decoração</option>
            <option value="Bolo &amp; Doces">Bolo &amp; Doces</option>
            <option value="Bebidas">Bebidas</option>
            <option value="Artigos para Festas">Artigos para Festas</option>
            <option value="Lembranças &amp; Brindes">Lembranças &amp; Brindes</option>
            <option value="Cozinheira">Cozinheira</option>
            <option value="Churrasqueiro">Churrasqueiro</option>
            <option value="Açougue">Açougue</option>
            <option value="Serviço de Limpeza">Serviço de Limpeza</option>
            <option value="Animadores de Festa">Animadores de Festa</option>
            <option value="DJ">DJ</option>
            <option value="Seguranças">Seguranças</option>        
        </select>
        <input type="submit" value="Pesquisar" />
        </form>
    </div>
</div>