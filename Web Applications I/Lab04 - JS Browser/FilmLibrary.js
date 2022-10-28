'use strict' ;

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

    this.getByDateRange = (start, end) => {
        const filteredList = new FilmList();
        this.filmList.filter((film) => film.date !== undefined)
                    .filter((film) => film.date.format('YYYY-MM-DD') >= start && film.date.format('YYYY-MM-DD') <= end)
                    .forEach((film) => {filteredList.addNewFilm(film)});
        return filteredList;
    }

    /**
     * Return the list composed only of favorites films.
     * @returns the list of favorites films
     */
    this.getFavorites = () => {
        return this.filmList.filter((film) => film.favourite === true);
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
