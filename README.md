# Assignment 3

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

Launch Godot and open the file, `project.godot`, located in the `Assignment3/Project/` directory. Run the project!

**Note:** When opening the project in Godot, files will sometimes get modified unintentionally and may cause unintended behavior when running the project. Ensure that files are unmodified prior to running. If changes are made, restore the original files and reload changes in Godot:

```
git restore .
```

## Other

- [Video demo](https://www.youtube.com/watch?v=gef3XDT0EJc)
- [Milestone Report](Assignment%203%20Documentation.pdf)
