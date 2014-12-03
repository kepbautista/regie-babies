<?php
/**
	PHP File for parsing Insert Statements
**/
include('parseProcess.php');
class ProcessInsert extends ParseProcess{
	//parse values to be inserted
	public function parseInsertValues($stmt,$index){
		//echo $stmt[$index]['token']." ";
		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];
			$token_type=$stmt[$index]['token_type'];

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
						if(preg_match("/_LITERAL$/",$nextTok))
							$this->parseInsertValues($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case (preg_match("/.+_LITERAL$/",$token)? true: false): //literals (int,string,etc.)
						if(preg_match("/.+_LITERAL$/",$token)||$nextTok=="CLOSING_SYMBOL"){//comma or closing parenthesis
							$value['lexeme']=$lexeme;
							$value['token_type']=$token_type;
							array_push($_SESSION['set_values'], $value);
							$this->parseInsertValues($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR": // comma 
						if(preg_match("/_LITERAL$/",$nextTok))
							$this->parseInsertValues($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "CLOSING_SYMBOL": //closing parenthesis
						if($nextTok=="END_OF_STATEMENT") break;
			}
		}
	}

	//process column names
	public function parseColumnNames($stmt,$index){
		echo $stmt[$index]['token']." ";
		
		if($index<count($stmt)){
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];
			$token_type=$stmt[$index]['$token_type'];

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
						if($nextTok=="VALUE_SEPARATOR"||$nextTok=="CLOSING_SYMBOL"){//comma or closing parenthesis
							$value['lexeme']=$lexeme;
							$value['token_type']=$token_type;
							array_push($_SESSION['columns'], $value);
							$this->parseColumnNames($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "NUMERIC_COLUMN_NAME": //numeric column type
						if($nextTok=="VALUE_SEPARATOR"||$nextTok=="CLOSING_SYMBOL"){//comma or closing parenthesis
							if($nextTok=="VALUE_SEPARATOR") $_SESSION['columns'].=$lexeme.",";//not last column
							else $_SESSION['columns'].=$lexeme;//last column
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
		echo $stmt[$index]['token']." ";
		
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
								case "STUDCOURSE": $_SESSION['columns']=$studcourse_cols; break;
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