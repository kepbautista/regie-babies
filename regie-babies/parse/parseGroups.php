<?php
/*
	PHP File for parsing groups of data
*/
include('processInsert.php');
class ParseGroups{
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
						if($nextTok=="INSERT_VALUES"){
							$p = new ProcessInsert();
							$this->parseColumnNames($stmt,$index+1);
						}
						break;
			}
		}
	}
}
?>