var request;

if (window.XMLHttpRequest) {
	request=new XMLHttpRequest();
} else {
	request=new ActiveXObject("Microsoft.XMLHTTP");
}

request.open('GET', 'https://data.sparkfun.com/output/LQ37ad4Yy8Fx3YrKmZQ0.json');

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
                data.datasets[0].data.push(info[i].temperature); 
			}
			console.log(info[0]);
        
            var msg = 'The temperature is: '+info[info.length-1].temperature;
        
            if( info[info.length-1].temperature >= 90 ){
                msg += "<br>Temperature is way above safezone! Cooldown system Initiated.";
            }else if(info[info.length-1].temperature < 90 && info[info.length-1].temperature >= 80){
                msg += "<br>Temperature above safezone. Cooldown system Initiated.";
            }else if(info[info.length-1].temperature < 80 && info[info.length-1].temperature >= 70){
                msg += "<br>Temperature is leaving safezone. Cooldown system Initiated.";
            }else if(info[info.length-1].temperature < 70 && info[info.length-1].temperature >= 60){
                msg += "<br>Temperature is close to leaving safezone. Cooldown system Initiated.";
            }else if(info[info.length-1].temperature < 60 && info[info.length-1].temperature >= 40){
                msg += "<br>Temperature is in the safezone. Cooldown system Shutdown.";
            }else if(info[info.length-1].temperature < 40 && info[info.length-1].temperature >= 35){
                msg += "<br>Temperature is in the ideal level. Cooldown system Shutdown.";
            }else if(info[info.length-1].temperature < 35 && info[info.length-1].temperature >= 32){
                msg += "<br>Temperature is in safezone but is a little low. Too close to freezing. Cooldown system Shutdown.";
            }else if(info[info.length-1].temperature < 32 && info[info.length-1].temperature >= 30){
                msg += "<br>Temperature is too cold. Beware of freezing! Cooldown system Shutdown.";
            }else if(info[info.length-1].temperature < 30 ){
                msg += "<br>Temperature is in an extremely low temperature! Produce will freeze!.";
            }
        
        
            $( "#arduinoMsg" ).append(msg);
        
            var ctx = $("#tempChart").get(0).getContext("2d");
        
			window.myLine = new Chart(ctx).Line(data, {
                responsive: true  
            });
			
	}
} 
request.send();