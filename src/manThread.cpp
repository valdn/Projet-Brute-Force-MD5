#include "cryptlib.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "md5.h"
#include "hex.h"
#include "files.h"

#include <time.h>
#include <iostream>

typedef std::chrono::high_resolution_clock Clock;

bool trouve = false;
int nbThread;

void reset (std::string &s, char l) {

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

void inc (std::string &s, std::string &verif, int i, char l) {
	
	if (i < 0 || i > s.size()) {
		std::cerr << "ERREUR : indice impossible" << std::endl;
		return;
	}

	if (s == verif) {
		reset(s, l);
		s += 'a';
		verif += 'z';
	} else {
		if (i == 0) {
			s[i]+= (nbThread-1);
		}	
	
		if (s[i] >= 'z') {
			s[i] = 'a';
			inc(s, verif, i-1, l);
		} else {
			s[i]++;
		}
	}
}

void * dechiffre (void * arg) {

	std::string * param = (std::string*)arg;

	std::string result = param[0];
	std::string digestResult;

	std::string composition = param[1];
	std::string digestComposition;

	std::string endWord = param[2];

	hash(result, digestResult);
	hash(composition, digestComposition);

	while((digestComposition != digestResult) && (!trouve)) {
		inc(composition, endWord, composition.size() - 1, param[1][0]);
		
		hash(composition, digestComposition);
	}

	if (digestComposition == digestResult) {
		trouve = true;
		std::cerr << composition << "|" << result << std::endl;
	}

	return(NULL);
}

int main() {

	std::string result = "zzzza";
	std::cin >> nbThread;

	auto t1 = Clock::now();

	if (nbThread == 2) {
		std::string tab1[3]; tab1[0] = result; tab1[1] = "a"; tab1[2] = "y";
		std::string tab2[3]; tab2[0] = result; tab2[1] = "b"; tab2[2] = "z";

		pthread_t th1;
		pthread_t th2;

		pthread_create(&th1, NULL, dechiffre, (void *)tab1);
		pthread_create(&th2, NULL, dechiffre, (void *)tab2);
		
		pthread_join(th1, NULL);
		pthread_join(th2, NULL);
	}

	if (nbThread == 4) {
		std::string tab1[3]; tab1[0] = result; tab1[1] = "a"; tab1[2] = "y";
		std::string tab2[3]; tab2[0] = result; tab2[1] = "b"; tab2[2] = "z";
		std::string tab3[3]; tab3[0] = result; tab3[1] = "c"; tab3[2] = "w";
		std::string tab4[3]; tab4[0] = result; tab4[1] = "d"; tab4[2] = "x";

		pthread_t th1;
		pthread_t th2;
		pthread_t th3;
		pthread_t th4;

		pthread_create(&th1, NULL, dechiffre, (void *)tab1);
		pthread_create(&th2, NULL, dechiffre, (void *)tab2);
		pthread_create(&th3, NULL, dechiffre, (void *)tab3);
		pthread_create(&th4, NULL, dechiffre, (void *)tab4);
		
		pthread_join(th1, NULL);
		pthread_join(th2, NULL);
		pthread_join(th3, NULL);
		pthread_join(th4, NULL);
	}

	if (nbThread == 8) {
		std::string tab1[3]; tab1[0] = result; tab1[1] = "a"; tab1[2] = "y";
		std::string tab2[3]; tab2[0] = result; tab2[1] = "b"; tab2[2] = "z";
		std::string tab3[3]; tab3[0] = result; tab3[1] = "c"; tab3[2] = "s";
		std::string tab4[3]; tab4[0] = result; tab4[1] = "d"; tab4[2] = "t";
		std::string tab5[3]; tab5[0] = result; tab5[1] = "e"; tab5[2] = "u";
		std::string tab6[3]; tab6[0] = result; tab6[1] = "f"; tab6[2] = "v";
		std::string tab7[3]; tab7[0] = result; tab7[1] = "g"; tab7[2] = "w";
		std::string tab8[3]; tab8[0] = result; tab8[1] = "h"; tab8[2] = "x";

		pthread_t th1;
		pthread_t th2;
		pthread_t th3;
		pthread_t th4;
		pthread_t th5;
		pthread_t th6;
		pthread_t th7;
		pthread_t th8;

		pthread_create(&th1, NULL, dechiffre, (void *)tab1);
		pthread_create(&th2, NULL, dechiffre, (void *)tab2);
		pthread_create(&th3, NULL, dechiffre, (void *)tab3);
		pthread_create(&th4, NULL, dechiffre, (void *)tab4);
		pthread_create(&th5, NULL, dechiffre, (void *)tab5);
		pthread_create(&th6, NULL, dechiffre, (void *)tab6);
		pthread_create(&th7, NULL, dechiffre, (void *)tab7);
		pthread_create(&th8, NULL, dechiffre, (void *)tab8);
		
		pthread_join(th1, NULL);
		pthread_join(th2, NULL);
		pthread_join(th3, NULL);
		pthread_join(th4, NULL);
		pthread_join(th5, NULL);
		pthread_join(th6, NULL);
		pthread_join(th7, NULL);
		pthread_join(th8, NULL);
	}

	auto t2 = Clock::now();
	float temps = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	temps = temps / 1000;
	std::cerr << "Temps: " << temps << " seconds" << std::endl;

  	return 0;
}
