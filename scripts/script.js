function getTemp(){
    while (true){
        const url = "/get_js_data"
        fetch(url)
        .then(response => response.json())
        .then(json => {
            console.log(json);
            document.getElementById("temperature").innerHTML = JSON.stringify(json)
        })
    }

}

