<?php
/**
	PHP File for processing WHERE statements
**/
class ProcessWhereStmt extends ParseProcess{
	//parse start of WHERE Statement
	public function parseWhereStmt($stmt,$index){
		//echo $stmt[$index]['token']." ";
		
		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			//evaluate each token
			switch($token){
				case "SELECT_OPERATOR": //WHERE clause start
						if(in_array($nextTok,array("COLUMN_NAME","NUMERIC_COLUMN_NAME","OPENING_SYMBOL"))
							||preg_match("/_LITERAL$/",$nextTok))
							$this->parseWhereStmt($stmt,$index+1);
						//ELSE IF FOR ARITHMETIC EXPRESSIONS!!!
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "OPENING_SYMBOL": //opening parenthesis
						if(in_array($nextTok,array("COLUMN_NAME","NUMERIC_COLUMN_NAME"))
							||preg_match("/_LITERAL$/",$nextTok))
							$this->parseWhereStmt($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "CLOSING_SYMBOL":
						break;
				case "COLUMN_NAME": //non-numeric column name
						if(in_array($nextTok,array("COMPARISON_OPERATOR","CLOSING_SYMBOL")))
							$this->parseWhereStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						break;
				case "NUMERIC_COLUMN_NAME":
						break;
				case "NULL_LITERAL":
						break;
				case "DATE_LITERAL":
						break;
				case "TIME_LITERAL":
						break;
				case "STUDENT_NUMBER_LITERAL":
						break;
				case "STRING_LITERAL":
						break;
				case "INTEGER_LITERAL":
						break;
				/**
				case for ARITHMETIC EXPRESSIONS!!!
				**/
				case "COMPARISON_OPERATOR": //comparison operators like =, <, >
						if(in_array($nextTok,array("COLUMN_NAME","NUMERIC_COLUMN_NAME"))
							||preg_match("/_LITERAL$/",$nextTok))
							$this->parseWhereStmt($stmt,$index+1);
						//ELSE IF FOR ARITHMETIC EXPRESSIONS!!!
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
			}
		}
	}
}
?>