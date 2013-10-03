/*---------------------------------------------------------------------------------------------------------
	NAMES:		Jose Roman; Reynaldo Galvan
	COURSE:		CS-323
	PROJECT:	#1 The LEXER (LEXICAL ANALYZER)
	DATE:		15 June 2013
	PROFESSOR:	Dr. Choi
PURPOSE: This C++ program implements a lexical analyzer function to determine & display individual tokens
and lexemes from information extracted from a user-specified file containing source code of Rat13SU.
Moreover, the program exports the data in a table with two fields: one for the token & another for the 
lexeme to a different file called "filename-output.txt".
---------------------------------------------------------------------------------------------------------*/
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<iomanip>
using namespace std;

//FUNCTION PROTOTYPES
void Lexer(ifstream&,ofstream&);
bool DFSM(char a[][51],int);
int ch_to_col(char);
bool isRejected(int);
bool isKeyWord(char a[][51], int);
bool isSep(char);
bool isOp(char);
bool isInteger(char a[][51], int);

int main()
{	
	ifstream fin; 
	ofstream fout;
	string fileName;
	char ch;
	
	cout << "Please specify the name of the file (including its file extention, e.g., .txt): ";
	cin >> fileName;
	cout <<"\n\t\tYOUR OUTPUT FILE is: "<< fileName+"_Results.txt" <<endl<<endl;
		
	//OPEN file specified by user
	fin.open(fileName);
	
	//VERIFIES if FILE exists
	if(!fin){cout<<"\t\tUh-oh!!\t"<<fileName<<" was not found.\n"<<endl; exit(0);}
	else
	{	//BEGIN writing output to new file
		fout.open(fileName+"_Results.txt");
		
		//WRITE TABLE
		fout<<"\t";
		for(int j=0; j<62; j++)
			fout << "=";
		fout << endl;
		fout <<"\t"<< left << setw(11)<<"TOKEN"<<right<<setw(51)<<"LEXEME"<<endl;
		fout<<"\t";
		for(int j=0; j<62; j++)
			fout << "=";
		fout<<endl;
		fout << setfill('.');
		
		//FUNCTION CALL LEXER
		Lexer(fin,fout);

		//CLOSE input file
		fin.close();

		fout<<"\t";
		for(int j=0; j<62; j++)
			fout << '='; fout <<endl;		

	}
	//CLOSE output file
	fout.close();

	return 0;
}

/*---------------------------------------------------------------------------------------------------------
	NAME:		Lexer
	PURPOSE:	Identifies Valid Tokens for RAT13SU
---------------------------------------------------------------------------------------------------------*/
void Lexer(ifstream& fin,ofstream& fout)
{	//DECLARE VARIABLES
	char ch,cch;
	int wID_row,wID_col,wInRe_row,wInRe_col;
	char wID[100][51]={""};
	char wInRe[100][51]={""};
	//INITIALIZE VARIABLES
	wID_row=wID_col=wInRe_row=wInRe_col=0;
do
{	//FUNCTION CALL LEXER
	fin.get(ch);
	//BEGIN READING UNTIL VALID TOKEN IS FOUND
	if(isalpha(ch) || ch=='_')
	{
		wID[wID_row][wID_col]=ch; wID_col+=1;
		cch=fin.peek();
		if(isalpha(cch) || cch == '_')
		{
			fin>>ch; wID[wID_row][wID_col]=ch; wID_col+=1;
			if(isKeyWord(wID, wID_row))
			{
				fout <<"\t"<< left << setw(11)<<"Keyword"<<right<<setw(51)<<wID[wID_row]<<endl;
				wID_col=0;
				wID_row+=1;
			}
		}
		else
		{	//FUNCTION CALL DFSM
			if (DFSM(wID,wID_row)==true)
			{	//FUNCTION CALL isKeyWord 
				if(isKeyWord(wID, wID_row))
					fout <<"\t"<< left << setw(11)<<"Keyword"<<right<<setw(51)<<wID[wID_row]<<endl;
				else
					fout <<"\t"<< left << setw(11)<<"Identifier"<<right<<setw(51)<<wID[wID_row]<<endl;
				wID_col=0;
				wID_row+=1;
			}
			else if(DFSM(wID,wID_row)==false)
				fout <<"\t"<< left << setw(11)<<"INVALID (1)"<<right<<setw(51)<<wID[wID_row]<<endl;
		}
		if (DFSM(wInRe,wInRe_row)==true)
		{	//FUNCTION CALL isInteger 
			if(isInteger(wInRe, wInRe_row))
			{						
				fout <<"\t"<< left << setw(11)<<"Integer"<<right<<setw(51);
				for(int h=0; h<wInRe_col; h++)
					fout << wInRe[wInRe_row][h];
				fout <<endl;
			}
			else//Real Number
				fout <<"\t"<< left << setw(11)<<"Real"<<right<<setw(51)<<wInRe[wInRe_row]<<endl;
			wInRe_col=0;
			wInRe_row+=1;
		}
	}//SEPARATOR or OPERATOR character
	else if(isSep(ch) || isOp(ch))
	{
		if (DFSM(wID,wID_row)==true)
		{	//FUNCTION CALL isKeyWord 
			if(isKeyWord(wID, wID_row))
				fout <<"\t"<< left << setw(11)<<"Keyword"<<right<<setw(51)<<wID[wID_row]<<endl;
			else
				fout <<"\t"<< left << setw(11)<<"Identifier"<<right<<setw(51)<<wID[wID_row]<<endl;
			wID_col=0;
			wID_row+=1;
		}
		if (DFSM(wInRe,wInRe_row)==true)
		{	//FUNCTION CALL isInteger 
			if(isInteger(wInRe, wInRe_row))
			{						
				fout <<"\t"<< left << setw(11)<<"Integer"<<right<<setw(51);
				for(int h=0; h<wInRe_col; h++)
					fout << wInRe[wInRe_row][h];
				fout <<endl;
			}
			else//Real Number
				fout <<"\t"<< left << setw(11)<<"Real"<<right<<setw(51)<<wInRe[wInRe_row]<<endl;
			wInRe_col=0;
			wInRe_row+=1;
		}

		if (ch=='(')fout <<"\t"<< left << setw(11)<<"Separator"<<right<<setw(51)<<ch<<endl;
		else if (ch==')')fout <<"\t"<< left << setw(11)<<"Separator"<<right<<setw(51)<<ch<<endl;
		else if (ch=='{')fout <<"\t"<< left << setw(11)<<"Separator"<<right<<setw(51)<<ch<<endl;
		else if (ch=='}')fout <<"\t"<< left << setw(11)<<"Separator"<<right<<setw(51)<<ch<<endl;
		else if (ch==',')fout <<"\t"<< left << setw(11)<<"Separator"<<right<<setw(51)<<ch<<endl;
		else if (ch==';')fout <<"\t"<< left << setw(11)<<"Separator"<<right<<setw(51)<<ch<<endl;
		else if (ch==':')
		{
			cch=fin.peek();
			if (cch=='=')
			{
				fout <<"\t"<< left << setw(11)<<"Operator"<<right<<setw(51)<<ch;
				fin>>ch;
				fout<<ch<<endl;					
			}
			else 
				fout <<"\t"<< left << setw(11)<<"Separator"<<right<<setw(51)<<ch<<endl;
		}
		else if (ch=='=')
		{
			cch=fin.peek();
			if (cch =='=')
			{
				fout <<"\t"<< left << setw(11)<<"Operator"<<right<<setw(51)<<ch;
				fin>>ch;
				fout<<ch<<endl;
			}
			else 
				fout <<"\t"<< left << setw(11)<<"INVALID"<<right<<setw(51)<<ch<<endl;
		}
		else if (ch=='!')
		{
			cch=fin.peek();
			if (cch=='=')
			{
				fout <<"\t"<< left << setw(11)<<"Operator"<<right<<setw(51)<<ch;
				fin>>ch;
				fout<<ch<<endl;				
			}
			else 
				fout<<"\t"<< left << setw(11)<<"INVALID"<<right<<setw(51)<<ch<<endl;
		}
		else if (ch=='<')fout <<"\t"<< left << setw(11)<<"Operator"<<right<<setw(51)<<ch<<endl;	
		else if (ch=='>')fout <<"\t"<< left << setw(11)<<"Operator"<<right<<setw(51)<<ch<<endl;	
		else if (ch=='+')fout <<"\t"<< left << setw(11)<<"Operator"<<right<<setw(51)<<ch<<endl;
		else if (ch=='-')fout <<"\t"<< left << setw(11)<<"Operator"<<right<<setw(51)<<ch<<endl;
		else if (ch=='*')fout <<"\t"<< left << setw(11)<<"Operator"<<right<<setw(51)<<ch<<endl;
		else if (ch=='/')fout <<"\t"<< left << setw(11)<<"Operator"<<right<<setw(51)<<ch<<endl;
	}
	else if(isdigit(ch) || ch=='.') //INTEGER or REAL character
	{
		if (DFSM(wID,wID_row)==true)
		{	//FUNCTION CALL isKeyWord 
			if(isKeyWord(wID, wID_row))
				fout <<"\t"<< left << setw(11)<<"Keyword"<<right<<setw(51)<<wID[wID_row]<<endl;
			else
				fout <<"\t"<< left << setw(11)<<"Identifier"<<right<<setw(51)<<wID[wID_row]<<endl;
			wID_col=0;
			wID_row+=1;
		}
		
		wInRe[wInRe_row][wInRe_col]=ch; wInRe_col+=1;
		cch=fin.peek();

		if(isdigit(cch) || cch=='.')
		{
			fin>> ch; wInRe[wInRe_row][wInRe_col]=ch; wInRe_col+=1;
		}
		else
		{	//FUNCTION CALL DFSM
			if (DFSM(wInRe,wInRe_row)==true)
			{	//FUNCTION CALL isInteger 
				if(isInteger(wInRe, wInRe_row))
				{						
					fout <<"\t"<< left << setw(11)<<"Integer"<<right<<setw(51);
					for(int h=0; h<wInRe_col; h++)
					{fout << wInRe[wInRe_row][h];}
					fout <<endl;
				}
				else //Real Number
					fout <<"\t"<< left << setw(11)<<"Real"<<right<<setw(51)<<wInRe[wInRe_row]<<endl;
				wInRe_col=0;
				wInRe_row+=1;
			}
		}
}
	else if(isspace(ch)){}
	else//INVALID CHARACTER
	{
		fout <<"\t"<< left << setw(11)<<"INVALID (3*)"<<right<<setw(51)<<ch<<endl;
		if (DFSM(wInRe,wInRe_row)==true)
		{	//FUNCTION CALL isInteger 
			if(isInteger(wInRe, wInRe_row))
			{						
				fout <<"\t"<< left << setw(11)<<"Integer"<<right<<setw(51);
				for(int h=0; h<wInRe_col; h++)
					fout << wInRe[wInRe_row][h];
				fout <<endl;
			}
			else //Real Number
				fout <<"\t"<< left << setw(11)<<"Real"<<right<<setw(51)<<wInRe[wInRe_row]<<endl;
			wInRe_col=0;
			wInRe_row+=1;
		}
		else if(DFSM(wInRe,wInRe_row)==false)
			fout <<"\t"<< left << setw(11)<<"INVALID (1)"<<right<<setw(51)<<wInRe[wInRe_row]<<endl;

		if (DFSM(wID,wID_row)==true)
		{	//FUNCTION CALL isKeyWord 
			if(isKeyWord(wID, wID_row))
				fout <<"\t"<< left << setw(11)<<"Keyword"<<right<<setw(51)<<wID[wID_row]<<endl;
			else
				fout <<"\t"<< left << setw(11)<<"Identifier"<<right<<setw(51)<<wID[wID_row]<<endl;
			wID_col=0;
			wID_row+=1;
		}
		else if(DFSM(wID,wID_row)==false)
			fout <<"\t"<< left << setw(11)<<"INVALID (1)"<<right<<setw(51)<<wID[wID_row]<<endl;
	}
}while(!fin.eof());
}
/*---------------------------------------------------------------------------------------------------------
	NAME:		DFSM
	PURPOSE:	Determines whether a given C-String is ACCEPTED or REJECTED by RAT13SU language
---------------------------------------------------------------------------------------------------------*/
bool DFSM(char a[][51],int currentRow)
{
	int state;
	int Values[27][20];//a 27 by 20 matrix to store the table values for the DFSM
	
	// Initialize Table ARRAY for DFSM to EMPTY
	for(int row=0; row<27;row++)
		for(int col=0; col<20; col++)
			Values[row][col]=26;

	//RE-initialize indices
	Values[0][0]=1; Values[0][2]=4; Values[0][3]=5; Values[0][4]=10; Values[0][5]=11; 
	Values[0][6]=12; Values[0][7]=13; Values[0][8]=14; Values[0][9]=15; Values[0][10]=16;
	Values[0][11]=17; Values[0][12]=18; Values[0][13]=19; Values[0][14]=20; Values[0][15]=21;
	Values[0][16]=22; Values[0][17]=23; Values[0][18]=24; Values[0][19]=25;

	Values[1][0]=2;Values[1][1]=3;
	Values[2][0]=2;Values[2][1]=3;
	Values[3][0]=9;Values[3][1]=3;
	
	Values[9][0]=1;
	Values[9][1]=3;
	
	Values[4][2]=4;Values[4][3]=5;
	Values[5][2]=7;
	Values[6][2]=8;
	Values[7][2]=7;
	Values[8][2]=8;

	//INITIALIZE STARTING STATE
	state=0;

	for(int i=0;i<strlen(a[currentRow]);i++)
		state=Values[state][ch_to_col(a[currentRow][i])];
	if (isRejected(state))
		return false;
	else
		return true;
}
/*---------------------------------------------------------------------------------------------------------
	NAME:		ch_to_col
	PURPOSE:	Generates the column number of a particular character
---------------------------------------------------------------------------------------------------------*/
int ch_to_col(char CP)
{
	if (isalpha(CP))return 0;
	else if (CP=='_')return 1;
	else if (isdigit(CP))return 2;
	else if (CP=='.')return 3;
}
/*---------------------------------------------------------------------------------------------------------
	NAME:		isRejected
	PURPOSE:	Determines whether a given C-String is either rejected or accepted
---------------------------------------------------------------------------------------------------------*/
bool isRejected(int currentState)
{
	switch(currentState)
	{
	case 0:
	case 3:
	case 5:
	case 26:
		return true;
	default:
		return false;
	}
}
/*---------------------------------------------------------------------------------------------------------
	NAME:		isKeyWord
	PURPOSE:	Determines whether an identifier is a RAT13SU Keyword
---------------------------------------------------------------------------------------------------------*/
bool isKeyWord(char a[][51], int currentRow)
{
	char keyWords[13][9]={"int","if","else","fi","return","read",
		"function","boolean","real","write","while","true","false"};
	
	int L=0;
	for (int L=0; L<13;L++)
		if(strcmp(keyWords[L],a[currentRow])==0)
			return true;
	return false;
}
/*---------------------------------------------------------------------------------------------------------
	NAME:		isSep.
	PURPOSE:	Determines whether a character is a separator.
---------------------------------------------------------------------------------------------------------*/
bool isSep(char c)
{
	switch(c)
	{
	case ':':
	case '(':
	case ')':
	case '{':
	case '}':
	case ',':
	case ';': return true;
	default: return false;
	}
}
/*---------------------------------------------------------------------------------------------------------
	NAME:		isOp.
	PURPOSE:	Determines whether a character is an operator.
---------------------------------------------------------------------------------------------------------*/
bool isOp(char c)
{	
	char cc;
	if(c==':' || c=='=' || c=='!')
	{
		cc=cin.peek();
		if(cc=='=')
			return true;
	}
	else
	{
		switch(c)
		{
		case '<':
		case '>':
		case '+':
		case '-':
		case '*':
		case '/':
			return true;
		default:
			return false;
		}
	}
}
/*---------------------------------------------------------------------------------------------------------
	NAME:		isInteger.
	PURPOSE:	Distinguishes whether a character is an Integer.
---------------------------------------------------------------------------------------------------------*/
bool isInteger(char a[][51], int currentRow)
{
	for (int i=0; i<strlen(a[currentRow]);i++)
		if (a[currentRow][i]=='.')
			return false;
	return true;
}

/**********************************SAMPLE_OUTPUT**********************************
Please specify the name of the file (including its file extention, e.g., .txt):
SAMPLE.txt

                YOUR OUTPUT FILE is: SAMPLE.txt_Results.txt

Press any key to continue . . .

	==============================================================
	TOKEN                                                   LEXEME
	==============================================================
	Keyword...............................................function
	Identifier.............................................convert
	Separator....................................................(
	Identifier................................................fahr
	Separator....................................................:
	Keyword....................................................int
	Separator....................................................)
	Separator....................................................{
	Keyword.................................................return
	Integer......................................................5
	Operator.....................................................*
	Separator....................................................(
	Identifier................................................fahr
	Operator.....................................................-
	Integer......................................................32
	Separator....................................................)
	Operator...................................................../
	Integer......................................................9
	Separator....................................................;
	Separator....................................................}
	Keyword....................................................int
	Identifier.................................................low
	Separator....................................................,
	Identifier................................................high
	Separator....................................................,
	Identifier................................................step
	Separator....................................................;
	Keyword...................................................read
	Separator....................................................(
	Identifier.................................................low
	Separator....................................................,
	Identifier................................................high
	Separator....................................................,
	Identifier................................................step
	Separator....................................................)
	Separator....................................................;
	Keyword..................................................while
	Separator....................................................(
	Identifier.................................................low
	Operator.....................................................<
	Identifier................................................high
	Separator....................................................)
	Separator....................................................{
	Keyword..................................................write
	Separator....................................................(
	Identifier.................................................low
	Separator....................................................)
	Separator....................................................;
	Keyword..................................................write
	Separator....................................................(
	Identifier.............................................convert
	Separator....................................................(
	Identifier.................................................low
	Separator....................................................)
	Separator....................................................)
	Separator....................................................;
	Identifier.................................................low
	Operator.....................................................:=
	Identifier.................................................low
	Operator.....................................................+
	Identifier................................................step
	Separator....................................................;
	Separator....................................................}
	Keyword....................................................int
	Identifier.................................................low
	Operator.....................................................:=
	Integer......................................................124
	Separator....................................................,
	Identifier................................................high
	Operator.....................................................:=
	Real.....................................................23.90
	Separator....................................................,
	Identifier................................................step
	Separator....................................................;
	Keyword..................................................while
	Separator....................................................(
	Identifier................................................farh
	Operator.....................................................<
	Identifier...............................................upper
	Separator....................................................)
	Identifier...................................................a
	Operator.....................................................:=
	Integer......................................................23
	Separator....................................................;
	Keyword..................................................while
	Separator....................................................(
	Identifier................................................farh
	Operator.....................................................<
	Identifier...............................................upper
	Separator....................................................)
	Identifier...................................................a
	Operator.....................................................:=
	Real.....................................................23.00
	Separator....................................................;
	Separator....................................................;
	==============================================================

**********************************SAMPLE_OUTPUT**********************************/