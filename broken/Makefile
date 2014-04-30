all: shell subprocess

shell: shell.c
	gcc -ggdb shell.c -o shell

subprocess: subprocess.c
	gcc -ggdb subprocess.c -o subprocess

clean:
	-rm shell subprocess
