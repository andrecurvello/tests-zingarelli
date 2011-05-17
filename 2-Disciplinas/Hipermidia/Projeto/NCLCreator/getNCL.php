<?php
require_once 'XMLClass.php';
$nclDoc = new NCLDocument();

foreach($_POST["array"] as $arr) {
	if($arr[0] == 'region') {
		$nclDoc->addRegion($arr[1],$arr[2],$arr[3],$arr[4],$arr[5],$arr[6]);
	} else {
		//Caso não seja uma região....
	}
}
$nclDoc->createXML('teste');
echo 'true';
?>
