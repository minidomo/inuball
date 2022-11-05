# Assignment 4

## Building on Lab Machines

1. Clone the repository and enter the directory

```
git clone https://gitlab.com/minidomo/cs354r-a2.git
cd cs354r-a2
```

2. Build the Godot bindings for C++ 

Run the following command from the root directory:

```
make bindings-lab
```

2. Build the plugin

Run the following command from the root directory:

```
make plugin-lab
```

3. Running the project

Launch Godot and open both `Assignment/Server/project.godot` and `Assignment/Client/project.godot`. Run the projects! On the client, connect to localhost (default), then run single player.

**Note:** When opening the project in Godot, files will sometimes get modified unintentionally and may cause unintended behavior when running the project. Ensure that files are unmodified prior to running. If changes are made, restore the original files and reload changes in Godot:

```
git restore .
```

## Other

- [User Manual](/Assignment%204%20User%20Manual.pdf)
- [Assignment Report](/Assignment%204%20Report.pdf)
- [Video demo](https://youtu.be/9uF7_9L10O0)
