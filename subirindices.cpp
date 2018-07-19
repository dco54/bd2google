#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <vector>
#include <queue>
using namespace std;
int nVal;
int nPointer;
struct feature{
    long long int index;
    string archivo;
    vector<long long int>content;
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

node* getTargetNode(node *tNode, long long int val);
node* getNewNode(bool isLeaf,bool isRoot);

void insertInParentNode(node *n, long long int kprime, node *nprime);
void insertInLeafNode(node *leafNode, long long int k, node *p,string fichero,long long int cadena);
void insert2(long long int k, node *p,string fichero,vector<long long int> referencias);
void valueOfNodeInBox(node* tNode);
void bfsTraverse(node *tNode);
void phDelete(node* N, long long int k, node* p);
bool tooFewEntry(node *N);
void deleteEntry(node* N, long long int k, node* p);
void delet(long long int k, node* p);
bool isCoalesce(node *N, node *Nprime);

/* MIO */
bool valueOfNodeInBoxSearch(node* tNode,long long int value);
bool Search(node *tNode,long long int value);
feature buscarDato(node *tNode,long long int indice);

void indexacion(string fichero){
    ifstream archivo(fichero,std::ifstream::binary);
    long long int ind;
    string refe;
    vector<long long int> cont;

    if (archivo){
        int tam=fichero.size();
        fichero[tam-3]='c';
        fichero[tam-2]='s';
        fichero[tam-1]='v';
        char *buffer=new char[200];
        while(!archivo.eof()){

            archivo.getline(buffer,200,'|');
            ind=atoll(buffer);
            //cout<<"Indice: "<<ind<<endl;

            archivo.getline(buffer,5,'|');
            long long int cantidad=atoll(buffer);
            //cout<<"CANTIDAD: "<<cantidad<<endl;

            for(long long int k=0;k<cantidad;k++){
                archivo.getline(buffer,15,'|');
                long long int numero=atoll(buffer);
                cont.push_back(numero);

            }
            insert2(ind,NULL,fichero,cont);
            cont.clear();

        }
        delete[] buffer;
        archivo.close();
    }
    else{
        cout<<"El archivo no existe.\n";
    }
}
string dato(feature hola){
    string fichero=hola.archivo;
    char *buffer=new char[500];
    ifstream archivo(fichero,std::ifstream::binary);
    archivo.seekg(hola.content[0]);
    archivo.getline(buffer,500,'\n');
    string rpta=buffer;
    return rpta;
}





void inser(string s,int campos)
{
    fstream fi;
    fi.open("Tabla1/nuevo.csv");
    int con=0,i=0;
    fi.seekg(0,fi.end);

    for(int i=0; i<s.size() ; i++)
    {
        if(s[i]==',')
        {
            con++;
        }
    }
    if(con!=campos-1)
    {
        cout<<"Error: numero incorrecto de campos "<<endl;
        return;
    }
    else
    {
        fi<<s<<endl;
    }
}

void delet(feature hola)
{
    cout<<hola.archivo<<endl;
    cout<<hola.content[0]<<endl;

    string s;
    string fichero=hola.archivo;
    fstream fi;
    fi.open(fichero);
    fi.seekp(0,fi.beg);
    //cout<<fi.tellp()<<endl;
    int pos=hola.content[0];
    fi.seekp(pos,fi.cur);
    getline(fi,s);
    int tam=s.size();
    fi.seekp(0,fi.beg);
    fi.seekp(pos,fi.cur);
    s="";
    for(int i=0; i<tam; i++)
    {
        s+=' ';
    }
    fi<<s;
}


int main(){
    printf("How many values in each node?\n");

    nPointer = 100;
    nVal = nPointer-1;
    cout<<"nPointer: "<<nPointer<<endl;
    string fichero="Tabla1/part-00000-of-00500.txt";
    int i=1;
    while (i<500){
        indexacion(fichero);
        string cero="",token=to_string(i);
        if(i<=9){
            cero="0000";
        }else if(i>9 && i<=99){cero="000";}
        else if(i>99 && i<=999){cero="00";}
        fichero="Tabla1/part-"+cero+token+"-of-00500.txt";
        i++;
    }

    /*
    feature A;
    A=buscarDato(Root,6440283993);
    cout<<"\nIndice: "<<A.index;
    cout<<"\nFichero: "<<A.archivo;
    cout<<"\nReferencia: ";    for(long long int k=0;k<A.content.size();k++)     cout<<A.content[k]<<"|";
    string cadenita=dato(A);
    cout<<"\nContenido: "<<cadenita;
    */


    while(true){
        printf("\nAction: \npress 1 to insert\npress 3 to delet\n npress 4 to search a value\npress 0 for exit\n");
        int choice;
        cin>>choice;
        if (choice==1){
            string nuevacadena;
            cin>>nuevacadena;
            inser(nuevacadena,8);
        }
        if(choice==3){
            long long int indiceborrar;    cin>>indiceborrar;
            feature borrar=buscarDato(Root,indiceborrar);
            delet(borrar);
        }
        if(choice==2){
            printf("\n\n\n");
            bfsTraverse(Root);
            printf("\n\n\n");
        }else if(choice==4){
            long long int indice;    cin>>indice;
            if(Search(Root,indice))    cout<<"\nFounded Number\n";
            else    cout<<"\nNot Founded Number\n";
        }else if(choice==0) break;
    }
    return 0;
}
node* getTargetNode(node *tNode, long long int val){
    if(tNode->leaf) return tNode;
    int i;
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
void insertInParentNode(node *n, long long int kprime, node *nprime){
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


        int i;
        for(i=0;i<=p->value.size();i++){
            if(p->value[i].index>kprime) break;
        }
        long long int tmpK;
        node *tmpP;

        for(int j = i; j<p->value.size(); j++){
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
            long long int nbytwoceil = (nPointer+1)/2;
            long long int kdoubleprime = p->value[nbytwoceil-1].index;
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

void insertInLeafNode(node *leafNode, long long int k, node *p,string fichero,vector<long long int> referencias){
    long long int i;
    for(i=0;i<leafNode->value.size();i++){
        if(k<=leafNode->value[i].index){ break; }
    }
    feature aux;    aux.index=k;    aux.archivo=fichero;    aux.content=referencias;
    feature tmpK;
    node *tmpP;

    for(long long int j = i; j<leafNode->value.size(); j++){
        tmpP = leafNode->child[j];
        tmpK = leafNode->value[j];
        leafNode->child[j] = p;
        leafNode->value[j] = aux;

        p = tmpP;
        aux = tmpK;
    }
    leafNode->child.push_back(p);
    leafNode->value.push_back(aux);
}


void insert2(long long int k, node *p,string fichero,vector<long long int> referencias){
    node *leafNode;
    if(Root==NULL){
        Root = getNewNode(true,true);
        leafNode = Root;
    }else leafNode = getTargetNode(Root,k);
    //printf("dbg: target node content:\n");
    //if(leafNode->value.size()>0) bfsTraverse(leafNode);


    long long int keyValueCount = leafNode->value.size();
    if(keyValueCount<nVal) insertInLeafNode(leafNode,k,p,fichero,referencias);
    else{

        //printf("dbg: reached in else1\n");
        node* leafNode2 = getNewNode(true,false);
        insertInLeafNode(leafNode,k,p,fichero,referencias);

        //printf("dbg: inserted in leaf node\n");
       // printf("dbg: content\n");
        //bfsTraverse(leafNode);

        leafNode2->last = leafNode->last;
        leafNode2->par = leafNode->par;

        leafNode->last=leafNode2;
        int nbytwoceil = (nPointer+1)/2;

        for(int i = nbytwoceil; i<nPointer ; i++){
            leafNode2->child.push_back(leafNode->child[i]);
            leafNode2->value.push_back(leafNode->value[i]);
        }
        leafNode->value.erase(leafNode->value.begin()+nbytwoceil,leafNode->value.end());
        leafNode->child.erase(leafNode->child.begin()+nbytwoceil,leafNode->child.end());


        long long int kprime = leafNode2->value[0].index;
        insertInParentNode(leafNode,kprime,leafNode2);
    }
}
void valueOfNodeInBox(node* tNode){
    cout<<" [";
    int i ;
     for(i=0; i<tNode->value.size()-1;i++){
        cout<<tNode->value[i].index<<"|";
    }
    if(tNode->value.size()>0)
        cout<<tNode->value[i].index<<"]";
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
        int i;

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

bool tooFewEntry(node *N){

    if(N->leaf){
        long long int minV = nPointer/2;
        if(N->value.size()<minV) return true;
    }else{
        long long int minC = (nPointer+1)/2;
        if(N->child.size()<minC) return true;
    }
    return false;
}

/* MIO NO SE SI TARA BIEN */

bool valueOfNodeInBoxSearch(node* tNode,long long int value){
    if (tNode->leaf==0)    return false;
     for(long long int i=0;i<tNode->value.size();i++){
        if (tNode->value[i].index == value){
            cout<<"Indice: "<<tNode->value[i].index<<endl;
            cout<<"Fichero: "<<tNode->value[i].archivo<<endl;
            cout<<"Referencia: ";
            for(long long int j=0;j<tNode->value[i].content.size();j++){
                cout<<tNode->value[i].content[j]<<"|";
            cout<<dato(tNode->value[i]);
            }
            return true;
        }
    }
    return false;
}

bool Search(node *tNode,long long int value){

    q.push(pNode(tNode,true));
    while(!q.empty()){
        pNode p = q.front();
        //printf("  Got pNode ");
        node *temp = p.tNode;
        q.pop();
        if (valueOfNodeInBoxSearch(temp,value)==true){   return true;   }

        if(p.nl) printf("");
        int i;
        if(!temp->leaf){
        for(i=0;i<temp->child.size()-1;i++){
            q.push(pNode(temp->child[i],false));
        }
        if(p.nl) q.push(pNode(temp->child[i],true));
        else q.push(pNode(temp->child[i],false));
        }

    }
    return false;
}
feature valueOfNodeInBoxS(node* tNode,long long int value){
    if (tNode->leaf==true){
        for(long long int i=0;i<tNode->value.size();i++){
            if (tNode->value[i].index == value){
            return tNode->value[i];
            }
        }
    }
}

feature buscarDato(node *tNode,long long int indice){
    feature rpta;
    q.push(pNode(tNode,true));
    while(!q.empty()){
        pNode p = q.front();
        //printf("  Got pNode ");
        node *temp = p.tNode;
        q.pop();

        if (temp->leaf==true){
        for(long long int j=0;j<temp->value.size();j++){
            if (temp->value[j].index == indice){
            return temp->value[j];
            }
        }
        }

        int i;
        if(!temp->leaf){
        for(i=0;i<temp->child.size()-1;i++){
            q.push(pNode(temp->child[i],false));
        }
        if(p.nl) q.push(pNode(temp->child[i],true));
        else q.push(pNode(temp->child[i],false));
        }
    }
    cout<<"Numero no encontrado";
    return rpta;
}

