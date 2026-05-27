Secure password generator, 100% written in C language. Only for Linux/BSD systems.

# How it works

This password generator is based on `getrandom()` syscall from `sys/random.h` header file which is usually present only on Linux/BSD systems (Mac too, but its support isnt present in this version). `getrandom()` allows user/developer to generate cryptographically secure random numbers.    

Generator consists of 2 configuration macros at the start of `genpass.c` file: `WORDS` and `MAX_WORD_LEN`.  
`WORDS` macro defines amount of words you want to be present in your generated password.  
`MAX_WORD_LEN` macro defines max amount of characters you want each word to contain in your password.    

More precise info about this program you can obtain by analysing `getpass.c`.

# How to use it
1. Clone this repository somewhere on your disk or download `genpass.c` file.
2. Configure `WORDS` and `MAX_WORD_LEN` macros in `genpass.c` based on your needs using your code editor (by default `WORDS` is 10 and `MAX_WORD_LEN` is 64).
3. Compile `genpass.c` by yourself with `gcc genpass.c -o generator` in your terminal (everytime you change macros at the start of `genpass.c` you would have to compile it again.).
4. Create file named `words.txt` in the same directory as freshly built `generator` file (example of `words.txt` is present in this repository).
`words.txt` should contain words separated with the space, tab or new line symbols (unwanted space, tab and new line symbols that go in a row are not processed).  

`words.txt` Example 1 (words separated using new line symbols):
```
word1
word2
word3
...
```        
`words.txt` Example 2 (separated using space symbols):
```
word1 word2 word3 ...
```
(you can have any amount of spaces, tabs or new line symbols between words as they are not gonna be included in the password)

5. Run `generator`, that you've recently built at step 2, with `./generator` in your terminal.
You should get output like this (based on your `words.txt` and configuration): `randomword1-randomword2-randomword3-...-randomwordN`. This is your freshly generated password.

#
Please leave a star if you like my first program **<3**    

EOF

