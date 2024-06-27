#define DEBUG_TYPE "finalproject"
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Pass.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/InlineAsm.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/ADT/SetVector.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Analysis/InlineCost.h>
#include <llvm/Transforms/Utils/ValueMapper.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/SymbolTableListTraits.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/MathExtras.h>
#include <llvm/IR/User.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/InstIterator.h>
#include <llvm/Support/Compiler.h>
#include <iterator>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>

using namespace llvm;


namespace {
        struct finalproject : public ModulePass {                      
                        static char ID;
                        finalproject() : ModulePass(ID) {}         
                        virtual bool runOnModule(Module &M) override{    //Module Pass 
                                        LLVMContext &C = M.getContext();
                                        GlobalVariable *g = new GlobalVariable(M,IntegerType::get(M.getContext(), 32),false,GlobalValue::CommonLinkage,0, "g");
                                        g->setAlignment(4);
                                        ConstantInt* const_int32_1 = ConstantInt::get(C, APInt(32,0));
                                        g->setInitializer(const_int32_1); //Global Variable Declaration                                    
                                        Function *output = M.getFunction("pop_direct_branch"); // Calling the function pop_direct_branch into the module
                                        for(Module::iterator b = M.begin(); b != M.end(); ++b){
                                           for(Function::iterator f = b->begin(); f != b->end(); ++f){
                                                for(BasicBlock::iterator i = f->begin(); i != f->end(); ++i){  //iterating over instructions
                                                         Instruction *I = &*i;
                                                         if (CallInst *call = dyn_cast<CallInst>(I)){
                                                            if(Function *fun = call->getCalledFunction()){    //get the function which is called
                                                                        StringRef start = fun->getName();
                                                                        errs() << start << "\n";
                                                                        if(start[0] == 'p' && !fun->isDeclaration()){
                                                                          if(start != "pop_direct_branch"){          // check the function which is not starting with 'p' and not a declaration
                                                                             llvm::ValueToValueMapTy VMap;
                                                                             Function *clone = CloneFunction(fun,VMap,false);  //clone the function 
                                                                             fun->getParent()->getFunctionList().push_back(clone); // Get the clone function into the module 
                                                                             for(Function::iterator F = clone->begin(); F != clone->end(); ++F){
                                                                                 for(BasicBlock::iterator i = F->begin(); i != F->end(); ++i){
                                                                                        Instruction *I = &*i;
                                                                                        errs()<< *I << "\n";
                                                                                        if(ReturnInst *return_val = dyn_cast<ReturnInst>(&*I)){
                                                                                             if(Value* return_clone = return_val->getReturnValue()){ // get the return value from the clone function                                                                                                  
                                                                                                   StoreInst *store = new StoreInst(return_clone, g,return_val); //Store instruction                             
                                                                                                   Instruction *output_pll = CallInst::Create(output,"",return_val); //Call the pop_direct_branch                                                                                                   
                                                                                                   IRBuilder<>builder(call->getNextNode());  
                                                                                                   LoadInst *load =builder.CreateLoad(g,"");  //Load instruction
                                                                                                   call->replaceAllUsesWith(load);

                                                                        }                                                                        
                                                                        }
                                                                        call->setCalledFunction(clone);  //set the new called function to clone
                                                                        }
                                                                        }
                                                                        errs() <<  "after modification" << *clone <<"\n"; //printing the changed clone function

}
}
}
}
}
}
}
return true; //Pass has been modified
}
};
}


char finalproject::ID = 0;
static RegisterPass<finalproject> X("finalproject", "finalproject", false, false);  //Registering the pass

