<?php
/**
	PHP File for Parsing Processes
**/
class ParseProcess{
	//table columns
	public $student_cols = "STUDNO,STUDENTNAME,BIRTHDAY,DEGREE,MAJOR,UNITSEARNED";

	//print an error message after a certain token
	public function printErrorMessageAfter($current,$next){
		echo 'Syntax error: Unexpected "'.$next.'" after "'.$current.'".<br/>"';
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