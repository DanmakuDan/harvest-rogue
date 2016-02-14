var express = require('express');
var everyauth = require('everyauth');
var markdown = require('markdown').markdown;
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

function getDocumentationPage(pageName, callbackPass, callbackFail) {
   var realPageName = pageName.toLowerCase().replace(/[^A-Z0-9]+/ig, "_");
   sqlConnect(function(c) {
      c.query('SELECT * FROM Documentation WHERE Name = ? LIMIT 1', [realPageName], function(err, results) {
         c.destroy();
         if (results == null || results.length == 0) {
            callbackFail();
         } else {
            callbackPass(results[0]);
         }
      });
   });
}


function createDocumentationPage(userId, pageName, title, content, callbackDone) {
   var realPageName = pageName.toLowerCase().replace(/[^A-Z0-9]+/ig, "_");
   sqlConnect(function(c) {
      c.query('INSERT INTO Documentation (Name, Title, Content, LastEditedBy, LastEditedOn) VALUES (?, ?, ?, ?, NOW())', 
      [realPageName, title, content, userId], function(err, results) {
         c.destroy();
         callbackDone();
      });
   });
}

function setDocumentationPage(userId, pageName, title, content, callbackDone) {
   var realPageName = pageName.toLowerCase().replace(/[^A-Z0-9]+/ig, "_");
   sqlConnect(function(c) {
      c.query('UPDATE Documentation SET Title = ?, Content = ?, LastEditedOn = NOW(), LastedEditedBy = ? WHERE Name = ?', 
      [title, content, userId, realPageName], function(err, results) {
         console.log(JSON.stringify(err));
         c.destroy();
         callbackDone();
      });
   });
}

everyauth.everymodule.findUserById( function (id, callback) {
   sqlConnect(function(c) {
      c.query('SELECT * FROM UserAccount WHERE Id = ? LIMIT 1', [id], function(err, results) {
         results[0].id = results[0].Id;
         c.destroy();
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
               c.destroy();
               promise.fulfill([err]);
            } else {
               if (results.length == 0) {
                  c.query(
                     'INSERT INTO UserAccount (UserName, ProviderName, ProviderAccount, ProviderId, CreatedOn) VALUES (?, ?, ?, ?, NOW())', [
                        fbUserMetadata.name, 'facebook', fbUserMetadata.name, fbUserMetadata.id
                     ], function(err2, results2) {
                        if(err) {
                           c.destroy();
                           promise.fulfill([err2]);
                        } else {
                           c.query(
                              'SELECT * FROM UserAccount WHERE ProviderName = \'facebook\' AND ProviderAccount = ? AND ProviderId = ? LIMIT 1', [
                              fbUserMetadata.name, fbUserMetadata.id
                           ], function(err3, results3) {
                              if (err) {
                                 c.destroy();
                                 promise.fulfill([err3]);
                              } else {
                                 results3[0].id = results3[0].Id;
                                 c.destroy();
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
                     c.destroy();
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
   res.render('pages/index', { pageTitle: 'Home', user: everyauth.user });
});

app.get('/screenshots', function (req, res) {
   res.render('pages/screenshots', { pageTitle: 'Screenshots' });
});

app.get('/forums', function (req, res) {
   res.render('pages/forums', { pageTitle: 'Forums' });
});

app.get('/docs', function (req, res) {
   res.redirect('/docs/home');
});


app.get('/docs/:docName', function (req, res) {
   getDocumentationPage(req.params.docName, function(doc) {
      // Page exists
      var docTitle = doc.Title;
      var docContent = doc.Content;
      
      res.render('pages/docs', { 
         isNew: false,
         pageTitle: 'Documentation [' + docTitle + ']', 
         sub: req.params.docName.toLowerCase().replace(/[^A-Z0-9]+/ig, "_"),
         title: docTitle, 
         content: docContent,
         data: markdown.toHTML(docContent) 
      });
   }, function() {
      // Page does not exist
      var docTitle = "No Page";
      var docContent = "No page content.";
      
      res.render('pages/docs', {
         isNew: true, 
         pageTitle: 'Documentation [' + docTitle + ']', 
         sub: req.params.docName.toLowerCase().replace(/[^A-Z0-9]+/ig, "_"), 
         title: docTitle,
         content: '', 
         data: markdown.toHTML(docContent) 
         });   
   });
   
   
});


app.post('/docs/:docName', function (req, res) {
   if ((req.user == null) || (req.user.CanEditDocs != 1)) {
      console.log("Not logged in...");
      console.log(JSON.stringify(req.user));
      res.redirect('/');
      return;
   }
   
   if (req.body.isNew == "true") {
      createDocumentationPage(req.user.Id, req.params.docName, req.body.pageTitle, req.body.pageContent, function() {
         res.redirect('/docs/' + req.params.docName);
      });
   } else {
      setDocumentationPage(req.user.Id, req.params.docName, req.body.pageTitle, req.body.pageContent, function() {
         res.redirect('/docs/' + req.params.docName);
      });
   }
   
   
});

app.get('/downloads', function (req, res) {
   res.render('pages/downloads', { pageTitle: 'Downloads' });
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