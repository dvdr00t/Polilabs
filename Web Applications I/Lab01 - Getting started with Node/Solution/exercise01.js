'use strict' ;
const dayjs = require('dayjs');
const customParseFormat = require('dayjs/plugin/customParseFormat');
dayjs.extend(customParseFormat);

/**
 * In this exercise, you will implement a simple application to track the films that a person 
 * wants to watch and the ones they have already watched. Each film is represented by the following fields:
 *      - A unique numerical id (mandatory)
 *      - A title (mandatory)
 *      - A Boolean value to represent whether the film is among the person’s favorites (default value: false)
 *      - A date corresponding to the date when the person watched the film (optional)
 *      - A numerical value between 1 and 5 to represent the rating that the person has given to the film after watching it (optional)
 */


/* FILM CONSTRUCTOR FUNCTION */
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

/* FILM LIST CONSTRUCTOR FUNCITON */
function FilmList() {

    /* ATTRIBUTES */
    this.filmList = [];

    /* METHODS */
    this.addNewFilm = (film) => {
        this.filmList.push(film);
    }

    this.printList = () => {
        this.filmList.forEach((film) => {
            console.log(film.title)
        });
    }

    this.getFilmName = (identifier = undefined) => {
        if (identifier === undefined) {
            console.log("[ERROR] to get a film name, insert the film ID.");
        }
        return this.filmList.filter((film) => film.film_ID === identifier)[0].title;
    }

    /**
     * Returns a new array containing the Films within the FilmLibrary instance sorted in ascending order of the watch date. 
     * The movies that the user has not already watched should be put at the end.
     * @returns the sorted list of films
     */
    this.sortByDate = () => {
        const sortedList = new FilmList();
        this.filmList.filter((film) => film.date !== undefined).sort((a, b) => a.date - b.date).forEach((film) => {
            sortedList.addNewFilm(film);
        });
        this.filmList.filter((film) => film.date === undefined).forEach((film) => {
            sortedList.addNewFilm(film);
        });
        return sortedList
    }

    /**
     * Removes a film from the library, given its title or ID.
     * 
     * @param {*} film_ID 
     * @param {*} title 
     */
    this.deleteFilm = (film_ID = undefined, title = undefined) => {
        if (film_ID === undefined && title === undefined) {
            console.log("[ERROR] to delete a film, insert the film ID or the title.");
        }
        const index = this.filmList.forEach((film, index) => {
            if (film.film_ID === film_ID || film.title === title) {
                console.log(`[UPDATE] film ${film.title} deleted successfully.`);
                this.filmList.splice(index, 1);
            }
        });
    }

    /**
     * Deletes the Watch date of all the Films in the FilmLibrary.
     */
    this.resetWatchedFilms = () => {
        this.filmList.forEach((film) => {
            film.date = undefined;
        });
    }

    /**
     * Selects the films that do have a defined score. Only movies with an assigned score should be returned, ordered by decreasing score.
     * @returns the list of films sorted by ratings
     */
    this.getRated = () => {
        return this.filmList.filter((film) => film.rating !== undefined).sort((a, b) => b.rating - a.rating);
    }
}

/* POPULATING THE LIST OF FILMS */
const myFilmList = new FilmList();
myFilmList.addNewFilm(new Film(1, "Pulp Fiction", true, "2022-03-10", 5));
myFilmList.addNewFilm(new Film(2, "21 Grams", true, "2022-03-17", 4));
myFilmList.addNewFilm(new Film(3, "Star Wars", false));
myFilmList.addNewFilm(new Film(4, "Matrix", false));
myFilmList.addNewFilm(new Film(5, "Shrek", false, "2022-02-21", 3));

/* SORT BY DATES */
const sortedList = myFilmList.sortByDate();
sortedList.printList();

/* DELETE A FILM */
myFilmList.deleteFilm();
myFilmList.deleteFilm(4);

/* GET FILM NAME */
const filmName = myFilmList.getFilmName(2);
console.log(`[NAME] name of the film: ${filmName}`);

/* RESET THE DATES */
myFilmList.resetWatchedFilms()
myFilmList.printList();

