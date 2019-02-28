	function max_min_speed_or_angle(type){

		var s_min = 0;
		var s_max = 0;
		
		for(i = 0; i < gpx_matrix.length; i++){
			if(gpx_matrix[i][type]<0)
				k = (-1)*gpx_matrix[i][type];
			else
				k = gpx_matrix[i][type];		
			if(k < s_min)
				s_min = k;
			if(k > s_max)
				s_max = k;
		}
		var ret = [];
		ret[0] = s_min;
		ret[1] = s_max;
		return ret;
	};
	
	
	function get_gpx_file() {
		 var jqXHR = $.ajax({
			async: false,
			url: 'gpx_data/gpx_data2.txt',
			dataType:'text',
		});
		return jqXHR.responseText;
	}
	
	function parse_gpx_file(){	
		var array = gpx_file.split('\n');
		for (i=0; i< array.length; i++) {
			var str = array[i];
			var str_parsed = str.split(',',4);
			var newElement = {};
			newElement.lat = parseFloat(str_parsed[0]);
			newElement.lng = parseFloat(str_parsed[1]);
			newElement.speed = parseFloat(str_parsed[2]);
			newElement.angle = parseFloat(str_parsed[3]);
			gpx_matrix.push(newElement);
		}
	};

	function draw_map_ptp(map, type, precision) {
		var type_ = type; 

		var ret = max_min_speed_or_angle(type);
		//alert(ret);
		var s_min = ret[0]; 
		var s_max = ret[1]; 
		var dim = (s_max - s_min) / 11;

		var gpx_matrix_ptp = [];
		var color = "#000000";
		var x,y;
		var i = 0;
		do{
			if(i == 0){
				x = gpx_matrix[0]["lat"];	
				y = gpx_matrix[0]["lng"];
				i++;
			}
			else {
				var newElement = {};
				medx1 = x;
				medy1 = y;
				newElement.lat = x;	
				newElement.lng = y;
				gpx_matrix_ptp.push(newElement);
				
				var newElement = {};
				x = gpx_matrix[i]["lat"];	
				y = gpx_matrix[i]["lng"];
				medx2 = x;
				medy2 = y;
				newElement.lat = x;	
				newElement.lng = y;
				
				medx = (medx1 + medx2)/2;
				medy = (medy1 + medy2)/2;
				
				gpx_matrix_ptp.push(newElement);
			
				if(type_ == "speed"){
					speed_or_angle = gpx_matrix[i]["speed"];
					string = speed_or_angle + 'km/h';
				}
				if(type_ == "angle"){
					if(gpx_matrix[i]["angle"] < 0)
						speed_or_angle = gpx_matrix[i]["angle"] * (-1);
					else
						speed_or_angle = gpx_matrix[i]["angle"];
					
					string = speed_or_angle + 'o';
				}
			
			color = select_colour(speed_or_angle, s_min, s_max, dim, type_);

			var flightPath = new google.maps.Polyline({
				path: gpx_matrix_ptp,
				geodesic: true,
				strokeColor: color,
				strokeOpacity: 1.0,
				strokeWeight: 3
			});
			flightPath.setMap(map);
			
			var mapLabel = new MapLabel({
				text: string,
				map: map,
				fontSize: 13,
				minZoom:17,
				align: 'right'
			});
			mapLabel.set('position', new google.maps.LatLng(medx, medy));
			
			gpx_matrix_ptp = [];
			i=i+precision;
			}
		} while(i < (gpx_matrix.length));
	};
	
	function select_colour(speed_or_angle, s_min, s_max, dim, type_) {
		var t = type_;
		var color="";
		if(t == "speed"){
			if(speed_or_angle >= s_min && speed_or_angle < dim )
				color = "#39e600";
			if(speed_or_angle >= dim   && speed_or_angle <dim*2)
				color = "#33ff33";
			if(speed_or_angle >= dim*2 && speed_or_angle < dim*3)
				color = "#66ff33";
			if(speed_or_angle >= dim*3  && speed_or_angle < dim*4)
				color = "#99ff33";
			if(speed_or_angle >= dim*4 && speed_or_angle <dim*5)
				color = "#ccff33";
			if(speed_or_angle >= dim*5 && speed_or_angle < dim*6 )
				color = "#ffff33";
			if(speed_or_angle >= dim*6   && speed_or_angle <dim*7)
				color = "#ffcc33";
			if(speed_or_angle >= dim*7 && speed_or_angle < dim*8)
				color = "#ff8000";
			if(speed_or_angle >= dim*8  && speed_or_angle < dim*9)
				color = "#ff6600";
			if(speed_or_angle >= dim*9 && speed_or_angle <dim*10)
				color = "#ff4000";
			if(speed_or_angle >= dim*10 && speed_or_angle <=s_max)
				color = "#ff0000";
		}
		if(t == "angle"){
			if(speed_or_angle >= s_min && speed_or_angle < dim )
				color = "#e5f7ff";
			if(speed_or_angle >= dim   && speed_or_angle <dim*2)
				color = "#b3e6ff";
			if(speed_or_angle >= dim*2 && speed_or_angle < dim*3)
				color = "#80d5ff";
			if(speed_or_angle >= dim*3  && speed_or_angle < dim*4)
				color = "#4dc4ff";
			if(speed_or_angle >= dim*4 && speed_or_angle <dim*5)
				color = "#1ab3ff";
			if(speed_or_angle >= dim*5 && speed_or_angle < dim*6 )
				color = "#0099e6";
			if(speed_or_angle >= dim*6   && speed_or_angle <dim*7)
				color = "#0088cc";
			if(speed_or_angle >= dim*7 && speed_or_angle < dim*8)
				color = "#006699";
			if(speed_or_angle >= dim*8  && speed_or_angle < dim*9)
				color = "#006699";
			if(speed_or_angle >= dim*9 && speed_or_angle <dim*10)
				color = "#004466";
			if(speed_or_angle >= dim*10 && speed_or_angle <=s_max)
				color = "#002233";
			}
		
		
		if(t != "speed" && t != "angle"){
			color = "#000000";
		}
		return color;
	};
	
	
	//////////////////////////////
	var  gpx_matrix = [];
	var gpx_file = get_gpx_file();
	parse_gpx_file();

	//////////////////////////////
	