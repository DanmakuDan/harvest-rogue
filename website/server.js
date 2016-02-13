var express = require('express');
var app = express();
var everyauth = require('everyauth');
var bodyParser = require('body-parser')
var cookieParser = require('cookie-parser');
var expressSession = require('express-session');
var fs = require('fs');

function addUser (source, sourceUser) {
  var user;
  if (arguments.length === 1) { // password-based
    user = sourceUser = source;
    user.id = ++nextUserId;
    return usersById[nextUserId] = user;
  } else { // non-password-based
    user = usersById[++nextUserId] = {id: nextUserId};
    user[source] = sourceUser;
  }
  return user;
}

everyauth.facebook
  .appId(fs.readFileSync(__dirname + "../../../facebook.appid", "utf8"))
  .appSecret(fs.readFileSync(__dirname + "../../../facebook.secret", "utf8"))
  .handleAuthCallbackError( function (req, res) {
     res.redirect("/");
  })
  .findOrCreateUser( function (session, accessToken, accessTokExtra, fbUserMetadata) {
    console.log(JSON.stringify(fbUserMetadata));
    return usersByFbId[fbUserMetadata.id] || (usersByFbId[fbUserMetadata.id] = addUser('facebook', fbUserMetadata));
  });
  
var usersByFbId = {};

app
   .use(express.static(__dirname + '/public'))
   .use(expressSession({secret: 'harvest-rogue-123'}))
   .use(everyauth.middleware(app))
   .use(bodyParser())
   .use(cookieParser('harvestRogue123'));

app.set('views', __dirname + '/views');
app.set('view engine', 'jade');

app.get('/', function (req, res) {
   res.render('pages/index', { pageTitle: 'Home', user: everyauth.user });
});

app.get('/screenshots', function (req, res) {
   res.render('pages/screenshots', { pageTitle: 'Screenshots' });
});

app.get('/forums', function (req, res) {
   res.render('pages/forums', { pageTitle: 'Forums' });
});

app.get('/login', function (req, res) {
   if (everyauth.loggedIn) {
      res.redirect("/");
   } else {
      res.render('pages/login', { pageTitle: 'Account Login' });
   }
});




app.listen(80, function () {
   console.log('Listening on port 80..');
});