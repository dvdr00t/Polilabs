const dayjs = require('dayjs');

/**
 * Constructor function for new Film objects
 * @param {string} id unique ID of the film (e.g., '1')
 * @param {string} title the title of the movie
 * @param {boolean} favorite it the movie is among favorite or not
 * @param {string} watchDate when the movie was watched, in a format parseable by dayjs()
 * @param {number} rating the rating assigned to the movie
 * @param {number} user the id of the user who belong the movie
*/
function Film({ id, title, owner, privateFilm, watchDate, rating, favorite } = {}) {

    if(id)
         this.id = id;
    this.title = title;
    this.owner = owner;
    this.private = privateFilm;
    //this.watchDate = "";
    if(watchDate){
        this.watchDate = dayjs(watchDate);
        this.watchDate.toJSON = function () { return this.isValid() ? this.format('YYYY-MM-DD') : "" ; }
    }
        
    if(rating)
        this.rating = parseInt(rating);
    if(favorite != undefined)
        this.favorite = favorite;

    this.hasDate = () => this.watchDate != undefined && this.watchDate != "";
}

export { Film }


