const coap = require('coap');// Import thư viện CoAP
const server = coap.createServer();// Tạo một server CoAP mới
server.on('request', (req, res) => { // Xử lý yêu cầu đến
console.log('Received:', req.payload.toString());

// CỐ TÌNH KHÔNG PHẢN HỒI ACK TRONG LẦN ĐẦU
    res.end('OK'); // Comment dòng này trong lần test đầu
});
server.listen(() => {
console.log('CoAP server started');});