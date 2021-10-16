// Ref: https://stackoverflow.com/a/912796
char getch() 
{
    char buf = 0;
    struct termios old;
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
    return (buf);
}

int8_t get_arrow_input()
{
        // up:      27 91 65 10 (= ESC [value] ENTER)
        // left:    27 91 68 10 
        // down:    27 91 66 10 
        // right:   27 91 67 10 

        if (getch() != 27)
        {
                printf("exit 1\n");
                return 0;
        }

        if (getch() != 91)
        {
                printf("exit 2\n");
                return 0;
        }

        switch(getch()) 
        {
                case 68: { printf("got left \n"); return -1; }
                case 67: { printf("got right \n"); return 1; }
                default: { printf("exit 3\n"); return 0; }
        }
}