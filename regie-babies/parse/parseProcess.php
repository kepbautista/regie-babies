<?php
/**
	PHP File for Parsing Processes
**/
class ParseProcess{
	// Hash with a hash (containing key => value pairs of tables and their columns)
	public $tables = array(
		"STUDENT" => array("STUDNO", "STUDENTNAME", "BIRTHDAY", "DEGREE", "MAJOR","UNITSEARNED",
							"STUDENT.STUDNO", "STUDENT.STUDENTNAME", "STUDENT.BIRTHDAY", 
							"STUDENT.DEGREE", "STUDENT.MAJOR", "STUDENT.UNITSEARNED"),
		"STUDENTHISTORY" => array("STUDNO", "DESCRIPTION", "ACTION", "DATEFILED", "DATERESOLVED",
							"STUDENTHISTORY.STUDNO", "STUDENTHISTORY.DESCRIPTION", "STUDENTHISTORY.ACTION", 
							"STUDENTHISTORY.DATEFILED", "STUDENTHISTORY.DATERESOLVED"),
		"COURSE" => array("CNO", "CTITLE", "CDESC", "NOOFUNITS", "HASLAB", "SEMOFFERED",
							"COURSE.CNO", "COURSE.CTITLE", "COURSE.CDESC", "COURSE.NOOFUNITS",
							"COURSE.HASLAB", "COURSE.SEMOFFERED"),
		"COURSEOFFERING" => array("SEMESTER", "ACADYEAR", "CNO", "SECTION", "TIME", "MAXSTUD",
							"COURSEOFFERING.SEMESTER","COURSEOFFERING.ACADYEAR",
							"COURSEOFFERING.CNO","COURSEOFFERING.SECTION","COURSEOFFERING.TIME",
							"COURSEOFFERING.MAXSTUD"),
		"STUDCOURSE" => array("STUDNO","CNO","SEMESTER","ACADYEAR",
							"STUDCOURSE.STUDNO","STUDCOURSE.CNO","STUDCOURSE.SEMESTER","STUDCOURSE.ACADYEAR")
	);

	public $table_columns = array(
		"STUDENT" => array("STUDENT.STUDNO", "STUDENT.STUDENTNAME", "STUDENT.BIRTHDAY", 
							"STUDENT.DEGREE", "STUDENT.MAJOR", "STUDENT.UNITSEARNED"),
		"STUDENTHISTORY" => array("STUDENTHISTORY.STUDNO", "STUDENTHISTORY.DESCRIPTION", "STUDENTHISTORY.ACTION", 
							"STUDENTHISTORY.DATEFILED", "STUDENTHISTORY.DATERESOLVED"),
		"COURSE" => array("COURSE.CNO", "COURSE.CTITLE", "COURSE.CDESC", "COURSE.NOOFUNITS",
							"COURSE.HASLAB", "COURSE.SEMOFFERED"),
		"COURSEOFFERING" => array("COURSEOFFERING.SEMESTER","COURSEOFFERING.ACADYEAR",
							"COURSEOFFERING.CNO","COURSEOFFERING.SECTION","COURSEOFFERING.TIME",
							"COURSEOFFERING.MAXSTUD"),
		"STUDCOURSE" => array("STUDCOURSE.STUDNO","STUDCOURSE.CNO","STUDCOURSE.SEMESTER","STUDCOURSE.ACADYEAR")
	);

	//table columns
	public $student_cols = array(//STUDENT table
								0 => array("lexeme"=>"STUDNO","token_type"=>"STUDENT_NUMBER_TOKEN"),
								1 => array("lexeme"=>"STUDENTNAME","token_type"=>"NORMAL_WORD"),
								2 => array("lexeme"=>"BIRTHDAY","token_type"=>"DATE_TOKEN"),
								3 => array("lexeme"=>"DEGREE","token_type"=>"NORMAL_WORD"),
								4 => array("lexeme"=>"MAJOR","token_type"=>"NORMAL_WORD"),
								5 => array("lexeme"=>"UNITSEARNED","token_type"=>"INTEGER_TOKEN")
						   );
	public $student_hist_cols = array(//STUDENTHISTORY table
								0 => array("lexeme"=>"STUDNO","token_type"=>"STUDENT_NUMBER_TOKEN"),
								1 => array("lexeme"=>"DESCRIPTION","token_type"=>"NORMAL_WORD"),
								2 => array("lexeme"=>"ACTION","token_type"=>"NORMAL_WORD"),
								3 => array("lexeme"=>"DATEFILED","token_type"=>"DATE_TOKEN"),
								4 => array("lexeme"=>"DATE_TOKEN","token_type"=>"DATE_TOKEN")
						   );
	public $course_cols = array(//COURSE table
								0 => array("lexeme"=>"CNO","token_type"=>"NORMAL_WORD"),
								1 => array("lexeme"=>"CTITLE","token_type"=>"NORMAL_WORD"),
								2 => array("lexeme"=>"CDESC","token_type"=>"NORMAL_WORD"),
								3 => array("lexeme"=>"NOOFUNITS","token_type"=>"INTEGER_TOKEN"),
								4 => array("lexeme"=>"HASLAB","token_type"=>"BOOLEAN_TOKEN"),
								5 => array("lexeme"=>"SEMOFFERED","token_type"=>"SEMESTER_TOKEN")
						   );
	public $course_off_cols = array(//COURSEOFFERING table
								0 => array("lexeme"=>"SEMESTER","token_type"=>"SEMESTER_TOKEN"),
								1 => array("lexeme"=>"ACADYEAR","token_type"=>"NORMAL_WORD"),
								2 => array("lexeme"=>"CNO","token_type"=>"NORMAL_WORD"),
								3 => array("lexeme"=>"SECTION","token_type"=>"NORMAL_WORD"),
								4 => array("lexeme"=>"TIME","token_type"=>"TIME_TOKEN"),
								5 => array("lexeme"=>"MAXSTUD","token_type"=>"INTEGER_TOKEN")
						   );
	public $studcourse_cols = array(//STUDCOURSE table
								0 => array("lexeme"=>"STUDNO","token_type"=>"STUDENT_NUMBER_TOKEN"),
								1 => array("lexeme"=>"CNO","token_type"=>"NORMAL_WORD"),
								2 => array("lexeme"=>"SEMESTER","token_type"=>"SEMESTER_TOKEN"),
								3 => array("lexeme"=>"ACADYEAR","token_type"=>"NORMAL_WORD")
						   );
	
	// check if the column exists in the tables
	public function matchTables($column){
		$tables = $_SESSION['tables'];//get names of current tables
		$table_names = explode(",",$tables);// split the table names using ","

		//traverse each table
		foreach($table_names as $table){
			// if column is found in the table
			if(in_array($column, $this->tables[$table]))
				return true;
		}

		return false;
	}

	// clean the string by removing the slashes \' and \"
	public function removeQuotes($str){
		$str = str_replace('\"', "", $str);
		$str = str_replace("\'", "", $str);
		$str = str_replace("\n", " ", $str);
		return $str;
	}

	//print an error message after a certain token
	public function printErrorMessageAfter($current,$next){
		// remove slashes
		$current = $this->removeQuotes($current);
		$next = $this->removeQuotes($next);

		$_SESSION['error']=1;
		echo '<br/>Syntax error: Unexpected "'.$next.'" after "'.$current.'".';
	}

	//print an error message after a certain token
	public function printErrorMessageTable($column,$table){
		// remove slashes
		$column = $this->removeQuotes($column);
		$table = $this->removeQuotes($table);

		$_SESSION['error']=1;
		echo '<br/>Syntax error: Column "'.$column.'" does not exist in "'.$table.'".';
	}

	//print an error message that set type is not compatible
	public function printErrorMessageSetType($lexeme,$type){
		// remove slashes
		$lexeme = $this->removeQuotes($lexeme);
		$type = $this->removeQuotes($type);

		$_SESSION['error']=1;
		echo '<br/>Syntax error: "'.$lexeme.'" is not a/an "'.$type.'".';
	}

	//get value of next lexeme
	public function getNextLexeme($stmt,$index){
		if($index+1<count($stmt))
			return $stmt[$index+1]['lexeme'];
	}	

	//take a peek at the next token
	public function getNextToken($stmt,$index){
		if($index+1<count($stmt))
			return $stmt[$index+1]['token'];
	}

}
?>