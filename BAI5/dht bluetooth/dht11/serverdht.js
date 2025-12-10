// server.js
const express = require("express");
const http = require("http");
const { Server } = require("socket.io");
const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

// ==== Serve file HTML trong thư mục /public ====
app.use(express.static("public"));

// ==== Serial port (Bluetooth / COM ảo) ====
// ĐỔI "COM10" thành cổng đúng của bạn (xem trong Device Manager / Proteus COMPIM)
const portName = "COM1";

const serialPort = new SerialPort({
  path: portName,
  baudRate: 9600,
});

const parser = serialPort.pipe(new ReadlineParser({ delimiter: "\r\n" }));

serialPort.on("open", () => {
  console.log("Serial opened on", portName);
});

serialPort.on("error", (err) => {
  console.error("Serial error:", err.message);
});

// Nhận từng dòng từ Arduino: "temp,hum"
parser.on("data", (line) => {
  console.log("Serial raw:", line);

  const parts = line.split(",");
  if (parts.length === 2) {
    const temp = parseFloat(parts[0]);
    const hum = parseFloat(parts[1]);

    if (!isNaN(temp) && !isNaN(hum)) {
      // Gửi cho mọi client Web
      io.emit("sensor-data", { temp, hum });
      console.log("Emit:", { temp, hum });
    }
  }
});

// ==== Socket.io ====
io.on("connection", (socket) => {
  console.log("Client connected:", socket.id);

  socket.on("disconnect", () => {
    console.log("Client disconnected:", socket.id);
  });
});

// ==== Start server ====
const PORT = 3000;
server.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
});
