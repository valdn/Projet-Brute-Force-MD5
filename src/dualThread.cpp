#include "cryptlib.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "md5.h"
#include "hex.h"
#include "files.h"

#include <time.h>
#include <iostream>

bool trouve = false;

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
			s[i]++;
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

	std::string composition = param[2];
	std::string digestComposition;

	std::string endWord = param[4];

	hash(result, digestResult);
	hash(composition, digestComposition);

	while((digestComposition != digestResult) && (!trouve)) {
		//std::cerr << param[2] << std::endl;
		inc(composition, endWord, composition.size() - 1, param[5][0]);
		
		hash(composition, digestComposition);
	}

	if (digestComposition == digestResult) {
		trouve = true;
		std::cerr << result << " | " << composition << std::endl; 
	}

	return(NULL);
}

int main() {
	clock_t t1=clock();

	std::string * param = new std::string[6];
	std::string * param2 = new std::string[6];

	param[0] = "zzza";		// Mot voulant etre obtenu
	param[1]; 				// Digest du mot voulante etre obtenu
	param[2] = "a"; 		// Lettre depart thread1
	param[3]; 				// Digest mot depart thread1
	param[4] = "y";
	param[5] = 'a';

	param2[0] = "zzza";	// Mot voulant etre obtenu
	param2[1];
	param2[2] = "b";
	param2[3];
	param2[4] = "z"; 		// Verification mot de fin thread1
	param2[5] = 'b';

	pthread_t th1;
	pthread_t th2;

	pthread_create(&th1, NULL, dechiffre, (void *)param);
	pthread_create(&th2, NULL, dechiffre, (void *)param2);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	if((param[2].size() && param[4].size()) > param[0].size())
		std::cerr << "Salut mon pote" << std::endl;
	else {
		clock_t t2 = clock();
		float temps = (float)(t2-t1)/CLOCKS_PER_SEC;
		std::cout << temps << " s"<< std::endl;
	}

  	return 0;
}
