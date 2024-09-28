Building The Project and it's dependencies

As you can see in the build.bat file
- we use cl.exe directly for building and compiling the project in Debug and Release modes
- we add related lib files from OS and pre-requisites
- for running the project we should run this command: (for having an output) (for now I am using VSCode task to run the program)
 $ project\bin\Rend.exe > project\output\render.ppm
- for debugging you should run this command:
 $ devenv.exe project\bin\Rend.exe
 and then in visual studio save the sln file and hit F11
