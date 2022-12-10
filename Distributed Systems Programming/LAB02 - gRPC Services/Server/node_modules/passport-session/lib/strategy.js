/**
 * Module dependencies.
 */
var passport = require('passport-strategy')
  , util = require('util')
  , lookup = require('./utils').lookup;

function Strategy(options, verify) {
  if (typeof options == 'function') {
    verify = options;
    options = {};
  }
  if (!verify) { throw new TypeError('LocalStrategy requires a verify callback'); }

  this.usernameField = options.usernameField || 'username';
  this._passwordField = options.passwordField || 'password';
  this._sessionIdField = options.sessionIdField || 'sessionId';

  passport.Strategy.call(this);
  this.name = 'bySession';
  this._verify = verify;
  this._passReqToCallback = options.passReqToCallback;
}

/**
 * Inherit from `passport.Strategy`.
 */
util.inherits(Strategy, passport.Strategy);

/**
 * Authenticate request based on the contents of a form submission.
 *
 * @param {Object} req
 * @api protected
 */
Strategy.prototype.authenticate = function(req, options) {
  options = options || {};
  var username = lookup(req.body, this.usernameField) || lookup(req.query, this.usernameField);
  var password = lookup(req.body, this._passwordField) || lookup(req.query, this._passwordField);
  var sessionId = lookup(req.params, this._sessionIdField) || lookup(req.body, this._sessionIdField) || lookup(req.query, this._sessionIdField);

  if (!username || !password) {
    return this.fail({ message: options.badRequestMessage || 'Missing credentials' }, 400);
  }

  if(!sessionId) {
    return this.fail({ message: options.badRequestMessage || 'You need to provide a sessionId to connect to' }, 400);
  }

  var self = this;

  function verified(err, user, info) {
    if (err) { return self.error(err); }
    if (!user) { return self.fail(info); }
    self.success(user, info);
  }

  try {
    if (self._passReqToCallback) {
      this._verify(req, username, password, sessionId, verified);
    } else {
      this._verify(username, password, sessionId, verified);
    }
  } catch (ex) {
    return self.error(ex);
  }
};

/**
 * Expose `Strategy`.
 */
module.exports = Strategy;
