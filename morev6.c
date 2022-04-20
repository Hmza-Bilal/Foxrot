/* 
*  Video Lecture 11
*  morev6.c: Enter is not Displayed on terminal
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define	PAGELEN	30
#define	LINELEN	512

void do_more(FILE *);
int  get_input(FILE*);

int percentage;	
double resolution;
struct termios prev,curr;
char getch();

int main(int argc , char *argv[])
{
   int i=0;
   if (argc == 1){
      do_more(stdin);
   }
   FILE * fp;
   while(++i < argc){
      fp = fopen(argv[i] , "r");
      if (fp == NULL){
         perror("Can't open file");
         exit (1);
      }
      fseek(fp,0,SEEK_END);
      resolution = ftell(fp);
      fseek(fp,0,0);
      do_more(fp);
      fclose(fp);
   }  
   return 0;
}
int get_percent(FILE *fp){
	fseek(fp,0,SEEK_CUR);
	double position = ftell(fp);
	return (position/resolution)*100;	
}

void do_more(FILE *fp)
{
   int num_of_lines = 0;
   int count = 0;
   int rv;
   char buffer[LINELEN];
   FILE* fp_tty = fopen("/dev//tty", "r");
   while (fgets(buffer, LINELEN, fp)){
      fputs(buffer, stdout);
      percentage = get_percent(fp);
      num_of_lines++;
      count++;
      
      if (num_of_lines == PAGELEN){
         rv = get_input(fp_tty);		
         if (rv == 0){//user pressed q
            printf("\033[1A \033[2K \033[1G");
            break;//
         }
         else if (rv == 1){//user pressed space bar
            printf("\033[1A \033[2K \033[1G");
            num_of_lines -= PAGELEN;
         }
         else if (rv == 2){//user pressed return/enter
            printf("\033[1A \033[2K \033[1G");
	         num_of_lines -= 1; //show one more line
            }
         else if (rv == 3){ //invalid character
            printf("\033[1A \033[2K \033[1G");
            break; 
         }
      }
  }
}



int get_input(FILE* cmdstream)
{
   int c;
   printf("\033[7m --more--(%d%%) \033[m", percentage);
     c = getch();
      if(c == 'q')
	 return 0;
      if ( c == ' ' )			
	 return 1;
      if ( c == '\n' )	
	 return 2;	
      return 3;
   return 0;
}

void initialize_term(){
	tcgetattr(0,&prev);
	curr = prev;
	curr.c_lflag &= ~ICANON;
	curr.c_lflag &= ~ECHO;
	tcsetattr(0,TCSANOW,&curr);
	}
	
void reset_term(){
	tcsetattr(0,TCSANOW,&prev);
	}	
	
char getch(){
	char ch;
	initialize_term();
	 ch = getchar();
	 reset_term();
	 return ch;
}	
