<?php
/**
	PHP File for parsing Update Statements
**/
class processUpdate extends ParseProcess{
	public $assign_flag = 0;// flag to check if we're assigning values

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
				case "OPENING_SYMBOL":
						break;
				case "CLOSING_SYMBOL":
						break;
				case "COLUMN_NAME":
						// if the column does not exist in the table
						if(!in_array(strtoupper($lexeme), $this->tables[$table]))
							$this->printErrorMessageTable($lexeme,$table);
						// the column has the value to be edited
						else if($assign_flag==0||$nextLexeme=="="){
							$value['lexeme']=$lexeme;
							$value['token_type']=$token_type;
							array_push($_SESSION['columns'], $value);
							$assign_flag=1;
							$this->parseUpdate($stmt,$index+1);
						}
						else $this->printErrorMessageAfter($lexeme,$nextLex);
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
				case "ARITHMETIC_OPERATOR":
						break;
				case "ASTERISK_CHARACTER":
						break;
				case "COMPARISON_OPERATOR_EQUALITY": // ASSIGNMENT operator
						//not an assignment operator
						if($lexeme!="=") $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR":
						$assign_flag = 0;
						break;
			}
		}
	}
}
?>