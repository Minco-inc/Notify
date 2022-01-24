Target = notify

$(Target): notify.o jsoncpp.o
	g++ -o notify notify.o jsoncpp.o -lcurl

notify.o:
	g++ -c -o notify.o notify.cpp

jsoncpp.o:
	g++ -c -o jsoncpp.o json/jsoncpp.cpp

clean:
	rm -f *.o
	rm -f $(Target)

install:
	bash ./bash/install

help:
	bash ./bash/help
