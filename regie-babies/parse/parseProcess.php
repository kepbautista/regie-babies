<?php
/**
	PHP File for Parsing Processes
**/
class ParseProcess{
	//table columns
	public $student_cols = "STUDNO,STUDENTNAME,BIRTHDAY,DEGREE,MAJOR,UNITSEARNED";//student table
	public $student_hist_cols = "STUDNO,DESCRIPTION,ACTION,DATEFILED,DATERESOLVED";//studenthistory table
	public $course_cols = "CNO,CTITILE,CDESC,NOOFUNITS,HASLAB,SEMOFFERRED";//course table
	public $course_off_cols = "SEMESTER,ACADYEAR,CNO,SECTION,TIME,MAXSTUD";//courseoffering table
	public $studcourse_cols = "STUDNO,CNO,SEMESTER,ACADYEAR";//studcourse table

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