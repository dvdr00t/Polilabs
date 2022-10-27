'use strict' ;

/**
 * Create a function that, given an array of strings, for each string computes and prints 
 * a new one composed by the first two and last two characters. For instance, ‘spring’ yields ‘spng’.
 * 
 * If the word is shorter than two characters, it prints an empty string. Otherwise, if the word is 
 * two or three characters long, the function prints the same character two times. For instance, 
 * ‘it’ yields ‘itit’ and ‘cat’ yields ‘caat’.
 */
const shrink_that_strings = (input_array) => {
    return input_array.map((string) => {
        if (string.length < 2) {
            return [];
        } else if (string.length < 4) {
            return string.length == 2 ? string + string : string + string.slice(0, 1);
        } else {
            return string.slice(0, 2) + string.slice(string.length-2, string.length);
        }
    });
}

const string_test = ["cat", "spring", "a", "it", "Hello World!"];
console.log(shrink_that_strings(string_test)); 