<?php
class NCLDocument {
	private $regionBase;
	private $port;
	private $media;
	//Parte 2....
//	private $link;
//	private $bind;
		
	public function addRegion($id,$left,$top,$width,$height,$descriptorName) {
		$this->regionBase[] = array(
			'id'			=>	$id,
			'left'			=>	$left,	
			'top'			=>	$top,
			'width'			=>	$width,
			'height' 		=>	$height,
			'descriptorName'	=> 	$descriptorName
			);	
	}
	
	public function addPort($id,$component) {
		$this->port[] = array(
			'id'		=>	$id,
			'component'	=>	$component
		);
	}
	
	public function addMedia($id,$type,$src,$descriptor) {
		$this->media[] = array(
			'id'		=>	$id,
			'type'		=>	$type,
			'src'		=>	$src,
			'descriptor'	=>	$descriptor
			);
	}
	//Parte 2...
	/*
	public function addLink($id,$xconnector) {
		$this->link[] = array (
			'id'		=> 	$id,
			'xconnector'	=>	$xconnector
		);	
	}
	
	public function addBind($linkid,$component,$role) {
		$this->bind[] = array (
			'linkid' 	=> 	$linkid,
			'component'	=>	$component,
			'role'		=> 	$role		
		);	
	}*/

	public function createXML($appName) {
		$doc = new DOMDocument('1.0','ISO-8859-1');
		$doc->formatOutput = true;
		
		/*set file to be downloaded by user
		header('Content-type: "text/xml"; charset="ISO-8859-1"');
    		header('Content-disposition: attachment; filename="example.xml"');
    		*/
		
		$ncl = $doc->createElement("ncl");
		$ncl->setAttribute("id",$appName);
		$ncl->setAttribute("xmlns","http://www.ncl.org.br/NCL3.0/EDTVProfile");
		$ncl = $doc->appendChild($ncl);
		
		$head = $doc->createElement("head");
		$head = $ncl->appendChild($head);
		
		$body = $doc->createElement("body");
		$body = $ncl->appendChild($body);
				
		$rb = $doc->createElement("regionBase");
		$rb = $head->appendChild($rb);	
		
		foreach($this->regionBase as $reg) {					
			$region = $doc->createElement("region");
			$region->setAttribute("id",$reg['id']);
			$region->setAttribute("top",$reg['top']);
			$region->setAttribute("left",$reg['left']);
			$region->setAttribute("width",$reg['width']);
			$region->setAttribute("height",$reg['height']);
			$region = $rb->appendChild($region);		
		}		
		
		$descriptorbase = $doc->createElement("descriptorBase");
		$descriptorbase = $head->appendChild($descriptorbase);	
		
		foreach($this->regionBase as $reg) {
			$desc = $doc->createElement("descriptor");
			$desc->setAttribute("id",$reg['descriptorName']);
			$desc->setAttribute("region",$reg['id']);
			$desc = $descriptorbase->appendChild($desc);
		}
		
		
		//Comentado == Parte 2 do projeto...
		/*
		$connector_base = $doc->createElement("connectorBase");
		$connector_base = $head->appendChild($connector_base);
		
		foreach($this->connectorBase as $con) {
			$connector = $doc->createElement("causalConnector");
			$connector->setAttribute('id',$con['id']);
			$connector = $connector_base->appendChild($connector);
			
			foreach($this->connectorParam as $con_par) {
				if($con_par['connectorid'] == $con['id']) {
					$aux = $doc->createElement("connectorParam");
					$aux->setAttribute("name",$con_par['name']);
					$aux = $connector->appendChild($aux);
				}			
			}
			
			foreach($this->simpleCondition as $simc) {
				if($simc['connectorid'] == $con['id']) {
					$aux = $doc->createElement("simpleCondition");
					$aux->setAttribute("role",$simc['role']);
					$aux->setAttribute("key",$simc['key']);
					$aux = $connector->appendChild($aux);
				}			
			}
			
			//Incompleto....
					
		}
		*/
		
		
		
		/*
		foreach($this->port as $reg) {
			$port = $doc->createElement("port");
			$port->setAttribute('id',$reg['id']);
			$port->setAttribute('component',$reg['component']);
			$port = $body->appendChild($port);
		}
		
		foreach($this->media as $reg) {
			$media = $doc->createElement("media");
			$media->setAttribute("id",$reg['id']);
			$media->setAttribute("type",$reg['type']);
			$media->setAttribute("src",$reg['src']);
			$media->setAttribute("descriptor",$reg['descriptor']);
			$media = $body->appendChild($media);
		}	
		*/
		
		
		//Cria os links e os binds....
		//Parte 2 do projeto...
		/*
		foreach($this->link as $alink) {
			$link = $doc->createElement("link");
			$link->setAttribute('id',$alink['id']);
			$link->setAttribute('xconnector',$alink['xconnector']);
			$link = $body->appendChild($link);	
			foreach($this->bind as $abind) {
				if($abind['linkid'] == $alink['id']) {
					$bind = $doc->createElement("bind");
					$bind->setAttribute('component',$abind['component']);
					$bind->setAttribute('role',$abind['role']);
					$bind = $link->appendChild($bind);
				}
			}	
		}	*/
		
		
		$doc->save("teste.xml");
	}
}?>
