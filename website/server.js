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


everyauth.everymodule.findUserById( function (id, callback) {
   console.log("Find User:");
   sqlConnect(function(c) {
      c.query('SELECT TOP 1 * FROM UserAccount WHERE Id = ?', [id], function(err, results) {
         console.log("Find User Result: " + id);
         results[0].id = results[0].Id;
         callback(null, results[0]);
      });
   });
});
  
everyauth.facebook
   .appId("1683955285227175")
   .appSecret("cb3685564098f77f8c95d614eb3eab47")
   .findOrCreateUser( function (session, accessToken, accessTokenExtra, fbUserMetadata) {
      var promise = this.Promise();
      
      sqlConnect(function(c) { c.query(
         'SELECT * FROM UserAccount WHERE ProviderName = \'facebook\' AND ProviderAccount = ? AND ProviderId = ? LIMIT 1', [
            fbUserMetadata.name, fbUserMetadata.id
         ], function(err, results) {
            if (err) {
               promise.fulfill([err]);
            } else {
               if (results.length == 0) {
                  c.query(
                     'INSERT INTO UserAccount (UserName, ProviderName, ProviderAccount, ProviderId, CreatedOn) VALUES (?, ?, ?, ?, NOW())', [
                        fbUserMetadata.name, 'facebook', fbUserMetadata.name, fbUserMetadata.id
                     ], function(err2, results2) {
                        if(err) {
                           promise.fulfill([err2]);
                        } else {
                           c.query(
                              'SELECT * FROM UserAccount WHERE ProviderName = \'facebook\' AND ProviderAccount = ? AND ProviderId = ? LIMIT 1', [
                              fbUserMetadata.name, fbUserMetadata.id
                           ], function(err3, results3) {
                              if (err) {
                                 promise.fulfill([err3]);
                              } else {
                                 results3[0].id = results3[0].Id;
                                 promise.fulfill(results3[0]);
                              }
                           })
                        }
                     }
                  )
               } else {
                  c.query('UPDATE UserAccount SET LastAccessedOn = NOW() WHERE ProviderName = \'facebook\' AND ProviderAccount = ? AND ProviderId = ? LIMIT 1', [
                  fbUserMetadata.name, fbUserMetadata.id], function(err4, results4) {
                     results[0].id = results[0].Id;
                     promise.fulfill(results[0]);
                  });
               }
            }
         });
      });
      
      return promise;
   })
   .redirectPath('/');


var app = express.createServer();

app
   .use(express.static(__dirname + '/public'))
   .use(express.bodyParser())
   .use(express.cookieParser('hrjas9fj3'))
   .use(express.session())
   .use(everyauth.middleware());

app.set('views', __dirname + '/views');
app.set('view engine', 'jade');

everyauth.helpExpress(app);

app.get('/', function (req, res) {
   console.log(JSON.stringify(req.user));
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