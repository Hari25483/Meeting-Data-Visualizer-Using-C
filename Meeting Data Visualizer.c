//Importing required libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Creating a struct with next pointer so that we can create linked lists.
typedef struct key_value
{
    char name[50];
    int participants;
    int duration;
    int name_freq;
    struct key_value * next;
} dict;


//Defining head, tail for linked list operations.
dict * head=NULL,*tail=NULL;


//Function prototypes
void swap(dict *a,dict *b);
void sort_by_participants(int,int );
void sort_by_duration(int max,int);
void scaled(int total1,int max,dict **head_reference,int x);
void sort_by_name_frequency(int,int );
void non_scaled(int total1,int max,dict **head_reference,int x);
int convert_to_minutes(char duration[]);


//These variables are used to find the total participants, total duration, total name frequencies for plotting the graph.
int total_name_freq=0,total_participants=0,total_duration=0;


int main(int argc, char **argv){
    char line[1000];
    //max variable is defined to find the number of variables that have to be printed in the graph.
    int column_count= 0,max=10;
    //one string, and two integer variables are created to store the read information from csv files.
    char name[50]; 
    int participants,duration;
    int l_value,flag_number=0,flag_count=0,scaled_num=0,l_flag=0,corrupted_file=0; //flag_count keeps track of number of correct flags
    char l_string[10];
    int argument_count;

    //Argument handling
    if(argc < 2){
        printf("No input files were given\nusage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
        return 0;
    }


    for(int k=1;k<argc;k++){   // Iterating throught the arguments given by user
        if(strcmp(argv[k], "-p")==0){ //If argument is -p, set flag_number to 1 which  calls sort by participants function. 
                flag_count=flag_count+1;
                flag_number=1;
            }
            else if(strcmp(argv[k], "-m")==0){ //If argument is -m, set flag_number to 2 which  calls sort by name frequency function. 
                flag_count=flag_count+1;
                flag_number=2;
            }
            else if(strcmp(argv[k], "-t")==0){ //If argument is -t, set flag_number to 3 which  calls sort by duration function. 
                flag_count=flag_count+1;
                flag_number=3;
                
            }
            else if(strcmp(argv[k], "--scaled")==0){ //If argument is -scaled, call scaled printing function by making scaled number as 1. 
                scaled_num=1;
            }
            else if(strcmp(argv[k], "-l")==0){ //If argument is -l, get the required numbers of printing rows.
                if (argv[k+1]!=NULL){
                    if (isdigit(argv[k+1][0])){
                        strcpy(l_string,argv[k+1]);
                        l_value = atoi(argv[k+1]);
                        l_flag=1;
                        max=l_value;
                        if(l_value == 0){
                            return 0;
                        }
                    }
                    else{
                        printf("Invalid options for [-l]\n");
                        printf("usage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
                        return 0;
                        }
                    }
                else{
                    printf("Not enough options for [-l]\n");
                    printf("usage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
                    return 0;
                    }
                }
            //This else block checks for invalid arguments with flag. If there is an argument after -(flag) rather than p or t or m or l, program prints error message.
            else{
                if (argv[k][0]=='-'){

                    if (argv[k][1]=='l'){

                        if (argv[k][2]!='\0'){
                            printf("Invalid option [%s]\n",argv[k]);
                            printf("usage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
                            return 0;
                        }

                        else{
                            printf("Not enough options for [-l]\n");
                            printf("usage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
                            return 0;
                        }
                    }

                    else{
                        printf("Invalid option [%s]\n",argv[k]);
                        printf("usage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
                        return 0;
                    }
                }
            }   
        }
    //If more arguments with '-' is given, then display this message.
    if(flag_count>1){
        printf("Cannot plot multiple parameters in same graph.\n");
        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);
        return 0;
        }  
    int l_check=0;
    for(int k=1;k<argc;k++){   // Iterating throught the input files

        FILE *file_pointer = fopen(argv[k],"r");
        if (!file_pointer) //Handling the corrupted or null files
        {
            if ((strcmp(argv[k], "-t")==0)||(strcmp(argv[k], "-l")==0)||(strcmp(argv[k], "-m")==0)||(strcmp(argv[k], "-p")==0)||(strcmp(argv[k], "--scaled")==0)){
                continue;
            }
            if (strcmp(argv[k],l_string)==0){
                for (int m=0;m<strlen(l_string);m++){
                    if (isdigit(argv[k][m])){
                        l_check=1;
                    }
                    else{
                        l_check=2;
                        break;
                    }
                }            
                if (l_check==1){
                    continue;
                }
                else{
                    printf("Invalid options for [-l]\n");
                    printf("usage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n");
                    return 0;
                }
            }
            else{
                printf("Only .csv files should be given as inputs.\n");
                corrupted_file=1;
                return 0;
            }
        }
        //Reading the contents in file using while loop
        while (fgets(line,1000,file_pointer))
        {
            column_count = 0;  
            char *information = strtok(line,",");
            while (information) //Read data in rows by tokenizing every row by coma.
            {   
                if (column_count == 0)
                {
                    strcpy(name,information); //Read the name
                }
                if (column_count == 1)
                {
                    participants= atoi(information); //Read the participants
                }
                if (column_count == 2)
                {
                    duration= convert_to_minutes(information); //Read the time duration by converting it to minutes using a function.
                }
                information = strtok(NULL,",");
                column_count++;
            }
                dict *temp=head;
                //If the name read is already in our linked list, then dont add a new entry. Instead update the details of existing element.
                for (temp = head; temp != NULL; temp = temp->next) {
                        if (strcmp(temp->name, name) == 0) {
                            break;
                        }
                    }

                //If linked list is not initialized, add data to head pointer.
                if (head==NULL) {
                    //Allocate a memory for new node, and copy the required elements.
                    dict * data= (dict *)(malloc(sizeof(dict)));
                    strcpy(data->name, name);
                    data->name_freq= 1;
                    data->participants =participants;
                    data->duration =duration;
                    total_duration=total_duration+duration;
                    total_name_freq=total_name_freq+1;
                    total_participants=total_participants+participants;
                    head = data;
                    tail = data;
                    head->next=NULL;
                    
                }
                // If linked list was initialized already, add required data.
                else{
                    //If the name is new, add the data to the last node.
                    if (temp == NULL) {
                        dict * data= (dict *)(malloc(sizeof(dict)));
                        strcpy(data->name, name);
                        data->name_freq= 1;
                        data->participants =participants;
                        data->duration =duration;
                        total_duration=total_duration+duration;
                        total_name_freq=total_name_freq+1;
                        total_participants=total_participants+participants;
                        tail->next=data;
                        data->next =NULL;
                        tail = data;
                    }
                    //If the same name with data exists, update that node with new data.
                    else {
                    temp->name_freq=temp->name_freq+1;
                    temp->participants =temp->participants+participants;
                    temp->duration=temp->duration+duration;
                    total_duration=total_duration+duration;
                    total_name_freq=total_name_freq+1;
                    total_participants=total_participants+participants;
                    }
                } 
            }
        fclose(file_pointer);
        }

        
    argument_count=argc;    
    //For scaled option of the graph
    if (scaled_num==1){
        if (flag_number==1){
            sort_by_participants(max,2);
            return 0;
        }
        else if (flag_number==2){
            sort_by_name_frequency(max,2);
            return 0;    
        }
        else if (flag_number==3){
            sort_by_duration(max,2);
            return 0;    
        }
        else if (flag_number==0&&l_flag==1){
            sort_by_name_frequency(max,2);
            return 0;
        }
     }
    if (flag_number==1){
            sort_by_participants(max,1);
            return 0;    
        }
    else if (flag_number==2){
            sort_by_name_frequency(max,1);
            return 0;    
        }
    else if (flag_number==3){
            sort_by_duration(max,1);
            return 0;    
        }
    if (flag_number==0&&l_flag==1){
        sort_by_name_frequency(max,1);
        return 0;
    }
    //Without any flags, check for correct files, and print name frequencies.
    if ((argc==argument_count)&&(corrupted_file==0)){
        if (scaled_num==1){
            sort_by_name_frequency(max,2);
            return 0;
        }
        else if (scaled_num==0){
            sort_by_name_frequency(max,1);
            return 0;
        }
    }

    return 0;
}

//Functon to change the read duration value to minutes
int convert_to_minutes(char duration[8]){
    char *read_time = strtok(duration,":"); //Duration that was read is tokenized by ':' to identify hour, and minutes.
    int minutes,colon_count = 0,hour,min;
    while (read_time)
        {   
            if (colon_count == 0)
            {
                hour = atoi(read_time);
            }
            if (colon_count == 1)
            {
                min = atoi(read_time);            
            }
            read_time = strtok(NULL,":");
            colon_count++;
        }
        minutes = (hour * 60) + min;
    return minutes;
}


//Function to sort by participants
void sort_by_participants(int max,int num){
    dict *node=head, *temporary_node = NULL;
    //iterate through nodes to sort our data.
    while(node!=NULL)
    {
        temporary_node=head; 
        while(temporary_node->next!=NULL)//Transverse till the second last node 
        {
           if(temporary_node->participants <temporary_node->next->participants)// compare the participants of the nodes 
            {
              swap(temporary_node,temporary_node->next); //If the condition is satisfied, then swap the data in those nodes.
            }
         temporary_node = temporary_node->next;    // Move to the next node  in the loop 
        }

        node = node->next;    // move to the next node in the big loop
    }

    if (num==1){    //Call the non-scaled graph function
        non_scaled(total_participants,max,&head,1);
    }
    else if (num==2){   //Call the scaled graph function
        scaled(total_participants,max,&head,1);
    }
}

//Function to sort by name frequency
void sort_by_name_frequency(int max,int num){
    dict *node=head, *temporary_node = NULL;
    //iterate through nodes to sort our data.
    while(node!=NULL)
    {
        temporary_node=head; 
        while(temporary_node->next!=NULL)//transverse till the second last node
        {
           if(temporary_node->name_freq <temporary_node->next->name_freq)// compare the name freuency of the nodes 
            {
              swap(temporary_node,temporary_node->next);  //If the condition is satisfied, then swap the data in those nodes.
            }
         temporary_node = temporary_node->next;    // move to the next node 
        }
        node = node->next;    // move to the next node
    }

    if (num==1){   //Call the non-scaled graph function
        non_scaled(total_name_freq,max,&head,3);
    }
    else if (num==2){  //Call the scaled graph function
        scaled(total_name_freq,max,&head,3);
    }
}

//Function to sort by duration
void sort_by_duration(int max,int num){
    dict *node=NULL, *temporary_node = NULL;
    node = head;
    while(node != NULL)
    {
        temporary_node=head; 
        while (temporary_node->next!=NULL)//transverse till the second last node 
        {
            
           if(temporary_node->duration <temporary_node->next->duration)// compare the duration of the nodes 
            {
              swap(temporary_node,temporary_node->next);  //If the condition is satisfied, then swap the data in those nodes.
            }
         temporary_node = temporary_node->next;    // move to the next element 
        }
        node = node->next;    // move to the next node
    }

    if (num==1){
        non_scaled(total_duration,max,&head,2);   //Call the non-scaled graph function
    }
    else if (num==2){
        scaled(total_duration,max,&head,2);  //Call the scaled graph function
    }
}


//Function to swap the data between two nodes during sorting.
void swap(dict *a,dict *b)
{
    int tempvar;
    char name[50];
    tempvar = a->participants;
    a->participants = b->participants;// swap the participants
    b->participants = tempvar;

    tempvar = a->duration;
    a->duration = b->duration;// swap the durations
    b->duration = tempvar;

    tempvar = a->name_freq;
    a->name_freq = b->name_freq;// swap the name frequencies
    b->name_freq = tempvar;

    strcpy(name,a->name);
    strcpy(a->name,b->name); //swap the names of two nodes by using a tempropry string, and strcpy function.
    strcpy(b->name,name);
}

//Function to print non-scaled version of the graph
void non_scaled(int total1,int max,dict **head_reference,int x){
    //Declaring two head pointers so that one can be used to loop through linked list to print graph, and other can be used for finding the size of largest string in the required graph.
    dict *y=*head_reference;
    dict *z=*head_reference;
    int plotting_spaces; //used for counting spaces to print the plotting_spaces.
    int largest_string_len=0,e=0; //largest_string_len is used to find maximum name size.
    int spaces,number_len=0; //number_len is usesd to find the length of the biggest number.
    while(z!=NULL){
        if (e==max){
            break;
        }
        if (largest_string_len<strlen(z->name)){
            largest_string_len=strlen(z->name);
            }
        e=e+1;
        z=z->next;
    }
    int limit=0,n=0;  //limit is used to break once maximum requested number of rows is exceeded.
    printf("\n");
    while(y!=NULL){
        if (limit==max){
            break;
        }
        if (x==1){
            if (limit==0){
                n=y->participants;
                while(n!=0){  
                n=n/10;
                number_len++;
                }  
            }
            spaces=77-largest_string_len-number_len;
            plotting_spaces=(int)((float)(spaces*y->participants)/total1);
        }
        else if (x==2){
            n=y->duration;
            if (limit==0){
                while(n!=0){  
                n=n/10;
                number_len++;
                }  
            }
            spaces=77-largest_string_len-number_len;
            plotting_spaces=(int)((float)(spaces*y->duration)/total1);          
        }
        else if (x==3){
            n=y->name_freq;
            if (limit==0){
                while(n!=0){  
                n=n/10;
                number_len++;
                }  
            }
            spaces=77-largest_string_len-number_len;
            plotting_spaces=(int)((float)(spaces*y->name_freq)/total1);            
        }
        for(int i=0;i<(largest_string_len+2);i++){
            printf(" ");
            }
            printf("\u2502");
            for(int j=0;j<plotting_spaces;j++){
            printf("░");
            }
            printf("\n");
            printf(" %s",y->name);
            for(int i=0;i<(largest_string_len-(strlen(y->name)-1));i++){
            printf(" ");
            }
            printf("│");
            
            for(int j=0;j<plotting_spaces;j++){
            printf("░");
            }
            //Print the numbers according to options
            if (x==1){
                printf("%d\n",y->participants);
            }
            else if (x==2){
                printf("%d\n",y->duration);            
            }
            else if (x==3){
                printf("%d\n",y->name_freq);            
            }

        for(int i=0;i<(largest_string_len+2);i++){
            printf(" ");
            }
            printf("│");
            
            for(int j=0;j<plotting_spaces;j++){
            printf("░");
            }
            printf("\n");
        
        for(int i=0;i<(largest_string_len+2);i++){
          printf(" ");
          }
        printf("│\n");
        y=y->next;
        limit=limit+1;
    }
    //For the bottom line required spaces, and horizontal bars are printed.
    for(int i=0;i<(largest_string_len+2);i++){    
        printf(" ");
    }
    printf("└");
    for(int i=0;i<(80-(largest_string_len+3));i++){
    printf("─");
    }
    printf("\n");
}


//Function to print scaled version of graph
void scaled(int total1,int max,dict **head_reference,int x){
    //Declaring two head pointers so that one can be used to loop through linked list to print graph, and other can be used for finding the size of largest string in the required graph.
    dict *y=*head_reference;
    dict *f=*head_reference;
    int plotting_spaces; //used for counting spaces to print the plotting_pattern.
    int largest_string_len=0,e=0; //largest_string_len is used to find maximum name size.
    int spaces,number_len=0; //number_len is usesd to find the length of the biggest number.
    while(f!=NULL){
        if (e==max){
            break;
        }
        if (largest_string_len<strlen(f->name)){
            largest_string_len=strlen(f->name);
            }
        e=e+1;
        f=f->next;
    }
    int limit=0,z=0,n=0,l=0; 
    printf("\n");
    while(y!=NULL){
        if (limit==max){   //limit is used to break once maximum requested number of rows is exceeded.
            break;
        }
        if (x==1){
            n=y->participants;
            z=l;
            if (limit==0){
                while(n!=0){
                    n=n/10;  
                    number_len++;
                    }  
                spaces=77-largest_string_len-number_len;
                plotting_spaces=spaces;
                l=y->participants;
            }
            else if(limit!=0) {
                plotting_spaces=((float)spaces/(float)z)*y->participants;
            }
            
        }
        else if (x==2){
            n=y->duration;
            z=l;
            if (limit==0){
                while(n!=0){  
                n=n/10;
                number_len++;
                    }  
                spaces=77-largest_string_len-number_len;
                plotting_spaces=spaces;
                l=y->duration;
            }
            else if(limit!=0) {
                plotting_spaces=((float)spaces/(float)z)*y->duration;
            }
        }
        else if (x==3){
            n=y->name_freq;
            z=l;
            if (limit==0){
                while(n!=0)  {  
                n=n/10;
                number_len++;
                    }  
                spaces=77-largest_string_len-number_len;
                plotting_spaces=spaces;
                l=y->name_freq;
            }
            else if(limit!=0) {
                plotting_spaces=((float)spaces/(float)z)*y->name_freq;
            }
        }
        //For the first line spaces are printed, and then the vertical line, and then the plotting_pattern.
        for(int i=0;i<(largest_string_len+2);i++){ 
            printf(" ");
            }
            printf("\u2502");
            for(int j=0;j<plotting_spaces;j++){
            printf("░");
            }
            printf("\n");
            //For the second line a space, then the name, and required spaces are printed. then the straight line, and after that plotting_pattern, and the numbers are printed.
            printf(" %s",y->name);
            for(int i=0;i<(largest_string_len-(strlen(y->name)-1));i++){
            printf(" ");
            }
            printf("│");
            
            for(int j=0;j<plotting_spaces;j++){
            printf("░");
            }
            //Print the numbers according to options
            if (x==1){
                printf("%d\n",y->participants);
            }
            else if (x==2){
                printf("%d\n",y->duration);
            }
            else if (x==3){
                printf("%d\n",y->name_freq);
            }

        for(int i=0;i<(largest_string_len+2);i++){
            printf(" ");
            }
            printf("│");
            
            for(int j=0;j<plotting_spaces;j++){
            printf("░");
            }
            printf("\n");
        
        for(int i=0;i<(largest_string_len+2);i++){
          printf(" ");
          }
        printf("│\n");
        y=y->next;
        limit=limit+1;
    }
    //For the bottom line required spaces, and horizontal bars are printed.
    for(int i=0;i<(largest_string_len+2);i++){    
        printf(" ");
    }
    printf("└");
    for(int i=0;i<(80-(largest_string_len+3));i++){
    printf("─");
    }
    printf("\n");
}
