<?php
/**
	PHP File for parsing groups of data
**/
include('processInsert.php');
class ParseGroups{
	//process column names
	public function parseColumnNames($stmt,$index){
		echo $stmt[$index]['token']." ";
		
		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			//evaluate each token
			switch($token){
				case "OPENING_SYMBOL": // open parenthesis
						if($nextTok=="COLUMN_NAME")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "COLUMN_NAME": //column name of a table
						if($nextTok=="VALUE_SEPARATOR")
							$this->parseColumnNames($stmt,$index+1);
						else if($nextTok=="CLOSING_SYMBOL")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "VALUE_SEPARATOR": //comma ,
						if($nextTok=="COLUMN_NAME")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				case "CLOSING_SYMBOL":// close parenthesis
						if($nextTok=="INSERT_VALUES"){
							$p = new ProcessInsert();
							$p->insertValues($stmt,$index+1);
						}
						break;
			}
		}
	}

	//process conditions
	public function parseConditions($stmt,$index){
		echo $stmt[$index]['token']." ";
		
		if($index<count($stmt)){
			//current lexeme and token
			$token=$stmt[$index]['token'];
			$lexeme=$stmt[$index]['lexeme'];

			//next lexeme and token
			$nextTok=$this->getNextToken($stmt,$index);
			$nextLex=$this->getNextLexeme($stmt,$index);

			//evaluate each token
			switch($token){
				case "SELECT_OPERATOR": //start of WHERE clause 
						if($nextTok=="COLUMN_NAME")
							$this->parseColumnNames($stmt,$index+1);
						else $this->printErrorMessageAfter($lexeme,$nextLex);
						break;
				
			}
		}
	}
}
?>