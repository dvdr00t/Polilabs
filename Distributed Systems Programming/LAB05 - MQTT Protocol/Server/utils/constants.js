module.exports = Object.freeze({
    PAGE_OFFSET: '10',
    VALID_EXTENSIONS: ["jpg", "png", "gif"],
    IMAGES_PATH: "uploadedImages",
    PROTO_PATH: "proto/conversion.proto",
    REMOTE_URL: "localhost:50051",
    MQTTBROKER_URL: '127.0.0.1:9090',
    WEBSOCKET_PORT: 50012,
    MAX_CHUNK_SIZE: 1024, // 1 MB

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
    },

    ERROR_415: {
        code: 415,
        message: 'Invalid Media Content'
    }
});