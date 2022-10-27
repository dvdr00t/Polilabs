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
Sviluppare una sistema che assista i professori nel costruire i propri libri di test,
aggiungendo capitoli contenenti teoria o esercizi e verificando la coerenza del risultato finale.

Tutte le classi devono essere nel package `it.polito.oop.book`. La classe *facade* attraverso cui tutte le operazioni sono eseguite è `Book`.

La classe `TestApp` nel default package contiene alcuni esempi che permettono di verificare l'applicazione.

> La documentazione JDK è accessibile all'URL <http://softeng.polito.it/courses/docs/api/index.html>. 


## R1: Argomenti

Un argomento (`Topic`) può essere uno degli argomenti di un capitolo di teoria o il focus di una domanda. Un argomento è definito dalla parola chiave, che è unica. 
<!-- That is, the methods `hashCode` and `equals` should consider **only** the keyword. -->

Il factory method `getTopic(String)` in `Book` restituisce il `Topic` associato ad una data parola chiave. Esso crea un nuovo argomento, se non esiste, oppure recupera l'argomento esistente, se è già stato creato.
Il metodo accetta come unico parametro la parola chiave e lancia un'eccezione `BookException` se la parola chiave è vuota oppure `null`. 
Il metodo `getKeyword()` restituisce la parola chiave come pure il metodo `toString()`.

Un argomento può avere altri argomenti come sotto-argomenti.
Il metodo `addSubTopic(Topic)` accetta un _Topic_ e lo aggiunge come sotto-argomento; restituisce `true` se il sotto-argomento è stato aggiunto o `false` se il sotto-argomento non è stato aggiunto perchè già presente. 
Il metodo `getSubTopics()` restituisce una `List<Topic>` con tutti i sotto-argomenti, ordinati alfabeticamente. N.B. Il metodo **non** restituisce ricorsivamente i sotto-argomenti di ogni sotto-argomento del topic.



## R2: Domande

Per i diversi argomenti è possibile definire degli esercizi.

Una domanda (`Question`) rappresenta una domanda a risposta multipla. Può essere creata tramite il metodo `createQuestion(String,Topic)` che accetta il testo della domanda ed il suo argomento principale. Sono disponibili i metodi getter per i due campi: `getQuestion()`, `getMainTopic()`.

Il metodo `toString()` restituice la domanda seguita dall'argomento principale tra parentesi tonde, ad es. `“Cosa è una interfaccia funzionale? (Interfacce)"`.

Il metodo `addAnswer(String, boolean)` della classe _Question_ accetta una risposta ed un booleano che indica se la risposta è corretta per la domanda.
Il metodo `numAnswers()` restituisce il numero totale di risposte aggiunte alla domanda.
I metodi `getCorrectAnswers()` e `getIncorrectAnswers()` restituiscono gli insiemi delle risposte (`Set<String>`) che sono rispettivamente corrette e non corrette per la domanda.



## R3: Capitoli

Ci sono due tipi di capitoli: *teoria* e *esercizi*.

Un capitolo di teoria può essere costruito con il metodo `createTheoryChapter(String title, int numPages, String text)` che accetta il titolo del capitolo, il numero di pagine e una stringa con tutto il testo del capitolo. Il metodo restituisce un oggetto `TheoryChapter`.
E' possibile aggiungere degli argomenti ad un capitolo usando il metodo `addTopic(Topic)`; il metodo aggiunge un dato argomento al capitolo e, ricorsivamente, tutti i sotto-argomenti. 

> Per esempio, se *Class* è un sotto-argomento di *Java*, e *Abstract Class* è un sotto-argomento di *Class*, aggiungere l'argomento *Java* significa aggiungere anche *Class* e *Abstract Class*. 

La classe fornisce i metodi getter e setter per il testo. 
Il metodo `getTopics()` restituisce tutti gli argomenti inseriti con _addTopic()_, senza ripetizioni e ordinati alfabeticamente.

Un capitolo di esercizi può essere aggiunto al libro con il metodo `createExerciseChapter(String title, int numPages)` che accetta il titolo del capitolo ed il numero di pagine. Il metodo restituisce un oggetto `ExerciseChapter`.
Si possono aggiungere domande al capitolo tramite il metodo  `addQuestion(Question)` che riceve un oggetto di tipo _Question_. Il metodo `getTopics()` restituisce tutti gli argomenti definiti per le domande, senza ripetizioni ed ordinati alfabeticamente.

Entrambe le classi forniscono i metodi getter e setter per entrambi gli attributi, titolo e pagine: `getTitle()`, `setTitle(String)`, `getNumPages()` and `setNumPages(int)`.

## R4: Statistiche e controlli

Il metodo `getAllTopics()` in _Book_ restituisce una lista `List<Topic>` di tutti gli argomenti specificati nei capitoli del libro (sia di teoria che di esercizi) senza ripetizioni e ordinati alfabeticamente.

Il metodo  `checkTopics` restituisce `true` se tutti gli argomenti specificati in tutti i capitoli di esercizi sono contenuti in almeno un capitolo di teoria.

Il metodo  `questionOptions()` in _Book_ restituisce una mappa avente come chiave il numero di risposte e come valore la lista di domande che hanno quel numero di risposte.

<!--
Il metodo `topicPopularity()` in `Book` restituisce la lista delle stringhe formattate come `"<Topic> : <num>"` dove  `<Topic>` è la parola chiave dell'argomento e `<num>` è il numero di capitoli di teoria che trattano quell'argomento. La lista è ordinata per numero di capitoli decrescente, e in caso di parità, alfabeticamente per parola chiave.
-->

## R5: Compiti

Un compito (`Assignment`) rappresenta le risposte fornite per una domanda da uno studente.

Il metodo `newAssignment(String, ExerciseChapter)` crea un nuovo compito per lo studente la cui matricola è passata come parametro assieme al capitolo di esercizi. Restituisce un oggetto `Assignment` che fornisce i metodi getter per le due informazioni menzionate.

Il metodo `addResponse(Question, List<String>)` aggiunge le risposte dello studente alla domanda data: la lista contiene le risposte che lo studente considera corrette. Il metodo restistuisce il punteggio raggiunto dallo studente per le risposte a quella domanda; il punteggio è calcolato come `(N - FP - FN)/N` dove:

* `N` è il numero di risposte totali per la domande,
* `FP`  è il numero di risposte fornite dallo studente che non sono corrette per la domanda (falsi positivi),
* `FN`  è il numero di risposte corrette per la domanda che non sono state fornite dallo studente (falsi negativi).

> Per esempio, se consideriamo una domanda con le risposte `A` (errata), `B` (errata), `c` (corretta) e `D` (corretta); e una risposta dello studente che include le risposte `B` e `C`, abbiamo:`N=4`, `FP=1` (dato che `B` è nella risposta dello studente ma non è corretta), `FN=1` (dato che `D` è corretta ma non presente nelle risposte dello studente), allora il punteggio sarà `0.5`.

Il metodo  `totalScore()` restituisce la somma dei punteggi per le risposte alle singole domande.
