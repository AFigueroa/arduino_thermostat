var request;

if (window.XMLHttpRequest) {
	request=new XMLHttpRequest();
} else {
	request=new ActiveXObject("Microsoft.XMLHTTP");
}

request.open('GET', '/js/json/light_data.json');

request.onreadystatechange = function() {
	if ((request.status === 200) &&
		(request.readyState === 4)) {

			info = JSON.parse(request.responseText);
        
			var data = {
                labels: [],
                datasets: [
                    {
                        label: "Light Measurements",
                        fillColor: "rgb(239, 242, 213)",
                        strokeColor: "rgb(25, 41, 126)",
                        pointColor: "rgb(240, 215, 108)",
                        pointStrokeColor: "rgb(25, 41, 126)",
                        pointHighlightFill: "#f0f0f0",
                        pointHighlightStroke: "rgba(220,220,220,1)",
                        data: []
                    }
                ]
            };        
        
			for (var i = 0; i <= info.length-1; i++) {
                info.reverse();
				data.labels.push(info[i].timestamp);    
                data.datasets[0].data.push(info[i].light); 
			}
			console.log(data);
			
            var ctx = $("#lightChart").get(0).getContext("2d");
        
			window.myLine = new Chart(ctx).Line(data, {
                responsive: true  
            });
			
	}
} 
request.send();