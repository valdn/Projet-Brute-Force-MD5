# M3101-MD5-BruteForce
Projet c++ de cassage d'empreinte MD5 en brute force.  
Propose par notre professeur.  
Programme par jeremy et valentin.

**Sommaire**

[TOC]

## Compilation et Execution
Pour utiliser ce programme il vous faudrat d'abord extraire et compiler la librairie cryptoPP. 
Il faudrat ensuite compiler separement multiThread.cpp et sequentiel.cpp en sachant que multithread necessite la librairie -lpthread.  
Le script bash compil.sh s'occupe de tout faire pour vous.

## Fonctionnement de la separation des lettres et des threads
Le cassage d'empreinte s'effectue par le test de toutes les combinaisons possibles pour obtenir une chaine md5.  
Pour faire cela nous utilisons une chaine de caracteres qui commencent par a et qui va etre incrementer a chaque iteration.  
L'incrementation de cette chaine de caracteres s'effectue a la fin de la chaine et remonte peu a peu ce qui donne par exemple:

- a
- b
- ...
- aa
- ab
- ac
- ...  

Pour optimiser la vitesse de cassage d'empreinte MD5, nous utilisons un nombre predefinis de thread qui peut etre 2, 4, 8 ou 26. Le nombre de thread modifie l'incrementation de la chaine de caracteres. Pour faciliter l'utilisation des threads, au lieu de decouper en fonction du nombre de threads, nous incrementons le premier caracteres par le nombre de threads. C'est a dire que pour 4 thread par exemple, on obtiendra :

|Numero Thread |Lettre initiale |Premieres lettres en incrementation|Lettre de fin|
|---|---|---|---|
|1|a|a -> e -> i -> m -> q -> u -> y|y|
|2|b|b -> f -> j -> n -> r -> v -> z|z|
|3|c|c -> g -> k -> o -> s -> w|w|
|4|d|d -> h -> l -> p -> t -> x|x|

## Algorithme sequentiel
### Fonction Main
![](https://raw.githubusercontent.com/Jeremy-JRM/M3101-MD5-BruteForce/master/algo/AlgoSequentielMain.png "Main function algo")

### Fonction incremente
![](https://raw.githubusercontent.com/Jeremy-JRM/M3101-MD5-BruteForce/master/algo/AlgoSequentielIncremente.png "Incremente function algo")

### Fonction reset
![](https://raw.githubusercontent.com/Jeremy-JRM/M3101-MD5-BruteForce/master/algo/AlgoSequentielReset.png "Reset function algo")


## Disposition des lettres
### Pour 2 threads
|Thread 1|Thread 2|
|---|---|
|aa|ba|
|ca|da|
|ea|fa|
|ga|ha|
|ia|ja|
|ka|la|
|ma|na|
|oa|pa|
|qa|ra|
|sa|ta|
|ua|va|
|wa|xa|
|ya|za|

### Pour 4 threads
|Thread 1|Thread 2|Thread 3|Thread 4|
|---|---|---|---|
|aa|ba|ca|da|
|ea|fa|ga|ha|
|ia|ja|ka|la|
|ma|na|oa|pa|
|qa|ra|sa|ta|
|ua|va|wa|xa|
|ya|za|

### Pour 8 threads
|Thread 1|Thread 2|Thread 3|Thread 4|Thread 5|Thread 6|Thread 7|Thread 8|
|---|---|---|---|---|---|---|---|
|aa|ba|ca|da|ea|fa|ga|ha|
|ia|ja|ka|la|ma|na|oa|pa|
|qa|ra|sa|ta|ua|va|wa|xa|
|ya|za|