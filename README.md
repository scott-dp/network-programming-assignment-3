The assignment:
![image](https://github.com/user-attachments/assets/9f1e6b8b-1d9c-4d01-97d1-fce88c815440)

compile on windows: 
g++ main.cpp -o WebServer -D_WIN32_WINNT=0x0A00 -std=c++20 -lws2_32 -lmswsock
The cmake file is configured correctly also if you run the project through an IDE
You may need to download asio library

compile on unix:
g++ main.cpp -o WebServer -std=c++20 -pthread
The cmake file will not work for unix
You may need to download asio library
