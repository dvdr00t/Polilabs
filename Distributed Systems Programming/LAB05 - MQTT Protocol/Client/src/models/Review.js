const dayjs = require('dayjs');

/**
 * Constructor function for new Review objects
 * @param {string} id unique ID of the film (e.g., '1')
 * @param {string} title the title of the movie
 * @param {boolean} favorite it the movie is among favorite or not
 * @param {string} watchDate when the movie was watched, in a format parseable by dayjs()
 * @param {number} rating the rating assigned to the movie
 * @param {number} user the id of the user who belong the movie
*/
function Review({ filmId, reviewerId, completed, reviewDate, rating, review } = {}) {

    this.filmId = filmId;
    this.reviewerId = reviewerId;
    this.completed = completed;

    if(reviewDate)
    {
        this.reviewDate = dayjs(reviewDate);
        this.reviewDate.toJSON = function () { return this.isValid() ? this.format('YYYY-MM-DD') : "" ; }
    }
    if(rating)
        this.rating = rating;
    if(review)
        this.review = review;
        
    //this.hasDate = () => this.watchDate != undefined && this.watchDate != "";
}

export { Review }
