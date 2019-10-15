#include "cryptlib.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "md5.h"
#include "hex.h"
#include "files.h"

#include <time.h>
#include <iostream>

typedef std::chrono::high_resolution_clock Clock;

int nbThread;
bool trouve = false;
std::string result = "farine"; //resultat voulu

void reset (std::string &s, char l) { //on reset le mot quand tous les threads sont arrivés a leur dernier mot possible

	s[0] = l;

	for (int i = 1; i < s.size(); i++)
		s[i] = 'a';
}

void hash (std::string &s, std::string &digest){
	using namespace CryptoPP;
	HexEncoder encoder(new FileSink(std::cout));
	Weak::MD5 hash;
	hash.Update((const byte*)s.data(), s.size());
	digest.resize(hash.DigestSize());
	hash.Final((byte*)&digest[0]);
}

void inc (std::string &s, std::string &verif, int i, char l) { //incrémentation du mot
	
	if (i < 0 || i > s.size()) {
		std::cerr << "ERREUR : indice impossible" << std::endl;
		return;
	}

	if (s == verif) {
		reset(s, l);
		s += 'a';
		verif += 'z';
	} else {
		if (i == 0) {  //incrémentation de la premiere lettre en fonction du nombre de thread
			s[i]+=(nbThread-1);
		}
	
		if (s[i] >= 'z') { //si la lettre est z (derniere lettre de l'alphabet)
			s[i] = 'a';   //on la reset a 'a'
			inc(s, verif, i-1, l);
		} else {
			s[i]++; //incrémentation des lettres
		}
	}
}

void * dechiffre (void * arg) { //fonction pour déchiffrer la chaine de caractere

	std::string * param = (std::string*)arg;

	std::string digestResult;

	std::string composition = param[0];
	std::string digestComposition;

	std::string endWord = param[1];

	hash(result, digestResult);
	hash(composition, digestComposition);

	while((digestComposition != digestResult) && (!trouve)) {
		inc(composition, endWord, composition.size() - 1, param[0][0]);
		
		hash(composition, digestComposition);
	}

	if (digestComposition == digestResult) {
		trouve = true;
	}

	return(NULL);
}

int main() {
	do {
	std::cout<< "Nombre de thread (2,4,8,26) > " <<std::endl;
	std::cin >> nbThread;                                         //nombre de thread voulu
	} while (nbThread!=2 && nbThread!=4 && nbThread!=8 && nbThread!=26);

	std::string ** tab = new std::string * [nbThread];

	for (int i = 0; i < nbThread; i++)
		tab[i] = new std::string[2];

	char lettre = 'a';
	for (int i = 0; i < nbThread; i++) { //remplissage des tableaux
		tab[i][0] = (lettre + i); //charactere de base du thread
		tab[i][1] = (lettre + (26 - (((25 - i)%nbThread)) - 1)); //charactere de fin du thread
	}


	auto t1 = Clock::now(); //initialisation du compteur de temps
	pthread_t tabTh[nbThread];

	for (int i = 0; i < nbThread; i++) {
		pthread_create(&tabTh[i], NULL, dechiffre, (void *)tab[i]); //création des threads
	}

	for (int i = 0; i < nbThread; i++) {
		pthread_join(tabTh[i], NULL);
	}

	auto t2 = Clock::now(); //fin du chrono
	float temps = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	temps = temps / 1000;
	std::cerr << "Temps: " << temps << " seconds" << std::endl;

  	return 0;
}
