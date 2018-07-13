#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;
int nVal;
int nPointer;
struct feature{
    float index;
    vector<string>content;
};
struct node {
    bool leaf;
    bool isRoot;
    node *par;
    vector<feature>value;
    vector<node*>child;

    node *last;
};
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
node *Root=NULL;
queue<pNode>q;

node* getTargetNode(node *tNode, float val);
node* getNewNode(bool isLeaf,bool isRoot);

void insertInParentNode(node *n, float kprime, node *nprime);
void insertInLeafNode(node *leafNode, float k, node *p,string cadena);
void insert2(float k, node *p,string cadena);
void valueOfNodeInBox(node* tNode);
void bfsTraverse(node *tNode);
void phDelete(node* N, float k, node* p);
bool tooFewEntry(node *N);
void deleteEntry(node* N, float k, node* p);
void delet(float k, node* p);
bool isCoalesce(node *N, node *Nprime);

/* MIO */
bool valueOfNodeInBoxSearch(node* tNode,float value);
bool Search(node *tNode,float value);
bool valueOfNodeInBoxRec(node* tNode,float value);


void indexacion(int campo){
    int contador=0;
    float k,len;
    float c;
    FILE *fp;
    fp = fopen("part-00000-of-00500.csv","r");

    while((c=fgetc(fp))!=EOF){
        if (c==','){
        contador++;}
        if (contador==campo){
            fscanf(fp,"%f",&k);
            len=ftell(fp);
            //printf("VALOR: %f\n",k);
            insert2(k,NULL,"hola");
        }
        if (c=='\n'){
            contador=0;

        }
    }
    fclose(fp);
}

int main(){
    printf("How many values in each node?\n");

    nPointer = 100;
    nVal = nPointer-1;
    cout<<"nPointer: "<<nPointer<<endl;

    indexacion(2);
    while(true){
        printf("Action: \npress 1 to insert\npress 2 to print in tree structure\npress 3 for delete\npress 4 to search a value\npress 0 for exit\n");
        int choice;
        scanf("%d",&choice);
        if(choice==1){
            float value;
            cin>>value;
            string cadena;    cin>>cadena;
            insert2(value,NULL,cadena);
        }else if(choice==2){
            printf("\n\n\n");
            bfsTraverse(Root);
            printf("\n\n\n");
        }else if(choice==3){
            int delV;
            scanf("%f",&delV);
            delet(delV,NULL);
        }else if(choice==4){
            float indice;    cin>>indice;
            if(Search(Root,indice))    cout<<"Founded Number\n";
            else    cout<<"Not Founded Number\n";
        }else if(choice==0) break;
    }
    //fclose(fp);
    return 0;
}
node* getTargetNode(node *tNode, float val){
    if(tNode->leaf) return tNode;
    float i;
    for(i=0;i<tNode->value.size();i++){
        if(tNode->value[i].index > val) break;
    }
    return getTargetNode(tNode->child[i],val);
}
node* getNewNode(bool isLeaf,bool isRoot){
    node* tmp = new node;
    tmp->isRoot = isRoot;
    tmp->leaf = isLeaf;
    tmp->last = NULL;
    return tmp;
}
void insertInParentNode(node *n, float kprime, node *nprime){
   // printf("dbg: reached insertinparennode\n");
//
    if(n->isRoot){
        Root = getNewNode(false,true);
        n->isRoot=false;

        Root->child.push_back(n);
        Root->child.push_back(nprime);
        Root->value.push_back( {kprime} );
        n->par = Root;
        nprime->par = Root;
    }else{
        //printf("dbg: reached insertinparent node else\n");
        node *p = n->par;

        //printf("dbg: parent found\n");
        //printf("dbg: content of parent\n");
        //bfsTraverse(p);


        float i;
        for(i=0;i<=p->value.size();i++){
            if(p->value[i].index>kprime) break;
        }
        float tmpK;
        node *tmpP;

        for(float j = i; j<p->value.size(); j++){
            tmpK = p->value[j].index;
            tmpP = p->child[j+1];
            if (tmpK=kprime)    return;
            p->value[j].index = kprime;
            p->child[j+1] = nprime;

            kprime = tmpK;
            nprime = tmpP;
        }
        p->value.push_back({kprime});
        p->child.push_back({nprime});
        nprime->par = p;

        //printf("dbg: reached before split\n");

        //printf("\n\np->child size: %d\n\n",p->child.size());

        if(p->child.size()>nPointer){
            node *pprime = getNewNode(false,false);
            float nbytwoceil = (nPointer+1)/2;
            float kdoubleprime = p->value[nbytwoceil-1].index;
            for(i = nbytwoceil; i<p->value.size();i++){
                pprime->child.push_back(p->child[i]);
                p->child[i]->par = pprime;
                pprime->value.push_back({p->value[i].index});
            }
            pprime->child.push_back(p->child[i]);
            p->child[i]->par = pprime;


            p->value.erase(p->value.begin()+nbytwoceil-1,p->value.end());
            p->child.erase(p->child.begin()+nbytwoceil,p->child.end());

            insertInParentNode(p,kdoubleprime,pprime);
        }
    }

}


void insertInLeafNode(node *leafNode, float k, node *p,string cadena){
    int i;
    for(i=0;i<leafNode->value.size();i++){
        if(k<=leafNode->value[i].index) break;
    }

    feature aux;    aux.index=k;    aux.content.push_back(cadena);
    feature tmpK;
    node *tmpP;

    for(int j = i; j<leafNode->value.size(); j++){
        tmpP = leafNode->child[j];
        tmpK = leafNode->value[j];
        if(tmpK.index==k){
            leafNode->value[j].content.push_back(cadena);    return;
        }
        leafNode->child[j] = p;
        leafNode->value[j] = aux;

        p = tmpP;
        aux = tmpK;
    }
    leafNode->child.push_back(p);
    leafNode->value.push_back(aux);
}


void insert2(float k, node *p,string cadena){
    node *leafNode;
    if(Root==NULL){
        Root = getNewNode(true,true);
        leafNode = Root;
    }else leafNode = getTargetNode(Root,k);
    //printf("dbg: target node content:\n");
    //if(leafNode->value.size()>0) bfsTraverse(leafNode);

    float keyValueCount = leafNode->value.size();
    if(keyValueCount<nVal) insertInLeafNode(leafNode,k,p,cadena);
    else{
        if (valueOfNodeInBoxRec(leafNode,k)==true){insertInLeafNode(leafNode,k,p,cadena);    return;}  //LINEA MODIFICADA
        //printf("dbg: reached in else1\n");
        node* leafNode2 = getNewNode(true,false);
        insertInLeafNode(leafNode,k,p,cadena);

        //printf("dbg: inserted in leaf node\n");
       // printf("dbg: content\n");
        //bfsTraverse(leafNode);

        leafNode2->last = leafNode->last;
        leafNode2->par = leafNode->par;

        leafNode->last=leafNode2;
        float nbytwoceil = (nPointer+1)/2;

        for(int i = nbytwoceil; i<nPointer ; i++){
            leafNode2->child.push_back(leafNode->child[i]);
            leafNode2->value.push_back(leafNode->value[i]);
        }
        leafNode->value.erase(leafNode->value.begin()+nbytwoceil,leafNode->value.end());
        leafNode->child.erase(leafNode->child.begin()+nbytwoceil,leafNode->child.end());

        //printf("dbg: leafnode split complete\n");
        //printf("dbg: leafnode1 content:\n");
       // bfsTraverse(leafNode);
        //printf("dbg: leafnode2 content:\n");
        //bfsTraverse(leafNode2);

        float kprime = leafNode2->value[0].index;
        insertInParentNode(leafNode,kprime,leafNode2);
    }
}
void valueOfNodeInBox(node* tNode){
    cout<<" [";
    float i ;
     for(i=0; i<tNode->value.size()-1;i++){
        //printf("%d|",tNode->value[i].index);
        cout<<tNode->value[i].index<<"|";
    }
    if(tNode->value.size()>0) //printf("%d]",tNode->value[i].index);
        cout<<tNode->value[i].index<<"]";
    //printf(" ");
}




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

void phDelete(node* N, float k, node* p){
    float i;
    for(i=0;i<N->value.size();i++){
        if(N->value[i].index==k) break;
    }
    N->value.erase(N->value.begin()+i);
    if(!N->leaf) N->child.erase(N->child.begin()+i+1);
    printf("After ph delete contentent: \n");
    if(N->value.size()>0)bfsTraverse(N);
}


bool tooFewEntry(node *N){

    if(N->leaf){
        float minV = nPointer/2;
        if(N->value.size()<minV) return true;
    }else{
        float minC = (nPointer+1)/2;
        if(N->child.size()<minC) return true;
    }
    return false;
}

void deleteEntry(node* N, float k, node* p){
    phDelete(N,k,p);
    if(N->isRoot && N->child.size()==1){
      Root = N->child[0];
      Root->par = NULL;
      Root->isRoot = true;
    }
    if(tooFewEntry(N) && !N->isRoot){
        printf("too few entry\n");
        node *parN = N->par;
        float i;
        for(i=0;i<parN->child.size();i++){
            if(parN->child[i]==N) break;
        }
        printf("dbg: The node is %dth child of its parent\n",i);
        printf("dbg: parent has %d childs\n",parN->child.size());

        node *leftSib, *rightSib, *Nprime;
        bool left;
        if(i>0) leftSib = parN->child[i-1];
        else leftSib = NULL;

        if(i<parN->child.size()-1) rightSib = parN->child[i+1];
        else rightSib = NULL;

        if(leftSib==NULL) left=false;
        else if(rightSib==NULL) left = true;
        else if(leftSib->value.size()>rightSib->value.size()) left=false;
        else left = true;
        float kprime;

        if(left){
            Nprime = leftSib;
            kprime = parN->value[i-1].index;
            printf("Left sibling selected\n");
        }
        else {
            Nprime = rightSib;
            kprime = parN->value[i].index;
            printf("Right sibling selected\n");
        }
        printf("kprime %f\n",kprime);

        //int totalValue = N->child.size()+Nprime->child.size();
        //printf("dbg: totalValue: %d\n",totalValue);
        if(isCoalesce(N,Nprime)){
                printf("Coalesce\n");
                if(!left){
                    node *tmp = N;
                    N = Nprime;
                    Nprime = tmp;
                }
               // printf("reached after swap\n");
                if(!N->leaf){
                    Nprime->value.push_back({kprime});
                    float j;
                    for(j = 0; j<N->value.size();j++){
                        Nprime->child.push_back(N->child[j]);
                        Nprime->value.push_back(N->value[j]);
                        N->child[j]->par = Nprime;
                    }
                    Nprime->child.push_back(N->child[j]);
                    N->child[j]->par = Nprime;
                    //printf("reached not leaf complete\n");
                }else{
                     //printf("reached leaf start\n");
                    // printf("Content of N\n");
                     //bfsTraverse(N);
                     printf("Content of Nprime(before)\n");
                     bfsTraverse(Nprime);



                    for(float j = 0; j<N->value.size();j++){
                        Nprime->value.push_back(N->value[j]);
                        ///Nprime->child.push_back(N->child[j]);
                        ///N->child[j]->par = Nprime;
                    }
                     //printf("reached leaf after loop\n");
                     Nprime->last = N->last;
                    //printf("reached leaf complete\n");
                }
                printf("Nprime content(after): \n");
                bfsTraverse(Nprime);
                deleteEntry(parN,kprime,N);

        }else{
                printf("dbg: Redistribute\n");

                printf("dbg: content of Nprime\n");
                bfsTraverse(Nprime);

                if(left){
                    printf("dbg: redistribute for left\n");

                    if(!N->leaf){
                        float m = Nprime->child.size()-1;
                        float tmpV = Nprime->value[m-1].index;
                        node *tmpP = Nprime->child[m];
                        Nprime->child.erase(Nprime->child.begin()+m);
                        Nprime->value.erase(Nprime->value.begin()+m-1);
                        N->child.insert(N->child.begin(),tmpP);
                        N->value.insert(N->value.begin(),{kprime});
                        parN->value[i-1].index = tmpV;//???
                      // parN->value[i-1] = N->
                    }else{
                        float m = Nprime->value.size()-1;
                        float tmpV = Nprime->value[m].index;
                        ///node *tmpP = Nprime->child[m];
                        ///Nprime->child.erase(Nprime->child.begin()+m);
                        Nprime->value.erase(Nprime->value.begin()+m);
                        ///N->child.insert(N->child.begin(),tmpP);
                        N->value.insert(N->value.begin(),{tmpV});
                        parN->value[i-1].index = tmpV;
                    }
                }else{

                    if(!N->leaf){
                        float m = 0;
                        float tmpV = Nprime->value[m].index;
                        node *tmpP = Nprime->child[m];

                        Nprime->child.erase(Nprime->child.begin()+m);
                        Nprime->value.erase(Nprime->value.begin()+m);

                        N->child.push_back(tmpP);
                        N->value.push_back({kprime});

                       // N->child.insert(N->child.begin(),tmpP);
                        //N->value.insert(N->value.begin(),kprime);

                        parN->value[i-1].index = tmpV;
                    }else{
                        float m = 0;
                        float tmpV = Nprime->value[m].index;
                        ///node *tmpP = Nprime->child[m];
                        ///Nprime->child.erase(Nprime->child.begin()+m);
                        Nprime->value.erase(Nprime->value.begin());
                        ///N->child.insert(N->child.begin(),tmpP);
                       // N->value.insert(N->value.begin(),tmpV);
                        N->value.push_back({tmpV});
                        parN->value[i] = Nprime->value[0];
                    }

                }
        }


    }else{
        printf("Enough Entry\n");
    }

}

void delet(float k, node* p){
    node *L = getTargetNode(Root,k);
    printf("content: \n");
    bfsTraverse(L);
    deleteEntry(L,k,p);

}


bool isCoalesce(node *N, node *Nprime){
    if(N->leaf){
        if(nVal>=(N->value.size()+Nprime->value.size())) return true;
        return false;
    }
    if(nPointer>=(N->child.size()+Nprime->child.size())) return true;
    return false;
}



/* MIO NO SE SI TARA BIEN */

bool valueOfNodeInBoxRec(node* tNode,float value){
    if (tNode->leaf==0)    return false;
     for(float i=0; i<tNode->value.size();i++){
        if (tNode->value[i].index==value){
            for(int j=0;j<tNode->value[i].content.size();j++){
            return true;
        }
    }
    return false;
    }
}


bool valueOfNodeInBoxSearch(node* tNode,float value){
    if (tNode->leaf==0)    return false;
     for(float i=0; i<tNode->value.size();i++){
        if (tNode->value[i].index==value){
            cout<<"Indice: "<<tNode->value[i].index<<endl;
            for(int j=0;j<tNode->value[i].content.size();j++){
                cout<<"Referencia: "<<tNode->value[i].content[j]<<endl;}
            return true;

        }
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
        if (valueOfNodeInBoxSearch(temp,value))    return true;

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
