<?php
include('parse/processInsert.php');
include('parse/processDelete.php');
include('parse/processUpdate.php');
include('parse/processSelect.php');
include('parse/parseGroups.php');
include('parse/processJoin.php');

class Parser{
	//property declarations

	//add double quotes at the start & end of string
	public function addDoubleQuotes($str){
		return '"'.$str.'"';
	}

	public function checkMatchingSymbols($stmts){
		//initialize stack
		$stack=array();//stack variable
		$msg="";//variable for storing error message

		foreach($stmts as $stmt){
			foreach($stmt as $lexeme){
				//if opening symbol, push to stack
				if($lexeme['token']=="OPENING_SYMBOL"){
					array_push($stack,$lexeme['token']);
				}
				/* Check if closing symbol */
				else if($lexeme['token']=="CLOSING_SYMBOL"){
					//error in parentheses
					if(empty($stack)||(($lexeme['token']=='CLOSING_SYMBOL')&&(array_pop($stack)!='OPENING_SYMBOL'))){
						$_SESSION['error']=1;
						return "<br/>Syntax error: expected '(' or identifier before ')'";
					}
				}
			}
		}

		//stack is not empty and there is an unclosed opening symbol
		if(!empty($stack)){
			$_SESSION['error']=1;
			$msg="<br/>Syntax error: unclosed (";
		}

		return $msg;
	}

	//check if closing and opening quotes match each other
	public function checkMatchingQuotes($query){
		//initialize stack
		$stack=array();//stack variable
		$msg="";//variable for storing error message

		$quotectr1 = 0;//count number of single quotes
		$quotectr2 = 0;//count number of double quotes

		//end of the statement is not a ";" character
		if($query[strlen($query)-1]!=";"){
			$_SESSION['error']=1;
			return "<br/>Syntax error: expected ';' at the end of input";
		}
	
		for($i=0;$i<strlen($query);$i++){
			//if symbol is an opening single quote
			if(($query[$i]=="'")&&($quotectr1%2==0)){
				array_push($stack,$query[$i]);
				$quotectr1+=1;
			}
			//if symbol is an opening double quote
			else if(($query[$i]=='"')&&($quotectr2%2==0)){
				array_push($stack,$query[$i]);
				$quotectr2+=1;
			}
			/* Check if closing quotes */
			//if closing symbol, check if it matches at the top of stack
			else if((($query[$i]=="'")&&($quotectr1%2!=0))//closing single quote
					||(($query[$i]=='"')&&($quotectr2%2!=0))//closing double quote
					){

				//error in single quotes
				if(($query[$i]=="'")&&(array_pop($stack)!="'")){
					$_SESSION['error']=1;
					return "<br/>Syntax error: expected ' or identifier before ' ";
				}
				//error in double quotes
				else if(($query[$i]=='"')&&(array_pop($stack)!='"')){
					$_SESSION['error']=1;
					return '<br/>Syntax error: expected " or identifier before " ';
				}
				else if($query[$i]=="'") $quotectr1+=1;//increment number of single quotes
				else if($query[$i]=='"') $quotectr2+=1;//increment number of double quotes
			}
		}

		//stack is not empty and there is an unclosed opening symbol
		if(!empty($stack)) $msg="<br/>Syntax error: unclosed ' or \"";

		return $msg;
	}

	public function tokenizeQuery($query){
		$whiteSpcs=array(" ","\t","\n","\r");//if white spaces
		$spChars=array("(",")","=",",",">","<","+","-","*","/");//list of special characters
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

					//if the operator is a <=, >= and !=
					if((($char=="<")||($char==">")||($char=="!"))
						&&($query[$i+1]=="=")){
						$char=$char."=";
						$i+=1;//move to 2nd next character
					}
              if(($char=="<")&&($query[$i+1]==">")){
                $char=$char.">";
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
	public function printLex($stmts){
		//table header
		echo "<table><tr><th>Lexeme</th><th>Token</th><th>Type</th><th>Length</th></tr>";

		foreach($stmts as $line){
			foreach($line as $value){
				echo "<tr><td>".$value['lexeme']."</td><td>".$value['token']."</td><td>";
				echo $value['token_type']."</td><td>";
				if ($value['token']=="STRING_LITERAL") echo strlen($value['lexeme'])-4;
				else echo strlen($value['lexeme']);
				echo "</td></tr>";
			}
		}

		echo "</table>";
	}

	//function for performing lexical analysis
	public function lexer($lines){
		$lexemes=array();//initialize lexemes
		$stmts=array();//initialize statements
		$comparators=array(">","<","<=",">=","<>");//list of comparison operators
		$arithmetics=array("+","-","/");//list of arithmetic operators
    	$grp_comparators=array("IN","ANY","ALL","SOME");
		$semesters=array("\'1ST\'","\'2ND\'","\'SUM\'",'\"1ST\"','\"2ND\"','\"SUM\"');//list of semesters
		$table_names=array("STUDENT","STUDENTHISTORY","COURSE","COURSEOFFERING","STUDCOURSE");//list of table names
		
		//list of non-numeric column names 
		$column_names=array("STUDNO", "STUDENTNAME", "DEGREE", "MAJOR",
							"STUDENT.STUDNO", "STUDENT.STUDENTNAME", 
							"STUDENT.DEGREE", "STUDENT.MAJOR",
							"STUDNO", "DESCRIPTION", "ACTION",
							"STUDENTHISTORY.STUDNO", "STUDENTHISTORY.DESCRIPTION", "STUDENTHISTORY.ACTION",
							"CNO", "CTITLE", "CDESC", "SEMOFFERED",
							"COURSE.CNO", "COURSE.CTITLE", "COURSE.CDESC", 
							"COURSE.SEMOFFERED","SEMESTER", "ACADYEAR", "CNO", "SECTION", "TIME", 
							"COURSEOFFERING.SEMESTER", "COURSEOFFERING.ACADYEAR", "COURSEOFFERING.CNO", 
							"COURSEOFFERING.SECTION", "COURSEOFFERING.TIME",
							"STUDNO", "CNO", "SEMESTER", "ACADYEAR",
							"STUDCOURSE.STUDNO", "STUDCOURSE.CNO", "STUDCOURSE.SEMESTER", "STUDCOURSE.ACADYEAR",
							"BIRTHDAY","STUDENT.BIRTHDAY","DATEFILED","DATERESOLVED",
							"STUDENTHISTORY.DATEFILED", "STUDENTHISTORY.DATERESOLVED"
							);

		//list of numeric column names
		$numeric_columns=array("UNITSEARNED", "NOOFUNITS", "HASLAB", "MAXSTUD",
							   "STUDENT.UNITSEARNED", "COURSE.NOOFUNITS",
							   "COURSE.HASLAB", "COURSEOFFERING.MAXSTUD"
							  );

		//list of date-typed column names
		$date_columns = array("BIRTHDAY","STUDENT.BIRTHDAY","DATEFILED","DATERESOLVED",
							  "STUDENTHISTORY.DATEFILED", "STUDENTHISTORY.DATERESOLVED"
							  );


		//evaluate each SQL statement
		foreach($lines as $line){
			//evaluate each lexeme of the SQL statement
			foreach($line as $lexeme){
				$token="UNRECOGNIZED_WORD";
				$token_type="NORMAL_WORD";
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
				else if(strtoupper($lexeme)=="VALUES") $token="INSERT_VALUES";
				else if(strtoupper($lexeme)=="SET") $token="UPDATE_OPERATOR";
		        else if(strtoupper($lexeme)=="IS") $token="NULL_COMPARISON_KEYWORD";
		        else if(strtoupper($lexeme)=="NOT") $token="NOT_NULL_COMPARISON_KEYWORD";
		        else if(strtoupper($lexeme)=="NULL"){
		        	$token="NULL_LITERAL";
		        	$token_type="NULL_TOKEN";
		        }
		        else if(strtoupper($lexeme)=="BETWEEN") $token="BETWEEN_COMPARATOR";
		        else if(strtoupper($lexeme)=="AND") $token="BETWEEN_CONDITION_OPERATOR";
		        else if(in_array(strtoupper($lexeme),$grp_comparators)) $token="GROUP_COMPARISON_OPERATOR";
        		else if(strtoupper($lexeme)=="LIMIT") $token="LIMIT_ROWS_OPERATOR";
        		else if(strtoupper($lexeme)=="LIKE") $token="COMPARISON_OPERATOR_STRING";

				//special characters
				else if(in_array($lexeme, $comparators)) $token="COMPARISON_OPERATOR";
				else if(in_array($lexeme, $arithmetics)) $token="ARITHMETIC_OPERATOR";
				else if($lexeme=="*") $token="ASTERISK_CHARACTER";
				else if($lexeme=="="||$lexeme=="!=") $token="COMPARISON_OPERATOR_EQUALITY";
				else if($lexeme==";") $token="END_OF_STATEMENT";
				else if($lexeme==",") $token="VALUE_SEPARATOR";
				else if($lexeme=="(") $token="OPENING_SYMBOL";
				else if($lexeme==")") $token="CLOSING_SYMBOL";

				//existing tables
				else if(in_array(strtoupper($lexeme), $table_names)) $token="TABLE_NAME";

				//exisiting columns (non-numeric)
				else if(in_array(strtoupper($lexeme), $column_names)) $token="COLUMN_NAME";

				//existing columns (numeric)
				else if(in_array(strtoupper($lexeme), $numeric_columns)){
					$token="NUMERIC_COLUMN_NAME";
					$token_type="INTEGER_TOKEN";
				}

				//semester value literals
				else if(in_array(strtoupper($lexeme), $semesters)){
					$token="SEMESTER_LITERAL";
					$token_type="SEMESTER_TOKEN";
				}

				//Regular expression for checking dates (YYYY-MM-DD)
				else if(preg_match("/^\"[0-9]{1,4}\-[0-9]{1,2}\-[0-9]{1,2}\"$/", str_replace("\\","",$lexeme))){
					$token="DATE_LITERAL";
					$token_type="DATE_TOKEN";	
				} 
				else if(preg_match("/^\'[0-9]{1,4}\-[0-9]{1,2}\-[0-9]{1,2}\'$/", str_replace("\\","",$lexeme))){
					$token="DATE_LITERAL";
					$token_type="DATE_TOKEN";
				}

				//Regular expression for checking time (Military Time Format)
				else if(preg_match("/^\"[012]?[0-9]\:[0-5][0-9]\"$/", str_replace("\\","",$lexeme))){
					$token="TIME_LITERAL";
					$token_type="TIME_TOKEN";
				}
				else if(preg_match("/^\'[012]?[0-9]\:[0-5][0-9]\'$/", str_replace("\\","",$lexeme))){
					$token="TIME_LITERAL";
					$token_type="TIME_TOKEN";
				}

				//Regular expression for checking student number (YYYY-XXXXX)
				else if(preg_match("/^\"[0-9]{4}\-[0-9]{5}\"$/", str_replace("\\","",$lexeme))){
					$token="STUDENT_NUMBER_LITERAL";
					$token_type="STUDENT_NUMBER_TOKEN";	
				}
				else if(preg_match("/^\'[0-9]{4}\-[0-9]{5}\'$/", str_replace("\\","",$lexeme))){
					$token="STUDENT_NUMBER_LITERAL";
					$token_type="STUDENT_NUMBER_TOKEN";
				}

				//Regular expression for String Literals
				else if(preg_match("/^\"(.|\s){0,50}\"$/", str_replace("\\","",$lexeme))) $token="STRING_LITERAL";
				else if(preg_match("/^\'(.|\s){0,50}\'$/", str_replace("\\","",$lexeme))) $token="STRING_LITERAL";

				//Regular expression for checking integer literals
				else if(preg_match("/^\-?[0-9]+$/", $lexeme)){
					$token="INTEGER_LITERAL";
					$token_type="INTEGER_TOKEN";
				}

				//assign special types of data
				//date
				if(in_array(strtoupper($lexeme),$date_columns)) $token_type="DATE_TOKEN";
				//time
				else if(preg_match("/.*TIME$/", strtoupper($lexeme))) $token_type="TIME_TOKEN";
				//student number
				else if(preg_match("/.*STUDNO$/", strtoupper($lexeme))) $token_type="STUDENT_NUMBER_TOKEN";
				//semester
				else if(preg_match("/.*SEMESTER$/", strtoupper($lexeme))) $token_type="SEMESTER_TOKEN";
				//semester
				else if(preg_match("/.*SEMOFFERED$/", strtoupper($lexeme))) $token_type="SEMESTER_TOKEN";
				//HASLAB is a BOOLEAN (can only store 1 and 0)
				else if(preg_match("/.*HASLAB$/", strtoupper($lexeme))) $token_type="BOOLEAN_TOKEN";

				//add lexeme and token to list of lexemes
				$value["lexeme"]=$lexeme;
				$value["token"]=$token;
				$value["token_type"]=$token_type;
				array_push($lexemes,$value);
			}
			array_push($stmts,$lexemes);//include all lexemes of a certain statement
			$lexemes=array();//reset lexemes array for the next statement
		}

		return $stmts;
	}

	/*
		Determine what kind of SQL Command to execute
	*/
	public function parseExpression($stmt){
		switch($stmt[0]['token']){
			case "PROJECT_COMMAND": // SELECT statement
					$_SESSION['columns'] = ""; //empty array because no need for type checking
					$_SESSION['command'] = "SELECT";
					$p = new ProcessSelect();
					$p->parseSelect($stmt,0);
					break;
			case "INSERT_COMMAND": //INSERT statement
					$_SESSION['command'] = "INSERT";
					$p = new ProcessInsert();
					$p->parseInsert($stmt,0);
					break;
			case "UPDATE_COMMAND":
					$_SESSION['command'] = "UPDATE";
					$p = new ProcessUpdate();
					$p->parseUpdate($stmt,0);
					break;
			case "DELETE_COMMAND"://DELETE statement
					$_SESSION['command'] = "DELETE";
					$p = new ProcessDelete();
					$p->parseDelete($stmt,0);
					break;
		}
	}
}
?>
