#pragma once // NOLINT(llvm-header-guard)

#include <vector>

namespace dfa {

template <typename TDomainElemRepr> //
class MeetOp {
protected:
  using DomainVal_t = std::vector<TDomainElemRepr>;

public:
  virtual DomainVal_t operator()(const DomainVal_t &LHS,
                                 const DomainVal_t &RHS) const = 0;
  virtual DomainVal_t top(const size_t DomainSize) const = 0;
};

/**
 * @brief Intersection Meet Operator
 *
 * @todo(cscd70) Please complete the definition of the intersection meet
 *               operator, and modify the existing definition if necessary.
 */
class Intersect final : public MeetOp<bool> {
public:
  virtual DomainVal_t operator()(const DomainVal_t &LHS,
                                 const DomainVal_t &RHS) const override {
    assert(LHS.size() == RHS.size());
    const size_t DomainSize=LHS.size();
    DomainVal_t Ret=DomainVal_t(DomainSize,false);
    for(size_t I=0;I<DomainSize;I++){
      Ret[I]=LHS.at(I) & RHS.at(I);
    }
    return Ret;
  }
  virtual DomainVal_t top(const size_t DomainSize) const override {
    return DomainVal_t(DomainSize, true);
  }
};

class Union final : public MeetOp<bool> {
public:
  virtual DomainVal_t operator()(const DomainVal_t &LHS,
                                 const DomainVal_t &RHS) const override {
    assert(LHS.size() == RHS.size());
    const size_t DomainSize=LHS.size();
    DomainVal_t Ret=DomainVal_t(DomainSize,false);
    for(size_t I=0;I<DomainSize;I++){
      Ret[I]=LHS.at(I) | RHS.at(I);
    }
    return Ret;
  }
  virtual DomainVal_t top(const size_t DomainSize) const override {
    return DomainVal_t(DomainSize, false);
  }
};

} // namespace dfa
