# Автоматизація процесу для Linux

all: app

# 1. Патчер
patch_so: patch_so.cpp
	g++ patch_so.cpp -o patch_so

# 2. Компілюємо бібліотеку .so
libmylib.so: mylib.cpp patch_so
	g++ -shared -fPIC mylib.cpp -o libmylib.so
	./patch_so libmylib.so

# 3. Збираємо основний додаток
app: main.cpp libmylib.so
	g++ main.cpp -o app -ldl

clean:
	rm -f patch_so libmylib.so app