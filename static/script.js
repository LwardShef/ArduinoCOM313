const off = document.querySelector("#off");
const yellow = document.querySelector("#yellow");
const red = document.querySelector("#red");
const green = document.querySelector("#green");
const rainbow = document.querySelector("#rainbow");
const chase = document.querySelector("#chase");
const flame = document.querySelector("#flame");

function getData(){
    const url = "/get_js_data"
    fetch(url)
        .then(response => response.json())
        .then(json => {
            console.log(json);
            document.getElementById("temperature").innerHTML = JSON.stringify(json.Temperature)
            document.getElementById("pattern").innerHTML = JSON.stringify(json.Pattern).replace("\"", "").replace("\"", "")
            if (JSON.stringify(json.ButtonPressed) === "1"){
                document.getElementById("button").innerHTML = "False"
            }
            else{
              document.getElementById("button").innerHTML = "True"
            }
        })
}

async function sendData(button){
    const formData = new FormData()
    formData.append("Pattern", button)
    console.log(formData)
    try {
        const response = await fetch("/send_js_data", {
            method: "POST",
            // Set the FormData instance as the request body
            body: formData
        });
        console.log(await response.json());
    }
    catch (e) {
        console.error(e);
    }
}

off.addEventListener("submit", (event) => {
  event.preventDefault();
  sendData("Off");
});
yellow.addEventListener("submit", (event) => {
  event.preventDefault();
  sendData("SolidYellow");
});
red.addEventListener("submit", (event) => {
  event.preventDefault();
  sendData("SolidRed");
});
green.addEventListener("submit", (event) => {
  event.preventDefault();
  sendData("SolidGreen");
});
rainbow.addEventListener("submit", (event) => {
  event.preventDefault();
  sendData("Rainbow");
});
chase.addEventListener("submit", (event) => {
  event.preventDefault();
  sendData("Chase");
});
flame.addEventListener("submit", (event) => {
  event.preventDefault();
  sendData("Flame");
});

