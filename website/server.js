var express = require('express');
var everyauth = require('everyauth');

var mysql      = require('mysql');


function sqlConnect(callback) {
   var conn = mysql.createConnection({
   host     : 'localhost',
   user     : 'root',
   database : 'harvestrogue'
   });
   
   conn.connect(function(err) {
      callback(conn);
   });
}


var nextUserId = 1;
var usersById = {};

function addUser (source, sourceUser) {
   var user;
   user = usersById[++nextUserId] = {id: nextUserId};
   user[source] = sourceUser;
   console.log(source);
   console.log(JSON.stringify(user));
   sqlConnect(function(c) {
      c.query(
         'INSERT INTO UserAccount SET ? WHERE NOT EXISTS (SELECT * FROM UserAccount WHERE ProviderName=\'facebook\' AND ProviderId=\'' + user.facebook.id + '\')', {
         "UserName": user.facebook.name, 
         "ProviderName": source, 
         "ProviderAccount": user.facebook.name, 
         "ProviderId": user.facebook.id,
         "CreatedOn" : new Date()
         }, function(err, results) {
            results.insertId
         }
      );
   });

   return user;
}


everyauth.everymodule
  .findUserById( function (id, callback) {
    callback(null, usersById[id]);
  });
  
everyauth.facebook
  .appId("1683955285227175")
  .appSecret("cb3685564098f77f8c95d614eb3eab47")
  .findOrCreateUser( function (session, accessToken, accessTokenExtra, fbUserMetadata) {

     
      return usersByFbId[fbUserMetadata.id] || (usersByFbId[fbUserMetadata.id] = addUser('facebook', fbUserMetadata));
   })
  .redirectPath('/');
  
  
  
var usersByFbId = {};

var app = express()
   .use(express.static(__dirname + '/public'))
   .use(express.bodyParser())
   .use(express.cookieParser('hrjas9fj3'))
   .use(express.session())
   .use(everyauth.middleware());

app.set('views', __dirname + '/views');
app.set('view engine', 'jade');

app.get('/', function (req, res) {
   console.log(req.user);
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