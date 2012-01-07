var socket = new WebSocket(Bridge.getUrl());
socket.onopen = function() { Bridge.emitOpened(); }
socket.onmessage = function(event) { Bridge.emitMessageReceived(event.data); }
socket.onerror = function(event) {Bridge.emitError(event.data); }
socket.onclose = function(event) {Bridge.emitClose(); }