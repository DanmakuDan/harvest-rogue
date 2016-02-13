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


/*
function addUser (source, sourceUser) {
   var user;
   user = usersById[++nextUserId] = {id: nextUserId};
   user[source] = sourceUser;
   console.log(source);
   console.log(JSON.stringify(user));
   sqlConnect(function(c) {
      c.query(
         'INSERT INTO UserAccount (UserName, ProviderName, ProviderAccount, ProviderId, CreatedOn) VALUES (??, ??, ??, ??, NOW()) WHERE NOT EXISTS ' +
         ' (SELECT 1 FROM UserAccount WHERE ProviderName = ?? AND ProviderAccount = ?? AND ProviderId = ??) ', [
            user.facebook.name, source, user.facebook.name, user.facebook.id, source, user.facebook.name, user.facebook.id], function(err, results) {
            
         }
      );
   });

   return user;
}
*/

everyauth.everymodule
  .findUserById( function (id, callback) {
     sqlConnect(function(c) {
      c.query(
         'SELECT TOP 1 * FROM UserAccount WHERE Id = ??', [id], function(err, results) {
            callback(err, results[0]);
         }
      );
   });
     
    callback(null, usersById[id]);
  });
  
everyauth.facebook
   .appId("1683955285227175")
   .appSecret("cb3685564098f77f8c95d614eb3eab47")
   .findOrCreateUser( function (session, accessToken, accessTokenExtra, fbUserMetadata) {
      var promise = this.Promise();
      
      sqlConnect(function(c) { c.query(
         'SELECT TOP 1 * FROM UserAccount WHERE ProviderName = \'facebook\' AND ProviderAccount = ?? AND ProviderId = ??', [
            fbUserMetadata.name, fbUserMetadata.id
         ], function(err, results) {
            if (err) {
               promise.fail(err);
            } else {
               if (results.length == 0) {
                  c.query(
                     'INSERT INTO UserAccount (UserName, ProviderName, ProviderAccount, ProviderId, CreatedOn) VALUES (??, ??, ??, ??, NOW())', [
                        fbUserMetadata.name, 'facebook', fbUserMetadata.name, fbUserMetadata.id
                     ], function(err2, results2) {
                        if(err) {
                           promise.fail(err2);
                        } else {
                           c.query(
                              'SELECT TOP 1 * FROM UserAccount WHERE ProviderName = \'facebook\' AND ProviderAccount = ?? AND ProviderId = ??', [
                              fbUserMetadata.name, fbUserMetadata.id
                           ], function(err, results) {
                              if (err) {
                                 promise.fail(err);
                              } else {
                                 promise.fuulfill(results[0]);
                              }
                           })
                        }
                     }
                  )
               } else {
                  promise.fulfill(results[0]);
               }
            }
         });
      });
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