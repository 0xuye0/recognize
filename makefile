recognize : statistics.o SmsCal.o result.o SmsExt.o code.o config.o RFile.o BacDel.o
	g++ -o recognize -g main.cpp statistics.o SmsCal.o result.o SmsExt.o code.o config.o RFile.o BacDel.o
statistics.o : statistics.cpp result.h statistics.h
	g++ -c -g statistics.cpp
SmsCal.o : SmsCal.cpp SmsCal.h
	g++ -c -g SmsCal.cpp
result.o : result.cpp SmsCal.h
	g++ -c -g result.cpp
SmsExt.o : SmsExt.cpp SmsExt.h
	g++ -c -g SmsExt.cpp
code.o : code.cpp head.h
	g++ -c -g code.cpp
RFile.o : RFile.cpp RFile.h
	g++ -c -g RFile.cpp
config.o : config.cpp config.h
	g++ -c -g config.cpp
BacDel.o : BacDel.cpp BacDel.h
	g++ -c -g BacDel.cpp
clean : 
	rm recognize statistics.o SmsCal.o result.o SmsExt.o code.o config.o BacDel.o RFile.o
