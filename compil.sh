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
			rm -R build/;
		fi
		if [ -e info ]
		then
			rm -R info/;
		fi
		if [ -e cpuinfo.txt ]
		then
			rm cpuinfo.txt;
		fi
		if [ -e meminfo.txt ]
		then
			rm meminfo.txt;
		fi
		if [ -e cacheinfo.txt ]
		then
			rm cacheinfo.txt;
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
	echo "Compilation singleThread.cpp reussie";

	g++ -DNDEBUG -g2 -O2 -I ${lib} src/maxThread.cpp -o build/maxThread.exe ${lib}/libcryptopp.a -lpthread;
	echo "Compilation maxThread.cpp reussie";
	echo;
	
	echo "Execution des tests de temps";

	echo "Mot: za" > temps.log;
	echo "=========" >> temps.log;

	./build/singleThread.exe za >> temps.log;
	./build/maxThread.exe 2 za >> temps.log;
	./build/maxThread.exe 4 za >> temps.log;
	./build/maxThread.exe 8 za >> temps.log;
	./build/maxThread.exe 26 za >> temps.log;
	echo "  Mot de 2 caracteres";


	echo >> temps.log;
	echo "Mot: zza" >> temps.log;
	echo "=========" >> temps.log;

	./build/singleThread.exe zza >> temps.log;
	./build/maxThread.exe 2 zza >> temps.log;
	./build/maxThread.exe 4 zza >> temps.log;
	./build/maxThread.exe 8 zza >> temps.log;
	./build/maxThread.exe 26 zza >> temps.log;
	echo "  Mot de 3 caracteres";

	
	echo >> temps.log;
	echo "Mot: zzza" >> temps.log;
	echo "=========" >> temps.log;

	./build/singleThread.exe zzza >> temps.log;
	./build/maxThread.exe 2 zzza >> temps.log;
	./build/maxThread.exe 4 zzza >> temps.log;
	./build/maxThread.exe 8 zzza >> temps.log;
	./build/maxThread.exe 26 zzza >> temps.log;
	echo "  Mot de 4 caracteres";


	echo >> temps.log;
	echo "Mot: zzzza" >> temps.log;
	echo "=========" >> temps.log;

	./build/singleThread.exe zzzza >> temps.log;
	./build/maxThread.exe 2 zzzza >> temps.log;
	./build/maxThread.exe 4 zzzza >> temps.log;
	./build/maxThread.exe 8 zzzza >> temps.log;
	./build/maxThread.exe 26 zzzza >> temps.log;
	echo "  Mot de 5 caracteres";


	echo >> temps.log;
	echo "Mot: zzzzza" >> temps.log;
	echo "=========" >> temps.log;

	./build/singleThread.exe zzzzza >> temps.log;
	./build/maxThread.exe 2 zzzzza >> temps.log;
	./build/maxThread.exe 4 zzzzza >> temps.log;
	./build/maxThread.exe 8 zzzzza >> temps.log;
	./build/maxThread.exe 26 zzzzza >> temps.log;
	echo "  Mot de 6 caracteres";

	echo;
	echo "Informations du cpu et memoire dans les fichiers:";
	echo "  info/cpuinfo.txt";
	echo "  info/meminfo.txt";
	echo "  info/cacheinfo.txt";
	
	if [ ! -e "info" ]
	then
		mkdir info;
	fi

	cat /proc/cpuinfo > cpuinfo.txt;
	cat /proc/meminfo > meminfo.txt;
	sudo lshw -C memory > cacheinfo.txt;
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
