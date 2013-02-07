#include "pdg2beeblebrox.hpp"
#include <stdio.h>


void beeble_context::eval_expr(Expression *e)
{
  t = NULL; isconst = 0;
  e -> welcome(*owner);
}

void beeble_context::do_statement(Statement *s, wire *flowin){
  t = NULL; isconst = 0;
  flow = flowin;
  s->welcome(*owner);
}

Status pdg2beeblebrox::visit(Emit &s)
{
  SignalSymbol *ss;
  wire *flow;
  zaphod_signal *sig;
  
  flow = bc.getflow();
  ss=s.signal;
  sig = signals[ss]; assert(sig);
  sig -> sigemit.push_back(flow);

  return Status();
}
Status pdg2beeblebrox::visit(Exit &s) // identical to Emit
{
  SignalSymbol *ss;
  wire *flow;
  zaphod_signal *sig;
  
  flow = bc.getflow();
  ss=s.trap;
  sig = signals[ss]; assert(sig);
  sig -> sigemit.push_back(flow);

  return Status();
}


Status pdg2beeblebrox::visit(LoadSignalExpression &s)
{
  SignalSymbol *ss;
    zaphod_signal *sig;
    
    ss = s.signal;
    sig = signals[ss]; assert(sig);
    
    if(ss->name == "tick") bc.setbool(w_1); else
      bc.setbool(sig -> signal);
    return Status();
}

Status pdg2beeblebrox::visit(BinaryOp &s)
{
  if (s.source1->type->name == "boolean") {
    wire *w1 = bc.getbool(s.source1);
    wire *w2 = bc.getbool(s.source2);
  
    if (s.op == "and") {
      bc.setbool ( current->build_andgate(w1, w2));
    } else if (s.op == "or") {
      bc.setbool ( current->build_orgate(w1, w2));
    } else {
      std::cerr << "Warning: BinaryOp " << s.op << "not supported\n";
      // a hack: shouldn't be this
      bc.setbool(w_0);
    }
  } else {
    std::cerr << "Warning: BinaryOp " << s.op << "not supported\n";
    bc.setbool(w_0);
  }
  
  return Status();
}

Status pdg2beeblebrox::visit(UnaryOp &s)
{
  wire *win;
  
  win = bc.getbool(s.source);
    if(s.op == "not"){
      bc.setbool(current-> build_notgate(win));
      return Status();
    }
    
    std::cerr<<"UnaryOp "<<s.op<<"not supported\n";
    assert(0);
    return Status();
}

Status pdg2beeblebrox::visit(Literal &s)
{
  // std::cerr << "Literal " << s.value << ' ' << s.type->name << '\n';
  if ( s.type->name == "boolean" ) {
    bc.setbool( (s.value == "1") ? w_1 : w_0 );
  } else if ( s.type->name == "integer" ) {
    int x;
    if(sscanf(s.value.c_str(),"%d",&x)!=1) assert(0);
    bc.setint(x,1); // integer is constant
  } else {
    std::cerr << "Warning: unsupported literal type\n";
    bc.setbool(w_0);
  }
  return Status();
}

Status pdg2beeblebrox::visit(VariableSymbol &s)
{
  std::cerr << "Warning: unsupported variable type " << s.type->name << '\n';
  bc.setbool(w_0); // a hack
  return Status();
}

Status pdg2beeblebrox::visit(LoadVariableExpression &e) {
  BuiltinConstantSymbol *bcs =
    dynamic_cast<BuiltinConstantSymbol*>(e.variable);
  if (bcs) {
    // Either true or false
    assert(bcs->initializer);
    bc.getbool(bcs->initializer);
  } else {
    std::cerr << "Warning: unsupported variable type "
	      << e.variable->name << '\n';
    bc.setbool(w_0);
  }
  return Status();
}


Status pdg2beeblebrox::visit(StartCounter &s)
{
  beeble_counter *cnt = counters[s.counter];
  cnt->starts.push_back(bc.getflow());
  cnt->startval = bc.getintconst(s.count);
  return Status();
}

Status pdg2beeblebrox::visit(CheckCounter &s)
{
  wire *wpred = bc.getbool(s.predicate);
  beeble_counter *cnt = counters[s.counter];  

  cnt->enables.push_back(bc.flow);
  //  cnt->enables.push_back(w_1);
  cnt->signal = wpred;

  bc.setbool(cnt->alarm);
  return Status();
}



beeble_counter::beeble_counter()
{
  alarm = new wire();
}

void beeble_counter::build()
{

  //  cerr<<"builing counter... starts="<<starts.size()<<"enables="<<enables.size()<<" \n";

  wire *srst, *ffset, *ffq, *ffd, *alarm_1, *enable;

  srst = current->build_orgate(starts);
  enable = current->build_andgate(signal, current->build_orgate(enables));

  if(!srst || !enable){
    cerr<<"counter ignored\n";
    new strap(alarm, w_0);
    return;
  }



  alarm_1 = new wire();
  new counter(alarm_1, srst, enable, startval - 2);
  //debug
  // new counter(alarm_1, srst, signal, startval - 2);


  ffq = new wire();
  ffset = current->build_andgate(alarm_1, enable);
  ffd = current->build_andgate(current->build_notgate(srst),
			       current->build_orgate(ffq, ffset)
			       );
  new d_ff(ffq, ffd);

  new strap(alarm, current->build_andgate(ffq, signal));

}
