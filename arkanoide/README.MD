# Arkanoide
Jogo arkanoite implementado com openGL, GLFW e GLAD

## Para rodar

### Instalar as dependências 
(Debian)
sudo apt-get install libglfw3 libgl1-mesa-dev libx11-dev libpthread-stubs0-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev

(RHEL)
sudo yum install glfw-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXinerama-devel libXcursor-devel

### Copilar e rodar
g++ -I . -I objects/block/ -I objects/ball/ -I objects/paddle/ -o main main.cpp objects/block/Block.cpp objects/paddle/Paddle.cpp objects/ball/Ball.cpp glad/glad.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
./main.cpp