<?php
/**
	PHP File for Join condition processing
**/
class ProcessJoin extends ParseProcess{
	// parse start of join condition
	public function parseJoinStmt($stmt,$index){
		//echo $stmt[$index]['token']." ";
		$table = $_SESSION['tables'];//get names of tables

		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			//evaluate each token
			switch($token){
				case "JOIN_CONDITION_OPERATOR": // ON statement
						if(in_array($nextTok,array("COLUMN_NAME","NUMERIC_COLUMN_NAME","OPENING_SYMBOL"))
							||preg_match("/_LITERAL$/",$nextTok))
							$this->parseJoinStmt($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "OPENING_SYMBOL": //opening parenthesis
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("COLUMN_NAME","NUMERIC_COLUMN_NAME","OPENING_SYMBOL"))
							||preg_match("/_LITERAL$/",$nextTok))
							$this->parseJoinStmt($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "CLOSING_SYMBOL":
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("COMPARISON_OPERATOR_EQUALITY","COMPARISON_OPERATOR_STRING"
							,"CLOSING_SYMBOL","ARITHMETIC_OPERATOR","ASTERISK_CHARACTER")))
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COLUMN_NAME": //non-numeric column name
						$_SESSION['join_on'].=strtoupper($lexeme);

						// if the column does not exist in the table
						if(!$this->matchTables(strtoupper($lexeme)))
							$this->printErrorMessageTable($lexeme,$table);
						else if(in_array($nextTok,array("COMPARISON_OPERATOR_EQUALITY","COMPARISON_OPERATOR_STRING"
							,"CLOSING_SYMBOL","NULL_COMPARISON_KEYWORD")))
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "NUMERIC_COLUMN_NAME"://columns where arithmetic operations can be performed
						$_SESSION['join_on'].=strtoupper($lexeme);
						
						// if the column does not exist in the table
						if(!$this->matchTables(strtoupper($lexeme)))
							$this->printErrorMessageTable($lexeme,$table);
						else if(in_array($nextTok,array("COMPARISON_OPERATOR_EQUALITY","COMPARISON_OPERATOR","OPENING_SYMBOL"
							,"CLOSING_SYMBOL","ARITHMETIC_OPERATOR","ASTERISK_CHARACTER","NULL_COMPARISON_KEYWORD",
							"COMPARISON_OPERATOR_STRING")))
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "NULL_COMPARISON_KEYWORD": // IS keyword for NULL comparison
						$_SESSION['join_on'].=" ".strtoupper($lexeme);
						if($nextTok=="NULL_LITERAL"||$nextTok=="NOT_NULL_COMPARISON_KEYWORD")
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "NOT_NULL_COMPARISON_KEYWORD":
						$_SESSION['join_on'].=" ".strtoupper($lexeme);
						if($nextTok=="NULL_LITERAL")
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						break;
				case "NULL_LITERAL": // compared to null
						$_SESSION['join_on'].=" ".strtoupper($lexeme);
						if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "DATE_LITERAL"://date values
						$_SESSION['join_on'].=strtoupper($lexeme);
						if(in_array($nextTok,array("COMPARISON_OPERATOR_EQUALITY","COMPARISON_OPERATOR_STRING"
							,"CLOSING_SYMBOL","NULL_COMPARISON_KEYWORD")))
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "TIME_LITERAL"://time values
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("COMPARISON_OPERATOR_EQUALITY","COMPARISON_OPERATOR_STRING"
							,"CLOSING_SYMBOL","NULL_COMPARISON_KEYWORD")))
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "SEMESTER_LITERAL"://semester values
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("COMPARISON_OPERATOR_EQUALITY","COMPARISON_OPERATOR_STRING"
							,"CLOSING_SYMBOL","NULL_COMPARISON_KEYWORD")))
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "STUDENT_NUMBER_LITERAL": //student number values
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("COMPARISON_OPERATOR_EQUALITY","COMPARISON_OPERATOR_STRING"
							,"CLOSING_SYMBOL","NULL_COMPARISON_KEYWORD")))
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "STRING_LITERAL": // String values
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("COMPARISON_OPERATOR_EQUALITY","COMPARISON_OPERATOR_STRING"
							,"CLOSING_SYMBOL","NULL_COMPARISON_KEYWORD")))
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "INTEGER_LITERAL"://numeric values
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("COMPARISON_OPERATOR_EQUALITY","COMPARISON_OPERATOR","OPENING_SYMBOL"
							,"CLOSING_SYMBOL","ARITHMETIC_OPERATOR","ASTERISK_CHARACTER","NULL_COMPARISON_KEYWORD")))
							$this->parseJoinStmt($stmt,$index+1);
						else if($nextTok=="END_OF_STATEMENT") break;//end of the statement
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "ARITHMETIC_OPERATOR": //arithmetic characters: +, -, /
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("NUMERIC_COLUMN_NAME","INTEGER_LITERAL",
							"OPENING_SYMBOL")))
							$this->parseJoinStmt($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "ASTERISK_CHARACTER": //multiplication character (*)
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("NUMERIC_COLUMN_NAME","INTEGER_LITERAL",
							"OPENING_SYMBOL")))							
							$this->parseJoinStmt($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COMPARISON_OPERATOR_EQUALITY": // comparison operator: =, !=
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("COLUMN_NAME","NUMERIC_COLUMN_NAME","OPENING_SYMBOL"))
							||preg_match("/[^(NULL)]_LITERAL$/",$nextTok))
							$this->parseJoinStmt($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COMPARISON_OPERATOR_STRING": // comparison operator "LIKE"
						$_SESSION['join_on'].=" ".strtoupper($lexeme)." ";
						if(in_array($nextTok,array("COLUMN_NAME","OPENING_SYMBOL","DATE_LITERAL",
							'TIME_LITERAL','STUDENT_NUMBER_LITERAL','STRING_LITERAL')))
							$this->parseJoinStmt($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COMPARISON_OPERATOR": //comparison operators like <, >
						$_SESSION['join_on'].=$lexeme;
						if(in_array($nextTok,array("NUMERIC_COLUMN_NAME","OPENING_SYMBOL",
							"INTEGER_LITERAL","DATE_LITERAL","TIME_LITERAL")))
							$this->parseJoinStmt($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				
			}
		}
	}
}
?>