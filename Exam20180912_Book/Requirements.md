---
title: Book
version: 0.3
---
<!--
> To generate html and pdf use
> ```bash
> $ pandoc -s -f markdown+Smart requirements.md -c .css.css -o requirements.html
> $ pandoc -s -f markdown+Smart requirements.md -o requirements.pdf
> ```
-->
Develop a system that assists Professors building their text books, adding chapters containing either theory or exercise and checking the coherence of the final result.

All classes must be inside the package `it.polito.oop.book`. The *facade class* is `Book` --- all operations can be performed through it.

The class `TestApp` in the default package contains some drivers to help testing the application.

> The JDK documentation is accessible from the URL <http://softeng.polito.it/courses/docs/api/index.html>. 

## R1: Topics

A `Topic` can be one of the topics of a theory chapter or the single topic of a question. A topic is defined by its keyword - which is unique. 
<!-- That is, the methods `hashCode` and `equals` should consider **only** the keyword. -->

The factory method `getTopic(String keyword)` in `Book` returns the `Topic` associated to the given keyword. It either creates a new topic, if it does not exist yet, or retrieves an existing one, if it has already been created. The method accepts the keyword as single argument and throws the exception `BookException` if the keyword is `null` or empty. The method `getKeyword()` in *Topic* returns the keyword, and the same is also returned by the `toString()` method.

A topic can have other topics as sub-topics. 
The method `addSubTopic(Topic)` accepts a _Topic_ as an argument and adds it as sub-topic; it returns `true` if the subtopic was added or `false` if the subtopic was not added because already listed as a sub-topic. 
The method `getSubTopics()` returns a `List<Topic>` of all direct subtopics sorted alphabetically.

## R2: Questions

For the different topics it is possible to define exercises.

A `Question` represents a multiple answer questions. It can be created using the method `createQuestion(String,Topic)` that accepts the text of the question and its main topic. Getters are available for both fields: `getQuestion()`, `getMainTopic()`. 

The method `toString()` returns the question text followed by the main topic in parenthesis, e.g. `“What is a functional interface? (Interface)"`.

The method `addAnswer(String,boolean)` of class _Question_ accepts an answer and a boolean value indicating whether the given answer is correct for the specific question. 
The method `numAnswers()` returns the total number of answers added for the question. 
The methods `getCorrectAnswers()` and `getIncorrectAnswers()` return the set of answers (`Set<String>`) that are respectively correct and not correct for the question.

## R3: Chapters

There are two types of chapters: *theory* and *exercise*.

A theory chapter can be built using the method `createTheoryChapter(String title, int numPages, String text)` that gets the title of the chapter, the number of pages, and a string with all the text of the chapter. The method returns a  `TheoryChapter` object. 
Topics may be added to the chapter using the method `addTopic(Topic)`; the method adds the given topic to the chapter and, recursively, all its subtopics, either directly or indirectly specified. 

> For instance, if *Class* is a subtopic of *Java* and *Abstract Class* is a subtopic of *Class*, adding the topic *Java* to a chapter would add both *Class* and *Abstract Class*. 

The class provides getter and setter for the text.
The method `getTopics()` returns all topics inserted by _addTopic()_, without repetitions and sorted in alphabetical order.

An exercise chapter can be added to the book using the method `createExerciseChapter(String title, int numPages)` that accepts  the title of the chapter and the number of pages. The method returns an `ExerciseChapter` object. Questions may be added to it using the method `addQuestion(Question)` that accepts a _Question_. The method `getTopics()` returns all topics specified as main topic in the questions, without repetitions and sorted in alphabetical order.

Both classes provide the getters and setters for both attributes title and pages: `getTitle()`, `setTitle(String)`, `getNumPages()` and `setNumPages(int)`.

## R4: Statistics and Checks

The method `getAllTopics()` in _Book_ returns a list of all topics `List<Topic>` specified in the book chapters -- both theory and exercise --; the list is sorted in alphabetical order and does not contains duplicates.

The method `checkTopics` return `true` if all the topics specified in all exercise chapters are contained in at least one theory chapter.

The method `questionOptions()` in _Book_ returns a map having as key the number of answers and as values the list of questions having that number of answers.

<!--
The method `topicPopularity()` in `Book` returns the list of strings formatted as `"<Topic> : <num>"` where `<Topic>` is the keyword of the topic and `<num>` is the number of theory chapters treating the topic. The list is sorted by decreasing number of chapters and, in case of tie, by topic keyword.
-->

## R5: Assignment

An assignment represents the answers to a question provided by a student.

The method `newAssignment(String, ExerciseChapter)` creates a new assignment for the given student whose ID is provided as argument and the given exercise chapter. It returns an `Assignment` object that provides the getter methods for the two pieces of information.

The method `addResponse(Question, List<String>)` adds the response by the student to the given question: the list contains the answers that the student considers correct. The method returns the score achieved by the student: the score is `(N - FP - FN)/N` where:

* `N` is the number of total answers for the question,
* `FP` is the number of answers provided by the student that are not correct for the question (false positives),
* `FN` is the number of correct answers for the question that are not provided by the student (false negatives).

> For instance, let us consider a question has answers `A` (wrong), `B`, (wrong), `C` (correct), and `D` (correct); and a response containing answers `B` and `C`, we have: `N=4`, `FP=1` (since `B` is in the response but is not correct), `FN=1` (since `D` is correct but is not in the response), so the score would be `0.5`.

The method `totalScore()` returns the sum of the scores for all provided responses.
