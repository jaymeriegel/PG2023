# Lista 1
Cada exercício númerado tem todas suas questões. Por exemplo, o exercício 5 tem o a,b,c,d basta descomentar na função main o exercício que quiser desenhar na tela.

## Para rodar

### Instalar as dependências 
(Debian)
sudo apt-get install libglfw3 libgl1-mesa-dev libx11-dev libpthread-stubs0-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev

(RHEL)
sudo yum install glfw-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXinerama-devel libXcursor-devel

### Copilar e rodar
g++ -I . -o output ex9_main.cpp glad/glad.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
./output