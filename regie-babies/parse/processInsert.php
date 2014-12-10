<?php
/**
	PHP File for parsing Insert Statements
**/
include('parseProcess.php');
class ProcessInsert extends ParseProcess{
	public $arith_flag = 0;//flag to check if we're performing arithmetic

	//parse values to be inserted
	public function parseInsertValues($stmt,$index){
		//echo $stmt[$index]['token']." ";
		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];
			$token_type=$stmt[$index]['token_type'];

			//get previous token
			if($index>0) $prevTok=$stmt[$index-1]['token'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);
					
			//evaluate each token
			switch ($token) {
				case "INSERT_VALUES": //VALUES keyword (for data insertion)
						if($nextTok=="OPENING_SYMBOL")
							$this->parseInsertValues($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "OPENING_SYMBOL": // opening parenthesis
						// check if previous lexeme is "VALUES"
						if(isset($prevTok)&&($prevTok=="INSERT_VALUES"||
							$nextTok!="INTEGER_LITERAL"))
							$this->parseInsertValues($stmt,$index+1);
						//check next character
						else if(in_array($nextTok, array("OPENING_SYMBOL","NUMERIC_COLUMN_NAME",
							"COLUMN_NAME"))||preg_match("/_LITERAL$/",$nextTok)){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseInsertValues($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "CLOSING_SYMBOL": //closing parenthesis
						//check if NOT in arithmetic mode 
						if($this->arith_flag==0)
							$this->parseInsertValues($stmt,$index+1);
						//check next character
						else if(in_array($nextTok, array("CLOSING_SYMBOL",
							"ARITHMETIC_OPERATOR","ASTERISK_CHARACTER","VALUE_SEPARATOR",
							"END_OF_STATEMENT","SELECT_OPERATOR"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseInsertValues($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "INTEGER_LITERAL": // integer values
						if(in_array($nextTok, array("ARITHMETIC_OPERATOR","ASTERISK_CHARACTER",
							"CLOSING_SYMBOL","VALUE_SEPARATOR"))){
							
							//we entered arithmetic mode
							if($this->arith_flag==0){ 
								$this->arith_flag=1;
								$_SESSION['temp_type'] = $token_type;
							}

							$_SESSION['temp_lex'].=$lexeme;
							$this->parseInsertValues($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case (preg_match("/.+_LITERAL$/",$token)? true: false): //literals (string,etc.)
						if(preg_match("/.+_LITERAL$/",$token)||$nextTok=="CLOSING_SYMBOL"){//comma or closing parenthesis
							$value['lexeme']=$lexeme;
							$value['token_type']=$token_type;
							array_push($_SESSION['set_values'], $value);
							$this->parseInsertValues($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "ARITHMETIC_OPERATOR": // +, -, / operators
						//check if previous value is a number
						if($_SESSION['temp_type']!="INTEGER_TOKEN"){
							// remove slashes
							$lexeme = $this->removeQuotes($lexeme);
		
							echo '<br/>Syntax error: Value next to '.$lexeme.' is not a number.<br/>';
							$_SESSION['error']=1;
						}
						//part of an insert value
						else if(in_array($nextTok,array("INTEGER_LITERAL","OPENING_SYMBOL"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseInsertValues($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "ASTERISK_CHARACTER": // multiplication operator
						//check if previous value is a number
						if($_SESSION['temp_type']!="INTEGER_TOKEN"){
							// remove slashes
							$lexeme = $this->removeQuotes($lexeme);

							echo '<br/>Syntax error: Value next to '.$lexeme.' is not a number.<br/>';
							$_SESSION['error']=1;
						}
						//part of an insert value
						else if(in_array($nextTok,array("INTEGER_LITERAL","OPENING_SYMBOL"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseInsertValues($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR": // comma 
						//do this when in arithmetic mode
						if($this->arith_flag==1){
							$value['lexeme']=$_SESSION['temp_lex'];
							$value['token_type']=$_SESSION['temp_type'];
							array_push($_SESSION['set_values'], $value);

							//reset temp values
							$_SESSION['temp_type']="";
							$_SESSION['temp_lex']="";
							$this->arith_flag = 0; // turn off arithmetic mode

							$this->parseInsertValues($stmt,$index+1);
						}
						else if(preg_match("/_LITERAL$/",$nextTok)||$nextTok=="OPENING_SYMBOL")
							$this->parseInsertValues($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
			}
		}
	}

	//process column names
	public function parseColumnNames($stmt,$index){
		//echo $stmt[$index]['token']." ";
		
		if($index<count($stmt)){
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];
			$token_type=$stmt[$index]['token_type'];
			$table=$_SESSION['tables'];//get table to be processed

			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			//evaluate each token
			switch($token){
				case "OPENING_SYMBOL": //opening parenthesis
						if($nextTok=="COLUMN_NAME") //column name
							$this->parseColumnNames($stmt,$index+1);
						else if($nextTok=="NUMERIC_COLUMN_NAME") //column name
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COLUMN_NAME": //string column type
						// if the column does not exist in the table
						if(!in_array(strtoupper($lexeme), $this->tables[$table]))
							$this->printErrorMessageTable($lexeme,$table);
						else if($nextTok=="VALUE_SEPARATOR"||$nextTok=="CLOSING_SYMBOL"){//comma or closing parenthesis
							$value['lexeme']=$lexeme;
							$value['token_type']=$token_type;
							array_push($_SESSION['columns'], $value);
							$this->parseColumnNames($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "NUMERIC_COLUMN_NAME": //numeric column type
						// if the column does not exist in the table
						if(!in_array(strtoupper($lexeme), $this->tables[$table]))
							$this->printErrorMessageTable($lexeme,$table);
						else if($nextTok=="VALUE_SEPARATOR"||$nextTok=="CLOSING_SYMBOL"){//comma or closing parenthesis
							$value['lexeme']=$lexeme;
							$value['token_type']=$token_type;
							array_push($_SESSION['columns'], $value);
							$this->parseColumnNames($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR": //comma ","
						if($nextTok=="COLUMN_NAME") //string column
							$this->parseColumnNames($stmt,$index+1);
						else if($nextTok=="NUMERIC_COLUMN_NAME") //numeric column
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "CLOSING_SYMBOL": //closing parenthesis
						if($nextTok=="INSERT_VALUES")
							$this->parseInsertValues($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
			}
		}
	}

	//parse start of INSERT Statement
	public function parseInsert($stmt,$index){
		//echo $stmt[$index]['token']." ";
		
		if($index<count($stmt)){
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			//evaluate each token
			switch($token){
				case "INSERT_COMMAND": // INSERT Lexeme
						if($nextTok=="INSERT_OPERATOR")
							$this->parseInsert($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "INSERT_OPERATOR": //INTO statement
						if($nextTok=="TABLE_NAME")
							$this->parseInsert($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "TABLE_NAME": //which table to insert values
						$_SESSION['tables'] = strtoupper($lexeme);//get table name and store in session
						if($nextTok=="INSERT_VALUES"){ //values to be inserted to the table
							$this->parseInsertValues($stmt,$index+1);
							switch($_SESSION['tables']){//all columns of the table will be processed
								case "STUDENT": $_SESSION['columns']=$this->student_cols; break;
								case "STUDENTHISTORY": $_SESSION['columns']=$this->student_hist_cols; break;
								case "COURSE": $_SESSION['columns']=$this->course_cols; break;
								case "COURSEOFFERING": $_SESSION['columns']=$this->course_off_cols; break;
								case "STUDCOURSE": $_SESSION['columns']=$this->studcourse_cols; break;
							}
						}
						else if($nextTok=="OPENING_SYMBOL")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
			}
		}
	}
}
?>