#include <stdio.h>
extern "C" { 
#include <simple.h>
#include "print.h"
}

#include<vector>
#include<hash_map>

using namespace std;
using namespace __gnu_cxx;
// data structures you should consider using are vector and hash_map from the STL
// refer to the following link as a starting point if you are not familiar with them: 
// http://www.sgi.com/tech/stl/Vector.html
// http://www.sgi.com/tech/stl/hash_map.html 

simple_instr* do_procedure (simple_instr *inlist, char *proc_name)
{
    // build flow control graph 
     simple_instr *i;

    printf("\nProcedure %s:\n", proc_name);
    vector<int> a;
    hash_map<const char*,int> b;
    i = inlist;
    while (i) {
	fprint_instr(stdout, i);
	i = i->next;
    }
    // find immediate dominators    

    return inlist;
}
