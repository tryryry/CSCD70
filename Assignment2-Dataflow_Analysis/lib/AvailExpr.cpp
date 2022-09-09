/**
 * @file Available Expression Dataflow Analysis
 */
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Pass.h>

#include <dfa/Framework.h>
#include <dfa/MeetOp.h>

#include "Expression.h"

using namespace dfa;
using namespace llvm;

namespace {

using AvailExprFrameworkBase =
    Framework<Expression, bool, Direction::kForward, Intersect>;

class AvailExpr final : public AvailExprFrameworkBase, public FunctionPass {
private:
  virtual void initializeDomainFromInst(const Instruction &Inst) override {
    if (const BinaryOperator *const BinaryOp =
            dyn_cast<BinaryOperator>(&Inst)) {
      /**
       * @todo(cscd70) Please complete the construction of domain.
       */
      Expression exp=Expression(*BinaryOp);
      if(std::find(Domain.begin(),Domain.end(),exp)==Domain.end()){
          Domain.emplace_back(exp);
      }
    }
  }
  virtual bool transferFunc(const Instruction &Inst, const DomainVal_t &IBV,
                            DomainVal_t &OBV) override {
    /**
     * @todo(cscd70) Please complete the definition of the transfer function.
     */
    DomainVal_t new_OBV=IBV;
    int id=0;
    for(Expression exp:Domain){
        bool flag=false;
        if(const Instruction *inst=dyn_cast<Instruction>(exp.LHS)){
          if(inst==&Inst){
            flag=true;
          }
        }
        if(const Instruction *inst=dyn_cast<Instruction>(exp.RHS)){
          if(inst==&Inst){
            flag=true;
          }
        }
        if(flag){
          new_OBV.at(id)=false;
        }
        id++;
    }
    if (const BinaryOperator *const BinaryOp =
            dyn_cast<BinaryOperator>(&Inst)) {
      auto iter=std::find(Domain.begin(),Domain.end(),Expression(*BinaryOp));
      if(iter!=Domain.end()){
        auto begin=Domain.begin();
        int pos=0;
        while(begin!=iter){
          begin++;
          pos++;
        }
        new_OBV.at(pos)=true;
      }
    }
    bool hasChange = new_OBV == OBV ? false:true;
    OBV=new_OBV;
    return hasChange;
  }

public:
  static char ID;

  AvailExpr() : AvailExprFrameworkBase(), FunctionPass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
  bool runOnFunction(Function &F) override {
    return AvailExprFrameworkBase::runOnFunction(F);
  }
};

char AvailExpr::ID = 0;
RegisterPass<AvailExpr> X("avail-expr", "Available Expression");

} // anonymous namespace
