/*
* This implements the bash for the OS
*
*/

# include <syscall.h>
# include <libc/malloc.h>
# include <bash.h>
# include <stdio.h> 
# include <defs.h>

#define BUFSIZE 20
#define TOKENSIZE 10

char *PATH = "/bin";	// Environment variable seprated by :
char *dummy = "\n\nbin/hello\n";	//dummy str for get char
char *ipBuf[TOKENSIZE];	// Array of pointers for the input commands (not parsed)
char *parseBuf;		// Pointer to parsed array of commands 
int dumi = 0;

//Returns length of string EXCluding the \0 

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

// Copy one string into another - strcpy
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

// Zero out any memory location till the len provided
void clear(char *pt, uint32_t len)
{
	uint32_t i = 0;
	while(i < len)
	{
		pt[i] = 0;
		++i;
	}
}

// Parses and tokenize the input string which users inputs from shell
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
			*(ipBuf[i] + (strln(tmp1) + 1)) = '\0';		// EOS
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
	uint32_t i = 0;
	char c;
	char *tmp = (char *) malloc(sizeof(char) * BUFSIZE);			// tmp buffer for shell
	char *cmd = (char *) malloc(sizeof(char) * BUFSIZE);			// Will hold the final command minus arguments if any
	int ret  = -1;

	clear(tmp, BUFSIZE);							// Zero out new memory allocalted
//	clear_screen();								// Kernel function should be a system call
	
	u_printf("\n[MY-SHELL: ]");
	while(1)								// First process will never terminate 
	{
		c = getchar();							// reads user typed input from stdio	how ??

		switch(c) 
		{
			case '\n':						// User pressing only enter
				if (tmp[0] == 0)
				{
					u_printf("\n [MY-SHELL: ]");
				}
				else 
				{
					parse_ip(tmp);				// parses the input when user hits Enter
					strcp(ipBuf[0], cmd, strln(ipBuf[0]));
					*(cmd + (strln(ipBuf[0]) + 1 )) = '\0';	

					/* check for the command in the /bin dir -- need dir calls for this - for now just test with a hello binary */
					// If cmd found call exceve with cmd binary
					// else u_printf("\n [MY-SHELL: ]");
					
					ret  = fork();
					if (!ret)
					{
						u_printf("In child");
            char filename[10] = "bin/hello\0";
            ret = __syscall1(SYSCALL_EXECVE,(uint64_t)(filename));
            while(1)
              u_printf("lalala\n");
						//execve(cmd, NULL, NULL);
					}
					else{
						u_printf("In parent");
            while(1);
						u_printf("In parent");
						//yield();
					}
				}	
				clear(tmp, BUFSIZE);				// clear tmp to prepare for new user input
				break;

			default:						// Just copy user input into tmp buffer
				tmp[i] = c;
				++i;
				break;
		}
		clear(cmd, BUFSIZE);
	}
	return 0;
}	
