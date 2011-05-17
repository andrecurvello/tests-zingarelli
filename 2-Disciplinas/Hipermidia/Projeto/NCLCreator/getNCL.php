<?php
require_once 'XMLClass.php';
$nclDoc = new NCLDocument();
$dadosRegion = ($_POST['array'][0]);
$nclDoc->addRegion($dadosRegion[0], $dadosRegion[1], $dadosRegion[2], $dadosRegion[3], '157px', 'test');
$nclDoc->createXML('teste');
echo 'true';
?>