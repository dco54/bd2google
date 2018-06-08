#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <ctime>
using namespace std;
int nVal;
int nPointer;
struct node {
    bool leaf;
    bool isRoot;
    node *par;
    vector<float>value;
    vector<node*>child;

    node *last;
};

node* getTargetNode(node *tNode, float val){
    if(tNode->leaf) return tNode;
    float i;
    for(i=0;i<tNode->value.size();i++){
        if(tNode->value[i]>val) break;
    }
    return getTargetNode(tNode->child[i],val);
}

node *Root=NULL;
node* getNewNode(bool isLeaf,bool isRoot){
    node* tmp = new node;
    tmp->isRoot = isRoot;
    tmp->leaf = isLeaf;
    tmp->last = NULL;
    return tmp;
}




void insertInParentNode(node *n, float kprime, node *nprime){
    if(n->isRoot){
        Root = getNewNode(false,true);
        n->isRoot=false;

        Root->child.push_back(n);
        Root->child.push_back(nprime);
        Root->value.push_back(kprime);
        n->par = Root;
        nprime->par = Root;
    }else{
        node *p = n->par;
        float i;
        for(i=0;i<p->value.size();i++){
            if(p->value[i]>kprime) break;
        }
        float tmpK;
        node *tmpP;

        for(float j = i; j<p->value.size(); j++){
            tmpK = p->value[j];
            tmpP = p->child[j+1];

            p->value[j] = kprime;
            p->child[j+1] = nprime;

            kprime = tmpK;
            nprime = tmpP;
        }
        p->value.push_back(kprime);
        p->child.push_back(nprime);
        nprime->par = p;


        //printf("\n\np->child size: %d\n\n",p->child.size());

        if(p->child.size()>nPointer){
            node *pprime = getNewNode(false,false);
            float nbytwoceil = (nPointer+1)/2;
            float kdoubleprime = p->value[nbytwoceil];
            for(i = nbytwoceil+1; i<p->value.size();i++){
                pprime->child.push_back(p->child[i]);
                p->child[i]->par = pprime;
                pprime->value.push_back(p->value[i]);
            }
            pprime->child.push_back(p->child[i]);
            p->child[i]->par = pprime;


            p->value.erase(p->value.begin()+nbytwoceil,p->value.end());
            p->child.erase(p->child.begin()+nbytwoceil+1,p->child.end());

            insertInParentNode(p,kdoubleprime,pprime);
        }
    }

}


void insertInLeafNode(node *leafNode, float k, node *p){
    float i;
    for(i=0;i<leafNode->value.size();i++){
        if(k<leafNode->value[i]) break;
    }
    float tmpK;
    node *tmpP;

    for(float j = i; j<leafNode->value.size(); j++){
        tmpP = leafNode->child[j];
        tmpK = leafNode->value[j];

        leafNode->child[j] = p;
        leafNode->value[j] = k;

        p = tmpP;
        k = tmpK;
    }
    leafNode->child.push_back(p);
    leafNode->value.push_back(k);

}


void insert2(float k, node *p){
    node *leafNode;
    if(Root==NULL){
        Root = getNewNode(true,true);
        leafNode = Root;
    }else leafNode = getTargetNode(Root,k);

    float keyValueCount = leafNode->value.size();
    if(keyValueCount<nVal) insertInLeafNode(leafNode,k,p);
    else{
        node* leafNode2 = getNewNode(true,false);
        insertInLeafNode(leafNode,k,p);
        leafNode2->last = leafNode->last;
        leafNode->last=leafNode2;
        float nbytwoceil = (nPointer+1)/2;

        for(float i = nbytwoceil; i<nPointer ; i++){
            leafNode2->child.push_back(leafNode->child[i]);
            leafNode2->value.push_back(leafNode->value[i]);
        }
        leafNode->value.erase(leafNode->value.begin()+nbytwoceil,leafNode->value.end());
        leafNode->child.erase(leafNode->child.begin()+nbytwoceil,leafNode->child.end());

        float kprime = leafNode2->value[0];
        insertInParentNode(leafNode,kprime,leafNode2);
    }
}

void valueOfNodeInBox(node* tNode){
    printf(" [");
    float i ;
     for(i=0; i<tNode->value.size()-1;i++){
        printf("%f|",tNode->value[i]);
    }
    if(tNode->value.size()>0) printf("%f]",tNode->value[i]);
    //printf(" ");
}

struct pNode{
    node *tNode;
    bool nl;
    pNode(node *Node, bool b){
        tNode = Node;
        nl = b;
    }
    pNode(){

    }
};

queue<pNode>q;
void bfsTraverse(node *tNode){

    q.push(pNode(tNode,true));
    while(!q.empty()){
        pNode p = q.front();
        //printf("  Got pNode ");
        node *temp = p.tNode;
        q.pop();
        valueOfNodeInBox(temp);
        //printf(" printed temp ");
        if(p.nl) printf("\n");
        float i;
        if(!temp->leaf){
        for(i=0;i<temp->child.size()-1;i++){
            q.push(pNode(temp->child[i],false));
        }
        //printf(" inserted second last child  ");
        if(p.nl) q.push(pNode(temp->child[i],true));
        else q.push(pNode(temp->child[i],false));
        //printf(" inserted  last child  ");
        }

    }
}
bool valueOfNodeInBox1(node* tNode,float value){
    float i ;
     for(i=0; i<tNode->value.size()-1;i++){
        if (tNode->value[i]==value)    return true;
    }
    return false;
}


bool Search(node *tNode,float value){

    q.push(pNode(tNode,true));
    while(!q.empty()){
        pNode p = q.front();
        //printf("  Got pNode ");
        node *temp = p.tNode;
        q.pop();
        if (valueOfNodeInBox1(temp,value))    return true;

        //printf(" printed temp ");
        if(p.nl) printf("");
        float i;
        if(!temp->leaf){
        for(i=0;i<temp->child.size()-1;i++){
            q.push(pNode(temp->child[i],false));
        }
        //printf(" inserted second last child  ");
        if(p.nl) q.push(pNode(temp->child[i],true));
        else q.push(pNode(temp->child[i],false));
        //printf(" inserted  last child  ");
        }

    }
    return false;
}


int main(){
    printf("How many values in each node?\n");
    FILE *fp;
    //fp = fopen("factura2.txt","r");
    fp = fopen("part-00000-of-00500.csv","r");
    nPointer=1000.0;   //CANTIDAD DE NODOS EN UN ARREGLO
    nVal = nPointer-1;

    printf("nPointer = %d\n",nPointer);
    float len,c;
    float k;
    /*
    fseek ( fp , 2 , SEEK_CUR );
    fscanf(fp,"%d",&k);
    cout<<"K: "<<k<<endl;
    insert2(k,NULL);
    */

    int contador=0;
    int campo=2;
    while((c=fgetc(fp))!=EOF){
        if (c==','){
        contador++;}
        if (contador==campo){
            fscanf(fp,"%f",&k);
            len=ftell(fp);
            insert2(k,NULL);
        }
        if (c=='\n'){
            contador=0;
        }
        /*
        fseek(fp,2,SEEK_CUR);
        fscanf(fp,"%d",&k);
        len = ftell(fp);
        insert2(k,NULL);
        */

    }
    while(true){
        printf("Action: \npress 1 to insert\npress 2 to print in tree structure\npress 3 to search a value\npress 0 for exit\n");
        int choice;
        scanf("%d",&choice);
        if(choice==1){
            float value;
            scanf("%f",&value);
            insert2(value,NULL);
        }else if(choice==2){
            printf("\n\n\n");
            bfsTraverse(Root);
            printf("\n\n\n");
        }
        else if(choice==3){
            float num;
            unsigned t0,t1;
            cout<<"Que valor desea buscar: ";    cin>>num;
            t0=clock();
            if(Search(Root,num)==true)    cout<<"Numero encontrado."<<endl;
            else    cout<<"Numero no encontrado."<<endl;
            t1=clock();
            double time1=(double(t1-t0)/CLOCKS_PER_SEC);
            cout<<"Tiempo de busqueda: "<<time1<<endl;
        }else if(choice==0) break;

    }
    fclose(fp);
    return 0;
}
