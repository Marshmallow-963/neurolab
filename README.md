# 🧠 Neurolab

**Neurolab** is software developed for educational purposes, focused on simulating computational neuroscience concepts.

---

## ⚠️ Important Warning Regarding Execution

This project **is not a program that you install on your system** (Linux, Windows ou macOS).

The executable file ('neurolab') is saved **inside `bin/` folder** within the project's own directory. You will not find it in `/usr/bin/` or in "Program Files".

---

## 🚀 How Compile and Run

To use the program, you must first compile it and then run it from the project's root folder.

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/Marshmallow-963/neurolab.git](https://github.com/Marshmallow-963/neurolab.git)
    cd neurolab
    ```

2.  **Compile the project:**
    The raylib and raygui files are already included in the project. You only need to have the `make` and `gcc` builds tools, and the base libraries that raylib depends on (such as X11, pthread, etc., which most Linux distributions already have or include in packages like `build-essential` or `libx11-dev`).

    ```bash
    make
    ```

3.  **Run the program:**
    After compilation, the executable will be available in the `bin/` folder. To run it, use the following command from the root folder (`neurolab/`):
    ```bash
    ./bin/neurolab
    ```

---

## 🎓 Authorship and Academic Context

This software was developed by an Undergraduate Research (IC) student from the **Computational Neuroscience Group** at the **nstitute of Physics (IF)** of the **Federal University of Alagoas (Ufal)**.

The project was carried out under the supervision of Prof. Dr. Fernada Selingrad.

