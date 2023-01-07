#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;

struct node* retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id=id;
    user->friends=(int*)malloc(MAX * sizeof(int));
    strcpy(user->name,name);


    //adding user's friend's IDs
    token = strtok(strfriends,"|");  
    int i=0;
    while( token != NULL ) 
    {

        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}


//search for user with id=key
struct node* search(int key, struct node *users)
{
   if(users==NULL)
		return users;
	else if(key == users->id)
		return users;
	else if(key < users->id)
		return search(key,users->left);
	else 
		return search(key,users->right);
}

void AdditionOfFriend(struct node* user,struct node* users){
    int i=0,j;
    while(i<user->numfren){
        node *p = search(user->friends[i],users);
        if(p == NULL){
            for(j=i;j<user->numfren-1;j++){
                user->friends[j]=user->friends[j+1];
            }
            user->numfren--; 
        }
        else{
            p->friends[p->numfren++]=user->id;
            i++;
        }
    }
    if(user->numfren==0){
        user->friends[0]=-1;
    }
}


//see document for explanattion
struct node* refineUser(struct node*user, struct node *users)
{
    if(users == NULL){
        AdditionOfFriend(user,users);
        return user;
    }
    int x=-1;
    while(x){
        struct node *temp = search(user->id,users);
        if(temp == NULL){
            AdditionOfFriend(user,users);
            x=0;
        }
        else{
            user->id += 1;
        }
    }
    return user;
}
//insert user with id
struct node* insertUser(struct node*root,int id,struct node*user)
{

    if(root==NULL) { 
        return user;
    }
    else if(id<root->id) { 
        root->left=insertUser(root->left,id,user);
    }
    else { 
        root->right=insertUser(root->right,id,user);
    }
    user->left=user->right=NULL;
    return root;
}

//prints friends list
void friends(int id, struct node *users) 
{
   node *p=search(id,users);
   if(p==NULL) { 
    return;
   }
   if(p->numfren==0) { 
    printf("-1\n");
   }
   else { 
    for(int i=0;i<p->numfren;i++) { 
        printf("%d\n",p->friends[i]);
    }
   }
}

//find child node with minimum value (inorder successor) - helper to delete node
struct node *minValueNode(struct node *node) {
    while(node && node->left) { 
        node=node->left;
    }
    return node;
}

void deleteElement(int arr[],int n,int x)
{
int i;
for (i=0;i<n;i++)
    if (arr[i]==x)
        break;
        
if (i<n)
{
    n=n-1;
    for (int j=i;j<n;j++)
        arr[j]=arr[j+1];
}
 
}

//deletes itself from its friend's nodes
struct node *deleteFriends(int key, struct node*users)
{
    node *p=search(key,users);
    if(p==NULL) { 
        return p;
    }
    else { 
        for(int i=0;i<p->numfren;i++) {
          node *q=search(p->friends[i],users);
          if(q!=NULL) {
                deleteElement(q->friends,q->numfren,p->id);
                q->numfren--;
          }
        }
    }
    return NULL;
} 

// Deleting a node
struct node *deleteNode(struct node *root, int key) {

    node *p=root;
    node *q=NULL;
    if(p==NULL) { 
        return p;
    }
    else if (key<p->id) { 
        p->left=deleteNode(p->left,key);
    }
    else if(key>p->id) { 
        p->right=deleteNode(p->right,key);
    }
    else { 
        if(p->left==NULL) {     
            q=p->right;
            free(p);
            return q;
        }
        else if(p->right==NULL) { 
            q=p->left;
            free(p);
            return q;
        }
        else { 
            q=minValueNode(p->right);
            p->id=q->id;
            p->numfren=q->numfren;
            strcpy(p->name,q->name);
            p->friends=q->friends;
            p->right=deleteNode(p->right,q->id);
        }
    }
    return p;
}

//Print USER's IDs in ascending order
void printInOrder(node* myusers) 
{
    if(myusers!=NULL) {
    printInOrder(myusers->left);
    printf("%d %s\n",myusers->id,myusers->name);
    printInOrder(myusers->right);
    }
}


int main(int argc, char **argv)
{
    node *users=NULL;   
    while(1)
    {

        int opt, id;
        fflush(stdin);
        scanf("%d",&opt);
        char str[MAX];
        switch (opt)
        {
            case 1:
      
                scanf("%s",str);
                struct node*tbins=retUser(str);
                tbins=refineUser(tbins, users);
                users=insertUser(users,tbins->id,tbins);
                break;

            case 2:
           
                scanf("%d", &id);
                deleteFriends(id, users);
                users=deleteNode(users, id);
                break;

            case 3:
        
                scanf("%d", &id);
                node* result=search(id,users);
                if(result==NULL) 
                    printf("USER NOT IN TREE.\n");
                else{
                    printf("%d\n",result->id);
                }
                break;

            case 4:
                scanf("%d", &id);
                friends(id, users);
                break;

            case 5:
                printInOrder(users);
                break;

            case 6:
                exit(0);
                break;

            default: 
                printf("Wrong input! \n");
                break;
        }

    }
    return 0;
}