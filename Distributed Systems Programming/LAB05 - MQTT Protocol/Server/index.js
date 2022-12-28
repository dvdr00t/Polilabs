'use strict';

/* --- MODULE IMPORT --- */
var path = require('path');
var http = require('http');
var cors = require('cors');

/* --- MQTT IMPORT --- */
const MQTT = require('./components/MQTT');

/* --- PASSPORT AUTHENTICATION STRATEGY --- */
const passport          = require("passport");
const LocalStrategy     = require("passport-local");
const session           = require('express-session');
const DAO               = require('./database/dao'); 
const dao               = new DAO();
const UsersController   = require('./controllers/UsersController');
const usersController   = new UsersController(dao);
passport.use(new LocalStrategy(
    {
        usernameField: 'username',
        passwordField: 'password'
    },
    async function verify (username, password, done) {
        const user = await usersController.getUser(username, password);
        return (!user) ? done(null, undefined) : done(null, user);
    }
));
passport.serializeUser(
    (user, callback) => {
        callback(null, user);
    }
);
passport.deserializeUser(
    (user, callback) => {
        callback(null, user);
    }
);


/* --- EXPRESS APP CONFIGURATION --- */
var oas3Tools   = require('oas3-tools');
var options = {
    routing: {
        controllers: path.join(__dirname, './controllers')
    },
};
var expressAppConfig = oas3Tools.expressAppConfig(path.join(__dirname, 'api/openapi.yaml'), options);
var app = expressAppConfig.getApp();
app.use(session(
    {
        secret: "shhhhh... it's a secret!",
        resave: false,
        saveUninitialized: false
    }
));
app.use(passport.authenticate('session'));


/* --- CROSS-ORIGIN RESOURCE SHARING CONFIGURATION --- */
const corsOption = {
    origin: 'http://localhost:3000',
    credentials: true
};
app.use(cors(corsOption));


/* --- SETTING UP ROUTES --- */
const filmsCollectionRouter = require('./routers/FilmsCollectionRouter');
const filmRouter            = require('./routers/FilmRouter');
const reviewsRouter         = require('./routers/ReviewsRouter');
const imagesRouter          = require('./routers/ImagesRouter');
const usersRouter           = require('./routers/UsersRouter');
app.use('/api/films', filmsCollectionRouter);
app.use('/api/films', filmRouter);
app.use('/api/films', reviewsRouter);
app.use('/api/films/public', imagesRouter);
app.use('/api/users', usersRouter);



/* --- STARTING THE SERVER --- */
const serverPort = 8080;
http.createServer(app).listen(serverPort, function () {
    console.log('Your server is listening on port %d (http://localhost:%d)', serverPort, serverPort);
    console.log('Swagger-ui is available on http://localhost:%d/docs', serverPort);
});

