<?php
/**
	PHP File for parsing groups of data
**/
class parseGroups extends ParseProcess{
	public $arith_flag = 0; // flag to check if we're performing arithmetic operations

	// reset temporary SESSION variables
	public function resetTempValues(){
		//first column to be projected
		if($_SESSION['columns']=="")
			$_SESSION['columns'].=$_SESSION['temp_lex'];
		//next column/s to be projected
		else $_SESSION['columns'].=",".$_SESSION['temp_lex'];

		//reset temp values
		$_SESSION['temp_type']="";
		$_SESSION['temp_lex']="";
		$this->arith_flag = 0; // turn off arithmetic mode
	}

	//process column names
	public function parseColumnNames($stmt,$index){
		//echo $stmt[$index]['token']." ";
		$table = $_SESSION['tables'];//get names of tables
		
		$p = new ParseProcess(); // instantiate a ParseProcess clas

		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];
			$token_type=$stmt[$index]['token_type'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			$tables = $_SESSION['tables'];//get names of current tables
			$table_names = explode(",",$tables);

			//evaluate each token
			switch($token){
				case "OPENING_SYMBOL": // open parenthesis
						// check next character
						if(in_array($nextTok, array("OPENING_SYMBOL","NUMERIC_COLUMN_NAME",
							"COLUMN_NAME"))||preg_match("/_LITERAL$/",$nextTok)){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseColumnNames($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "CLOSING_SYMBOL": //closing parenthesis
						//check next character
						if(in_array($nextTok, array("CLOSING_SYMBOL",
							"ARITHMETIC_OPERATOR","ASTERISK_CHARACTER","VALUE_SEPARATOR",
							"END_OF_STATEMENT","SELECT_OPERATOR"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseColumnNames($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COLUMN_NAME": //column name of a table
						// first column to be projected
						if($_SESSION['columns']=="")
							$_SESSION['columns'].=strtoupper($lexeme);
						// next column/s to be projected
						else $_SESSION['columns'].=",".strtoupper($lexeme);

						// if the column does not exist in the table
						if(!$this->matchTables(strtoupper($lexeme)))
							$this->printErrorMessageTable($lexeme,$table);
						else if($nextTok=="VALUE_SEPARATOR"||$nextTok=="CLOSING_SYMBOL")
							$this->parseColumnNames($stmt,$index+1);
						else if($nextTok=="TABLE_SELECT_OPERATOR"){ // FROM statement
							$p = new ProcessSelect();
							$p->parseSelect($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "NUMERIC_COLUMN_NAME": //column name of a table
						// if the column does not exist in the table
						if(!$this->matchTables(strtoupper($lexeme)))
							$this->printErrorMessageTable($lexeme,$table);
						else if(in_array($nextTok, array("VALUE_SEPARATOR","CLOSING_SYMBOL",
							"ARITHMETIC_OPERATOR","ASTERISK_CHARACTER"))){
							//we entered arithmetic mode
							if($this->arith_flag==0){ 
								$this->arith_flag=1;
								$_SESSION['temp_type'] = $token_type;
							}

							$_SESSION['temp_lex'].=strtoupper($lexeme);
							$this->parseColumnNames($stmt,$index+1);
						}
						else if($nextTok=="TABLE_SELECT_OPERATOR"){ // FROM statement
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseColumnNames($stmt,$index+1);
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
							$this->parseColumnNames($stmt,$index+1);
						}
						else if($nextTok=="TABLE_SELECT_OPERATOR"){ // FROM statement
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseColumnNames($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case (preg_match("/.+_LITERAL$/",$token)? true: false): //literals (string,etc.)
						if(preg_match("/.+_LITERAL$/",$token)||$nextTok=="CLOSING_SYMBOL"){//comma or closing parenthesis
							$value['lexeme']=$lexeme;
							$value['token_type']=$token_type;
							array_push($_SESSION['set_values'], $value);
							$this->parseColumnNames($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "ARITHMETIC_OPERATOR": // +, -, / operators
						//check if previous value is a number
						if($_SESSION['temp_type']!="INTEGER_TOKEN"){
							//remove slashes
							$lexeme = $this->removeQuotes($lexeme);

							echo '<br/>Syntax error: Value next to '.$lexeme.' is not a number.<br/>';
							$_SESSION['error']=1;
						}
						//part of an insert value
						else if(in_array($nextTok,array("INTEGER_LITERAL","OPENING_SYMBOL"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseColumnNames($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "ASTERISK_CHARACTER": // multiplication operator
						//check if previous value is a number
						if($_SESSION['temp_type']!="INTEGER_TOKEN"){
							//remove slashes
							$lexeme = $this->removeQuotes($lexeme);
							
							echo '<br/>Syntax error: Value next to '.$lexeme.' is not a number.<br/>';
							$_SESSION['error']=1;
						}
						//part of an insert value
						else if(in_array($nextTok,array("INTEGER_LITERAL","OPENING_SYMBOL"))){
							$_SESSION['temp_lex'].=$lexeme;
							$this->parseColumnNames($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR": // comma 
						//do this when in arithmetic mode
						if($this->arith_flag==1){
							$this->resetTempValues(); // reset variables
							$this->parseColumnNames($stmt,$index+1);
						}
						else if(preg_match("/_LITERAL$/",$nextTok)||in_array($nextTok, array("OPENING_SYMBOL",
							"NUMERIC_COLUMN_NAME","COLUMN_NAME")))
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "TABLE_SELECT_OPERATOR": // FROM command
						$this->resetTempValues(); // reset variables
						$p = new ProcessSelect();
						$p->parseSelect($stmt,$index+1);
						break;
			}
		}
	}
}
?>