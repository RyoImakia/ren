

let http = require('http');

        http.createServer(function (request, response) {
        response.end('{"text":"wa-i"}');

}).listen(1234);

console.log('Server http://localhost:1234/ running...');
