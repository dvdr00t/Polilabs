'use strict';

function displayAll(films) {

    /* RETRIEVING TITLE */
    const title = document.getElementById('title');
    title.innerHTML = '<h1>All</h1>';

    generateList(films);
}

function displayFavorite(films) {
    /* RETRIEVING TITLE */
    const title = document.getElementById('title');
    title.innerHTML = '<h1>Favorite</h1>';

    /* FILTERING FAVORITES */
    const filteredFilms = new FilmList();
    films.getFavorites().forEach((film) => {
        filteredFilms.addNewFilm(film);
    });
    generateList(filteredFilms);
}

function displayBestRated(films) {
    /* RETRIEVING TITLE */
    const title = document.getElementById('title');
    title.innerHTML = '<h1>Best Rated</h1>';

    /* FILTERING FAVORITES */
    const filteredFilms = new FilmList();
    films.getRated().forEach((film) => {
        filteredFilms.addNewFilm(film);
    });
    generateList(filteredFilms);
}

function displayLastSeen(films) {
     /* RETRIEVING TITLE */
     const title = document.getElementById('title');
     title.innerHTML = '<h1>Last Seen</h1>';
 
     /* FILTERING FAVORITES */
     const filteredFilms = films.sortByDate()
     generateList(filteredFilms);
}

function displaySeenLastMonth(films) {
    /* RETRIEVING TITLE */
    const title = document.getElementById('title');
    title.innerHTML = '<h1>Seen Last Month</h1>';

    /* FILTERING FAVORITES */
    const filteredFilms = films.getByDateRange('2022-03-10', '2022-03-17');
    generateList(filteredFilms);
}

/**
 * @brief Given a date, generates the HTML text to display it
 * @param {*} value 
 * @returns 
 */
function constructDate(value) {
    if (value === undefined) {
        return '';
    } else {
        return value.format('YYYY-MM-DD');
    }
}

/**
 * @brief Given a rating between 1 and 5, generates the HTML text to display it
 * @param {*} value 
 * @returns HTML text
 */
function constructRating(value) {
    var text = ''; 
    if (value === undefined) {
        for (let i = 0; i < 5; i++) {
            text += '<span class="fa fa-star"></span>\n';
        }
    } else {
        for (let i = 0; i < 5; i++) {
            if (i < value) {
                text += '<span class="fa fa-star checked"></span>\n';
            } else {
                text += '<span class="fa fa-star"></span>\n';
            }
        }
    }
    return text;
}

/**
 * @brief Generate the HTML code to display the films
 * @param {*} films 
 */
function generateList(films) {

    /* RETRIEVING THE SPACE TO BE FILLED WITH FILMS */
    const tbody = document.getElementById('filmList');

    /* STARTING CONSTRUCTING THE HTML PAGE */
    tbody.innerHTML = '';

    /* ITERATING TO ADD ALL THE FILMS */
    films.filmList.forEach(film => {
        const tr = document.createElement('tr');
        tr.innerHTML = `<td>${film.title}</td><td><div class="form-check"><input class="form-check-input" type="checkbox" value="" id="flexCheckDefault"><label class="form-check-label" for="flexCheckDefault">Favorite</label></div></td>`;
        tr.innerHTML += `<td>${constructDate(film.date)}</td>`
        tr.innerHTML += `<td>${constructRating(film.rating)}</td>`;
        tbody.appendChild(tr);
    });


}

/**
 * RUN WHEN THE DOM CONTENT IS LOADED ON THE WEB PAGE
 * --------------------------------------------------
 */
document.addEventListener('DOMContentLoaded', (event) => {

    /* CREATING THE FILM LIST */
    const myFilmList = new FilmList();

    /* POPULATING THE FILM LIST */
    myFilmList.addNewFilm(new Film(1, "Pulp Fiction", true, "2022-03-10", 5));
    myFilmList.addNewFilm(new Film(2, "21 Grams", true, "2022-03-17", 4));
    myFilmList.addNewFilm(new Film(3, "Star Wars", false));
    myFilmList.addNewFilm(new Film(4, "Matrix", false));
    myFilmList.addNewFilm(new Film(5, "Shrek", false, "2022-02-21", 3));

    /* GENERATING THE MAIN LIST TO BE DISPLAYED */ 
    displayAll(myFilmList);

    /* RETRIEVING THE NAVBAR FILTERS */
    const menu = document.getElementById('menu');
    menu.children[0].children[0].children[1].addEventListener('click', (event) => {
        displayAll(myFilmList);
    });
    menu.children[1].children[0].children[1].addEventListener('click', (event) => {
        displayFavorite(myFilmList);
    });
    menu.children[2].children[0].children[1].addEventListener('click', (event) => {
        displayBestRated(myFilmList);
    });
    menu.children[3].children[0].children[1].addEventListener('click', (event) => {
        displayLastSeen(myFilmList);
    });
    menu.children[4].children[0].children[1].addEventListener('click', (event) => {
        displaySeenLastMonth(myFilmList);
    });

    console.log("Hello developer!");
    
});