# linkedList
A C++ implementation of a linked list with some basic linked list related methods. The C++ code also uses a slave python program to sort the linked list
Information is exchanged using text files and messages are exchanged via sockets. The C++ program listens to port 5000 on the localhost for a message by the python program. This message notifies the C++ program that the list it wrote on the text file "arrayInput.txt" is sorted and written to "arrayOutput.txt".
After getting this message the C++ program continues execution and proceses this new sorted list and puts it in the place of the un-sorted list it had. 
Threads are used in C++. One thread runs the python program and the other listens to the post 5000 for the message by the python program telling when the array is sorted and ready for it to process. 
Sockets in C++ are defined in ./rot13server_cpp/rot13server.cpp courtesy of Thomas Bleeker in his Winsock networking tutorial found here https://www.madwizard.org/programming/tutorials/netcpp/7
Compile the .cpp using the -lwsock32 flag like shown here: g++ linkedList.cpp -o linkedList.exe -lwsock32
A sample test code is written in main()