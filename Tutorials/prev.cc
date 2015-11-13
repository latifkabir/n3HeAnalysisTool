// $Id: prev.C,v 1.1 2008/09/04 12:19:13 chris2 Exp $   -*-c++-*-
// by Chris Crawford <crawford@pa.uky.edu> 2008-06-21
//
// caches the argument, returning the old value
// used in TTree::Draw() to subtract the previous entry
// prev(), prev0(), prev1(), ... are different functions
//  which can be used independently with out interference

#define PREV(N)					\
  double prev##N(double newval)			\
  {						\
    static double oldval;			\
    double temp = oldval;			\
    oldval=newval;				\
    return temp;				\
  }

PREV()
PREV(0)  PREV(1)  PREV(2)  PREV(3)  PREV(4)
PREV(5)  PREV(6)  PREV(7)  PREV(8)  PREV(9)
PREV(10) PREV(11) PREV(12) PREV(13) PREV(14)
PREV(15) PREV(16) PREV(17) PREV(18) PREV(19)
PREV(20) PREV(21) PREV(22) PREV(23) PREV(24)
PREV(25) PREV(26) PREV(27) PREV(28) PREV(29)
PREV(30) PREV(31) PREV(32) PREV(33) PREV(34)
PREV(35) PREV(36) PREV(37) PREV(38) PREV(39)
