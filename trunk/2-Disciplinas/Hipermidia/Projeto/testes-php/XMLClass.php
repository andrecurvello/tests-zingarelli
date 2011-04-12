<?
class NCLDocument {
	private $regionBase;
	private $port;
	private $media;
		
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

	public function createXML($appName) {
		$doc = new DOMDocument('1.0','ISO-8859-1');
		$doc->formatOutput = true;
		
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
				
		echo "Resultado final...";
		$doc->save("/tmp/teste.xml");
	}
};
?>
