<?php
/**
	PHP File for Parsing Processes
**/e 
class ParseProcess{
	//print an error message after a certain token
	public function printErrorMessageAfter($current,$next){
		echo "Syntax error: Unexpected ".$next." after ".$current.".<br/>";
	}

	//get value of next lexeme
	public function getNextLexeme($stmt,$index){
		return $stmt[$index+1]['lexeme'];
	}	

	//take a peek at the next token
	public function getNextToken($stmt,$index){
		return $stmt[$index+1]['token'];
	}

}
?>