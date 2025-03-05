function fetchADC() {
    fetch('/adc')
        .then(response => response.text())
        .then(data => {
            document.getElementById("adcValue").innerText = data;
        })
        .catch(error => console.error('Fehler:', error));
}
setInterval(fetchADC, 1000);  // Automatisches Update alle 1 Sekunde
