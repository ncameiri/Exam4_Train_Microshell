#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//./a.out "/bin/ls" "|" "/bin/grep" shell
typedef struct t_data
{
    char **args;
    char *exee;
    int piped;
    int in;
    int out;

}s_data;
char **ac_env;
s_data data[1000];
void init_data(){
    int i=0;
    while(i<1000)
    {
        data[i].args=NULL;
        data[i].exee=NULL;
        i++;
    }
}
void ft_strcpy(char *dest,char *src)
{
    int i=0;
    while (src[i])
    {
        dest[i]=src[i];
        i++;
    }
    dest[i]=0;
    
}
int ft_strlen(char *str)
{
    int i=0;
    if(str)
    {
    while(str[i])
    i++;}
    return i;
}
void pipe_element()
{
    int i=0;
    while(data[i].exee)
        i++;
    if(i>0)
        i--;
    data[i].piped=1;
}
void add_elem(char *arg)
{
    int i=0;
    while(data[i].exee)
    {
        i++;
    }
    data[i].args=malloc(sizeof(char*));
    data[i].args[0]=NULL;
    data[i].exee=arg;
    data[i].piped=0;
    data[i].in=0;
    data[i].out=1;

}
void add_arg(char *arg)
{
     int i=0;
     int k=0;
     int x=0;
    while(data[i].exee)
        i++;
    if(i>0)
    i--;
    while(data[i].args[k])
    k++;
    data[i].args[k]=malloc(sizeof(char)*(ft_strlen(arg)+1));
    ft_strcpy(data[i].args[k],arg);
    char **temp=malloc(sizeof(char*)*(k+1));
    while(x<=k)
    {
        temp[x]=data[i].args[x];
        x++;
    }
    free(data[i].args);
    data[i].args[++k]=NULL;
    data[i].args=temp;


}
void print_cenas(){

    int i=0;
	int k=0;
	printf("PRINT\n");
	while(data[i].exee)
	{
		printf("ELEMENTO %d\n",i);
		k=0;
		while(data[i].args[k])
		printf("%s\n",data[i].args[k++]);
		printf("BIN %s\n",data[i].exee);
		printf("IN %d\n",data[i].in);
		printf("OUT %d\n",data[i].out);
		printf("PIPED %d\n",data[i].piped);
		printf("\n");
		i++;
	}
}

void parse_args(int argc, char **argv)
{
    int i;
    int first_elem = 1;
    i=1;
    while(i<argc)
    {
        if(!strcmp(argv[i],";"))
            first_elem = 1;
        else if(!strcmp(argv[i], "|"))
        {
            first_elem = 1;
            pipe_element();
        }
        else if(first_elem)
        {
            add_elem(argv[i]);
            add_arg(argv[i]);
            first_elem = 0;
        }
        else
            add_arg(argv[i]);

        i++;
    }
}
void redirect(int start,int end){
    int a=start;
    int fds[2];
    while(a<end)
    {
        pipe(fds);
        data[a].out=fds[1];
        data[a + 1].in=fds[0];
        ++a;
    }


}
void exec_t(int i)
{int pid = fork();
   if(pid==0){
   dup2(data[i].out,1);
   dup2(data[i].in,0);
    execve(data[i].exee,data[i].args,ac_env);
    exit(1);
  }
    else{
        waitpid(pid,NULL,0);
        if(data[i].in!=0)
        close(data[i].in);
         if(data[i].out!=1)
    close(data[i].out);
        
    }
}
void chand(int i)
{
    chdir(data[i].args[1]);
}
void execute(int start, int end)
{

    int s2;
    if(end > start)
    {
        redirect(start,end);
    }
    s2=start;
    while(s2 <= end)
    {
       if(strcmp(data[s2].exee, "cd")) 
        exec_t(s2);
        else
            chand(s2);
        ++s2;
    }


}

void execit()
{
    int i=0;
    int k;
    while(data[i].exee)
    {
        k=i;
        while(data[i].piped)
            ++i;
        execute(k,i);
        ++i;

    }

}

int main (int argc, char **argv, char **env)
{
    ac_env=env;
    init_data();
    parse_args(argc,argv);
    //print_cenas();
    execit();
    /*char ola[20];
    ft_strcpy((char*)&ola,argv[1]);
    printf("%s",ola);*/
    return (0);    
}