<?php
	require_once 'XMLClass.php';
	$nclDoc = new NCLDocument();
	$counter = 0;
	foreach($_POST["array"] as $arr) {
		switch($arr[0]) {
			case 'region':
				$nclDoc->addRegion($arr[1],$arr[2],$arr[3],$arr[4],$arr[5],$arr[6]);
				break;			
			case 'media':
				$nclDoc->addRegion($arr[1],$arr[2],$arr[3],$arr[4],$arr[5],$arr[6]);
				$nclDoc->addMedia($arr[7], $arr[8],$arr[9],$arr[6]);
				$nclDoc->addPort("Port-".$counter,$arr[7]);
				break;		
			default:
				break;
		}
		$counter++;
	}
	
	// media + idRegion + left + top + width + height + descriptorName + idMedia + imgType + src
			
	$nclDoc->createXML('teste');
	echo 'true';
?>
