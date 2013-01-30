#include <stdio.h>
extern "C" { 
#include <simple.h>
#include "print.h"
}
#include<hash_map>
#include<vector>
#include<set>



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

class Block{
 public:
  int blcIdx;
  set<int> instrIdx;
  vector<char*> instrName;
  set<int> successor;
  set<int> predecessor;
};

int instrCount=-1;//Store the temporary number of instructions
int blockCount=0;//Store the temporary number of blocks
int instrNum; //Store the total number of instructions
int blcNum;//Store the total number of blocks(including entry and exit ones)
hash_map<char*,int> LabelOutBlc;//Record which label going out from which block(for checking block from label)
hash_map<char*,int> LabelInBlc;//Record which label going to which block(for checking block from label)
vector<char*> OPR;//Record the list of operand code
vector<Block> blc; //Container of all block information;
hash_map<int,vector<char*> > BlcOutLabel;//Record which label going out from with block(for checking label from block)
hash_map<int,char*> BlcInLabel;////Record which label going to which block(for checking label from block)
vector<int> BlockLeader;



//Get the leader of each block
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

//Put an end to the blockleader list(Put the number of total instruction as then end sign)
void
putBlockEnd(int instrNum,vector<int> &BlockLeader)
{
   BlockLeader.push_back(instrNum);
}

//Get the block index of a given instruction
int
getBlockIdx(int instrCount,vector<int> BlockLeader)
{
   for(int i=BlockLeader.size()-1;i>=0;i--){
     if(instrCount>=BlockLeader[i]){
        return i; 
        break; 
     }
   }
}

//Get the successor list for a given block
set<int> 
getSuccessor(Block temp_blc,int blcNum,hash_map<int,vector<char*> > BlcOutLabel,hash_map<char*,int> LabelInBlc)
{ 
  vector<char*> temp;
  temp=BlcOutLabel[temp_blc.blcIdx];//Check which label(s) outgoing 
  set<int> temp_succIdx;
  vector<char*>::iterator it;
  set<int>::iterator itt;
  //If the block is the one immediately before exit block
  if(temp_blc.blcIdx==blcNum-2){
    temp_succIdx.insert(blcNum-1);
  }
  
  for(it=temp.begin();it!=temp.end();it++){
    temp_succIdx.insert(LabelInBlc[*it]);
  }
  //Insert the block immediately following the given block as successor;
  temp_succIdx.insert(temp_blc.blcIdx+1);
  //Scan the given block's instruction list, find situation where the block immediately following the given block is not successor
  for(it=temp_blc.instrName.begin();it!=temp_blc.instrName.end();it++){
    /*
    if(*it=="BFALSE_OP"){
    //If BFALSE_OP in this block, automatically add the immediate following block as one of successors 
     temp_succIdx.insert(temp_blc.blcIdx+1);
    }
    */
    if(*it=="JMP_OP"){
     temp_succIdx.erase(temp_blc.blcIdx+1);
    }
    /*
    else if(*it=="MBR_OP"){
    //If MBR_OP in this block, automatically add the immediate following block as one of successors 
     temp_succIdx.insert()
    }
    */
  }
  //Check outgoing the outgoing label leave to which block(this block should be successor)
 
  return temp_succIdx;
}

/*
set<int>
getPredecessor(Block temp_blc,){
}
*/

//Proccess a given procedure
simple_instr* do_procedure (simple_instr *inlist, char *proc_name)
{
    // build flow control graph 
    simple_instr *i;
    BlockLeader.push_back(-1);//entry block
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
        BlcOutLabel[getBlockIdx(instrCount,BlockLeader)].push_back(i->u.bj.target->name);
        printf("%s ","OUTGOING LABEL "); 
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
        BlcOutLabel[getBlockIdx(instrCount,BlockLeader)].push_back(i->u.bj.target->name);
      
        printf("%s ","OUTGOING LABEL "); 
        vector<char*>::iterator it;
        for(it=BlcOutLabel[getBlockIdx(instrCount,BlockLeader)].begin();it!=BlcOutLabel[getBlockIdx(instrCount,BlockLeader)].end();it++){
        printf("%s\n",*it);   
        }
         
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
         
        //Record the source of outgoing label(s)
        unsigned n, ntargets;
        for(n=0;n<ntargets-1;n++){
        LabelOutBlc[i->u.mbr.targets[n]->name]=getBlockIdx(instrCount,BlockLeader);
        BlcOutLabel[getBlockIdx(instrCount,BlockLeader)].push_back(i->u.mbr.targets[n]->name);
        }
   
        printf("%i",instrCount);   
	    break;
	}

	case LABEL_OP: {
        OPR.push_back("LABEL_OP");
        //Record the destination of incoming label
        LabelInBlc[i->u.label.lab->name]=getBlockIdx(instrCount,BlockLeader);
        BlcInLabel[getBlockIdx(instrCount,BlockLeader)]=i->u.label.lab->name;
        printf("%s ","Incoming LABEL "); 
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
    instrNum=instrCount+1;
    printf("\ninstrNum:%i ",instrNum);
    putBlockEnd(instrNum,BlockLeader);
    
    for(int i=0;i<BlockLeader.size();i++){
    printf("%i ",BlockLeader[i]);
    }

    blcNum=BlockLeader.size();

    //Initialize Block class members
    for(int i=0;i<blcNum;i++){
      Block temp_blc;
      temp_blc.blcIdx=i;
      //Entry block
      if(i==0){
         temp_blc.successor.insert(1);
      }
      //Exit block
      else if(i==blcNum-1){
         temp_blc.predecessor.insert(blcNum-1);
      }
      else{

       int a=BlockLeader[i];//instruction index starts from blockleader   
       //record instruction index and instruction name for a block
       while(a!=BlockLeader[i+1]){
       temp_blc.instrIdx.insert(a);
       temp_blc.instrName.push_back(OPR[a]); 
       printf("\na=%i",a);
       a++;
       }
      } 
      blc.push_back(temp_blc);
    }

    for(int i=0;i<blc.size();i++){
    printf("\nblock ");
    printf("%i",blc[i].blcIdx);
    printf("\n\t%s%i","instrs ",blc[i].instrIdx.size());
       set<int>::iterator ittt;
      //for(int j=0;j<blc[i].instrIdx.size();j++){
      for(ittt=blc[i].instrIdx.begin();ittt!=blc[i].instrIdx.end();ittt++){
       printf(" %i",*ittt);
      }
     
    }
    
    //Record successors 
    set<int> succ;
    for(int k=0;k<blcNum;k++){
     succ=getSuccessor(blc[k],blcNum,BlcOutLabel,LabelInBlc);
     set<int>::iterator iter;
     printf("\nsuccessors blc[%i] %i",k,succ.size());
     for(iter=succ.begin();iter!=succ.end();iter++){
      printf(" %i",*iter);
     }
     printf("\n");
    }
    //Record predecessors
    
    
    
    //Print block leader list
    printf("\n");
    for(int k=0;k<BlockLeader.size();k++){
    printf("\n%i",BlockLeader[k]);
    }
    /*
    //Print Instruction Name list
    printf("\n");
    vector<char*>::iterator it;
    for(it=OPR.begin();it!=OPR.end();it++){
      printf("%s\n",*it);
    }
    */
    //find immediate dominators    
    printf("\n" );
    return inlist;
}