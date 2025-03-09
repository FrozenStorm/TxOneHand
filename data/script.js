
async function fetchData() {
    try {
        const response = await fetch('/data'); // ESP32 API abrufen
        if (!response.ok) throw new Error("Netzwerkfehler");
        const data = await response.json();
        
        document.getElementById("status").innerText = "Verbunden ✅";

        const tableBody = document.getElementById("data-body");
        tableBody.innerHTML = ""; // Tabelle leeren

        for (const key in data) {
            const row = document.createElement("tr");

            const cellKey = document.createElement("td");
            cellKey.textContent = key;
            row.appendChild(cellKey);

            const cellValue = document.createElement("td");
            cellValue.textContent = data[key];
            row.appendChild(cellValue);

            tableBody.appendChild(row);
        }
    } catch (error) {
        document.getElementById("status").innerText = "Fehler ❌";
        console.error("Fehler beim Abrufen der Daten:", error);
    }
}

// Starte das Polling alle 1000ms
setInterval(fetchData, 1000);
fetchData(); // Erste Anfrage sofort