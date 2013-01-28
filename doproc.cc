#include <stdio.h>
extern "C" { 
#include <simple.h>
#include "print.h"
}
#include<hash_map>
#include<vector>




using namespace std; 
using namespace __gnu_cxx;
// data structures you should consider using are vector and hash_map from the STL
// refer to the following link as a starting point if you are not familiar with them: 
// http://www.sgi.com/tech/stl/Vector.html
// http://www.sgi.com/tech/stl/hash_map.html 
/*
void 
enter_labelOut(char* labelName, int blknum, hash_map<char*,int> &LabelOutBlc)
{   
    LabelOutBlc[labelName]=blknum;    
    printf("LABEL: %s %i \n",labelName,LabelOutBlc[labelName]);
}
*/
int instrCount=-1;
int blockCount=0;
hash_map<char*,int> LabelOutBlc;
hash_map<char*,int> LabelInBlc;
vector<char*> temp_successor;
vector<char*> OPR;

hash_map<int,vector<char*> > BlcOutLabel;
hash_map<int,char*> BlcInLabel;

vector<int> BlockLeader;
//BlockLeader.push_back(-1);//entry lock

void 
getBlockLeader(simple_instr *s,int instrCount,vector<int> &BlockLeader)
{ 
   bool a,b,c;
   a=(instrCount==0);//Entry point
   b=(s->opcode==BFALSE_OP||s->opcode==MBR_OP);//Instruction immediatelly following branch
   c=(s->opcode==LABEL_OP);//Label
   if(a||b||c){
     if(b){
     BlockLeader.push_back(instrCount+1);
     }
     else{
     BlockLeader.push_back(instrCount);
     }
   }
}

int
getBlockIdx(int instrCount,vector<int> BlockLeader)
{
   for(int i=BlockLeader.size()-1;i>=0;i--){
     if(instrCount>=BlockLeader[i]){
        return i+1; 
        break; 
     }
   }
}

void
printSuccessor(int blockIdx,hash_map<int,vector<char*> > BlcOutLabel,hash_map<char*,int> LabelInBlc)
{ vector<char*> temp;
  temp=BlcOutLabel[blockIdx];
  vector<int> temp_succIdx;
  
  
  vector<char*>::iterator it;
  for(it=temp.begin();it!=temp.end();it++){
    temp_succIdx.push_back(LabelInBlc[*it]);
  }
  
  printf("\nsuccessors %i %i ",temp_succIdx.size()+1,blockIdx+1);
  
  vector<int>::iterator itt;
  for(itt=temp_succIdx.begin();itt!=temp_succIdx.end();itt++){
   printf("%i ",*itt);
  }
   printf("\n");
}

simple_instr* do_procedure (simple_instr *inlist, char *proc_name)
{
    // build flow control graph 
    simple_instr *i;

    printf("\nProcedure %s:\n", proc_name);
    
    i = inlist;
    while (i) {
        instrCount++;
        getBlockLeader(i,instrCount,BlockLeader);
        switch (i->opcode){
        case LOAD_OP: {
	OPR.push_back("LOAD_OP");
        printf("%i",instrCount);
	    break;
	}

	case STR_OP: {
	OPR.push_back("STR_OP"); 
        printf("%i",instrCount);   
	    break;
	}

	case MCPY_OP: {
	OPR.push_back("MCPY_OP"); 
        printf("%i",instrCount);
	    break;
	}
        
	case LDC_OP: {
	OPR.push_back("LDC_OP");
        printf("%i",instrCount);
	    break;
	}

	case JMP_OP: {
	
        OPR.push_back("JMP_OP");
        printf("blockNum:%i\n",getBlockIdx(instrCount,BlockLeader));

        //Record the source of outgoing label
        LabelOutBlc[i->u.bj.target->name]=getBlockIdx(instrCount,BlockLeader);
        temp_successor.push_back(i->u.bj.target->name);
        BlcOutLabel[getBlockIdx(instrCount,BlockLeader)]=temp_successor;
        temp_successor.clear();
        printf("\n%s ","OUTGOING LABEL ");
        vector<char*>::iterator it;
        for(it=BlcOutLabel[getBlockIdx(instrCount,BlockLeader)].begin();it!=BlcOutLabel[getBlockIdx(instrCount,BlockLeader)].end();it++){
        printf("%s\n",*it);   
        }

        printf("%i",instrCount);   
	    break;
	}

	case BTRUE_OP:{
        OPR.push_back("BTRUE_OP");
        printf("%i",instrCount);}
	case BFALSE_OP: {
      
        OPR.push_back("BFALSE_OP");
        printf("blockNum:%i\n",getBlockIdx(instrCount,BlockLeader));
        
        //Record the source of outgoing label
        LabelOutBlc[i->u.bj.target->name]=getBlockIdx(instrCount,BlockLeader);
        temp_successor.push_back(i->u.bj.target->name);
        BlcOutLabel[getBlockIdx(instrCount,BlockLeader)]=temp_successor;
        temp_successor.clear();
        printf("\n%s ","OUTGOING LABEL "); 
        vector<char*>::iterator it;
        for(it=BlcOutLabel[getBlockIdx(instrCount,BlockLeader)].begin();it!=BlcOutLabel[getBlockIdx(instrCount,BlockLeader)].end();it++){
        printf("%s\n",*it);   
        }
         
     //  printf("LABEL %s %i \n",i->u.bj.target->name,LabelOutBlc[i->u.bj.target->name]); 
       
        printf("%i",instrCount); 
       
	    break;
	}

	case CALL_OP: {
	OPR.push_back("CALL_OP");
        printf("%i",instrCount);  
	    break;
	}

	case MBR_OP: {
	OPR.push_back("MBR_OP");
        
        
        printf("%i",instrCount);   
	    break;
	}

	case LABEL_OP: {
        OPR.push_back("LABEL_OP");
        //Record the destination of incoming label
        LabelInBlc[i->u.label.lab->name]=getBlockIdx(instrCount,BlockLeader);
      
        BlcInLabel[getBlockIdx(instrCount,BlockLeader)]=i->u.label.lab->name;
        printf("\n%s ","Incoming LABEL "); 
        printf("%s\n",BlcInLabel[getBlockIdx(instrCount,BlockLeader)]);
        
        printf("blockNum:%i\n",getBlockIdx(instrCount,BlockLeader));
       
        printf("%i",instrCount);
        
	    break;
	}

	case RET_OP: {
	OPR.push_back("RET_OP");
        printf("%i",instrCount); 
	    break;
	}

	case CVT_OP:{
        OPR.push_back("CVT_OP");
        printf("%i",instrCount);}
	case CPY_OP:{
        OPR.push_back("CPY_OP");
        printf("%i",instrCount);}
	case NEG_OP:{
        OPR.push_back("NEG_OP");
        printf("%i",instrCount);}
	case NOT_OP: {
        OPR.push_back("NOT_OP");
        printf("%i",instrCount);   
	    break;
	}

	default: {
        OPR.push_back("default");
        printf("%i",instrCount);
	    /* binary base instructions */
         
	}
       }
       
       fprint_instr(stdout, i);
	 i = i->next;
     
    }
    
    
    //printf("\n",)
    for(int k=0;k<BlockLeader.size();k++){
    printf("\n%i",BlockLeader[k]);
    }
    printf("\n" );
    vector<char*>::iterator it;
    for(it=OPR.begin();it!=OPR.end();it++){
      printf("%s\n",*it);
    }
    printSuccessor(2,BlcOutLabel,LabelInBlc);
    // find immediate dominators    
    printf("\n" );
    return inlist;
}
