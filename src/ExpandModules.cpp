#line 1686 "ExpandModules.nw"
#include <stdio.h>
#include "ExpandModules.hpp"

namespace ExpandModules {
  using namespace IR;
  using namespace AST;
  
#line 150 "ExpandModules.nw"
const set<Module*> *FindRoots::find()
{
  // Start the set of roots off as all modules
  for ( vector<Module*>::const_iterator i = modules->modules.begin() ;
	i != modules->modules.end() ; i++ ) {
    assert(*i);
    roots.insert(*i);
  }

  // Visit each of the modules and remove any run modules from the roots set

  for ( vector<Module*>::const_iterator i = modules->modules.begin() ;
	i != modules->modules.end() ; i++ )
    recurse(*i);

  return &roots;
}
#line 186 "ExpandModules.nw"
Status FindRoots::visit(Run &s)
{
  if ( !(modules->module_symbols.contains(s.old_name)) )
    throw IR::Error("Attempt to run unknown module " + s.old_name);
  ModuleSymbol *ms =
    dynamic_cast<ModuleSymbol*>(modules->module_symbols.get(s.old_name));
  assert(ms);
  Module *m = ms->module;
  assert(m);
  roots.erase(m);
  return Status();
}
#line 1693 "ExpandModules.nw"
  
#line 445 "ExpandModules.nw"
void Copier::copySymbolTable(const SymbolTable *source, SymbolTable *dest)
{
  assert(source);
  assert(dest);

  // std::cerr << "Copying a symbol table\n";
  for ( SymbolTable::const_iterator i = source->begin() ;
        i != source->end() ; i++) {
    assert(*i);
    Symbol *news = copy(*i);
    dest->enter(news);
    formalToActual.insert( std::make_pair(*i, news) );
    // std::cerr << "Copied " << news->name << std::endl;
  }

  if (source->parent)
    dest->parent = newSymbolTable(source->parent);

  copiedSymbolTableMap.insert( std::make_pair(source, dest) );
}
#line 479 "ExpandModules.nw"
Module *Copier::copyModule(Module *m)
{
  moduleBeingCopied = m;
  assert(m->symbol);
  ModuleSymbol *ms = new ModuleSymbol(m->symbol->name);
  Module *result = new Module(ms);
  newModule = result;
  ms->module = result;

  // std::cerr << "Copying " << m->symbol->name << std::endl;

  copySymbolTable(m->types, result->types);
  copySymbolTable(m->constants, result->constants);
  copySymbolTable(m->functions, result->functions);
  copySymbolTable(m->procedures, result->procedures);
  copySymbolTable(m->tasks, result->tasks);
  copySymbolTable(m->variables, result->variables);
  copySymbolTable(m->signals, result->signals);

  for ( vector<InputRelation*>::const_iterator i = m->relations.begin() ;
	i != m->relations.end() ; i++ ) {
    assert(*i);
    result->relations.push_back(copy(*i));
  }

  for ( vector<Counter*>::const_iterator i = m->counters.begin() ;
	i != m->counters.end() ; i++ ) {
    assert(*i);
    result->counters.push_back(*i);
    copiedCounterMap[*i] = *i;
  }

  result->body = copy(m->body);
  return result;  
}
#line 547 "ExpandModules.nw"
Status Copier::visit(Run &r)
{
  assert(newModule);

  // std::cerr << "run " << r.new_name << " / " << r.old_name << std::endl;

  // Locate the old module by its name
  assert(oldModules);
  if ( !(oldModules->module_symbols.contains(r.old_name)) )
    throw IR::Error("Attempt to run unknown module " + r.old_name);

  ModuleSymbol *ms =
    dynamic_cast<ModuleSymbol*>(oldModules->module_symbols.get(r.old_name));
  assert(ms);
  Module *moduleToCopy = ms->module;
  assert(moduleToCopy);

  Statement *body = dynamic_cast<Statement*>(moduleToCopy->body);
  assert(body);

  Copier newcopier(this);
  newcopier.moduleBeingCopied = moduleToCopy;

  // Map the declaration symbol tables in the module being run to the
  // equivalent ones in the new module
  newcopier.copiedSymbolTableMap.insert(std::make_pair( moduleToCopy->constants,
                                                        newModule->constants));

  
#line 619 "ExpandModules.nw"
  for ( vector<SignalRenaming*>::const_iterator i = r.signals.begin() ;
	i != r.signals.end() ; i++ ) {
    SignalRenaming *sr = *i;
    assert(sr);
    if ( !(moduleToCopy->signals->local_contains(sr->old_name)) )
      throw IR::Error("Attempting to rename unknown signal " + sr->old_name);
    SignalSymbol *formalSignalSymbol =
      dynamic_cast<SignalSymbol*>(moduleToCopy->signals->get(sr->old_name));
    assert(formalSignalSymbol);
    SignalSymbol *actualSignalSymbol = actualSymbol(sr->new_sig);
    newcopier.formalToActual.insert(std::make_pair(formalSignalSymbol,
						   actualSignalSymbol));
  }
#line 576 "ExpandModules.nw"
  
#line 642 "ExpandModules.nw"
  assert(r.signalScope); // Should have been put there during static semantics
  // Since the run statement's symbol table is merely a pointer to a
  // table in its scope, we should have already made a new version of
  // the symbol table when we were copying the scope that encloses the
  // current run statement.
  SymbolTable *newSignalTable = newSymbolTable(r.signalScope);

/*
  for (SymbolTable::const_iterator i = r.signalScope->begin() ;
	i != r.signalScope->end() ; i++ )
	std::cerr << "symbol " << (*i)->name << '\n';
  std::cerr << "end of symbols\n";
*/

  newcopier.copiedSymbolTableMap.insert(std::make_pair(moduleToCopy->signals,
	  					       newSignalTable));

  for ( SymbolTable::const_iterator i = moduleToCopy->signals->begin() ;
	i != moduleToCopy->signals->end() ; i++ ) {
    SignalSymbol *formalSignal = dynamic_cast<SignalSymbol*>(*i);
    assert(formalSignal);
    if ( newcopier.formalToActual.find(formalSignal) ==
	 newcopier.formalToActual.end() ) {
      // Haven't yet copied this symbol: locate its name in the scope of
      // the run statement
      if (!r.signalScope->contains(formalSignal->name))
	throw IR::Error("Could not find signal " + formalSignal->name +
			" in enclosing scope (run " + r.old_name + " in " +
	                moduleBeingCopied->symbol->name + ")");
      SignalSymbol *originalActualSymbol =
          dynamic_cast<SignalSymbol*>(r.signalScope->get(formalSignal->name));
      assert(originalActualSymbol);
      // Now that we have the symbol that was used earlier, i.e., before
      // any renaming by an earlier run statement, get the renamed symbol,
      // if there is one.
      assert(formalToActual.find(originalActualSymbol) !=
	     formalToActual.end());
      SignalSymbol *renamedActualSignal =
	dynamic_cast<SignalSymbol*>(formalToActual[originalActualSymbol]);
      assert(renamedActualSignal);
      newcopier.formalToActual.insert( std::make_pair(formalSignal,
						      renamedActualSignal) );
    }
  }

#line 578 "ExpandModules.nw"
  
#line 700 "ExpandModules.nw"
  for ( vector<TypeRenaming*>::const_iterator i = r.types.begin() ;
	i != r.types.end() ; i++ ) {
    TypeRenaming *tr = *i;
    assert(tr);
    if ( !(moduleToCopy->types->local_contains(tr->old_name)) )
      throw IR::Error("Attempting to rename unknown type " + tr->old_name);
    TypeSymbol *oldType =
      dynamic_cast<TypeSymbol*>(moduleToCopy->types->get(tr->old_name));
    assert(oldType);
    TypeSymbol *newType = actualSymbol(tr->new_type);
    newcopier.formalToActual.insert(std::make_pair(oldType, newType));
  }
#line 579 "ExpandModules.nw"
  
#line 720 "ExpandModules.nw"
/*
  std::cerr << "Start of symbol table" << std::endl;
  for ( SymbolTable::const_iterator i = newModule->types->begin() ;
	i != newModule->types->end() ; i++ ) {
    assert(*i);
    std::cerr << "type " << (*i)->name << std::endl;
  }
  std::cerr << "End of symbol table" << std::endl;
*/

  for ( SymbolTable::const_iterator i = moduleToCopy->types->begin() ;
	i != moduleToCopy->types->end() ; i++ ) {
    TypeSymbol *formalType = dynamic_cast<TypeSymbol*>(*i);
    assert(formalType);
    // std::cerr << "copying type " << formalType->name << std::endl;

    if ( newcopier.formalToActual.find(formalType) ==
	 newcopier.formalToActual.end() ) {

      // std::cerr << "did not find it already copied" << std::endl;

      if ( moduleBeingCopied->types->local_contains(formalType->name) ) {
	// Existed already in the enclosing scope; reuse that one
	// std::cerr << "has the same name as existing type" << std::endl;
        assert(moduleBeingCopied);
	TypeSymbol *originalActualType =
           dynamic_cast<TypeSymbol*>(moduleBeingCopied->types->get(formalType->name));
	assert(originalActualType);

	// This could have been renamed earlier.  In any case, recover
	// the actual type that we had been using for it.

	/*
	std::cerr << "formalToActual map:\n";
	for (map<const Symbol*, Symbol*>::const_iterator i =
	       formalToActual.begin() ; i != formalToActual.end() ; ++i) {
	  const Symbol *s1 = (*i).first;
	  const Symbol *s2 = (*i).second;
	  std::cerr  << (*i).first << ' ' << (*i).second << ' ' <<
	    s1->name << " -> " << s2->name << std::endl;
	}
	std::cerr << "Looking for " << originalActualType << std::endl;
	*/

        assert(formalToActual.find(originalActualType) !=
               formalToActual.end());
        TypeSymbol *renamedActualType =
           dynamic_cast<TypeSymbol*>(formalToActual[originalActualType]);
        assert(renamedActualType);
        newcopier.formalToActual.insert( std::make_pair(formalType,
                                                        renamedActualType) );

      } else {
	// Did not find in in the enclosing scope; check whether it's
	// already in the new top-level module
	if (newModule->types->local_contains(formalType->name)) {
	  TypeSymbol *originalActualType =
	    dynamic_cast<TypeSymbol*>(newModule->types->get(formalType->name));
	  assert(originalActualType);
	  newcopier.formalToActual.insert(std::make_pair( formalType,
							  originalActualType));
	} else {       
	  // Did not find the type in the new module.  Copy the new type
	  // into the global module we're constructing and build the link
	  TypeSymbol *originalActualType = newcopier.copy(formalType);
	  assert(originalActualType);
	  newModule->types->enter(originalActualType);
	  newcopier.formalToActual.insert( std::make_pair(formalType,
							  originalActualType) );
	}
      }
    }
  }

#line 581 "ExpandModules.nw"
  
#line 801 "ExpandModules.nw"
for ( vector<ConstantRenaming*>::const_iterator i = r.constants.begin() ;
	i != r.constants.end() ; i++ ) {
  ConstantRenaming *cr = *i;
  assert(cr);
  if ( !(moduleToCopy->constants->local_contains(cr->old_name)) )
    throw IR::Error("Attempting to rename unknown constant " + cr->old_name);
  ConstantSymbol *oldConstant =
    dynamic_cast<ConstantSymbol*>(moduleToCopy->constants->get(cr->old_name));
  assert(oldConstant);
  ExpressionCopier *ec = new ExpressionCopier(cr->new_value, this);
  newcopier.newConstantExpression.insert(std::make_pair(oldConstant, ec));
}
#line 582 "ExpandModules.nw"
  
#line 816 "ExpandModules.nw"
for ( SymbolTable::const_iterator i = moduleToCopy->constants->begin() ;
	i != moduleToCopy->constants->end() ; i++ ) {
  ConstantSymbol *formalConstant = dynamic_cast<ConstantSymbol*>(*i);
  assert(formalConstant);
  // Here, we have a constant in the instantiated module.  It may be that
  // 1. The constant is new: add it to the module being built
  // 2. The constant already exists in the main module: use that one
  // 3. The constant has been renamed: do nothing; this was handled above

  if ( newcopier.newConstantExpression.find(formalConstant) ==
	 newcopier.newConstantExpression.end() ) {

    // Have not already renamed this constant

    if ( moduleBeingCopied->constants->local_contains(formalConstant->name) ) {
	// Found the constant: use the existing one
      assert(moduleBeingCopied);
      ConstantSymbol *originalActualConstant =
	dynamic_cast<ConstantSymbol*>(moduleBeingCopied->constants->get(formalConstant->name));
      /* FIXME: Should verify that the initial value of the two
	   constants is consistent

	   module Foo:
	   constant c = 10 : integer;
	   run Bar
	   end module

	   module Bar:
	   constant c = 20 : integer;
	   nothing
	   end module
      */

      // This constant could have already been renamed.  In any case, recover
      // the expression we had been using for it

      /*
      std::cerr << "newConstantExpression map:\n";
      for (map<const ConstantSymbol*, ExpressionCopier*>::const_iterator i =
	     newConstantExpression.begin() ;
	   i != newConstantExpression.end() ; ++i) {
	const ConstantSymbol *s1 = (*i).first;
	std::cerr  << (*i).first << ' ' << s1->name << std::endl;
      }
      std::cerr << "Looking for " << originalActualConstant << ' ' <<
	originalActualConstant->name << std::endl;
      */

      if (newConstantExpression.find(originalActualConstant) !=
	  newConstantExpression.end()) {
	// Has been renamed to something new
	ExpressionCopier *renamedActualConstant =
	  dynamic_cast<ExpressionCopier*>(newConstantExpression[originalActualConstant]);
	assert(renamedActualConstant);
	newcopier.newConstantExpression.insert( std::make_pair(formalConstant,
							     renamedActualConstant));
      } else {
	// Just use existing symbol
	assert(formalToActual.find(originalActualConstant) !=
	       formalToActual.end());
	ConstantSymbol *renamedActualConstant =
	  dynamic_cast<ConstantSymbol*>(formalToActual[originalActualConstant]);
	assert(renamedActualConstant);
	newcopier.formalToActual.insert( std::make_pair(formalConstant,
							renamedActualConstant));
      }

    } else {
      // Did not find in the enclosing scope; check whether it's
      // already in the new top-level module
      if (newModule->constants->local_contains(formalConstant->name)) {
	ConstantSymbol *actualConstant =
	  dynamic_cast<ConstantSymbol*>(newModule->constants->get(formalConstant->name));
	assert(actualConstant);
	newcopier.formalToActual.insert(std::make_pair(formalConstant,
						       actualConstant));
      } else {

	// Constant doesn't already exist in the new module: copy it
	// std::cerr << "Making a new copy of " << oldConstant->name << std::endl; 
	ConstantSymbol *actualConstant = newcopier.copy(formalConstant);
	assert(actualConstant);
	newModule->constants->enter(actualConstant);
	
	newcopier.formalToActual.insert( std::make_pair(formalConstant,
							actualConstant) );
	
	/* FIXME: This won't generate re-parsable code if the
	   constant's name was hidden by a local variable decalaration, e.g.,

	   module Foo:
	   var c : integer in
	     run Bar
	   end
           end module

           module Bar:
           constant c : integer;
           nothing
           end module

	   Berry's iclc doesn't worry about this because it simply numbers
	   everything and stores the names separately.
	*/
      }
    }
  }
}

#line 584 "ExpandModules.nw"
  
#line 929 "ExpandModules.nw"
/* FIXME: Renaming built-in functions, such as +, will generate
   strange output,

   e.g., c := +(a,b)
 */

for ( vector<FunctionRenaming*>::const_iterator i = r.functions.begin() ;
	i != r.functions.end() ; i++ ) {
  FunctionRenaming *fr = *i;
  assert(fr);
  if ( !(moduleToCopy->functions->local_contains(fr->old_name)) )
    throw IR::Error("Attempting to rename unknown function " + fr->old_name);
  FunctionSymbol *oldFunction =
    dynamic_cast<FunctionSymbol*>(moduleToCopy->functions->get(fr->old_name));
  assert(oldFunction);
  FunctionSymbol *newFunction = actualSymbol(fr->new_func);
  newcopier.formalToActual.insert(std::make_pair(oldFunction, newFunction));
}
#line 585 "ExpandModules.nw"
  
#line 950 "ExpandModules.nw"
for ( SymbolTable::const_iterator i = moduleToCopy->functions->begin() ;
	i != moduleToCopy->functions->end() ; i++ ) {
  FunctionSymbol *formalFunction = dynamic_cast<FunctionSymbol*>(*i);
  assert(formalFunction);
  // Here, we have a function in the instantiated module.  It may be that
  // 1. The function is new: add it to the module being built
  // 2. The function already exists in the main module: use that one
  // 3. The function has been renamed: do nothing; this was handled above

  if ( newcopier.formalToActual.find(formalFunction) ==
	 newcopier.formalToActual.end() ) {

    // Haven't already renamed this function

    if ( moduleBeingCopied->functions->local_contains(formalFunction->name) ) {
	// Found the function: use the existing one
	FunctionSymbol *originalActualFunction =
         dynamic_cast<FunctionSymbol*>(moduleBeingCopied->functions->get(formalFunction->name));
	assert(originalActualFunction);

	// FIXME: Should check that the parameter types are consistent
	assert(formalToActual.find(originalActualFunction) !=
	       formalToActual.end());
	FunctionSymbol *renamedActualFunction =
         dynamic_cast<FunctionSymbol*>(formalToActual[originalActualFunction]);
	assert(renamedActualFunction);
	newcopier.formalToActual.insert( std::make_pair( formalFunction,
							 renamedActualFunction));

    } else {
      // Function isn't in the enclosing scope; see if it's already in
      // the new top-level module
      if (newModule->functions->local_contains(formalFunction->name)) {
	FunctionSymbol *originalActualFunction =
	  dynamic_cast<FunctionSymbol*>(newModule->functions->get(formalFunction->name));
	assert(originalActualFunction);
	newcopier.formalToActual.insert( std::make_pair( formalFunction,
							 originalActualFunction ));
	
      } else {
	// Function doesn't already exist in the new module: copy it
	FunctionSymbol *originalActualFunction =
	  newcopier.copy(formalFunction);
	assert(originalActualFunction);
	newModule->functions->enter(originalActualFunction);
	newcopier.formalToActual.insert( std::make_pair(formalFunction,
							originalActualFunction) );
      }
    }

  }
}

#line 587 "ExpandModules.nw"
  
#line 1010 "ExpandModules.nw"
for ( vector<ProcedureRenaming*>::const_iterator i = r.procedures.begin() ;
	i != r.procedures.end() ; i++ ) {
  ProcedureRenaming *pr = *i;
  assert(pr);
  if ( !(moduleToCopy->procedures->local_contains(pr->old_name)) )
    throw IR::Error("Attempting to rename unknown procedure " + pr->old_name);
  ProcedureSymbol *oldProcedure =
    dynamic_cast<ProcedureSymbol*>(moduleToCopy->procedures->get(pr->old_name));
  assert(oldProcedure);
  ProcedureSymbol *newProcedure = actualSymbol(pr->new_proc);
  newcopier.formalToActual.insert(std::make_pair(oldProcedure, newProcedure));
}
#line 588 "ExpandModules.nw"
  
#line 1035 "ExpandModules.nw"
for ( SymbolTable::const_iterator i = moduleToCopy->procedures->begin() ;
	i != moduleToCopy->procedures->end() ; i++ ) {
  ProcedureSymbol *formalProcedure = dynamic_cast<ProcedureSymbol*>(*i);
  assert(formalProcedure);

  if ( newcopier.formalToActual.find(formalProcedure) ==
       newcopier.formalToActual.end() ) {

    // Haven't already renamed this procedure

    if ( moduleBeingCopied->procedures->local_contains(formalProcedure->name) ) {
	// Found the procedure: use the existing one
      ProcedureSymbol *originalActualProcedure =
	dynamic_cast<ProcedureSymbol*>(moduleBeingCopied->procedures->get(formalProcedure->name));

	// FIXME: Verify the parameter count and types are consistent
      assert(originalActualProcedure);

      assert(formalToActual.find(originalActualProcedure) !=
	     formalToActual.end());
      ProcedureSymbol *renamedActualProcedure =
	dynamic_cast<ProcedureSymbol*>(formalToActual[originalActualProcedure]);
      assert(renamedActualProcedure);
      newcopier.formalToActual.insert(std::make_pair( formalProcedure,
						      renamedActualProcedure));
      

    } else {
	// Procedure isn't already in scope: check to see whether it
	// already exists (e.g., whether it was copied by another module)

	if (newModule->procedures->local_contains(formalProcedure->name)) {
	  // Procedure already exists; use that one
	  ProcedureSymbol *originalActualProcedure =
	    dynamic_cast<ProcedureSymbol*>(newModule->procedures->get(formalProcedure->name));
	  assert(originalActualProcedure);
	  newcopier.formalToActual.insert(std::make_pair( formalProcedure,
							  originalActualProcedure ));
	} else {
	  // Procedure doesn't already exist in the new module: copy it
	  ProcedureSymbol *originalActualProcedure =
	    newcopier.copy(formalProcedure);
	  assert(originalActualProcedure);
	  newModule->procedures->enter(originalActualProcedure);
	  newcopier.formalToActual.insert( std::make_pair(formalProcedure,
							  originalActualProcedure) );
	}
    }
  }
}

#line 590 "ExpandModules.nw"
  
#line 1092 "ExpandModules.nw"
for ( vector<ProcedureRenaming*>::const_iterator i = r.tasks.begin() ;
	i != r.tasks.end() ; i++ ) {
  ProcedureRenaming *pr = *i;
  assert(pr);
  if ( !(moduleToCopy->tasks->local_contains(pr->old_name)) )
    throw IR::Error("Attempting to rename unknown task " + pr->old_name);
  TaskSymbol *oldTask =
    dynamic_cast<TaskSymbol*>(moduleToCopy->tasks->get(pr->old_name));
  assert(oldTask);
  TaskSymbol *newTaskSymbol = dynamic_cast<TaskSymbol*>(pr->new_proc);
  assert(newTaskSymbol);
  TaskSymbol *newTask = actualSymbol(newTaskSymbol);
  newcopier.formalToActual.insert(std::make_pair(oldTask, newTask));
}
#line 591 "ExpandModules.nw"
  
#line 1109 "ExpandModules.nw"
for ( SymbolTable::const_iterator i = moduleToCopy->tasks->begin() ;
	i != moduleToCopy->tasks->end() ; i++ ) {
  TaskSymbol *formalTask = dynamic_cast<TaskSymbol*>(*i);
  assert(formalTask);

  if ( newcopier.formalToActual.find(formalTask) ==
       newcopier.formalToActual.end() ) {

    // Haven't already renamed this task

    if ( moduleBeingCopied->tasks->local_contains(formalTask->name) ) {
	// Found the task: use the existing one
	TaskSymbol *originalActualTask =
	  dynamic_cast<TaskSymbol*>(moduleBeingCopied->tasks->get(formalTask->name));
	assert(originalActualTask);

	// FIXME: Verify the parameter count and types are consistent

	assert(formalToActual.find(originalActualTask) !=
	       formalToActual.end());
	TaskSymbol *renamedActualTask =
	  dynamic_cast<TaskSymbol*>(formalToActual[originalActualTask]);
	assert(renamedActualTask);
	newcopier.formalToActual.insert( std::make_pair( formalTask,
							 renamedActualTask));

    } else {
	// Task doesn't already exist in the new module: copy it
	TaskSymbol *originalActualTask = newcopier.copy(formalTask);
	assert(originalActualTask);
	newModule->tasks->enter(originalActualTask);
	newcopier.formalToActual.insert( std::make_pair(formalTask,
							originalActualTask) );	
    }

  }
}

#line 593 "ExpandModules.nw"
  
#line 1157 "ExpandModules.nw"
for ( SymbolTable::const_iterator i = moduleToCopy->variables->begin() ;
	i != moduleToCopy->variables->end() ; i++ ) {
  VariableSymbol *oldVariable = dynamic_cast<VariableSymbol*>(*i);
  assert(oldVariable);
  string baseName = r.new_name + "_" + oldVariable->name;
  string newName = baseName;
  int next = 1;
  while (newModule->variables->local_contains(newName)) {
    char buf[10];
    sprintf(buf, "%d", next++);
    newName = baseName + '_' + buf;
  }
  TypeSymbol *newType = newcopier.actualSymbol(oldVariable->type);
  Expression *newInitializer = newcopier.copy(oldVariable->initializer);
  VariableSymbol *newVariable =
     new VariableSymbol(newName, newType, newInitializer);
  newModule->variables->enter(newVariable);
  newcopier.formalToActual.insert( std::make_pair(oldVariable, newVariable));
//  std::cerr << "copying " << oldVariable->name << " to " << newName << std::endl;
}

copiedSymbolTableMap.insert( std::make_pair(moduleToCopy->variables,
	                                    newModule->variables) );

#line 595 "ExpandModules.nw"
  
#line 1189 "ExpandModules.nw"
for ( vector<Counter*>::const_iterator i = moduleToCopy->counters.begin() ;
	i != moduleToCopy->counters.end() ; i++ ) {
  Counter *newCounter = new Counter();
  newModule->counters.push_back(newCounter);
  newcopier.copiedCounterMap[*i] = newCounter;
}

#line 597 "ExpandModules.nw"
  Statement *result = newcopier.copy(body);
  return result;
}
#line 1225 "ExpandModules.nw"
Status Copier::visit(FunctionSymbol &s)
{
  FunctionSymbol *result = new FunctionSymbol(s.name);
  result->result = actualSymbol(s.result);
  for ( vector<TypeSymbol*>::const_iterator i = s.arguments.begin() ;
        i != s.arguments.end() ; i++ ) {
    assert(*i);
    result->arguments.push_back(actualSymbol(*i));
  }
  return result;
}

Status Copier::visit(BuiltinFunctionSymbol &s)
{
  BuiltinFunctionSymbol *result = new BuiltinFunctionSymbol(s.name);
  result->result = actualSymbol(s.result);
  for ( vector<TypeSymbol*>::const_iterator i = s.arguments.begin() ;
        i != s.arguments.end() ; i++ ) {
    assert(*i);
    result->arguments.push_back(actualSymbol(*i));
  }
  return result;
}
#line 1251 "ExpandModules.nw"
Status Copier::visit(ProcedureSymbol &s)
{
  ProcedureSymbol *result = new ProcedureSymbol(s.name);
  for ( vector<TypeSymbol*>::const_iterator i = s.reference_arguments.begin() ;
        i != s.reference_arguments.end() ; i++ ) {
    assert(*i);
    result->reference_arguments.push_back(actualSymbol(*i));
  }
  for ( vector<TypeSymbol*>::const_iterator i = s.value_arguments.begin() ;
        i != s.value_arguments.end() ; i++ ) {
    assert(*i);
    result->value_arguments.push_back(actualSymbol(*i));
  }
  return result;
}

Status Copier::visit(TaskSymbol &s)
{
  TaskSymbol *result = new TaskSymbol(s.name);
  for ( vector<TypeSymbol*>::const_iterator i = s.reference_arguments.begin() ;
        i != s.reference_arguments.end() ; i++ ) {
    assert(*i);
    result->reference_arguments.push_back(actualSymbol(*i));
  }
  for ( vector<TypeSymbol*>::const_iterator i = s.value_arguments.begin() ;
        i != s.value_arguments.end() ; i++ ) {
    assert(*i);
    result->value_arguments.push_back(actualSymbol(*i));
  }
  return result;
}
#line 1348 "ExpandModules.nw"
Status Copier::visit(ProcedureCall &c)
{
  ProcedureCall *result = new ProcedureCall(actualSymbol(c.procedure));
  for ( vector<VariableSymbol*>::const_iterator i = c.reference_args.begin() ;
	i != c.reference_args.end() ; i++ ) {
    assert(*i);
    result->reference_args.push_back(actualSymbol(*i));
  }
  for ( vector<Expression*>::const_iterator i = c.value_args.begin() ;
	i != c.value_args.end() ; i++ ) {
    assert(*i);
    result->value_args.push_back(copy(*i));
  }
  return result;
}
#line 1375 "ExpandModules.nw"
Status Copier::visit(StatementList &l)
{
  StatementList *result = new StatementList();
  for (vector<Statement*>::iterator i = l.statements.begin() ;
       i != l.statements.end() ; i++ ) {
    *result << copy(*i);
  }
  return result;
}

Status Copier::visit(ParallelStatementList &l)
{
  ParallelStatementList *result = new ParallelStatementList();
  for (vector<Statement*>::iterator i = l.threads.begin() ;
       i != l.threads.end() ; i++ ) {
    result->threads.push_back(copy(*i));
  }
  return result;
}
#line 1451 "ExpandModules.nw"
Status Copier::visit(Exec &e)
{
  Exec *result = new Exec();
  for ( vector<TaskCall*>::const_iterator j = e.calls.begin() ;
	j != e.calls.end() ; j++ ) {
    TaskCall *c = *j;
    assert(c);
    TaskSymbol *ts = dynamic_cast<TaskSymbol*>(c->procedure);
    assert(ts);
    TaskCall *newCall = new TaskCall(actualSymbol(ts));
    newCall->signal = actualSymbol(c->signal);
    newCall->body = copy(c->body);
    for ( vector<VariableSymbol*>::const_iterator i = c->reference_args.begin() ;
	  i != c->reference_args.end() ; i++ ) {
      assert(*i);
      newCall->reference_args.push_back(actualSymbol(*i));
    }
    for ( vector<Expression*>::const_iterator i = c->value_args.begin() ;
	  i != c->value_args.end() ; i++ ) {
      assert(*i);
      newCall->value_args.push_back(copy(*i));
    }
    result->calls.push_back(newCall);
  }
  return result;
}
#line 1487 "ExpandModules.nw"
void Copier::copyCases(const CaseStatement &source, CaseStatement *dest)
{  
  assert(dest);
  for ( vector<PredicatedStatement*>::const_iterator i = source.cases.begin() ;
	i != source.cases.end() ; i++) {
    assert(*i);
    dest->newCase(copy((*i)->body), copy((*i)->predicate));
  }

  dest->default_stmt = copy(source.default_stmt);
}
#line 1646 "ExpandModules.nw"
Status Copier::visit(FunctionCall &c)
{
  FunctionCall *result = new FunctionCall(actualSymbol(c.callee));
  for ( vector<Expression*>::const_iterator i = c.arguments.begin() ;
        i != c.arguments.end() ; i++ ) {
    assert(*i);
    result->arguments.push_back(copy(*i));
  }
  return result;
}
#line 1694 "ExpandModules.nw"
  
#line 1593 "ExpandModules.nw"
ExpressionCopier::ExpressionCopier(Expression *e, Copier *c)
  : theExpr(e), theCopier(c) { assert(e); assert(c); }

Expression *ExpressionCopier::copy() { return theCopier->copy(theExpr); }
#line 1695 "ExpandModules.nw"
  
#line 94 "ExpandModules.nw"
Modules *find_roots_and_rewrite(Modules *m)
{
  assert(m);
  FindRoots fr(m);
  const set<Module*> *roots = fr.find();
  if (roots->empty())
    throw IR::Error("No root nodes.  Is there a cyclic module dependency?");

  // Copy each root module in the order in which it appeared
  // in the original list of modules (preserve order)
  Modules *result = new Modules();
  for ( vector<Module*>::const_iterator i = m->modules.begin() ;
	i != m->modules.end() ; i++ )
    if ( roots->find(*i) != roots->end() ) {
      Copier copier(m, result);
      result->add(copier.copyModule(*i));
    }
  
  return result;
}
#line 1696 "ExpandModules.nw"
}
