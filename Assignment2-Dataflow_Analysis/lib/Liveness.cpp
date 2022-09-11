/**
 * @file Liveness Dataflow Analysis
 */
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include <dfa/Framework.h>
#include <dfa/MeetOp.h>

#include "Variable.h"

using namespace llvm;
using namespace dfa;

namespace {
/**
 * @todo(cscd70) Implement @c Liveness using the @c dfa::Framework interface.
 */
using LivenessFrameworkBase=
       Framework<Variable, bool, Direction::kBackward, Union>;

class Liveness final :public LivenessFrameworkBase, public FunctionPass {
private:
  virtual void initializeDomainFromInst(const Instruction &Inst) override {
    Variable InstVar=Variable(&Inst);
    if(std::find(Domain.begin(),Domain.end(),InstVar)==Domain.end()){
        Domain.emplace_back(InstVar);
    }
    for (const auto *Iter = Inst.op_begin();Iter != Inst.op_end(); ++Iter) {
      Value *V = *Iter;
      if(isa<Instruction>(V) || isa<Argument>(V)){
        Variable Var=Variable(V);
        if(std::find(Domain.begin(),Domain.end(),Var)==Domain.end()){
          Domain.emplace_back(Var);
        }
      }
    }
  }
  virtual bool transferFunc(const Instruction &Inst, const DomainVal_t &IBV,
                            DomainVal_t &OBV) override {
    DomainVal_t NewObv=IBV;
    int Idx=0;
    for(Variable Var:Domain){
      if(const Instruction *I=dyn_cast<Instruction>(Var.V)){
        if(I==&Inst){
          NewObv.at(Idx)=false;
        }
      }
      Idx++;
    }
    for(auto InstBegin=Inst.op_begin(),InstEnd=Inst.op_end();InstBegin!=InstEnd;InstBegin++){
      Value *V = *InstBegin;
      if(isa<Instruction>(V) || isa<Argument>(V)){
        auto Iter=std::find(Domain.begin(),Domain.end(),Variable(V));
        if(Iter!=Domain.end()){
          NewObv.at(std::distance(Domain.begin(),Iter))=true;
        }
      }
    }
    bool HasChange = NewObv != OBV;
    OBV=NewObv;
    return HasChange;
  }
public:
  static char ID;

  Liveness() : LivenessFrameworkBase(),FunctionPass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  virtual bool runOnFunction(Function &F) override {
    // // clang-format off
    // errs() << "**************************************************" << "\n"
    //        << "* Instruction-Domain Value Mapping" << "\n"
    //        << "**************************************************" << "\n";
    // // clang-format on

    // return false;
    return LivenessFrameworkBase::runOnFunction(F);
  }
};

char Liveness::ID = 0;
RegisterPass<Liveness> X("liveness", "Liveness");

} // anonymous namespace
