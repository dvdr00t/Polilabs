/**
 * Constructor function for new Review objects
 * @param {string} id unique ID of the film (e.g., '1')
 * @param {string} title the title of the movie
 * @param {boolean} favorite it the movie is among favorite or not
 * @param {string} watchDate when the movie was watched, in a format parseable by dayjs()
 * @param {number} rating the rating assigned to the movie
 * @param {number} user the id of the user who belong the movie
*/
function User({ id, name, email } = {}) {

    this.userId = id;
    this.userName = name;
    this.userEmail = email;

}

export { User }