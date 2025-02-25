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

//if an error oucur print the error number n and if there state or transition Error send it as string st to print,after print exit from the peogram

void Error(ll n,string st=""){
 vector<vector<string>>Erroroutput(6,vector<string>(2));
 Erroroutput[1][0]="E1: A state '";Erroroutput[1][1]="' is not in the set of states";
 Erroroutput[2][0]="E2: Some states are disjoint";Erroroutput[2][1]="";
 Erroroutput[3][0]="E3: A transition '";Erroroutput[3][1]="' is not represented in the alphabet";
 Erroroutput[4][0]="E4: Initial state is not defined";Erroroutput[4][1]="";
 Erroroutput[5][0]="E5: Input file is malformed";Erroroutput[5][1]="";
 cout<<"Error:\n"<<Erroroutput[n][0]<<st<<Erroroutput[n][1];
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
    cout<<"Error:\nE5: Input file is malformed";
    exit(0);
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

void build_fsa(vector<string>& trans){

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
            string d ="E1: A state ";
    Error(1,st[2]);
         //

    }

fsa[st[0]].push_back(st[2]);
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
 if(warning)
cout<<"Warning:\n";
for(auto x:Warnings){
    if(x.first==1)
        cout<<x.second<<"\n";
}
}
int main() {
   go;
    freopen("fsa.txt", "r", stdin);
   freopen("result.txt", "w", stdout);
   string state,alph,init,fi,tran;
   cin>>state>>alph>>init>>fi>>tran;
   vector<string>states=split("states=[",state),alpha=split("alpha=[",alph),init_st=split("init.st=[",init),fi_st=split("fin.st=[",fi),trans=split("trans=[",tran);
check_states(states);
  check_alpha(alpha);
if(init_st.size()>1){
 Error(5);

}
if(init_st.size()==0){
     cout<<"Error:\nE4: Initial state is not defined";
    exit(0);
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

build_fsa(trans);
 is_disjoint(init_st[0]);

complete_deterministic(alpha.size());
is_all_states_reachable(init_st[0]);
dfs_reachable(init_st[0]);

if(is_complete==1)cout<<"FSA is complete\n";
else cout<<"FSA is incomplete\n";
print_warning();
}
