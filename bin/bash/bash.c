/*
* This implements the bash for the OS
*
*/

# include <libc/malloc.h>
# include <bash.h>
# include <stdio.h> 
# include <defs.h>
# include <sys/tarfs.h>
# include <libc/dir.h>

#define BUFSIZE 20
#define TOKENSIZE 10


char *PATH = "/bin";	// Environment variable seprated by :
char *dummy = "bin/hello";	//dummy str for get char
char ipBuf[20][20];	// Array of pointers for the input commands (not parsed)
char *parseBuf;		// Pointer to parsed array of commands 
int dumi = 0;


/*
*
*/




/*
* Returns length of string EXCluding the \0 
*/

int strln(char *st)
{
	uint32_t i = 0;

	while ((*st != '\0') && (st != NULL))
	{
		++st;
		++i;
	}

	return (i);
}

/*
* Copy one string into another - strcpy
*/

int strcp(char *src, char *dst, uint32_t len)
{
	uint32_t i = 0;

	if (src == NULL || dst == NULL)
	{
		u_printf("NULL src ot dst in strcp");
		return -1;
	}

	while(i < len)
	{
		*dst++ = *src++;
		++i;
	}	

	return 0;
}


/*
* Zero out any memory location till the len provided
*/

void clear(char *pt, uint32_t len)
{
	uint32_t i = 0;

	while(i < len)
	{
		pt[i] = 0;
		++i;
	}
}

/*
* Parses and tokenize the input string which users inputs from shell
*/

int parse_ip(char *str)
{
	int i = 0, j = 0,k=0;		// i is index in ipBuf , j is index in tmp1
  //char tempBuf[20][20];
	while (str[j] != '\0')
	{
    k = 0;
		while(str[j] != ' ' && str[j] != '\0')		// Get the token and save in ipBuf
		{
      ipBuf[i][k] = str[j];
      k++;
      j++;
		}
    ipBuf[i][k] = '\0';
    i++;
    j++;
	}	

	return i; 
}

/* dummy getchar() */
char getchar()
{
	return (dummy[dumi++]);		
}

int main()			// Process 0
{
//	uint32_t i = 0;
//	char c;
	char *tmp = (char *) malloc(sizeof(char) * BUFSIZE);			// tmp buffer for shell
	char *cur_PATH = (char *) malloc(sizeof(char) * 100);			// Will show current PATH (CWD)
	int ret  = -1;
  int num = 0;
	
	strcp(PATH, cur_PATH, strln(PATH));					//copying PATH to current PATH
//	clear_screen();								// Kernel function should be a system call
	
	while(1)								// First process will never terminate 
	{
	  u_printf("\n[MY-SHELL: ]");
		//strcp(dummy, tmp, strln(dummy));
		u_scanf("%s", (uint64_t)tmp);
		u_printf("AFTER%s::%d",(uint64_t)tmp, tmp[0]);
		switch(tmp[0]) 
		{
			case '\0':						// User pressing only enter
				u_printf("\n[MY-SHELL: ]");
				break;
			default:	
				u_printf("in ELSE");
        num--;
				num = parse_ip(tmp);				// parses the input when user hits Enter

        if(open(tmp) == -1)
        {
          u_printf("\nThe given command not found\n");
          break;
        }
        u_printf("Executing %s",ipBuf[0]);
				ret  = fork();
				if (!ret)
				{
					u_printf("In child");
 //         while(1);
//					while(1);
					execve(ipBuf[0]);
				}
				else{
					u_printf("In parent");
					yield();
				}
				break;

		}
	}
	return 0;
}	
