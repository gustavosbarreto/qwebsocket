var socket = new WebSocket(Bridge.getUrl());
socket.onopen = function() { Bridge.emitOpened(); }
socket.onmessage = function(data) { Bridge.emitMessageReceived(data); }
