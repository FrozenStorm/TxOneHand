function openTab(id) {
  document.querySelectorAll('.tab-content').forEach(el => el.classList.add('hidden'));
  document.getElementById(id).classList.remove('hidden');
}

// 🌐 Laden der Einstellungen
function loadSettings() {
  fetch("/web")
      .then(r => r.json())
      .then(data => {
          document.getElementById("settingsContent").innerHTML = `
              <label>SSID:</label><input id="ssid" class="form-control mb-2" value="${data.ssid}">
              <label>Passwort:</label><input id="password" class="form-control mb-2" value="${data.password}">
          `;
      });
}

function saveSettings() {
  const data = {
      ssid: document.getElementById("ssid").value,
      password: document.getElementById("password").value,
  };
  fetch("/web", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(data)
  }).then(() => alert("Gespeichert"));
}



// 📡 Live-Daten laden
function loadLiveView() {
  fetch("/digital")
      .then(r => r.json())
      .then(data => {
          document.getElementById("liveContent").innerHTML = `
              <p><strong>Arm:</strong> ${data.arm}</p>
              <p><strong>Pitch:</strong> ${data.pitch.toFixed(2)}</p>
              <p><strong>Roll:</strong> ${data.roll.toFixed(2)}</p>
              <p><strong>Yaw:</strong> ${data.yaw.toFixed(2)}</p>
              <p><strong>Altitude:</strong> ${data.altitude.toFixed(2)}</p>
              <p><strong>Temperature:</strong> ${data.temperature.toFixed(2)} °C</p>
              <p><strong>Orientation:</strong> ${data.orientation}</p>
          `;
          updateJoystick(data.stickLeftRight, data.stickUpDown);
      });
}

// 🎮 Joystick-Canvas aktualisieren
function updateJoystick(x, y) {
  const canvas = document.getElementById("joystickCanvas");
  const ctx = canvas.getContext("2d");
  const w = canvas.width;
  const h = canvas.height;

  const centerX = w / 2;
  const centerY = h / 2;

  const px = centerX + (x * centerX);
  const py = centerY - (y * centerY);

  ctx.clearRect(0, 0, w, h);
  ctx.strokeStyle = "#ccc";
  ctx.strokeRect(0, 0, w, h);
  ctx.beginPath();
  ctx.moveTo(centerX, 0);
  ctx.lineTo(centerX, h);
  ctx.moveTo(0, centerY);
  ctx.lineTo(w, centerY);
  ctx.stroke();

  ctx.beginPath();
  ctx.arc(px, py, 10, 0, 2 * Math.PI);
  ctx.fillStyle = "#007bff";
  ctx.fill();
  ctx.strokeStyle = "#000";
  ctx.stroke();

  document.getElementById("joystickText").innerText = `x: ${x.toFixed(2)}, y: ${y.toFixed(2)}`;
}

// 🧠 Modell laden/speichern
function loadModel() {
  fetch("/loadModel", {method: "POST"}).then(() => {
      alert("Modell geladen");
      loadModelData();
  });
}

function saveModel() {
  fetch("/saveModel", {method: "POST"}).then(() => {
      alert("Modell gespeichert");
  });
}

function loadModelData() {
  fetch("/modelname")
      .then(r => r.json())
      .then(data => {
          document.getElementById("modelContent").innerHTML = `
              <label>Modellname:</label><input id="modelName" class="form-control mb-2" value="${data.modelName}">
          `;
      });
}

// 🏁 Initialisierung
window.onload = function () {
  loadSettings();
  loadModelData();
  loadLiveView();
  setInterval(loadLiveView, 1000);
};
