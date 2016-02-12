var express = require('express');
var app = express();

app.use(express.static(__dirname + '/public'))

app.set('views', __dirname + '/views')
app.set('view engine', 'jade')


app.get('/', function(req, res) {
    res.render('pages/index', { pageTitle: 'Home' }); 
});

app.get('/screenshots', function(req, res) {
    res.render('pages/screenshots', { pageTitle: 'Screenshots' }); 
});

app.get('/forums', function(req, res) {
    res.render('pages/forums', { pageTitle: 'Forums' }); 
});

app.get('/login', function(req, res) {
    res.render('pages/login', { pageTitle: 'Account Login' }); 
});




app.listen(80, function () {
  console.log('Listening on port 80..');
});