#include <stdio.h>
#include <stdlib.h>
char input[100];
char post[100];
int l=-1;
char inputs[100];
int input_length=-1;
struct stack{
char arr[100];
int top;
};
struct stack st;
//stack for conversion and
int stack[100][2],top=-1,table[100][50],states=-1;
void postorder();
int priority(char);
void create_inputs();
int get_pos_inputs(char );
void print_dfa();
int main()
{
    st.top=-1;
   printf("Please enter Regular expression\t");
   fflush(stdin);
   gets(input);
   postorder();
   printf("\n\nYour Postfix Expression is as Follows\t");
   puts(post);
   printf("\n\nYour Inputs contains\t");
   create_inputs();
   puts(inputs);
   for(int i=0;i<100;i++)
    for(int j=0;j<50;j++)
    table[i][j]=-1;
   for(int i=0;i<=l;i++)
   {
       if(post[i]=='*'||post[i]=='+'||post[i]=='.')
       {

          if(post[i]=='.')
          {
             table[stack[top-1][1]][input_length+1]=stack[top][0];
             stack[top-1][1]=stack[top][1];
             top--;
          }
          else if(post[i]=='+')
          {
            //2 new states 4 epislon transition
            //states+2
            states++;
            table[states][input_length+1]=stack[top][0];
            table[states][input_length+2]=stack[top-1][0];
            states++;
            table[stack[top][1]][input_length+1]=states;
            table[stack[top-1][1]][input_length+1]=states;
            stack[top][0]=states-1;
            stack[top][1]=states;
          }
          else if(post[i]=='*')
          {
               //2 new states 4 eplison transition
               states++;
               table[states][input_length+1]=stack[top][0];
               table[states][input_length+2]=states+1;
               table[stack[top][1]][input_length+1]=states+1;
               table[stack[top][1]][input_length+2]=stack[top][0];
               states++;
               stack[top][0]=states-1;
               stack[top][1]=states;
          }
       }
       else
       {
           states++;

           table[states][get_pos_inputs(post[i])]=states+1;
           states++;
           stack[++top][0]=states-1;
           stack[top][1]=states;
       }
   }
   print_dfa();
}
void postorder()
{
  int i=0;
  while(input[i]>0)
  {
      if(input[i]=='*'||input[i]=='.'||input[i]=='+'||input[i]=='('||input[i]==')')
      {
          if(input[i]=='(')
          {
            st.top++;
            st.arr[st.top]='(';
          }
          else if(input[i]==')')
          {
              while(st.arr[st.top]!='(')
              {
                  l++;
                  post[l]=st.arr[st.top];
                  st.top--;
              }
              st.top--;
          }
          else
          {
              //
              if(st.top==-1)
              {
                  st.top++;
                  st.arr[st.top]=input[i];
              }
              //check proirity and cut down till greater priority
              else
              {
                  while(st.top!=-1&&priority(input[i])<=priority(st.arr[st.top]))
                  {
                      if(st.arr[st.top]=='(')
                      {
                          break;
                      }
                      post[++l]=st.arr[st.top];
                      st.top--;
                  }
                  st.top++;
                  st.arr[st.top]=input[i];
              }
          }
      }
      else
      {
          l++;
          post[l]=input[i];
      }
      i++;
  }
  while(st.top!=-1)
  {
      post[++l]=st.arr[st.top];
      st.top--;
  }
}
int priority(char a)
{
    //# was used for unary + but we may not use it
    if(a=='*'||a=='#')
        return 2;
    else if(a=='.')
        return 1;
    else if(a=='+')
        return 0;
    else
        return 3;
}
void create_inputs()
{
    int flag=0;
    for(int i=0;i<=l;i++)
    {
        if(post[i]=='+'||post[i]=='*'||post[i]=='.')
            continue;
        //printf("%c",post[i]);
        flag=0;
        for(int j=input_length;j>=0;j--)
        {
            if(inputs[j]==post[i])
            {
                flag=1;
                break;
            }
        }
        if(flag==0)
        {
            input_length++;
            inputs[input_length]=post[i];
        }
    }
}
int get_pos_inputs(char a)
{
    for(int i=0;i<=input_length;i++)
    {
        if(a==inputs[i])
            return i;
    }
}
void print_dfa()
{
    printf("\n\n\n\n\t DFA Table\n\ns/i|\t");
    for(int i=0;i<=input_length;i++)
      printf("%c|\t",inputs[i]);
    printf("epsilon|\n");
    for(int i=0;i<=5*(input_length+3);i++)
        printf("--");
     printf("\n");
    for(int i=0;i<=states;i++)
    {
        printf("%d|\t",i);
        for(int j=0;j<=input_length+2;j++)
        {
            if(table[i][j]!=-1)
            printf("%d|\t",table[i][j]);
            else
                printf("-|\t");
        }
        printf("\n");
    }
}
