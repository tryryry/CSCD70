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
      Expression Exp=Expression(*BinaryOp);
      if(std::find(Domain.begin(),Domain.end(),Exp)==Domain.end()){
          Domain.emplace_back(Exp);
      }
    }
  }
  virtual bool transferFunc(const Instruction &Inst, const DomainVal_t &IBV,
                            DomainVal_t &OBV) override {
    /**
     * @todo(cscd70) Please complete the definition of the transfer function.
     */
    DomainVal_t NewObv=IBV;
    int Idx=0;
    for(Expression Exp:Domain){
        bool Flag=false;
        if(const Instruction *I=dyn_cast<Instruction>(Exp.LHS)){
          if(I==&Inst){
            Flag=true;
          }
        }
        if(const Instruction *I=dyn_cast<Instruction>(Exp.RHS)){
          if(I==&Inst){
            Flag=true;
          }
        }
        if(Flag){
          NewObv.at(Idx)=false;
        }
        Idx++;
    }
    if (const BinaryOperator *const BinaryOp =
            dyn_cast<BinaryOperator>(&Inst)) {
      auto Iter=std::find(Domain.begin(),Domain.end(),Expression(*BinaryOp));
      if(Iter!=Domain.end()){   
        auto Begin=Domain.begin();
        int Pos=0;
        while(Begin!=Iter){
          Begin++;
          Pos++;
        }
        NewObv.at(Pos)=true;
      }
    }
    bool HasChange = NewObv == OBV ? false:true;
    OBV=NewObv;
    return HasChange;
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
