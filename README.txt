To run the Server:

1. Open a LCPU terminal in the directory on the CW3-ks847 folder
2. type "cd jni"
3. then type "gcc -fPIC -I /usr/lib/jvm/java-1.6.0-openjdk-1.6.0.38.x86_64/include 
-I /usr/lib/jvm/java-1.6.0-openjdk-1.6.0.38.x86_64/include/linux -c GameLogic.c -o GameLogic.o" 
4. type "gcc -fPIC  -c Map.c -o Map.o"
5. type "gcc -fPIC -Wl -shared -o liblogic.so GameLogic.o Map.o"
6. type "javac Server.java"
7. type "java Server"
   - Then the Server will start.
  
To run a Client:

1. Repeat 1-5 steps from running a Server 
2. type "javac PlayGame.java"
3  type "java PlayGame"
   - Then a Client will start.

