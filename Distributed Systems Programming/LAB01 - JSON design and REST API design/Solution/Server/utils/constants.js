module.exports = Object.freeze({
    PAGE_OFFSET: '2',

    /* --- GOOD THINGS --- */
    MESSAGE_201: {
        code: 201,
        message: "Created"
    },
    
    MESSAGE_204: {
        code: 204,
        message: "No content"
    },

    /* --- ERRORS --- */
    ERROR_400: {
        code: 400, 
        message: 'Bad request'
    },
    
    ERROR_404: {
        code: 404,
        message: 'Not found'
    },

    ERROR_500: {
        code: 500,
        message: 'Service error'
    },

    ERROR_401: {
        code: 401,
        message: 'Unauthorized'
    },

    ERROR_403: {
        code: 403,
        message: 'Forbidden'
    }
});