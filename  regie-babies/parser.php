<?php
class Parser{
	//property declarations

	//add double quotes at the start & end of string
	public function addDoubleQuotes($str){
		return '"'.$str.'"';
	}

	//check if closing and opening symbols match each other
	public function checkMatchingSymbols($query){
		//initialize stack
		$openSymbols=array("(");//list of opening symbols
		$closeSymbols=array(")");//list of closing symbols
		$stack=array();//stack variable
		$msg="";//variable for storing error message

		$quotectr1 = 0;//count number of single quotes
		$quotectr2 = 0;//count number of double quotes

		//end of the statement is not a ";" character
		if($query[strlen($query)-1]!=";")
			return "Syntax error: expected ';' at the end of input<br/>";
		
		for($i=0;$i<strlen($query);$i++){
			/* Check if opening symbols */
			//if opening symbol, push to stack
			if(in_array($query[$i], $openSymbols)){
				array_push($stack,$query[$i]);
			}
			//if symbol is an opening single quote
			else if(($query[$i]=="'")&&($quotectr1%2==0)){
				array_push($stack,$query[$i]);
				$quotectr1+=1;
			}
			//if symbol is an opening double quote
			else if(($query[$i]=='"')&&($quotectr2%2==0)){
				array_push($stack,$query[$i]);
				$quotectr2+=1;
			}

			/* Check if closing symbols */
			//if closing symbol, check if it matches at the top of stack
			else if((in_array($query[$i], $closeSymbols))
					||(($query[$i]=="'")&&($quotectr1%2!=0))//closing single quote
					||(($query[$i]=='"')&&($quotectr2%2!=0))//closing double quote
					){

				//error in parentheses
				if(empty($stack)||(($query[$i]==')')&&(array_pop($stack)!='(')))
					return "Syntax error: expected '(' or identifier before ')'<br/>";
				//error in single quotes
				else if(($query[$i]=="'")&&(array_pop($stack)!="'"))
					return "Syntax error: expected ' or identifier before ' <br/>";
				//error in double quotes
				else if(($query[$i]=='"')&&(array_pop($stack)!='"')){
					return 'Syntax error: expected " or identifier before " <br/>';
				}
				else if($query[$i]=="'") $quotectr1+=1;//increment number of single quotes
				else if($query[$i]=='"') $quotectr2+=1;//increment number of double quotes
			}
		}

		//stack is not empty and there is an unclosed opening symbol
		if(!empty($stack)) $msg="Syntax error: unclosed ', \" or ( <br/>";

		return $msg;
	}

	public function tokenizeQuery($query){
		$whiteSpcs=array(" ","\t","\n","\r");//if white spaces
		$spChars=array("(",")","=",",",">","<");//list of special characters
		$tokens=array();//tokens per statement
		$stmts=array();//array of statements
		$tok="";
		$flag1=$flag2=false;//flag1 is for double quotes while flag2 is for single quotes

		for($i=0;$i<strlen($query);$i++){
			if(($flag1==false)&&($query[$i]=='"')){
				$flag1=true;//opening double quote
				$tok=$tok.$query[$i];	
			}
			else if(($flag2==false)&&($query[$i]=="'")){
				$flag2=true;//opening single quote
				$tok=$tok.$query[$i];
			} 
			//processing inside double quotes
			else if($flag1==true){
				if($query[$i]==" ") $tok=$tok." ";
				else if($query[$i]=='"'){
					$flag1=false;//closing quote
					$tok=$tok.$query[$i];
				}
				else $tok=$tok.$query[$i];
			}
			//processing inside single quotes
			else if($flag2==true){
				if($query[$i]==" ") $tok=$tok." ";
				else if($query[$i]=="'"){
					$flag2=false;//closing quote
					$tok=$tok.$query[$i];
				}
				else $tok=$tok.$query[$i];
			}
			else{
				//ignore whitespaces if not found inside quotes
				if(in_array($query[$i], $whiteSpcs)){
					//push token if not empty string
					if($tok!="") array_push($tokens,$tok);
					$tok="";
				}
				//read "=",",","<",">","(" or ")" sign
				else if(in_array($query[$i], $spChars)){
					$char=$query[$i];

					//if the operator is a <= or >=
					if((($char=="<")||($char==">")||($char=="!"))
						&&($query[$i+1]=="=")){
						$char=$char."=";
						$i+=1;//move to 2nd next character
					}

					if($tok!="") array_push($tokens,$tok,$char);
					else array_push($tokens, $char);
					$tok="";
				}
				//end of query ";"
				else if($query[$i]==";"){
					if($tok!="") array_push($tokens,$tok,";");
					else array_push($tokens, ";");
					array_push($stmts,$tokens);
					$tokens=array();
					$tok="";
				}
				else $tok=$tok.$query[$i];
			}
		}

		//return $tokens;
		return $stmts;
	}

	//function for printing the results of the lexical analyzer in a table
	public function printLex($lexemes){
		//table header
		echo "<table><tr><th>Lexeme</th><th>Token</th></tr>";

		foreach($lexemes as $value){
			echo "<tr><td>".$value['lexeme']."</td><td>".$value['token']."</td></tr>";
		}

		echo "</table>";
	}

	//function for performing lexical analysis
	public function lexer($stmts){
		$lexemes=array();//initialize lexemes
		$comparators=array("=",">","<","<=",">=","!=");//list of comparison operators
    $grp_comparators=array("IN","ANY","ALL","SOME");
		$semesters=array("\'1ST\'","\'2ND\'","\'SUM\'",'\"1ST\"','\"2ND\"','\"SUM\"');//list of semesters
		$table_names=array("STUDENT","STUDENTHISTORY","COURSE","COURSEOFFERING","STUDCOURSE");//list of table names
		
		//list of column names
		$column_names=array("STUDNO", "STUDENTNAME", "BIRTHDAY", "DEGREE", "MAJOR", "UNITSEARNED",
							"STUDENT.STUDNO", "STUDENT.STUDENTNAME", "STUDENT.BIRTHDAY", 
							"STUDENT.DEGREE", "STUDENT.MAJOR", "STUDENT.UNITSEARNED",
							"STUDNO", "DESCRIPTION", "ACTION", "DATEFILED", "DATERESOLVED",
							"STUDENTHISTORY.STUDNO", "STUDENTHISTORY.DESCRIPTION", "STUDENTHISTORY.ACTION", 
							"STUDENTHISTORY.DATEFILED", "STUDENTHISTORY.DATERESOLVED",
							"CNO", "CTITLE", "CDESC", "NOOFUNITS", "HASLAB", "SEMOFFERED",
							"COURSE.CNO", "COURSE.CTITLE", "COURSE.CDESC", "COURSE.NOOFUNITS", "COURSE.HASLAB", 
							"COURSE.SEMOFFERED","SEMESTER", "ACADYEAR", "CNO", "SECTION", "TIME", "MAXSTUD",
							"COURSEOFFERING.SEMESTER", "COURSEOFFERING.ACADYEAR", "COURSEOFFERING.CNO", 
							"COURSEOFFERING.SECTION", "COURSEOFFERING.TIME", "COURSEOFFERING.MAXSTUD",
							"STUDNO", "CNO", "SEMESTER", "ACADYEAR",
							"STUDCOURSE.STUDNO", "STUDCOURSE.CNO", "STUDCOURSE.SEMESTER", "STUDCOURSE.ACADYEAR"
							);


		//evaluate each SQL statement
		foreach($stmts as $line){
			//evaluate each lexeme of the SQL statement
			foreach($line as $lexeme){
				$token="hey";
				/*
					check what kind of token is each lexeme
				*/

				//SQL Commands
				if(strtoupper($lexeme)=="SELECT") $token="PROJECT_COMMAND";
				else if(strtoupper($lexeme)=="INSERT") $token="INSERT_COMMAND";
				else if(strtoupper($lexeme)=="UPDATE") $token="UPDATE_COMMAND";
				else if(strtoupper($lexeme)=="DELETE") $token="DELETE_COMMAND";

				//Operators and Reserved Words
        else if(strtoupper($lexeme)=="DISTINCT") $token="DISTINCT_SELECTOR";
				else if(strtoupper($lexeme)=="JOIN") $token="JOIN_OPERATOR";
				else if(strtoupper($lexeme)=="WHERE") $token="SELECT_OPERATOR";
				else if(strtoupper($lexeme)=="FROM") $token="TABLE_SELECT_OPERATOR";
				else if(strtoupper($lexeme)=="ON") $token="JOIN_CONDITION_OPERATOR";
				else if(strtoupper($lexeme)=="INTO") $token="INSERT_OPERATOR";
				else if(strtoupper($lexeme)=="VALUES") $token="INSERT_OPERATOR";
				else if(strtoupper($lexeme)=="SET") $token="UDPATE_OPERATOR";
        else if(strtoupper($lexeme)=="IS") $token="NULL_COMPARISON_KEYWORD";
        else if(strtoupper($lexeme)=="NOT") $token="NOT_NULL_COMPARISON_KEYWORD";
        else if(strtoupper($lexeme)=="NULL") $token="NULL_LITERAL";
        else if(strtoupper($lexeme)=="BETWEEN") $token="BETWEEN_COMPARATOR";
        else if(strtoupper($lexeme)=="AND") $token="BETWEEN_CONDITION_OPERATOR";
        else if(in_array(strtoupper($lexeme),$grp_comparators)) $token="GROUP_COMPARISON_OPERATOR";
        
        else if(strtoupper($lexeme)=="LIMIT") $token="LIMIT_ROWS_OPERATOR";

				//special characters
				else if(in_array($lexeme, $comparators)) $token="COMPARISON_OPERATOR";
				else if($lexeme=="*") $token="ALL_COLUMN_SELECTOR";
				else if($lexeme==";") $token="END_OF_STATEMENT_LITERAL";
				else if($lexeme==",") $token="NAME_SEPARATOR";
				else if($lexeme=="(") $token="OPENING_SYMBOL_LITERAL";
				else if($lexeme==")") $token="CLOSING_SYMBOL_LITERAL";

				//existing tables
				else if(in_array(strtoupper($lexeme), $table_names)) $token="TABLE_NAME_LITERAL";

				//exisiting columns
				else if(in_array(strtoupper($lexeme), $column_names)) $token="COLUMN_NAME_LITERAL";

				//semester value literals
				else if(in_array(strtoupper($lexeme), $semesters)) $token="SEMESTER_LITERAL";

				//Regular expression for checking dates (YYYY-MM-DD)
				else if(preg_match("/^\"[0-9]{1,4}\-[0-9]{1,2}\-[0-9]{1,2}\"$/", str_replace("\\","",$lexeme))) $token="DATE_LITERAL";
				else if(preg_match("/^\'[0-9]{1,4}\-[0-9]{1,2}\-[0-9]{1,2}\'$/", str_replace("\\","",$lexeme))) $token="DATE_LITERAL";

				//Regular expression for checking time (Military Time Format)
				else if(preg_match("/^\"[012]?[0-9]\:[0-5][0-9]\"$/", str_replace("\\","",$lexeme))) $token="TIME_LITERAL";
				else if(preg_match("/^\'[012]?[0-9]\:[0-5][0-9]\'$/", str_replace("\\","",$lexeme))) $token="TIME_LITERAL";

				//Regular expression for checking student number (YYYY-XXXXX)
				else if(preg_match("/^\"[0-9]{4}\-[0-9]{5}\"$/", str_replace("\\","",$lexeme))) $token="STUDENT_NUMBER_LITERAL";
				else if(preg_match("/^\'[0-9]{4}\-[0-9]{5}\'$/", str_replace("\\","",$lexeme))) $token="STUDENT_NUMBER_LITERAL";

				//Regular expression for String Literals
				else if(preg_match("/^\".{1,50}\"$/", str_replace("\\","",$lexeme))) $token="STRING_LITERAL";
				else if(preg_match("/^\'.{1,50}\'$/", str_replace("\\","",$lexeme))) $token="STRING_LITERAL";

				//Regular expression for checking integer literals
				else if(preg_match("/\-?[0-9]+/", $lexeme)) $token="INTEGER_LITERAL";

				//add lexeme and token to list of lexemes
				$value["lexeme"]=$lexeme;
				$value["token"]=$token;
				array_push($lexemes,$value);
			}
		}

		return $lexemes;
	}
}
?>