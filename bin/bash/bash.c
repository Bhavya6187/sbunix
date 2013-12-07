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
char *ipBuf[TOKENSIZE];	// Array of pointers for the input commands (not parsed)
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
	char *pt = str;
	uint32_t i = 0, j = 0;		// i is index in ipBuf , j is index in tmp1
	char tmp1[BUFSIZE];

	while (*pt != '\0')
	{
		if ( i == TOKENSIZE && (*pt != '\0'))
		{
			u_printf("\n Input too long. Cant tokenize");
			return -1;
		}
		
		if (*pt == ' ')		// Get the token and save in ipBuf
		{
			if (ipBuf[i] == NULL)	// If entry is free
			{
				ipBuf[i] = (char *) malloc(sizeof(char) * 10);
			}
			else	// if entry not empty clear it before copying anything into it
			{
				clear(ipBuf[i], 10);
			}
			strcp(tmp1, ipBuf[i], strln(tmp1));		// Copying the found token in ipBuf
			*(ipBuf[i] + (strln(tmp1))+1) = '\0';		// EOS
			clear(tmp1, BUFSIZE);				// Clearing the next token
			++i;
			++pt;
		}
		else		// copy *pt straight into tmp1
		{
			tmp1[j] = *pt;				
			++j;
			++pt;
		}
	}	
	ipBuf[i] = (char *) malloc(sizeof(char) * 20);		// Case for last token or when the user inout has no spaces
	strcp(tmp1, ipBuf[i], strln(tmp1));
	*(ipBuf[i] + (strln(tmp1) + 1)) = '\0';		// EOS
	clear(tmp1, BUFSIZE);				// Clearing the next token

	return 0; 
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
	char *cmd = (char *) malloc(sizeof(char) * BUFSIZE);			// Will hold the final command minus arguments if any
	char *cur_PATH = (char *) malloc(sizeof(char) * 100);			// Will show current PATH (CWD)
	int ret  = -1;
	
	strcp(PATH, cur_PATH, strln(PATH));					//copying PATH to current PATH
	clear(tmp, BUFSIZE);							// Zero out new memory allocalted
//	clear_screen();								// Kernel function should be a system call
	
	u_printf("\n[MY-SHELL: ]");
	while(1)								// First process will never terminate 
	{
		//strcp(dummy, tmp, strln(dummy));
		u_scanf("%s", (uint64_t)tmp);
//		u_printf("AFTER%s::%d",(uint64_t)tmp, tmp[0]);
		switch(tmp[0]) 
		{
			case '\0':						// User pressing only enter
				u_printf("\n[MY-SHELL: ]");
				break;
			default:	
				u_printf("in ELSE");
				parse_ip(tmp);				// parses the input when user hits Enter
				strcp(ipBuf[0], cmd, strln(ipBuf[0]));
				*(cmd + (strln(ipBuf[0]))+1) = '\0';	
				u_printf("cmd is:%s", cmd);
        if(open(tmp) == -1)
        {
          u_printf("\nThe given command not found\n");
          break;
        }
        u_printf("Executing %s",tmp);
				ret  = fork();
				if (!ret)
				{
					u_printf("In child");
 //         while(1);
//					while(1);
					execve(tmp);
				}
				else{
					u_printf("In parent");
					yield();
				}
				clear(tmp, BUFSIZE);				// clear tmp to prepare for new user input
				break;

		}
		clear(tmp, BUFSIZE);				// clear tmp to prepare for new user input
		clear(cmd, BUFSIZE);
	}
	return 0;
}	
