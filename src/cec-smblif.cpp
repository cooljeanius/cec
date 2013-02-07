#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct ff {
  string name;
  ff(string name) : name(name) {};
};

struct sm {
  string name;
  int no_states, no_ff;
  int bootstate;
  vector<vector<char> > enc_table;
  vector<ff*> flipflops;
  sm(string name, int no_states, int no_ff): name(name), no_states(no_states), no_ff(no_ff),
					     bootstate(-1) {};
};

vector<sm*> machines;


void m2blif(sm* m, ostream &o)
{
  int i,j,k;

  //  cerr<<"building "<<m->name<<" no_states:"<<m->no_states<<" no_ff:"<<m->no_ff<<"\n";

  // the decoder
  for(i=0;i<m->no_states; i++){
    o <<".names ";
    for(j=0;j<m->no_ff;j++)
      if( m->enc_table[i][j] != '-' ) o << m->flipflops[j]->name<<"_q ";
    o << m->name <<"_chk_"<<i<<"\n";  

    for(j=0;j<m->no_ff; j++)
      if( m->enc_table[i][j] != '-') o<<m->enc_table[i][j];
    o<<" 1\n";
  }

  // the encoder
  for(j=0;j<m->no_ff;j++){
    
    // header
    o<<".names "<<m->name<<"_hold "<<m->flipflops[j]->name<<"_q ";
    for(i=0; i<m->no_states; i++)
      if(m->enc_table[i][j] == '1') o << m->name <<"_goto_"<<i<<" ";
    o<<m->flipflops[j]->name<<"_d\n";

    /*
    // hold
    o<<"10";
    for(i=0; i<m->no_states; i++) 
      if(m->enc_table[i][j]=='1') o<<"-";
    o<<" 0\n";

    // normal op
    o<<"0-";
    for(i=0; i<m->no_states; i++)
      if(m->enc_table[i][j] == '1') o << "0";
    o<<" 0\n";
    */

    // hold
    o<<"11"; for(i=0; i<m->no_states; i++) if(m->enc_table[i][j]=='1') o<<"-"; o<<" 1\n";
    // normal op
    for(i=0; i<m->no_states; i++) if(m->enc_table[i][j]=='1'){
      o<<"--"; 
      for(k=0; k<m->no_states; k++) if(m->enc_table[k][j]=='1')
	if(k==i) o<<"1"; else o<<"-";
      o<<" 1\n";
    }

  }

  // build flip - flops; TO DO: ff sharing !!!

  for(j=0;j<m->no_ff;j++)
    o<<".latch "<<m->flipflops[j]->name<<"_d "<<m->flipflops[j]->name<<"_q "<<
     (m->enc_table[m->bootstate][j]=='1')<<"\n";



}



int main(int argc, char *argv[])
{

  char buf[1024];

  if(argc != 3 ){ cerr <<"Usage: "<<argv[0]<<"smfile bliffile\n"; exit(-1); }
  ifstream smfile(argv[1]); if(!smfile.good()) exit(-1);
  ofstream bliffile(argv[2]); if(!bliffile.good()) exit(-1);

  string kw, tmp;

  string sm_name;
  int no_states, no_ff, st, i;
  sm *csm;

  while(1){
    smfile.getline(buf, 1024);
    if(smfile.fail()) break;
    if(!buf[0] || buf[0]=='#') continue;

    istringstream is(buf);

    is >> kw;

    if(kw == "begin"){
      is >> sm_name;
      is >> no_states;
      is >> no_ff;
      csm = new sm(sm_name, no_states, no_ff);
      csm -> enc_table.resize(no_states);
      for(i=0; i<no_states; i++) csm -> enc_table[i].resize(no_ff);
      csm->flipflops.resize(no_ff);
      machines.push_back(csm);
    }

    if(kw == "flipflops"){
      is >> tmp;
      for(i=0;i<csm->no_ff;i++){
	is >> tmp;
	csm->flipflops[i] = new ff(tmp);
      }
    }

    if(kw == "bootstate"){
      is >> tmp;
      is >> csm->bootstate;
    }

    if(kw == "state"){
      is >> st; is >> tmp;
      for(i=0;i<csm->no_ff;i++)
	is >> csm->enc_table[st][i];
    }

    if(kw == "end"){
    }

  }


  // now the d/s is in memory, dump blif;

  for(vector<sm*>::iterator ism = machines.begin(); ism!= machines.end(); ism++){
    m2blif(*ism, bliffile);
  }

  return 0;

}
  
