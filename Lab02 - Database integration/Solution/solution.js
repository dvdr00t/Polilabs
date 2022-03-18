'use strict';
const dayjs = require('dayjs');
const sqlite = require('sqlite3');

/**
 * CONSTRUCTOR OF: Film()
 * ======================
 * @param {*} film_ID 
 * @param {*} title 
 * @param {*} favourite 
 * @param {*} date 
 * @param {*} rating 
 */
function Film(film_ID, title, favourite = false, date, rating) {

    /* ATTRIBUTES */
    this.film_ID = film_ID;
    this.title = title;
    this.favourite = favourite;
    if (date !== undefined) {
        this.date = dayjs(date, "YYYY-MM-DD");
    }
    if (rating !== undefined) {
        this.rating = rating;
    }
}


function FilmLibrary() {

    /* OPENING DATABASE CONNECTION */
    const db = new sqlite.Database('./modified_films.db', (err) => {
        if (err) {
            console.log('[ERROR] Opening connection with the database failed execution.');
            throw err;
        }
    });

    /** 
     * +---------+
     * | METHODS |
     * +---------+
     * Retrieving information from the database.
    */

    /* RETRIEVING ALL FILMS IN THE DATABASE */
    this.getAll = () => {
        return new Promise((resolve, reject) => {
            const query_SQL = 'SELECT * FROM films';
            db.all(query_SQL, (err, rows) => {
                if (err) {
                    reject(err);
                } else {
                    const result = rows.map((film) => new Film(film.id, film.title, film.favorite, dayjs(film.watchdate, 'YYYY-MM-DD'), film.rating));
                    resolve(result);
                }
            });
        });
    }

    /* RETRIEVING FAVORITES FILM IN THE DATABASE */
    this.getFavorites = () => {
        return new Promise((resolve, reject) => {
            const query_SQL = 'SELECT * FROM films WHERE films.favorite == 1';
            db.all(query_SQL, (err, rows) => {
                if (err) {
                    reject(err);
                } else {
                    const result = rows.map((film) => new Film(film.id, film.title, film.favorite, dayjs(film.watchdate, 'YYYY-MM-DD'), film.rating));
                    resolve(result);
                }
            });
        });
    }

    /* RETRIEVING FILM WATCHED TODAY IN THE DATABASE */
    this.getWatchedToday = () => {
        return new Promise((resolve, reject) => {
            var now = dayjs().toJSON().slice(0, 10).toString();
            const query_SQL = 'SELECT * FROM films WHERE films.watchdate == date(?)';
            db.all(query_SQL, [now], (err, rows) => {
                if (err) {
                    reject(err);
                } else {
                    const result = rows.map((film) => new Film(film.id, film.title, film.favorite, dayjs(film.watchdate, 'YYYY-MM-DD'), film.rating));
                    resolve(result);
                }
            });
        });
    }

    /* RETIEVE FILM WATCHED BEFORE THE GIVEN DATE IN THE DATABASE */
    this.getWatchedBefore = (date) => {
        return new Promise((resolve, reject) => {
            const query_SQL = 'SELECT * FROM films WHERE films.watchdate <= date(?)';
            db.all(query_SQL, [date], (err, rows) => {
                if (err) {
                    reject(err);
                } else {
                    const result = rows.map((film) => new Film(film.id, film.title, film.favorite, dayjs(film.watchdate, 'YYYY-MM-DD'), film.rating));
                    resolve(result);
                }
            });
        });
    }

    /* RETRIEVE FILM WITH A GREATER RATING THAN THE PARAMETER IN THE DATABASE */
    this.getGreaterRatings = (value) => {
        return new Promise((resolve, reject) => {
            const query_SQL = 'SELECT * FROM films WHERE films.rating >= ?';
            db.all(query_SQL, [value], (err, rows) => {
                if (err) {
                    reject(err);
                } else {
                    const result = rows.map((film) => new Film(film.id, film.title, film.favorite, dayjs(film.watchdate, 'YYYY-MM-DD'), film.rating));
                    resolve(result);
                }
            });
        });
    }

    /* RETRIEVE FILM WITH MATCHING NAME IN THE DATABASE */
    this.getFilmByName = (name) => {
        return new Promise((resolve, reject) => {
            const query_SQL = 'SELECT * FROM films WHERE films.title == ?';
            db.all(query_SQL, [name], (err, rows) => {
                if (err) {
                    reject(err);
                } else {
                    const result = rows.map((film) => new Film(film.id, film.title, film.favorite, dayjs(film.watchdate, 'YYYY-MM-DD'), film.rating));
                    resolve(result);
                }
            });
        });
    }

    /** 
     * +---------+
     * | METHODS |
     * +---------+
     * Modifying the database.
    */

    /* STORE A NEW FILM INTO THE DATABASE */
    this.store = (target_film) => {
        return new Promise((resolve, reject) => {
            let query_SQL = 'SELECT films.id FROM films';
            db.all(query_SQL, (err, rows) => {
                if (err) {
                    console.log('[ERROR] Storing a new film failed!');
                    reject(err);
                } else {
                    if (rows.filter((film) => film.id == target_film.film_ID).length == 0) {
                        query_SQL = 'INSERT INTO films(id, title, favorite, watchdate, rating) VALUES(?, ?, ?, date(?), ?)';
                        var date = target_film.date.toJSON().slice(0, 10).toString();
                        db.run(query_SQL, [target_film.film_ID, target_film.title, target_film.favourite, date, target_film.rating], (err) => {
                            if (err) {
                                console.log('[ERROR] Storing a new film failed!');
                                reject(err);
                            } else {
                                console.log('[LOG] ... Film added successfully ...');
                                resolve()
                            }
                        })
                    } else {
                        reject();
                    }
                }
            });
        });
    }

    /* REMOVE AN EXISTING FILM FROM THE DATABASE */
    this.remove = (id) => {
        return new Promise((resolve, reject) => {
            let query_SQL = 'SELECT films.id FROM films WHERE films.id == ?';
            db.all(query_SQL, [id], (err, rows) => {
                if (err) {
                    console.log('[ERROR] Removing an existing film failed.');
                    reject(err);
                } else {
                    if (rows.length == 0) {
                        reject()
                    } else {
                        query_SQL = 'DELETE FROM films WHERE films.id == ?';
                        db.run(query_SQL, [id], (err) => {
                            if (err) {
                                console.log('[ERROR] Removing an existing film failed.');
                                reject(err);
                            } else {
                                console.log('[LOG] Film removed successfully!');
                                resolve()
                            }
                        });         
                    }
                }
            });
        });
    }

    /* DELETE ALL WATCHDATE FROM THE DATABASE */
    this.resetWatchdate = () => {
        return new Promise((resolve, reject) => {
            const query_SQL = 'UPDATE films SET watchdate = NULL';
            db.run(query_SQL, (err) => {
                if (err) {
                    console.log('[ERROR] Resetting watchdates failed.');
                    reject(err);
                } else {
                    console.log('[LOG] Resetting watchdates competed successfully!');
                    resolve()
                }
            });
        });
    }

}



async function tester() {

    /* CREATING THE DATABASE OBJECT */
    const film_database = new FilmLibrary();

    /* TESTING getAll() METHOD */
    const my_films = await film_database.getAll();
    console.log(my_films)

    /* TESTING getFavorites() METHOD */
    const my_favorites = await film_database.getFavorites();
    console.log(my_favorites);

    /* TESTING getWatchedToday() METHOD */
    const watched_today = await film_database.getWatchedToday();
    console.log(watched_today);

    /* TESTING getWatchedBefore() METHOD */
    const watched_before = await film_database.getWatchedBefore(dayjs('2022-03-17').format('YYYY-MM-DD'));
    console.log(watched_before);

    /* TESTING getGreaterRatings() METHOD */
    const my_ratings = await film_database.getGreaterRatings(3);
    console.log(my_ratings);

    /* TESTING getFilmByName() METHOD */
    const my_film_name = await film_database.getFilmByName('Pulp Fiction');
    console.log(my_film_name);

    /* TESTING store() METHOD */
    const new_film = new Film('7', 'Goodfellas', 1, dayjs(), 5);
    const almost_new_film = new Film('7', 'Godfather', 1, dayjs(), 5);
    await film_database.store(new_film).catch(() => {console.log('[ERROR] Film already present!');});
    await film_database.store(almost_new_film).catch(() => {console.log('[ERROR] Film already present!');});

    /* TESTING remove() METHOD */
    await film_database.remove(7).catch(() => {console.log('[ERROR] This film does not exist.');});

    /* TESTING resetWatchdate() METHOD */
    await film_database.resetWatchdate();
}   

tester();