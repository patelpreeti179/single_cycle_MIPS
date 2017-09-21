#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.
struct addresses{
				  string memAddress;
				  bitset<32> Instruction;
			  };
struct operands{
				  bitset<32> op1 ;
				  bitset<32> op2;
			  };


class RF
{
    public:

        bitset<32> ReadData1, ReadData2; 
     	RF() // Reading Register File
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  // Initializing Register 0 as "0"
		  ifstream registerfile;
		  ofstream rfout;
		  
                  string line;
                  int i=0;
                  registerfile.open("RFresult.txt");
                  if (registerfile.is_open())
                  {
                  while (getline(registerfile,line))
                       {      
                        Registers[i] = bitset<32>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  registerfile.close();
        }
	
        operands ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            // implement the funciton by you.  
			   int a, b, c ;
			   bitset <32> r1;
			   bitset <32> r2;
			   bitset <32> r3;
			   int d = 32;
			   operands op;
			   
			   
			   a = (int)(RdReg1.to_ulong());
			   b = (int)(RdReg2.to_ulong());
			   c = (int)(WrtReg.to_ulong());
			   op.op1 = Registers[a] ;
			   cout << " Reading from file register 1";
			   cout << op.op1;
			   cout << endl;
			   cout << " Reading from file register 2";
			   op.op2= Registers[b] ;
			   cout << op.op2;
			   return op;

			  }
 
		 
	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"Unable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<6> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {   
                 // implement the ALU operations by you. 
				 int operand1 = (int)(oprand1.to_ulong());
				 cout << "operand  1 is" ;
				 cout << operand1;
				 
				 int operand2 = (int)(oprand2.to_ulong());
				 cout << "operand  2 is ";
				 cout << operand2;
				 int Aluoper = (int)(ALUOP.to_ulong());
				 int Result; 
				 switch (Aluoper){
					 // R Type Operations
					 case 1: Result = operand1 + operand2;
							   ALUresult = bitset<32>(Result);
							   cout << "ALU OUTPUT";
							   cout << ALUresult;
							   break;
				     case 3: Result = operand1 - operand2;
							   ALUresult = bitset<32>(Result);
							   cout << "ALU OUTPUT";
							   cout << ALUresult;
							   break;
					 case 4: 
							   ALUresult = oprand1&oprand2 ;
							   cout << "ALU OUTPUT";
							   cout << ALUresult;
							   break;
				     case 5:   ALUresult = oprand1 |oprand2 ;
							   cout << "ALU OUTPUT";
							   cout << ALUresult;
							   break;
				     case 7:   ALUresult = ~(oprand1 |oprand2) ;
							   cout << "ALU OUTPUT";
							   cout << ALUresult;
							   break;
					 case 9:   Result = operand1 + operand2;
							   ALUresult = bitset<32>(Result);
							   cout << "ALU OUTPUT";
							   cout << ALUresult;
							   break;
					 // I Type Operations
					 
				 }
                 return ALUresult;
               }            
};

class INSMem
{
      public:
          ifstream Instruction;

           INSMem()
          {       IMem.resize(MemSize); 
                  ifstream imem;
                  string line;
                  int i=0;
				  
				  
                  imem.open("imem.txt");
                  if (imem.is_open())
                  

					{
                  while (getline(imem, line))
                     {      
                        IMem[i] = bitset<8>(line);
                        i++;
						
						
                     }
					 
						
                     
                  }
                  else cout<<"Unable to open file";
                  imem.close();
                   
									   
                  }
				 
                  
         addresses ReadMemory (bitset<32> ReadAddress) 
              {    
			   addresses address;
			   bitset<32> x;
			   int a;
			   a = (int)(ReadAddress.to_ulong()); // Converting ReadAddress from Bitset to int 
			  std::string add1 = IMem[a].to_string<char,std::string::traits_type,std::string::allocator_type>();   // Get first byte of address
			  std::string add2 = IMem[a+1].to_string<char,std::string::traits_type,std::string::allocator_type>(); // Get second byte of address
			  std::string add3 = IMem[a+2].to_string<char,std::string::traits_type,std::string::allocator_type>(); // Get third byte of address
			  std::string add4 = IMem[a+3].to_string<char,std::string::traits_type,std::string::allocator_type>(); // Get Fourth byte of address
			  address.memAddress = add1 + add2 + add3 + add4 ;													   // Concatinating the bytes of address
			  x = bitset<32>(address.memAddress); 																   // COnverting string to 32 Bitset
			  address.Instruction = x;
              return address;
			  }  
      
      private:
           vector<bitset<8> > IMem;
      
};
      


	  

   
int main()
{

    INSMem myInsMem;
	 RF myRF;
	ALU myALU;	 
	int PC =0;
	bitset<32> programCounter;
	bitset<5> ReadReg1;
	bitset<5> ReadReg2;
	bitset<5> WriteReg;
	bitset<6> AluOperation;
	bitset<6> opcode;
	bitset<1> WriteEnable;
	bitset<32> Writedata;
	string SignImmediate;
	bitset<32> SImm;
	
	
   
    while(1)
	{
			 
		programCounter = bitset<32>(PC);

        // Fetch
		
		// If current insturciton is "11111111111111111111111111111111", then break;
		if (myInsMem.ReadMemory(programCounter).memAddress == "11111111111111111111111111111111" )
		{
			break;
		}
        
		// decode(Read RF)
		cout << "this is entire instruction";
		cout << myInsMem.ReadMemory(programCounter).Instruction;
		cout << endl;
		cout << "this is short bitset in string ";
		std::string opcodestr = myInsMem.ReadMemory(programCounter).memAddress.substr(0,6);
		std::string ReadReg1str = myInsMem.ReadMemory(programCounter).memAddress.substr(6,11);
		std::string ReadReg2str = myInsMem.ReadMemory(programCounter).memAddress.substr(11,16);
		std::string WriteRegRtypestr = myInsMem.ReadMemory(programCounter).memAddress.substr(16,21);
		std::string WriteRegItypestr = myInsMem.ReadMemory(programCounter).memAddress.substr(11,16);
		std::string AluOperationRtypestr = myInsMem.ReadMemory(programCounter).memAddress.substr(26,32);
		std::string Immediate = myInsMem.ReadMemory(programCounter).memAddress.substr(16,32);
		std::string Sign = myInsMem.ReadMemory(programCounter).memAddress.substr(16,17);
		cout << "sign is "; 
		cout << Sign; 
		if (Sign =="1")
		{
			SignImmediate = ("1111111111111111" + Immediate);
			
		}
		else
		{
			SignImmediate = ("0000000000000000" + Immediate);
		}
		SImm = bitset<32>(SignImmediate);
		cout << opcodestr;
		cout << endl;
		cout << "this is short bitset in binary ";
		opcode = bitset<6>(opcodestr); 
		cout << opcode;
		cout << endl;
		// R Type
		if ((opcodestr == "000000"))
		{
		cout << " THIS IS R TYPE INSTRUCTIOn";
		ReadReg1 = bitset<5>(ReadReg1str) ; 
		cout << "this is readreg1";
		cout << ReadReg1;
		cout << endl;
		ReadReg2 = bitset<5>(ReadReg2str);
		cout <<"this is readreg2";
		cout << ReadReg2;
		cout << endl;
		WriteReg = bitset<5>(WriteRegRtypestr);
		cout <<"this is writereg";
		cout << WriteReg;
		cout << endl;
		AluOperation = bitset<6>(AluOperationRtypestr);
		cout <<"this is Aluoperation";
		cout << AluOperation;
		cout << endl;
		
		Writedata = myALU.ALUOperation(AluOperation,myRF.ReadWrite(ReadReg1,ReadReg2,WriteReg,0,1).op1,myRF.ReadWrite(ReadReg1,ReadReg2,WriteReg,0,1).op2); // calling Alu 
		cout << " THIS IS FINAL OUTPUT";
		cout << Writedata;
		WriteEnable = 1;
		}
		// I Type
		if ((opcodestr == "000100") || (opcodestr == "001001") || (opcodestr == "100011") || (opcodestr == "101011"))
		{
		cout << " THIS IS J TYPE INSTRUCTIOn";
		ReadReg1 = bitset<5>(ReadReg1str) ; 
		cout << "this is readreg1";
		cout << ReadReg1;
		cout << endl;
		ReadReg2 = bitset<5>(ReadReg2str);
		cout <<"this is readreg2";
		cout << ReadReg2;
		cout << endl;
		WriteReg = bitset<5>(WriteRegItypestr);
		cout <<"this is writereg";
		cout << WriteReg;
		cout << endl;
		AluOperation = opcode;
		cout <<"this is aluoperation";
		cout << AluOperation;
		cout << endl;
		bitset<32> x = myRF.ReadWrite(ReadReg1,ReadReg2,WriteReg,0,1).op1;
		cout << " value of x is " << x;
		Writedata = myALU.ALUOperation(001001,x, SImm ); // calling Alu 
		cout << " THIS IS FINAL OUTPUT";
		cout << Writedata;
		
		WriteEnable = 1;
		

		}
		if (opcodestr == "")
		{
			
		}
		// Execute
		
		// Read/Write Mem
		
		// Write back to RF
		 //myRF.ReadWrite(ReadReg1,ReadReg2,WriteReg,Writedata,WriteEnable);
		
       // myRF.OutputRF(); // dump RF;   
		PC = PC +4 ;
	

	   
	}

	//myRF.OutputRF();
    
    
         
    
        
      
        return 0;
        
}
