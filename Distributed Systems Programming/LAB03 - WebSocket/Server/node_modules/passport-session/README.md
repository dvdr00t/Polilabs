# passport-session

## Install

```bash
$ npm install passport-session
```

## Usage

It is the same as passport-local but with a sessionId. It allow you to authenticate your users on differents sessions (or "rooms").

#### Configure Strategy

```js
passport.use(new LocalStrategy(
  function(email, password, sessionId, done) {
    User.findOne({ email: email, sessionId: sessionId }, function (err, user) {
      if (err) { return done(err); }
      if (!user) { return done(null, false); }
      if (!user.verifyPassword(password)) { return done(null, false); }
      return done(null, user);
    });
  }
));
```

##### Available Options

This strategy takes an optional options hash before the function, e.g. `new LocalStrategy({/* options */, callback})`.

The available options are:

* `emailField` - Optional, defaults to 'email'
* `passwordField` - Optional, defaults to 'password'
* `sessionIdField` - Optional, defaults to 'sessionId'

This fields define the name of the properties in the POST body that are sent to the server.

