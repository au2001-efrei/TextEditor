# Hello, World!

Text editor written in the C programming language.\
Created as project for our studies in second year at **EFREI Paris**.\
By **Aurélien GARNIER** and **Rémi LEGRAND**.

## Setup

To use the program, you first need to install the library which we used to handle the command-line interface (CLI) and then compile the program.\
Here are the steps:
1. Install [ncurses](https://invisible-island.net/ncurses/)
2. Navigate to the projet's directory with `cd path/to/project/directory`
3. Compile the program with `gcc *.c -o main -lncurses`
4. Run the program with `./main`

## Usage

To start the program, you can simply type `./main`.\
If you want to instantly open a file, write its path as argument to the program (e.g. `./main README.md`).

Once the program starts, you are right away taken to the text editor.\
It means that you can start typing and the keys will appear on your screen.\
You can delete characters you just typed with the backspace key.

Just as a regular text editor, you can use the arrows on your keyboard to navigate across lines.\
Hit enter to create a new line wherever you'd like.

## Shortcuts

### Ctrl-Q

Quits the program.\
If you have unsaved changes, asks whether you'd like to save them, discard them or cancel and return to the editor.

### Ctrl-C & Ctrl-X

Respectively copies or cuts the selected line to your clipboard.\
You can only have one line in your clipboard, so beware that these shortcuts overwrite your previous clipboard.

### Ctrl-V

Pastes your clipboard to your cursor's location.

### Ctrl-D

Duplicates the selected line right below it.

### Ctrl-F & Ctrl-R

Opens a prompt at the bottom of your screen to search or replace text respectively.

The search function will have one field, the text to search. Hit enter to jump to the next occurrence of that string in your file.\
When there are no occurrences left in the file, jumps to the end of the file and displays a message briefly at the bottom of your screen.\
Use Ctrl-F again to start back the search at the beginning of the file.

The replace function will ask for a string to look for and another to replace the first with.\
When you hit enter, all occurrences of the first string will be replaced with the second.\
The number of replacements will be displayed briefly at the bottom of your screen.

### Ctrl-O

Shows a prompt to open a new file at the specified path.\
If you have unsaved changes, asks whether you'd like to save them, discard them or cancel and return to the editor.

### Ctrl-W

Closes the file you currently have open.\
If you have unsaved changes, asks whether you'd like to save them, discard them or cancel and return to the editor.

### Ctrl-S

Saves the modifications you have made to the file currently open.\
If you do not have any open file (you did not provide any while launching the program or you used the Ctrl-W shortcut to close it), shows a prompt at the bottom of your screen to enter the path at which to save the file.

### Ctrl-P

Reverses the selected line.

Example:
> Hello, World!

becomes
> !dlroW ,olleH
