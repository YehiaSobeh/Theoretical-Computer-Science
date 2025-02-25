//Yehia sobeh
#include<bits/stdc++.h>
using namespace std;
#define go std::ios_base::sync_with_stdio(0); cin.tie(NULL); cout.tie(NULL)
#define ll long long
#define ull unsigned long long
#define db double
#define ld long double
// Array to determine the warning in the code if the key 0 this warning dose not exist ,1 this warning dose exist
vector<pair<ll,string>>Warnings={{0,"W1: Accepting state is not defined"},{0,"W2: Some states are not reachable from the initial state"},{0,"W3: FSA is nondeterministic"}};
// Array to determine if the FSA is complete or not
vector<pair<ll,string>>Report={{0,"FSA is complete"},{0,"FSA is incomplete"}};
// map to build the FSA as graph fsa is directed graph as input,undirected_fsa is to determine is the fsa disjoint or not
map<string,vector<string>>fsa,undirectwd_fsa;
// this map to store each state with it's transitions to determine if it is deterministic and complet or not
map<string,vector<string>>deterministic;
// bolean variables that tell us if the fsa deterministic and complet after checing the map deterministic
bool is_deterministic=1,is_complete=1;

 set<string>set_states,set_alpha;
 map<string,ll>disjoint,reachable;
set<string>set_joint;
// boolean variable isvalid if any warning oucur
bool warning=0;
map<string,int>state_number;
//vector<vector<vector<string>>>R(100,vector<vector<string>>(100,vector<string>(100,"")));
//if an error oucur print the error number n and if there state or transition Error send it as string st to print,after print exit from the peogram
void Error(ll n,string st=""){
 vector<vector<string>>Erroroutput(8,vector<string>(2));
 Erroroutput[1][0]="E4: A state '";Erroroutput[1][1]="' is not in the set of states";
 Erroroutput[2][0]="E6: Some states are disjoint";Erroroutput[2][1]="";
 Erroroutput[3][0]="E5: A transition '";Erroroutput[3][1]="' is not represented in the alphabet";
 Erroroutput[4][0]="E2: Initial state is not defined";Erroroutput[4][1]="";
 Erroroutput[5][0]="E1: Input file is malformed";Erroroutput[5][1]="";
 Erroroutput[6][0]="E7: FSA is nondeterministic";Erroroutput[5][1]="";
 Erroroutput[7][0]="E3: Set of accepting states is empty";Erroroutput[5][1]="";
 cout<<Erroroutput[n][0]<<st<<Erroroutput[n][1];
 exit(0);
}
//function to split the input string by comma
vector<string> split(string a,string b){
ll i =a.size(),s=b.size();
vector<string>ans;
if(s==i+1){
    return {};
}
if(s<=i){
   Error(5);
}
s--;
if(b[s]!=']'){
    Error(5);
}
for(ll j=0;j<i;j++){
    if(a[j]!=b[j]){
        Error(5);
    }
}

while(i<s){
        ll j=i;
if(b[j]==',')
    j++;
string tmp="";
while(j<s&& b[j]!=',' && b[j]!=']' && b[j]!='['){
    tmp+=b[j];
    j++;
}
if(tmp!="")
ans.push_back(tmp);
   i=j+1;
}
return ans;
}


void check_states(vector<string>& states){
 for(auto x:states){
        set_states.insert(x);
  for(auto y:x){
   if(!((y<='9' && y>='0') || (y>='a'&& y<='z') || (y>='A'&&y<='Z'))){
   Error(5);
   }
  }
 }
}





void check_alpha(vector<string>& alpha){
 for(auto x:alpha){
        set_alpha.insert(x);
  for(auto y:x){
   if(!((y<='9' && y>='0') || (y>='a'&& y<='z') || (y>='A'&&y<='Z')|| (y=='_'))){
    Error(5);
   }
  }
 }
}
//split the transition string by >
vector<string> split_with(string a, char b){
ll s= a.size();
ll i=0;
vector<string>ans;
ll br=0;
while(i<s){
    string tmp="";
     if(br==2){
     while(i<s){
        tmp+=a[i];
        i++;
     }
    }
    ll j=i;
    if(a[j]==b)
        j++;

    while(j<s && a[j]!=b){
        tmp+=a[j];
        j++;
    }
    br++;
    if(tmp!="")
    ans.push_back(tmp);
    i=j+1;

}
return ans;
}


// building fsa and undirected_fsa

void build_fsa(vector<string>& trans, vector<vector<vector<string>>>& R ){

 for(ll i=0;i<trans.size();i++){
    vector<string>st = split_with(trans[i],'>');
ll   st_size=st.size();

    if(st_size>=1&&set_states.find(st[0])==set_states.end()){
             Error(1,st[0]);

    }

     if(st_size>=2 &&set_alpha.find(st[1])==set_alpha.end()){


 Error(3,st[1]);



    }

    if(st_size>=3&&set_states.find(st[2])==set_states.end()){
    Error(1,st[2]);
         //

    }
   // cout<<st[0]<<" "<<st[2]<<" "<<st[1]<<"\n";
     //cout<<state_number[st[0]]<<" "<<state_number[st[2]]<<" "<<st[1]<<"\n";
fsa[st[0]].push_back(st[2]);
if(R[0][state_number[st[0]]][state_number[st[2]]] == "")
R[0][state_number[st[0]]][state_number[st[2]]] += st[1];
else{
R[0][state_number[st[0]]][state_number[st[2]]] += "|";
R[0][state_number[st[0]]][state_number[st[2]]] += st[1];
}
/*if(state_number[st[0]] == state_number[st[2]]){
 R[0][state_number[st[0]]][state_number[st[2]]] += " epslon";
}*/

    undirectwd_fsa[st[0]].push_back(st[2]);
    undirectwd_fsa[st[2]].push_back(st[0]);
set_joint.insert(st[0]);
set_joint.insert(st[2]);
    deterministic[st[0]].push_back(st[1]);
   }

}



// to check the fsa if is complete and detrminstic by checking the map determinstic
void complete_deterministic(ll alpha_siaze){
for(auto x:deterministic){
    map<string,ll>visit;
    ll dif=0;
    for(auto y:x.second){
        if(visit[y]==0){
            dif++;
            visit[y]=1;
        }
        else {
         warning=1;
            Warnings[2].first=1;
            is_deterministic=0;
        }
    }
    if(dif!=alpha_siaze){
        is_complete=0;
    }
}
if(is_deterministic==false){
    Report[2].first=1;
}
}





// this function is used when chcking if fsa is disjoint by go over each state i can reach from initial state and mark it as visited (fsa here is undircted graph to )
void dfs(string a){
disjoint[a]=1;
for(auto x:undirectwd_fsa[a]){

    if(disjoint[x]==0){

        dfs(x);
    }
}

}

//  this function is used when chcking if each state is reachable from initial state by go over each state i can reach from initial state and mark it as visited (fsa here is dircted graph as input  )
void dfs_reachable(string a){
reachable[a]=1;
for(auto x:fsa[a]){

    if(reachable[x]==0){
        dfs_reachable(x);
    }
}
}
//invoking the function dfs and print the resutl
void is_disjoint(string init_st){
 dfs(init_st);
for(auto x:set_joint){
    if(disjoint[x]==0){
     Error(2);

    }
}
}
//invoking the function dfs_reachable and print the resutl
void is_all_states_reachable(string init_st){
dfs_reachable(init_st);
for(auto x:set_joint){
    if(reachable[x]==0){
      Warnings[1].first=1;
      warning=1;
    }

}}
// printing the warning if exits and no errors
void print_warning(){
// if(warning)
//cout<<"Warning:\n";
int i = 0;
for(auto x:Warnings){
    if(x.first==1){
     if(i==0)
     Error(7);
     else Error(6);
    }
    i++;
        //cout<<x.second<<"\n";

}
}











void sorting_base_case(int nn, vector<vector<vector<string>>> R){

 
 for(int i=0;i<nn;i++){
 
    for(int j =0 ;j<nn;j++){
        if(R[0][i][j]=="")
            continue;
        vector<string>tmp;
        int jj =0;
 
        int s = R[0][i][j].size();
        while(jj<nn){
 
            string t="";
            while(jj<nn && R[0][i][j][jj]!='|'){
                t+=R[0][i][j][jj];
                jj++;
            }
            tmp.push_back(t);
            jj++;
 
        }
        sort(tmp.begin(),tmp.end());
        R[0][i][j] ="";
        if(tmp[0]!=" ")
        R[0][i][j] = tmp[0];
        for(int gg =1 ;gg<tmp.size();gg++){
                R[0][i][j] += "|";
            R[0][i][j] += tmp[gg];
        }
    }
}
}











int main() {
   go;
freopen("input.txt", "r", stdin);
  // freopen("output.txt", "w", stdout);
   string state,alph,init,fi,tran;
   cin>>state>>alph>>init>>fi>>tran;
   vector<string>states=split("states=[",state),alpha=split("alpha=[",alph),init_st=split("initial=[",init),fi_st=split("accepting=[",fi),trans=split("trans=[",tran);
check_states(states);
  check_alpha(alpha);
  if(states.size()==0){
    Error(5);
  }
/*if(init_st.size()>1){
 Error(5);

}*/
if(init_st.size()==0){
   Error(4);
}
if(fi_st.size()==0){
 Error(7);
}
if(set_states.find(init_st[0])==set_states.end()){
   Error(1,init_st[0]);

}
if(fi_st.size()>0)
for(auto x:fi_st){
    if(set_states.find(x)==set_states.end()){
      Error(1,x);

}}
else{
    warning=1;
    Warnings[0].first= 1;
}

int nn = states.size();
int nnn= nn-fi_st.size()+1;

nnn =0;
// indexing the state (map each state to integer index)
for(auto x : states){
 state_number[x] = nnn;
 nnn++;
}
// making the vector to store the result of formula in each step
vector<vector<vector<string>>>R(nn+1,vector<vector<string>>(nn+1,vector<string>(nn+1,"")));
// building the fsa and initialize the base case of the our rule
build_fsa(trans,R);

 is_disjoint(init_st[0]);

complete_deterministic(alpha.size());

print_warning();
//sorting base case 
sorting_base_case(nn,R);
//Filling each state does not need transition to go to other state
for( int i=0;i<nn;i++){
 if(R[0][i][i] == "")
 R[0][i][i]+="eps";
 else
 R[0][i][i]+="|eps";
}
//iterateing from -1 to n-1 and fill the element by the formula 
for(int k =1 ;k<=nn ; k++){

  for(int  i = 0;i<nn;i++){
   for(int j = 0 ; j<nn ;j++){



                    R[k][i][j]="(";
                    if(R[k-1][i][k-1]=="")
                    R[k][i][j]+="{}";
                        else
                    R[k][i][j]+=R[k-1][i][k-1];
                    R[k][i][j]+=")(";
                    if(R[k-1][k-1][k-1]=="")
                        R[k][i][j]+="{}";
                       else
                    R[k][i][j]+=R[k-1][k-1][k-1];
                    R[k][i][j]+=")*(";
                    if(R[k-1][k-1][j]=="")
                    R[k][i][j]+="{}";
                        else
                    R[k][i][j]+=R[k-1][k-1][j];
                    R[k][i][j]+=")|(";
                    if(R[k-1][i][j]=="")
                    R[k][i][j]+="{}";
                        else
                    R[k][i][j]+=R[k-1][i][j];
                    R[k][i][j]+=")";
      
   }
  }
 }
 int fsize= fi_st.size();
 int fs=1;
 for(int i = 0;i<fsize-1;i++){
     

    cout<<"("<<R[nn][state_number[init_st[0]]][state_number[fi_st[i]]]<<")|";

    
 }
 cout<<"("<<R[nn][state_number[init_st[0]]][state_number[fi_st[fsize-1]]]<<")\n";


}
