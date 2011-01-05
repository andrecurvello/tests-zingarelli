$(document).ready(function(){
		/* Handles photoslider */
		//change the 'baseURL' to reflect the host and or path to your images
		FOTO.Slider.baseURL = 'images/photos/';
	
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
				9: {'thumb': 't_09.JPG', 'main': '09.JPG'},
				10: {'thumb': 't_10.JPG', 'main': '10.JPG'},
				11: {'thumb': 't_11.JPG', 'main': '11.JPG'},
				12: {'thumb': 't_12.JPG', 'main': '12.JPG'},
				13: {'thumb': 't_13.JPG', 'main': '13.JPG'},
				14: {'thumb': 't_14.JPG', 'main': '14.JPG'},
				15: {'thumb': 't_15.JPG', 'main': '15.JPG'},
				16: {'thumb': 't_16.JPG', 'main': '16.JPG'},
				17: {'thumb': 't_17.JPG', 'main': '17.JPG'},
				18: {'thumb': 't_18.JPG', 'main': '18.JPG'},
				19: {'thumb': 't_19.JPG', 'main': '19.JPG'},
				20: {'thumb': 't_20.JPG', 'main': '20.JPG'},
				21: {'thumb': 't_21.JPG', 'main': '21.JPG'},
				22: {'thumb': 't_22.JPG', 'main': '22.JPG'},
				23: {'thumb': 't_23.JPG', 'main': '23.JPG'},
				24: {'thumb': 't_24.JPG', 'main': '24.JPG'},
				25: {'thumb': 't_25.JPG', 'main': '25.JPG'},
				26: {'thumb': 't_26.JPG', 'main': '26.JPG'},
				27: {'thumb': 't_27.JPG', 'main': '27.JPG'},
				28: {'thumb': 't_28.JPG', 'main': '28.JPG'},
				29: {'thumb': 't_29.JPG', 'main': '29.JPG'},
				30: {'thumb': 't_30.JPG', 'main': '30.JPG'},
				31: {'thumb': 't_31.JPG', 'main': '31.JPG'},
				32: {'thumb': 't_32.JPG', 'main': '32.JPG'},
				33: {'thumb': 't_33.JPG', 'main': '33.JPG'},
				34: {'thumb': 't_34.JPG', 'main': '34.JPG'},
				35: {'thumb': 't_35.JPG', 'main': '35.JPG'},
				36: {'thumb': 't_36.JPG', 'main': '36.JPG'},
				37: {'thumb': 't_37.JPG', 'main': '37.JPG'},
				38: {'thumb': 't_38.JPG', 'main': '38.JPG'},
				39: {'thumb': 't_39.JPG', 'main': '39.JPG'},
				40: {'thumb': 't_40.JPG', 'main': '40.JPG'},
				41: {'thumb': 't_41.JPG', 'main': '41.JPG'},
				42: {'thumb': 't_42.JPG', 'main': '42.JPG'},
				43: {'thumb': 't_43.JPG', 'main': '43.JPG'}
			}
		};
		//load photoslider to site
		FOTO.Slider.reload('default');
		
		//enables slideshow
		FOTO.Slider.preloadImages('default');
		
				
		/* PNGFix */
		/*$(document).pngFix(); */
		

});