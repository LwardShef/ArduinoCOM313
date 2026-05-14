        var intervalId = setInterval(getTemp(), 2000);
        function getTemp(){
            console.log("js Running")
            const url = "/get_js_data"
            fetch(url)
            .then(response => response.json())
            .then(json => {
                console.log(json);
                document.getElementById("temperature").innerHTML = JSON.stringify(json)
            })
        }