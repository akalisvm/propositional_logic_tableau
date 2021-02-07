#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */

/*
 * Your program will read its input from a file called input.txt,
 * parse each of the ten formulas and output their types, build a tableau for each of the
 * ten formulas, expand till completed, test to see if they are closed, and then output whether the
 * formula is satisfiable or not. All the outputs go to a file called output.txt.
 * */

int Fsize=50; /*maximum formula length*/
int inputs=10;/* number of formulas expected in input.txt*/
int ThSize=100;/* maximum size of set of formulas, if needed*/
int TabSize=500; /*maximum length of tableau queue, if needed*/



/* A set will contain a list of words. Use NULL for emptyset.  */
struct set {
    char *item;/*first word of non-empty set*/
    struct set *tail;/*remaining words in the set*/
};

/* A tableau will contain a list of pointers to sets (of words).  Use NULL for empty list.*/
struct tableau {
    struct set *S; /* pointer to first set in non-empty list */
    struct tableau *rest; /*list of pointers to other sets*/
};

int isOperator(char c) {
    int flag = 0;
    if(c=='v'||c=='^'||c=='>') {
        flag = 1;
    }
    return flag;
}

int isLetter(char c) {
    int flag = 0;
    if(c=='p'||c=='q'||c=='r') {
        flag = 1;
    }
    return flag;
}

int isLegalCharacter(char c) {
    int flag = 1;
    if (c!='p'&&c!='q'&&c!='r'&&c!='v'&&c!='^'&&c!='>'&&c!='-'&&c!='('&&c!=')') {
        flag = 0;
        return flag;
    }
    return flag;
}

int n = 0;
int flag = 1;
int numOfBranches = 0;
int key = 0;
char operator = ' ';
char *item = NULL;
struct set *currSet;

/*You need to change this next bit and include functions for parse, closed and complete.*/
int parse(char str[Fsize]) {
    int numOfLeftBracket = 0;
    int numOfRightBracket = 0;
    char temp = 0;
    for(int i = 0; str[i]; i++) {
        if(isLegalCharacter(str[i]) == 0) return 0;
        else {
            if(isLetter(str[0]) == 0 && str[0] != '-' && str[0] != '(') return 0;
            else if(isLetter(str[strlen(str)-1]) == 0 && str[strlen(str)-1] != ')' ) return 0;
            else if(str[i] == '(') {
                numOfLeftBracket++;
                temp = str[i];
                if(i > 0 && (isLetter(str[i-1]) == 1 || str[i-1] == ')')) return 0;
                else if(isOperator(str[i+1]) == 1) return 0;
            }
            else if(str[i] == ')') {
                temp = str[i];
                numOfRightBracket++;
                if((isLetter(str[i+1]) == 1 || (str[i+1] == '-'))) return 0;
            }
            else if((isOperator(str[i]) == 1) || (str[i] == '-')) {
                if(isOperator(str[i+1]) == 1 || str[i+1] == ')') return 0;
                else if(isOperator(str[i]) == 1 ) {
                    if(isOperator(temp) == 0) temp = str[i];
                    else if(isOperator(temp) == 1) return 0;
                }
            }
        }
    }
    if(numOfLeftBracket != numOfRightBracket) return 0;
    n = numOfLeftBracket;
    if (strlen(str) == 1) return 1;
    else if (str[0] == '-') return 2;
    else return 3;
}

void countNumOfLeftBracket(const char str[Fsize]) {
    n = 0;
    for(int i = 0; str[i]; i++) {
        if(str[i] == '(') n++;
    }
}

void checkOperator(const char str[Fsize]) {
    if(n == 1) {
        for(int i = 0; str[i]; i++) {
            if(isOperator(str[i])) operator = str[i];
        }
    } else {
        int a = 1;
        for(int i = 1; str[i]; i++) {
            if(a == 1 && isOperator(str[i])) operator = str[i];
            else if(str[i] == '(') a++;
            else if(str[i] == ')') a--;
        }
    }
}

char *partone(char str[Fsize]) {
    char *partone=(char *)calloc(Fsize, sizeof(char));
    int pos = 0;
    if(n == 1) {
        for(int i = 0; str[i]; i++) {
            if(isOperator(str[i])) pos = i;
        }
        strncpy(partone, str+1, pos-1);
        partone[pos-1] = '\0';
    }
    else {
        int a = 1;
        for(int i = 1; str[i]; i++) {
            if(a == 1 && isOperator(str[i])) pos = i;
            else if(str[i] == '(') a++;
            else if(str[i] == ')') a--;
        }
        strncpy(partone, str+1, pos-1);
        partone[pos-1] = '\0';
    }
    return partone;
}

char *parttwo(char str[Fsize]) {
    char *parttwo=(char *)calloc(Fsize, sizeof(char));
    int pos = 0;
    if(n == 1) {
        for(int i = 0; str[i]; i++) {
            if(isOperator(str[i])) pos = i;
        }
        strncpy(parttwo, str+1+pos, strlen(str)-pos-2);
        parttwo[strlen(str)-pos-2] = '\0';
    }
    else {
        int a = 1;
        for(int i = 1; str[i]; i++) {
            if(a == 1 && isOperator(str[i])) pos = i;
            else if(str[i] == '(') a++;
            else if(str[i] == ')') a--;
        }
        strncpy(parttwo, str+1+pos, strlen(str)-pos-2);
        parttwo[strlen(str)-pos-2] = '\0';
    }
    return parttwo;
}

struct set *create() {
    struct set *head=(struct set *)calloc(ThSize, sizeof(char));
    return head;
}

void appendItem(struct set *head, char str[Fsize]) {
    struct set *q=create();
    q->item=str;
    q->tail=NULL;
    struct set *p;
    for(p=head; p->tail!=NULL; p=p->tail);
    p->tail=q;
}

struct set *removeItem(struct set *head, const char str[Fsize]) {
    struct set *p, *q;
    for(p=head,q=head; p->item!=str && p->tail!=NULL; q=p,p=p->tail);
    if(p==head) {
        head=p->tail;
        free(p);
    } else {
        q->tail=p->tail;
        free(p);
    }
    return head;
}

int numOfItems(struct set *head) {
    struct set *p;
    int numOfItems = 0;
    for(p=head; p!=NULL; p=p->tail) {
        if(p->item!=NULL) {
            numOfItems++;
        }
    }
    return numOfItems;
}

void addSet(struct tableau *head, struct set *set) {
    numOfBranches++;
    struct tableau *q = (struct tableau *)calloc(TabSize, sizeof(set));
    q->rest = NULL;
    q->S = set;
    struct tableau *p;
    for(p=head; p->rest!=NULL; p=p->rest);
    p->rest = q;
}

int closed(struct tableau *t) {
    if(key == 1) return 0;
    struct tableau *p;
    struct set *q;
    int closed = 0;
    for(p=t->rest; p!=NULL; p=p->rest) {
        if(numOfItems(p->S) == 0 || numOfItems(p->S) == 1) continue;
        else {
            for(q=p->S; q!=NULL; q=q->tail) {
                if(q->item!=NULL) {
                    struct set *r;
                    char *str=(char *)calloc(Fsize, sizeof(char));
                    item = q->item;
                    if(strlen(item) == 1) {
                        strcat(str, "-");
                        strcat(str, item);
                    } else {
                        strncpy(str, item+1, strlen(item)-1);
                        str[strlen(item)-1] = '\0';
                    }
                    for(r=q; r!=NULL; r=r->tail) {
                        if(r->item!=NULL && r->item!=item && strcmp(r->item, str) == 0) {
                            closed++;
                            continue;
                        }
                    }
                }
            }
        }
    }
    if(closed == numOfBranches) return 1;
    return 0;
}

void complete(struct tableau *t) {
    n = 0;
    flag = 1;
    key = 0;
    operator = ' ';
    item = NULL;
    struct tableau *p;
    struct set *q;
    for(p=t->rest; p!=NULL; p=p->rest) {
        for(q=p->S; q!=NULL; q=q->tail) {
            if(q->item!=NULL) {
                if(!(strlen(q->item) == 1 || strlen(q->item) == 2)) {
                    item = q->item;
                    currSet = p->S;
                    flag = 0;
                    break;
                }
            }
        }
    }
}

struct tableau *initialise(char str[Fsize]) {
    n = 0;
    flag = 1;
    numOfBranches = 0;
    key = 0;
    operator = ' ';
    item = NULL;
    currSet = NULL;
    struct tableau *tableau = (struct tableau *)malloc(TabSize);
    struct set *set = create();
    appendItem(set, str);
    addSet(tableau, set);
    if(strlen(str) == 1 || strlen(str) > 24) {
        key = 1;
        return tableau;
    }
    for(;;) {
        complete(tableau);
        if(flag == 1) {
            break;
        } else {
            countNumOfLeftBracket(item);
            if (item[0] == '-') {
                char *itemRemoveNeg=(char *)calloc(Fsize, sizeof(char));
                strncpy(itemRemoveNeg, item+1, strlen(item)-1);
                itemRemoveNeg[strlen(item)-1] = '\0';
                checkOperator(itemRemoveNeg);
                if (itemRemoveNeg[0] == '-' && operator== ' ') {
                    char *negStr=(char *)calloc(Fsize, sizeof(char));
                    strncpy(negStr, item+2, strlen(item)-2);
                    negStr[strlen(item)-2] = '\0';
                    appendItem(currSet, negStr);
                    removeItem(currSet, item);
                } else if (operator == '^') {
                    struct set *tempSet, *newSet=(struct set *)calloc(ThSize, sizeof(char));
                    addSet(tableau, newSet);
                    for(tempSet=currSet;tempSet!=NULL;tempSet=tempSet->tail) {
                        if(tempSet->item!=NULL&&tempSet->item!=item) appendItem(newSet, tempSet->item);
                    }
                    char *temp1=(char *)calloc(Fsize, sizeof(char));
                    char *temp2=(char *)calloc(Fsize, sizeof(char));
                    strcat(temp1, "-");
                    strcat(temp1, partone(itemRemoveNeg));
                    strcat(temp2, "-");
                    strcat(temp2, parttwo(itemRemoveNeg));
                    appendItem(currSet, temp1);
                    appendItem(newSet, temp2);
                    removeItem(currSet, item);
                } else if (operator == 'v') {
                    char *temp1=(char *)calloc(Fsize, sizeof(char));
                    char *temp2=(char *)calloc(Fsize, sizeof(char));
                    strcat(temp1, "-");
                    strcat(temp1, partone(itemRemoveNeg));
                    strcat(temp2, "-");
                    strcat(temp2, parttwo(itemRemoveNeg));
                    appendItem(currSet, temp1);
                    appendItem(currSet, temp2);
                    removeItem(currSet, item);
                } else if (operator == '>') {
                    char *temp=(char *)calloc(Fsize, sizeof(char));
                    strcat(temp, "-");
                    strcat(temp, parttwo(itemRemoveNeg));
                    appendItem(currSet, partone(itemRemoveNeg));
                    appendItem(currSet, temp);
                    removeItem(currSet, item);
                }
            } else {
                checkOperator(item);
                if (operator == '^') {
                    appendItem(currSet, partone(item));
                    appendItem(currSet, parttwo(item));
                    removeItem(currSet, item);
                } else if (operator == 'v') {
                    struct set *tempSet, *newSet=(struct set *)calloc(ThSize, sizeof(char));
                    addSet(tableau, newSet);
                    for(tempSet=currSet;tempSet!=NULL;tempSet=tempSet->tail) {
                        if(tempSet->item!=NULL&&tempSet->item!=item) appendItem(newSet, tempSet->item);
                    }
                    appendItem(currSet, partone(item));
                    appendItem(newSet, parttwo(item));
                    removeItem(currSet, item);
                } else if (operator == '>') {
                    struct set *tempSet, *newSet=(struct set *)calloc(ThSize, sizeof(char));
                    addSet(tableau, newSet);
                    for(tempSet=currSet;tempSet!=NULL;tempSet=tempSet->tail) {
                        if(tempSet->item!=NULL&&tempSet->item!=item) appendItem(newSet, tempSet->item);
                    }
                    char *temp=(char *)calloc(Fsize, sizeof(char));
                    strcat(temp, "-");
                    strcat(temp, partone(item));
                    appendItem(currSet, temp);
                    appendItem(newSet, parttwo(item));
                    removeItem(currSet, item);
                }
            }
        }
    }
    return tableau;
}


int main()

{
    /*You should not need to alter the input and output parts of the program below.*/
    char *name = malloc(Fsize);

    /*input 10 strings from "input.txt" */
    FILE *fp, *fpout;

    /*reads from input.txt, writes to output.txt*/
    if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
    if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

    int j;

    struct set *S = NULL;
    struct table *t = NULL;

    for(j=0;j<inputs;j++)
    {
        fscanf(fp, "%s",name);/*read formula*/
        switch (parse(name))
        {
            case(0): fprintf(fpout, "%s is not a formula.  \n", name);break;
            case(1): fprintf(fpout, "%s is a proposition. \n ", name);break;
            case(2): fprintf(fpout, "%s is a negation.  \n", name);break;
            case(3):fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name, partone(name), parttwo(name));break;
            default:fprintf(fpout, "What the f***!  ");
        }




        if (parse(name)!=0)
        {
            /* here you should initialise a theory S with one formula (name) in it and then initialise a tableau t with on theory (S) in it*/
            /* then you should call a function that completes the tableau t*/
            if (closed(initialise(name)))  fprintf(fpout, "%s is not satisfiable.\n", name);
            else fprintf(fpout, "%s is satisfiable.\n", name);
        }
        else  fprintf(fpout, "I told you, %s is not a formula.\n", name);
    }


    fclose(fp);
    fclose(fpout);
    free(name);
    free(S);
    free(t);

    return(0);
}