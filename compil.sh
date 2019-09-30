#!/bin/bash

lib="lib/CryptoPP";

if [ $# -eq 1 ]
then
	if [ $1 == "clean" ]
	then
		if [ -e ${lib} ]
		then
			rm -R ${lib};
		fi
		if [ -e build ]
		then
			rm build/*;
		fi
	fi
else
	echo "Recherche de la librarie...";

	if [ -e "${lib}/libcryptopp.a" ]
	then
		echo "Librairie deja compilee";
	else
		echo "Extraction de la librairie...";
		mkdir ${lib};
		unzip lib/cryptopp820.zip -d ${lib};
		echo "Fin de l'extraction";
		echo "Compilation de la librairie";
		cd ${lib};
		make;
		rm *.o;
		rm *.exe;
		cd ../..;
		echo "Librairie compilee";
	fi

	echo "Compilation du programme";

	if [ ! -e "build" ]
	then
		mkdir build;
	fi

	g++ -DNDEBUG -g2 -O2 -I ${lib} src/singleThread.cpp -o build/singleThread.exe ${lib}/libcryptopp.a;
	g++ -DNDEBUG -g2 -O2 -I ${lib} src/dualThread.cpp -o build/dualThread.exe ${lib}/libcryptopp.a;
	g++ -DNDEBUG -g2 -O2 -I ${lib} src/quadThread.cpp -o build/quadThread.exe ${lib}/libcryptopp.a;
fi

#Verifier le nombre d'arguments

#Si 0 arg
	#Si libcryptopp.a existe
		#Afficher que la librairie existe et est compilee
	#Sinon
		#Demande d'extraire la librairie
		#Si Oui
			#Extraire
		#FinSi
		#Demande de compilee la librairie
		#Si Oui
			#Compiler
		#FinSi
	#FinSI

	#Compiler le programme
#Sinon si 1 arg
	#Si arg = clean
		#Demande d'accepter
		#Si Oui
			#Supprimer la librairie extraite et compilee
		#FinSi
	#FinSi
#FinSi