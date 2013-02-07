#include "pdg2beeblebrox.hpp"

state_machine::state_machine(pdg2beeblebrox *owner, STNode *st) : owner(owner), st(st)
{

  int i;

  boot = -1;
  no_states = st->children.size();
  checks.resize(no_states);
  enters.resize(no_states);

  if(no_states == 1) checks[0]=w_1; else

    for(i=0; i<no_states; i++){
      stringstream name;
      name << "SM"<<owner->stmap[st]<<"_chk_"<<i;
      checks[i] = new wire(name.str());
      checks[i]->monitor = 2;
    }
}

void state_machine::addEnter(int no, wire *flow)
{
  enters[no].push_back(flow);
}
wire *state_machine::chkState(int no)
{
  return checks[no];
}

void state_machine::setSuspend(wire *susp)
{
  suspend = susp;
}


void state_machine::onehot()
{
  ostream &o = owner->opt.smfile;
  int sm_id, i, j;

  sm_id = owner->stmap[st];
  o<<"# ST node "<<sm_id<<", "<<no_states<<" state[s], "<<no_states<<" ffs  onehot\n";
  o<<"begin SM"<<sm_id<<" "<<no_states<<" "<<no_states<<"\n";
  
  o<<"flipflops : ";
  for(i=0;i<no_states;i++){
    o<<"ff_"<<sm_id<<"_"<<i<<" ";
  }
  o<<"\n";

  o<<"bootstate : "<< ( boot>0 ? boot : 0) <<"\n";;

  for(i=0;i<no_states;i++){
    o<<"state "<<i<<" : ";
    for(j=0; j<no_states; j++) o<<(i==j ? "1  " : "-  ");
    o<<"\n";
  }

  o<<"end\n";
}

#define LAST 0

char *enc2[2]={"0","1"};
char *enc3[3]={"1 -","- 1","0 0"};
char *enc4[4]={"1 0","1 1","0 1","0 0"};

void state_machine::fixencoding(int no_ff, char **enc)
{
  ostream &o = owner->opt.smfile;
  
  int sm_id, i;
  sm_id = owner->stmap[st];

  o<<"# ST node "<<sm_id<<", "<<no_states<<" state[s], "<<no_ff <<" ffs  fixed \n";
  o<<"begin SM"<<sm_id<<" "<<no_states<<" "<<no_ff <<"\n";

  o<<"flipflops : ";
  for(i=0;i<no_ff;i++){
    o<<"ff_"<<sm_id<<"_"<<i<<" ";
  }
  o<<"\n";

  o<<"bootstate : "<< ( boot>0 ? boot : 0) <<"\n";;
  
   for(i=0;i<no_states ;i++){
     o<<"state "<<i<<" : "<<enc[i]<<"\n";
  }

   o<<"end\n";

}

void state_machine::lastzero()
{
  ostream &o = owner->opt.smfile;
  int sm_id, i, j, no_ff;

  sm_id = owner->stmap[st];

  no_ff = no_states - 1;
  o<<"# ST node "<<sm_id<<", "<<no_states<<" state[s], "<<no_ff <<" ffs  lastzero \n";
  o<<"begin SM"<<sm_id<<" "<<no_states<<" "<<no_ff <<"\n";
  
  o<<"flipflops : ";
  for(i=0;i<no_ff;i++){
    o<<"ff_"<<sm_id<<"_"<<i<<" ";
  }
  o<<"\n";

  o<<"bootstate : "<< ( boot>0 ? boot : 0) <<"\n";;
  

#if LAST
  
  for(i=0;i<no_states - 1 ;i++){
    o<<"state "<<i<<" : ";
    for(j=0; j<no_ff; j++) o<<(i==j ? "1  " : "-  ");
    o<<"\n";
  }
  o<<"state "<<no_states-1<<" : ";
  for(j=0;j<no_ff; j++) o<<"0  ";
  o<<"\n";
  
#else

  o<<"state "<<0<<" : ";
  for(j=0;j<no_ff; j++) o<<"0  ";
  o<<"\n";
  for(i=1;i<no_states;i++){
    o<<"state "<<i<<" : ";
    for(j=0; j<no_ff; j++) o<<(i-1==j ? "1  " : "-  ");
    o<<"\n";
  }

#endif

  o<<"end\n";  

}

#define GOTO_OUT 0
// quick hack: one hot encoding
void state_machine::build_circuit()
{
  int sm_id, i;

  if(no_states == 1) return;
  /*  
  switch(no_states){
  case 2: fixencoding(1,enc2); break;
  case 3: fixencoding(2,enc3); break;
  case 4: fixencoding(2,enc4); break;
  default: onehot(); 
  }
  */

  //  onehot();

    if(no_states <= 4) lastzero(); else onehot();

  // now builds the control signals, i.e. goto's
  vector<wire*> en;
  wire *w;
  sm_id = owner->stmap[st];

  en.resize(no_states);
  for(i=0;i<no_states;i++){
    en[i] = current ->build_orgate(enters[i]);
    stringstream name;
    name << "SM"<<sm_id<<"_goto_"<<i;
    w = new wire(name.str()); 
    w->monitor = 2;
    w->required = 1;
    if(GOTO_OUT) w->output = 1;
    new strap(w, en[i] ? en[i] : w_0 );
  }

  stringstream spname;
  spname << "SM"<<sm_id<<"_hold";
  w = new wire(spname.str());
  w->monitor = 2;
  w->required = 1;
  //debug
  if(GOTO_OUT) w->output = 1;
  new strap(w, suspend);

}

// quick hack: one hot encoding
void state_machine::build_circuit_ff()
{

  int sm_id, i;

  if(no_states == 1) return;

  sm_id = owner->stmap[st];
  
  // now builds the control signals, i.e. goto's

  vector<wire*> en;
  wire *w;

  en.resize(no_states);
  for(i=0;i<no_states;i++)
    en[i] = current ->build_orgate(enters[i]);


  for(i=0;i<no_states; i++){
    w = current->build_orgate(en[i], current->build_andgate(suspend, checks[i]));
    d_ff *ff = new d_ff(checks[i], w ? w : w_0, "D", sm_id*100+i);
    if(boot == i) ff -> init = 1;
  }

}
