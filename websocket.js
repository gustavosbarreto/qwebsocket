var socket = new WebSocket("ws://" + Bridge.getAddress() + ":8081");
socket.onopen = function() { Bridge.emitOpened(); }
socket.onmessage = function(data) { Bridge.emitMessageReceived(data); }
