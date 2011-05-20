<?xml version="1.0" encoding="ISO-8859-1"?>
<ncl xmlns="http://www.ncl.org.br/NCL3.0/EDTVProfile" id="teste">
  <head>
    <regionBase>
      <region id="region-image-1" top="440px" left="90px" width="100px" height="100px"/>
      <region id="region-image-2" top="440px" left="240px" width="100px" height="100px"/>
      <region id="region-image-3" top="440px" left="410px" width="100px" height="100px"/>
      <region id="region-image-4" top="40px" left="540px" width="330px" height="370px"/>
      <region id="region-video-0" top="30px" left="80px" width="440px" height="400px"/>
    </regionBase>
    <descriptorBase>
      <descriptor id="descriptor-image-1" region="region-image-1"/>
      <descriptor id="descriptor-image-2" region="region-image-2"/>
      <descriptor id="descriptor-image-3" region="region-image-3"/>
      <descriptor id="descriptor-image-4" region="region-image-4"/>
      <descriptor id="descriptor-video-0" region="region-video-0"/>
    </descriptorBase>
  </head>
  <body>
    <port id="Port-0" component="image-1"/>
    <port id="Port-1" component="image-2"/>
    <port id="Port-2" component="image-3"/>
    <port id="Port-3" component="image-4"/>
    <port id="Port-4" component="video-0"/>
    <media id="image-1" type="image/jpeg" src="play.jpg" descriptor="descriptor-image-1"/>
    <media id="image-2" type="image/jpeg" src="pause.jpg" descriptor="descriptor-image-2"/>
    <media id="image-3" type="image/jpeg" src="stop.jpg" descriptor="descriptor-image-3"/>
    <media id="image-4" type="image/jpeg" src="text.jpg" descriptor="descriptor-image-4"/>
    <media id="video-0" type="image/mpeg" src="black-swan.mp4" descriptor="descriptor-video-0"/>
  </body>
</ncl>
