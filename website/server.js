var express = require('express');
var everyauth = require('everyauth');
var marked = require('marked');
var mysql      = require('mysql');

marked.setOptions({
  renderer: new marked.Renderer(),
  gfm: true,
  tables: true,
  breaks: false,
  pedantic: false,
  sanitize: true,
  smartLists: true,
  smartypants: true
});

var markedRenderer = new marked.Renderer();
markedRenderer.table = function (header, body) {
     return '<table class="ui very compact striped table">\n'
    + '<thead>\n'
    + header
    + '</thead>\n'
    + '<tbody>\n'
    + body
    + '</tbody>\n'
    + '</table>\n';
}

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

function getForums(callbackPass, callbackFail) {
   sqlConnect(function(c) {
      c.query('SELECT * FROM Forum ORDER BY TableOrder ASC', function(err, results) {
         c.destroy();
         if (results == null || results.length == 0) {
            callbackFail();
         } else {
            callbackPass(results);
         }
      });
   });
}

function getForum(forumName, callbackPass, callbackFail) {
   sqlConnect(function(c) {
      c.query('SELECT * FROM Forum WHERE Name = ? LIMIT 1', [forumName], function(err, results) {
         c.destroy();
         if (results == null || results.length == 0) {
            callbackFail();
         } else {
            callbackPass(results[0]);
         }
      });
   });
}

function getForumPosts(forumName, callbackPass, callbackFail) {
   getForum(forumName, function(forum) {
      updateForumViewCount(forumName, function() {
         sqlConnect(function(c) {
            c.query('SELECT * FROM ForumPost WHERE ForumId = ? ORDER BY CreatedOn DESC', [forum.Id], function(err, results) {
               c.destroy();
               if (results == null) {
                  callbackFail();
               } else {
                  callbackPass(forum.Title, results);
               }
            });
         });
      });
   }, function () { callbackFail(); })
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
      c.query('UPDATE Documentation SET Title = ?, Content = ?, LastEditedOn = NOW(), LastEditedBy = ? WHERE Name = ?', 
      [title, content, userId, realPageName], function(err, results) {
         c.destroy();
         callbackDone();
      });
   });
}

function updateForumViewCount(forumName, callback) {
   sqlConnect(function(c) {
      c.query('UPDATE Forum SET Views = Views+1 WHERE Name = ?', [forumName], function(err, results) {
         c.destroy();
         callback();
      });
   });
}

function addForumReply(userId, forumPostId, message, callbackPass, callbackFail) {
   sqlConnect(function(c) {
      c.query('INSERT INTO PostReply (ForumPostId, Content, CreatedBy, CreatedOn) VALUES (?, ?, ?, NOW())', 
      [forumPostId, message, userId], function(err, results) {
         c.destroy();
         if (results == null) {
            callbackFail();
         } else {
            callbackPass();
         }
      });
   });
}

function addForumPost(userId, forumId, title, message, callbackPass, callbackFail) {
   sqlConnect(function(c) {
      c.query('INSERT INTO ForumPost (ForumId, Title, CreatedBy, CreatedOn) VALUES (?, ?, ?, NOW())', 
      [forumId, title, userId], function(err, results) {
         c.destroy();
         if (results == null) {
            callbackFail();
         } else {
            addForumReply(userId, results.insertId, message, function() {
               callbackPass(results.insertId);
            }, function() { callbackFail(); });
         }
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

everyauth.github
  .appId('1d0ea07e8fcffa312e6d')
  .appSecret('7ebe0bfff0c66a62dc74bcf5debfb82ffe091252')
  .findOrCreateUser( function (session, accessToken, accessTokenExtra, githubUserMetadata) {
      var promise = this.Promise();
      
      sqlConnect(function(c) { c.query(
         'SELECT * FROM UserAccount WHERE ProviderName = \'github\' AND ProviderAccount = ? AND ProviderId = ? LIMIT 1', [
            githubUserMetadata.name, githubUserMetadata.id
         ], function(err, results) {
            if (err) {
               c.destroy();
               promise.fulfill([err]);
            } else {
               if (results.length == 0) {
                  c.query(
                     'INSERT INTO UserAccount (UserName, ProviderName, ProviderAccount, ProviderId, CreatedOn) VALUES (?, ?, ?, ?, NOW())', [
                        githubUserMetadata.name, 'github', githubUserMetadata.name, githubUserMetadata.id
                     ], function(err2, results2) {
                        if(err) {
                           c.destroy();
                           promise.fulfill([err2]);
                        } else {
                           c.query(
                              'SELECT * FROM UserAccount WHERE ProviderName = \'github\' AND ProviderAccount = ? AND ProviderId = ? LIMIT 1', [
                              githubUserMetadata.name, githubUserMetadata.id
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
                  githubUserMetadata.name, githubUserMetadata.id], function(err4, results4) {
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
   getForums(function(rows) {
      res.render('pages/forums', { pageTitle: 'Forums', forums: rows });
   }, function() {
      res.redirect("/");
   });
});

app.get('/forum/:name', function(req, res) {
   getForumPosts(req.params.name, function(forumTitle, rows) {
      res.render('pages/forumPosts', { 
         pageTitle: 'Forum - ' + forumTitle, 
         forumTitle: forumTitle, 
         forumName: req.params.name,
         posts: rows 
      }); 
   }, function() {
      res.redirect("/forums");
   })
});

app.get('/forum/:forumName/newpost', function (req, res) {
   getForum(req.params.forumName, function(forum) {
      res.render('pages/forumNewPost', { 
      pageTitle: forum.Title + ' - New Post', 
      forum: forum
   });
   }, function() { res.redirect("/forums"); }); 
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
         data: marked(docContent, { renderer: markedRenderer }) 
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
         data: marked(docContent, { renderer: markedRenderer }) 
         });   
   });
   
   
});


app.post('/docs/:docName', function (req, res) {
   if ((req.user == null) || (req.user.CanEditDocs != 1)) {
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
   if (req.user != null) {
      res.redirect("/");
   } else {
      res.render('pages/login', { pageTitle: 'Account Login' });
   }
});

app.post('/forums/:forumId/addPost', function(req, res) {
   if (req.user == null) {
      res.redirect("/error");
   } else if (req.body.postTitle.trim().length <= 0 || req.body.postContent.trim().length <= 0) {
      res.redirect("/error2");         
   } else {
      addForumPost(req.user.Id, req.params.forumId, req.body.postTitle, req.body.postContent, function(postId) {
         res.redirect("/forumPost/" + postId);
      }, function() { res.redirect("/"); })
   }
});



app.listen(80, function () {
   console.log('Listening on port 80..');
});