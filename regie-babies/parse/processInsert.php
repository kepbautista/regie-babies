<?php
/**
	PHP File for parsing Insert Statements
**/
include('parseProcess.php');
class ProcessInsert extends ParseProcess{
	//parse values to be inserted
	public function parseInsertValues($stmt,$index){
		echo $stmt[$index]['token']." ";
		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

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
						if($nextTok=="VALUE_SEPARATOR")
							$this->parseInsertValues($stmt,$index+1);
						else if($nextTok=="CLOSING_SYMBOL") // closing parenthesis
							$this->parseInsertValues($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR": // comma ,
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

			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			//evaluate each token
			switch($token){
				case "OPENING_SYMBOL": 
						if($nextTok=="COLUMN_NAME")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COLUMN_NAME":
						if($nextTok=="VALUE_SEPARATOR")
							$this->parseColumnNames($stmt,$index+1);
						else if($nextTok=="CLOSING_SYMBOL")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR":
						if($nextTok=="COLUMN_NAME")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "CLOSING_SYMBOL":
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
				case "INSERT_OPERATOR": 
						if($nextTok=="TABLE_NAME")
							$this->parseInsert($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "TABLE_NAME": 
						if($nextTok=="INSERT_VALUES")
							$this->parseInsertValues($stmt,$index+1);
						else if($nextTok=="OPENING_SYMBOL")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
			}
		}
	}
}
?>