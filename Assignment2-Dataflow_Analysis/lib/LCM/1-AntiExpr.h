#pragma once // NOLINT(llvm-header-guard)

/**
 * @file Anticipated Expression Dataflow Analysis
 */
#include <llvm/Pass.h>
#include <llvm/Transforms/Utils.h>

#include <dfa/Framework.h>
#include <dfa/MeetOp.h>

#include "../Expression.h"

using namespace dfa;

class AntiExprWrapperPass;
class AntiExprImpl;
/**
 * @todo(cscd70) Provide an implementation for the anticipated expressions, by
 *               inheriting from the DFA framework.
 */
using AntiExprFrameWorkBase = Framework<Expression, bool, Direction::kBackward, Intersect> ;
class AntiExprImpl :public AntiExprFrameWorkBase{
private:
  virtual void initializeDomainFromInst(const Instruction &Inst) override {
    if (const BinaryOperator *const BinaryOp =
            dyn_cast<BinaryOperator>(&Inst)) {
      Expression Exp=Expression(*BinaryOp);
      if(std::find(Domain.begin(),Domain.end(),Exp)==Domain.end()){
          Domain.emplace_back(Exp);
      }
    }
  }
  virtual bool transferFunc(const Instruction &Inst, const DomainVal_t &IBV,
                            DomainVal_t &OBV) override {
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
        NewObv.at(std::distance(Domain.begin(),Iter))=true;
      }
    }
    bool HasChange = NewObv == OBV ? false:true;
    OBV=NewObv;
    return HasChange;
  }
private:
  friend class AntiExprWrapperPass;
};

class AntiExprWrapperPass : public FunctionPass{
private:
  /**
   * @todo(cscd70) Uncomment the following line after the completion of
   *               @c AntiExprImpl .
   */
  AntiExprImpl AntiExpr;

public:
  static char ID;

  AntiExprWrapperPass() : FunctionPass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    /**
     * @todo(cscd70) Request that the LLVM built-in pass @c BreakCriticalEdges
     *               to run first, by leveraging its ID.
     */
    AU.addRequiredID(BreakCriticalEdgesID);
    AU.setPreservesAll();
  }

  bool runOnFunction(Function &F) override {
    errs() << "* Anticipated Expression *"
           << "\n";

    /**
     * @todo(cscd70) Invoke the method @c runOnFunction from @c AntiExprImpl .
     */
    //return false;
    return AntiExpr.runOnFunction(F);
  }

  /**
   * @todo(cscd70) Obtain the @c Domain and @c InstDomainValMap from
   *               @c AntiExprImpl .
   */
  std::vector<Expression> getDomain() const {
    //return std::vector<Expression>();
    return AntiExpr.Domain;
  }
  std::unordered_map<const Instruction *, std::vector<bool>>
  getInstDomainValMap() const {
    //eturn std::unordered_map<const Instruction *, std::vector<bool>>();
    return AntiExpr.InstDomainValMap;
  }
};
