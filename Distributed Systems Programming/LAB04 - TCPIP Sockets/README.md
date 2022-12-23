# How to run...
Some notes to remember...

All the source code is stored in the `src` folder. The executables can be found in `bin`.

### How to run the Server
Starting from the `.java` file, execute the following commands:
1. `javac *.java` to compile all the `.java` files into `.class` files
2. Create the `MANIFEST.MF` file containing the following line (it indicates where the `main` method is defined):
> Main-Class: Server
3. Create the `jar` file with the command `jar -cfm Server.jar MANIFEST.MF *.class`
4. Run the server with `java -jar Server.jar`

### How to run the Client

Starting from the `.java` file, execute the following commands:
1. `javac *.java` to compile all the `.java` files into `.class` files
2. Create the `MANIFEST.MF` file containing the following line (it indicates where the `main` method is defined):
> Main-Class: Client
3. Create the `jar` file with the command `jar -cfm Client.jar MANIFEST.MF *.class`
4. Run the client with `java -jar Client.jar <inputMediaType> <outputMediaType> <imagePathString>`