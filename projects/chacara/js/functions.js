$(document).ready(function(){
		/* Handles photoslider */
		//change the 'baseURL' to reflect the host and or path to your images
		FOTO.Slider.baseURL = 'img/photos/';
	
		//set images by filling our bucket directly
		FOTO.Slider.bucket = {
			'default': {
				1: {'thumb': 't_01.JPG', 'main': '01.JPG'},
				2: {'thumb': 't_02.JPG', 'main': '02.JPG'},
				3: {'thumb': 't_03.JPG', 'main': '03.JPG'},
				4: {'thumb': 't_04.JPG', 'main': '04.JPG'},
				5: {'thumb': 't_05.JPG', 'main': '05.JPG'},
				6: {'thumb': 't_06.JPG', 'main': '06.JPG'},
				7: {'thumb': 't_07.JPG', 'main': '07.JPG'},
				8: {'thumb': 't_08.JPG', 'main': '08.JPG'},
				9: {'thumb': 't_09.JPG', 'main': '09.JPG'}				
			}
		};
		//load photoslider to site
		FOTO.Slider.reload('default');
		
		//enables slideshow
		FOTO.Slider.preloadImages('default');
		
				
		/* PNGFix */
		/*$(document).pngFix(); */
		

});