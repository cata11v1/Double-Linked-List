#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//struct node
typedef struct node{      
    int timestamp;      
    double value;
    struct node * next;
    struct node * prev;
}node;
//List struct
typedef struct TDoubleLinkedList {    
    node *head;                       
    node *tail;
    int len;
} TDoubleLinkedList;
//initialize the list and memory alloc
void init(TDoubleLinkedList** list)    
{                                      
*list = (TDoubleLinkedList*)malloc(sizeof(**list));
    (*list)->len = 0;               
    (*list)->head = NULL;           
    (*list)->tail =NULL;        
}
//add a node at the end of the list
void append(TDoubleLinkedList* list, int timestamp, double value)
{                                     
    if (!list->head) {      
        list->head = (node*)malloc(sizeof(node));
        list->head->value = value;
        list->head->timestamp = timestamp;
        list->head->next = NULL;
        list->head->prev=NULL;
        list->len++;
        return;
    }
    node* p = list->tail, * nou;
    //allocate memory for the new node
    nou = (node*)malloc(sizeof(node)); 
    //add data in the new node
    nou->value = value;     
    nou->timestamp = timestamp;
    while (p->next) {
        p = p->next;
    }
    p->next = nou;
    nou->prev=p;
    nou->next=NULL;
    list->len++;
    list->tail=nou;
}
//add a node at the beginning of List
void push(TDoubleLinkedList* list, int timestamp, double value)
{                                   
    node* nou;
    //allocate memory for the new node
    nou = (node*)malloc(sizeof(node));
    //add data in the new node
    nou->timestamp = timestamp;     
    nou->value = value;
    nou->next = list->head;
    nou->prev=NULL;
    if(nou->next!=NULL){
        nou->next->prev=nou;
    }
    list->head = nou;
    list->len++;
}
//add a node at the n position of List, if n>len(List) -> do nothing
int add(TDoubleLinkedList* list, int n, int timestamp, double value)
{                            
    if (n > list->len || n < 0) { 
        return -1;                
    }
    if (list->head == NULL) {
        list->head = (node*)malloc(sizeof(node));
        list->head->next=NULL;
        list->head->prev=NULL;
        list->head->timestamp=timestamp;
        list->head->value=value;
//if the list has no nodes, add the first node
        list->tail = list->head;
        list->len++;    
        return 1;
    }
    if (n == 0) {
        //add a node at the beginning of List 
        push(list, timestamp, value);
        return 1;
    }
    //add a node at the end of the list
    if(n==list->len) {
        append(list, timestamp, value);
        return 1;
    }
    //if no return => the position is beetween 0 and len(List)
    node* p = list->head, * q = list->head, * new;
    //allocate memory for the new node
    new = (node*) malloc(sizeof(node));  
    //add data in the node    
    new->timestamp = timestamp;     
    new->value = value;
    int i=0;
    //navigate to the position(n) we want to add the new node
    while(i<n && p){ 
        q=p;                                    
        p=p->next;
        i++;
    }
    //add the node(make connections with the neighbors)
    new->next = p;            
    p->prev=new;
    q->next = new;
    new->prev=q;
    list->len++;
    return 1;
}
//remove the one node
node* remove_node(TDoubleLinkedList* list, node* n)
{                              
    node* p, * q;
    p = q = list->head;
    //node=list head, remove it and all the positions -1 and len=len-1
    if (n == list->head) { 
        list->head = list->head->next;  
        if(list->head) {        
            list->head->prev=NULL; 
        }
        list->len--;
        return p;
    }
    //node=list-tail, remove it and len--
    if (n==list->tail){ 
        p=list->tail;   
        list->tail=list->tail->prev;
        list->tail->next=NULL;
        list->len--;
        return p;
    }
    //if no return=> navigate to the node and remove it
    for (node* i =list->head; i!=n;i=i->next, q = p, p = p->next);
    //with p & q we realise the connection beetween the neighbors
    q->next = p->next;  
    p->next->prev=q;    
    list->len--;        
    return p;        
}
//free function navigate list and free memory for every node
void free_list(TDoubleLinkedList** list)
{
    node* p, * q;
    p = q = (*list)->tail;
    while (p != NULL) {
        q = p->prev;
        free(p);
        p = q;
    }
    free(*list);
}
//navigate the list and display data from every node
void print_list(TDoubleLinkedList* list)
{
    for(node* i =list->head; i!=NULL;i=i->next){
        printf("%d %.2lf\n", i->timestamp, i->value);
    }
}
//calculate the average of a window with 5 elements
double media_val(node* nod)
{  
    double x=0;
    x= nod->value + nod->prev->value + nod->prev->prev->value +
        nod->next->value + nod->next->next->value;
    return x/5;
}
//calculate the standard deviation of a window with 5 elements
double deviatia_stand(node* nod)
{
    double x=0;    
    x= (nod->value-media_val(nod))*(nod->value-media_val(nod)) + 
    (nod->prev->value-media_val(nod))*(nod->prev->value-media_val(nod)) + 
    (nod->prev->prev->value-media_val(nod))*(nod->prev->prev->value -
    media_val(nod))+(nod->next->value-media_val(nod))*(nod->next->value -
    media_val(nod))+(nod->next->next->value - media_val(nod))*
    (nod->next->next->value - media_val(nod));
    x=x/5;
    x=sqrt(x);
    return x;
}
//this function return a list without all the nodes that don't fit beetween sum
//and difference of average and standard deviation
void e1(TDoubleLinkedList* list)
{
    TDoubleLinkedList *list1;   
    init(&list1); 
    int j=0;       
    for(node* i =list->head; i!=NULL;i=i->next){   
        add(list1, j, i->timestamp, i->value);  
        j++;  
    }
    node* aux;
    for(node* j1 =list1->head->next->next; j1!=list1->tail->prev;j1=j1->next){
        for(node* i=list->head->next->next; i!=list->tail->prev;i=i->next){
            if(j1->value < (media_val(j1)-deviatia_stand(j1)) ||
              (media_val(j1)-deviatia_stand(j1)) == j1->value ||
              j1->value == (media_val(j1)+deviatia_stand(j1)) ||
              j1->value > (media_val(j1)+deviatia_stand(j1))){
            aux = i->next;
            if(j1->timestamp==i->timestamp){
            free(remove_node(list, i));
            }
            i=aux->prev;
            }
        }
    }
    free_list(&list1);
}
//e2 create a list with 2 neighbors from left and right, ordonate them
//and return the middle
double e2(node* nod)
{   
    TDoubleLinkedList *list1;
    init(&list1);
    double aux;
    for(node* i=nod->prev->prev; i!=nod->next->next->next; i=i->next)
    {   
        int j=0;
        add(list1, j, i->timestamp, i->value);
        j++;
    }
    for(node* i=list1->head; i!=list1->tail; i=i->next)
    { 
        for(node* j=i->next; j!=NULL; j=j->next)
        {
            if(j->value>i->value)
            {
                aux=j->value;
                j->value=i->value;
                i->value=aux;
            }
        }
    }
    aux=list1->head->next->next->value;
    free_list(&list1);
    return aux;
}
//every node of the list is going through e2 and the result is saved in the new list
void remove_median(TDoubleLinkedList* list)
{
    TDoubleLinkedList *list1;  
    init(&list1);          
    int j=0;
    for(node* i =list->head; i!=NULL;i=i->next){
        add(list1, j, i->timestamp, i->value);
        j++;  
    }
    for(node* j1 =list1->head->next->next; j1!=list1->tail->prev;j1=j1->next){
        for(node* i=list->head->next->next; i!=list->tail->prev;i=i->next){
            if(j1->timestamp==i->timestamp){
                i->value=e2(j1);
            }
        }
    }
    free(remove_node(list, list->head));
    free(remove_node(list, list->head));
    free(remove_node(list, list->tail));
    free(remove_node(list, list->tail));
    free_list(&list1);

}
void remove_average(TDoubleLinkedList* list)
{//functia creeaza o copie a listei astfel incat elementele copiei sunt bagate
    TDoubleLinkedList *list1;   //in functia media_val care calculeaza media
    init(&list1);       //aritmetica a fiecarei fereastre de 5 noduri iar
    int j=0;            //rezultatul se adauga in lista originala
    for(node* i =list->head; i!=NULL;i=i->next){
        add(list1, j, i->timestamp, i->value);
        j++;  
    }
    for(node* j1 =list1->head->next->next; j1!=list1->tail->prev;j1=j1->next){
        for(node* i=list->head->next->next; i!=list->tail->prev;i=i->next){
            if(j1->timestamp==i->timestamp){
                i->value=media_val(j1);
            }
        }
    }
    free(remove_node(list, list->head));
    free(remove_node(list, list->head));
    free(remove_node(list, list->tail));
    free(remove_node(list, list->tail));
    free_list(&list1);
}
//prelucrate data to be more uniform
void u(TDoubleLinkedList* list)
{
    for(node* i =list->head->next; i!=NULL;i=i->next){
        if(i->timestamp - i->prev->timestamp<=1000 && 
           i->timestamp - i->prev->timestamp>=100){
            i->value=(i->prev->value + i->value)/2;
            i->timestamp=(i->prev->timestamp + i->timestamp)/2;
        }
    }   
}
double w(double i)
{
    double x=0, y=0;
    x=i*i/4*0.9 + 0.1;
    for(double j=0; j<3; j=j+1)
    {
        y=y+(((j*j/4)*0.9) + 0.1);
    }
    return x/y;
}
double C(int timestamp, int left_timestamp, int right_timestamp)
{
    return (double)(timestamp - left_timestamp)/
                    (right_timestamp - left_timestamp);
}
double f(double c,double l0,double l1,double l2,double r0,double r1,double r2)
{
    double F;   
    F=(1-c)*(l0 * w(0)+ l1 * w(1) + l2 * w(2)) + c * (r0 * w(0) + r1 * w(1) + r2 * w(2));
    return F;
}
//sort the list accordint to timestamp(time)
void sorting(TDoubleLinkedList* list)
{
    for(node* i=list->head; i!=NULL;i=i->next)
    {
        for(node* j=list->head; j!=NULL; j=j->next)
        if(i->timestamp < j->timestamp){
            double aux;
            int auxi;
            auxi=j->timestamp;
            j->timestamp=i->timestamp;
            i->timestamp=auxi;
            aux=j->value;
            j->value=i->value;
            i->value=aux;
        }
    }
}
//when we find a gap bigger than one second we add new node for every 0.2 seconds
void complete(TDoubleLinkedList* list)
{
    int k=2;   
    for(node* i=list->head->next; i!=list->tail->prev;i=i->next){
        if(i->timestamp - i->prev->timestamp > 1000){
            int left_timestamp, right_timestamp;
            left_timestamp = i->prev->timestamp;
            right_timestamp = i->timestamp;
            double l0, l1, l2, r0, r1, r2;
            l0=i->prev->prev->prev->value;
            l1=i->prev->prev->value;
            l2=i->prev->value;
            r0=i->next->next->value;
            r1=i->next->value;
            r2=i->value;
            for(int j=left_timestamp+200; j<right_timestamp; j=j+200){
                double c, v;
                c=C(j, left_timestamp, right_timestamp);
                //calculate the value for the new node
                v=f(c,l0,l1,l2,r0,r1,r2);
                k++;
                add(list,k,j,v);
                
            }//after we fill a gap we sort the list to prevent bugs
            sorting(list);
        }
    }
}
//this function divides the timestamp interval into several intervals 
//depending on n, and displays how many nodes are in every interval
void st(TDoubleLinkedList* list, int n)
{
    double max=list->head->value, min=list->head->value;
    for(node* i =list->head; i!=NULL;i=i->next){
        //naigate the list and search for min and max to make the
        //timestamp interval
        if(max<i->value){   
            max=i->value;   
        }
        if(min>i->value){
            min=i->value;
        }
    }//convert double data to int
    double max1, min1;
    max1=max;   
    min1=min;   
    max= round(max);
    min= round(min);
    if(min1 < min){
        min=min-1;
    }
    if(max1 > max){
        max=max+1;
    }
    int max2, min2;
    max2=(int)max;
    min2=(int)min;
    //divide in more intervals
    for(int i=min2;i<=max2;i=i+n){
        int k=0;   
        for(node* j =list->head; j!=NULL;j=j->next){ 
            //navigate through the interval and search for nodes
            if(j->value>=i && j->value<=i+n){ 
                //k->how many nodes are in that interval
                k++;    
            }
        }
        if(k!=0){
        //display the intervals and the numer of nodes/interval  
            printf("[%d, %d] %d\n", i, i+n, k);
        }                                      
    }
}
int main(int argc, char ** argv)
{
    TDoubleLinkedList *lista1;
    init(&lista1);  //initialize the list
    int n,x;
    double y;
    scanf("%d",&n); //read the lenght of list
    for(int i=0;i<n;i++)
    {
        scanf("%d",&x);     //read value and timestamp data and add them in 
        scanf("%lf",&y);    //nodes
        add(lista1,i,x,y);
    }
    int ok=0;  //it helps at st function
    for(int i=1; i<argc; i++){  //read the arguments and call for functions
        if(strcmp(argv[i], "--e1")==0){
           e1(lista1); 
        }                                                       
        if(strcmp(argv[i], "--e2")==0){
            remove_median(lista1); 
        }                                                        
        if(strcmp(argv[i], "--e3")==0){
          remove_average(lista1);
        }                                            
        if(strcmp(argv[i], "--u")==0){
            u(lista1);  
        }
        if(strcmp(argv[i], "--c")==0){
            complete(lista1); 
        }
        if(strstr(argv[i], "--st")){
            ok=1;   
            int v;
            v=atoi(argv[i]+4);
            st(lista1, v);
        }
    }//because st function have a different display, if st is called
    //we don't want to display the list
    if(ok==0){//if ok=0, st wasn't caled and we could display the list 
        printf("%d\n", lista1->len);
        print_list(lista1); 
    }
    free_list(&lista1);
    return 0;
}