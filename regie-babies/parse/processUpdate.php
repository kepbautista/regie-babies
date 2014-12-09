<?php
/**
	PHP File for parsing Update Statements
**/
class processUpdate extends ParseProcess{
	public $assign_flag = 0;// flag to check if we're assigning values
	public $values = "";

	// reset session variables used for temporary storage
	public function resetTempVariables(){
		$value['lexeme']=$_SESSION['temp_lex'];
		$value['token_type']=$_SESSION['temp_type'];

		//add to set_values here...
		array_push($_SESSION['set_values'], $value);

		//reset temporary values (for storage)
		$this->assign_flag = 0;
		$_SESSION['temp_lex'] = "";
		$_SESSION['temp_type'] = "";
	}

	//parse start of UPDATE statement
	public function parseUpdate($stmt,$index){
		//echo $stmt[$index]['token']." ";

		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];
			$token_type=$stmt[$index]['token_type'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			$table=$_SESSION['tables'];//get which table is currently being processed

			$p = new ProcessWhereStmt();//instantiate a ProcessWhereStmt class

			//evaluate each token
			switch($token){
				case "UPDATE_COMMAND": //UPDATE statement
						if($nextTok=="TABLE_NAME")
							$this->parseUpdate($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "TABLE_NAME": //name of the table
						$_SESSION['tables']=strtoupper($lexeme);
						if($nextTok=="UPDATE_OPERATOR")
							$this->parseUpdate($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;	
				case "UPDATE_OPERATOR": // SET lexeme
						// cannot find assignment operator for SET
						if($this->getNextLexeme($stmt,$index+1)!="="){
							$_SESSION['error']=1;
							echo '<br/>Syntax Error: Expected "=" near SET';
						}
						else if($nextTok=="COLUMN_NAME"||$nextTok=="NUMERIC_COLUMN_NAME")
							$this->parseUpdate($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "OPENING_SYMBOL": // opening parenthesis "("
						//check next character
						if(in_array($nextTok, array("OPENING_SYMBOL","NUMERIC_COLUMN_NAME",
							"COLUMN_NAME"))||preg_match("/_LITERAL$/",$nextTok)){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "CLOSING_SYMBOL": // closing parenthesis ")"
						//check next character
						if(in_array($nextTok, array("CLOSING_SYMBOL",
							"ARITHMETIC_OPERATOR","ASTERISK_CHARACTER","VALUE_SEPARATOR",
							"END_OF_STATEMENT","SELECT_OPERATOR"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COLUMN_NAME": //name of a non-numerical column
						// if the column does not exist in the table
						if(!in_array(strtoupper($lexeme), $this->tables[$table]))
							$this->printErrorMessageTable($lexeme,$table);
						// the column is to be updated
						else if(($this->assign_flag==0)&&($nextLex=="=")){
							$value['lexeme']=$lexeme;
							$value['token_type']=$token_type;
							$_SESSION['temp_type']=$token_type;
							array_push($_SESSION['columns'], $value);
							$this->assign_flag=1;
							$this->parseUpdate($stmt,$index+1);
						}
						//incompatible data types
						else if($_SESSION['temp_type']!=$token_type)
							$this->printErrorMessageSetType($lexeme,$token_type);
						/* 
							the column is part of the new value to be assigned and
							read a comma "," or closing parenthesis ")"
						*/
						else if(in_array($nextTok, array("VALUE_SEPARATOR","CLOSING_SYMBOL",
								"END_OF_STATEMENT","SELECT_OPERATOR"))&&$this->assign_flag==1){
								$_SESSION['temp_lex'].=$lexeme;
								$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "NUMERIC_COLUMN_NAME":
						// if the column does not exist in the table
						if(!in_array(strtoupper($lexeme), $this->tables[$table]))
							$this->printErrorMessageTable($lexeme,$table);
						//evaluate a WHERE clause
						else if($nextTok=="SELECT_OPERATOR") $this->parseUpdate($stmt,$index+1);
						//incompatible data types
						else if($_SESSION['temp_type']!=$token_type)
							$this->printErrorMessageSetType($lexeme,$token_type);
						// name of column to be updated
						else if(($this->assign_flag==0)&&($nextLex=="=")){
							$value['lexeme']=$lexeme;
							$value['token_type']=$token_type;
							$_SESSION['temp_type']=$token_type;
							array_push($_SESSION['columns'], $value);
							$this->assign_flag=1;
							$this->parseUpdate($stmt,$index+1);
						}
						else if(($this->assign_flag==1)&&in_array($nextTok, 
							array("OPENING_SYMBOL","CLOSING_SYMBOL",
							"ARITHMETIC_OPERATOR","ASTERISK_CHARACTER","VALUE_SEPARATOR",
							"END_OF_STATEMENT","SELECT_OPERATOR"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "NULL_LITERAL":
						//evaluate a WHERE clause
						if($nextTok=="SELECT_OPERATOR") $this->parseUpdate($stmt,$index+1);
						/* 
							the literal is part of the new value to be assigned and
							read a comma "," or closing parenthesis ")", semicolon ";", or WHERE
						*/
						else if($this->assign_flag==1&&in_array($nextTok, array("VALUE_SEPARATOR","CLOSING_SYMBOL",
								"END_OF_STATEMENT","SELECT_OPERATOR"))){
								$_SESSION['temp_lex'].=$lexeme;
								$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "DATE_LITERAL":
						//evaluate a WHERE clause
						if($nextTok=="SELECT_OPERATOR") $this->parseUpdate($stmt,$index+1);
						//incompatible data types
						else if($_SESSION['temp_type']!=$token_type)
							$this->printErrorMessageSetType($lexeme,$token_type);
						/* 
							the literal is part of the new value to be assigned and
							read a comma "," or closing parenthesis ")", semicolon ";", or WHERE
						*/
						else if($this->assign_flag==1&&in_array($nextTok, array("VALUE_SEPARATOR","CLOSING_SYMBOL",
								"END_OF_STATEMENT","SELECT_OPERATOR"))){
								$_SESSION['temp_lex'].=$lexeme;
								$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "TIME_LITERAL":
						//evaluate a WHERE clause
						if($nextTok=="SELECT_OPERATOR") $this->parseUpdate($stmt,$index+1);
						//incompatible data types
						else if($_SESSION['temp_type']!=$token_type)
							$this->printErrorMessageSetType($lexeme,$token_type);
						/* 
							the literal is part of the new value to be assigned and
							read a comma "," or closing parenthesis ")", semicolon ";", or WHERE
						*/
						else if($this->assign_flag==1&&in_array($nextTok, array("VALUE_SEPARATOR","CLOSING_SYMBOL",
								"END_OF_STATEMENT","SELECT_OPERATOR"))){
								$_SESSION['temp_lex'].=$lexeme;
								$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "STUDENT_NUMBER_LITERAL":
						//evaluate a WHERE clause
						if($nextTok=="SELECT_OPERATOR") $this->parseUpdate($stmt,$index+1);
						//incompatible data types
						else if($_SESSION['temp_type']!=$token_type)
							$this->printErrorMessageSetType($lexeme,$token_type);
						/* 
							the literal is part of the new value to be assigned and
							read a comma "," or closing parenthesis ")", semicolon ";", or WHERE
						*/
						else if($this->assign_flag==1&&in_array($nextTok, array("VALUE_SEPARATOR","CLOSING_SYMBOL",
								"END_OF_STATEMENT","SELECT_OPERATOR"))){
								$_SESSION['temp_lex'].=$lexeme;
								$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "STRING_LITERAL":
						//evaluate a WHERE clause
						if($nextTok=="SELECT_OPERATOR") $this->parseUpdate($stmt,$index+1);
						//incompatible data types
						else if($_SESSION['temp_type']!=$token_type)
							$this->printErrorMessageSetType($lexeme,$token_type);
						/* 
							the literal is part of the new value to be assigned and
							read a comma "," or closing parenthesis ")", semicolon ";", or WHERE
						*/
						else if($this->assign_flag==1&&in_array($nextTok, array("VALUE_SEPARATOR","CLOSING_SYMBOL",
								"END_OF_STATEMENT","SELECT_OPERATOR"))){
								$_SESSION['temp_lex'].=$lexeme;
								$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "INTEGER_LITERAL":
						//evaluate a WHERE clause
						if($nextTok=="SELECT_OPERATOR") $this->parseUpdate($stmt,$index+1);
						//incompatible data types
						else if($_SESSION['temp_type']!=$token_type)
							$this->printErrorMessageSetType($lexeme,$token_type);
						//check next lexeme
						else if(in_array($nextTok, array("OPENING_SYMBOL","CLOSING_SYMBOL",
							"ARITHMETIC_OPERATOR","ASTERISK_CHARACTER","VALUE_SEPARATOR",
							"END_OF_STATEMENT","SELECT_OPERATOR"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "ARITHMETIC_OPERATOR": // +, -, / operators
						//check if previous value is a number
						if($_SESSION['temp_type']!="INTEGER_TOKEN"){
							echo '<br/>Syntax error: Value next to '.$lexeme.' is not a number.<br/>';
							$_SESSION['error']=1;
						}
						//part of an update value
						else if(in_array($nextTok,array("NUMERIC_COLUMN_NAME","INTEGER_LITERAL",
							"OPENING_SYMBOL"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "ASTERISK_CHARACTER": //multiplication operator
						//check if previous value is a number
						if($_SESSION['temp_type']!="INTEGER_TOKEN"){
							echo '<br/>Syntax error: Value next to '.$lexeme.' is not a number.<br/>';
							$_SESSION['error']=1;
						}
						//part of an update value
						else if(in_array($nextTok,array("NUMERIC_COLUMN_NAME","INTEGER_LITERAL",
							"OPENING_SYMBOL"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COMPARISON_OPERATOR_EQUALITY": // ASSIGNMENT operator
						//not an assignment operator
						if($lexeme!="=") $this->printErrorMessageAfter($lexeme,$nextLex);
						else if(in_array($nextTok,array("COLUMN_NAME","NUMERIC_COLUMN_NAME","OPENING_SYMBOL"))
							||preg_match("/_LITERAL$/",$nextTok)){
							$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR": // comma (,) character
						$this->resetTempVariables();

						//execute next set value
						if($nextTok=="COLUMN_NAME"||$nextTok=="NUMERIC_COLUMN_NAME")
							$this->parseUpdate($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "END_OF_STATEMENT": // semicolon (;) character
						$this->resetTempVariables();
						break;
				case "SELECT_OPERATOR": // WHERE clause
						$this->resetTempVariables();
						$p->parseWhereStmt($stmt,$index+1);
						break;
			}
		}
	}
}
?>